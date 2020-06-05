/*--------------------------------------------------------------------------

MaintenanceTool.cc

Implementation
Maintenance tool support

Autor: Steffen Simon

$Date: 2018-06-28 09:40:03 +0200 (Do, 28. Jun 2018) $
$Revision: 7964 $
$Author: steffen $

--------------------------------------------------------------------------*/

#include <string.h>
#include "MaintenanceTool.h"
#include "eDeviceStream.h"
#include "version.h"
#if PARTNUMBER == 0xBABE
	#include "eDevice.h"
	#include "GAINClass.h"
	#include "error.h"
#else
	#include "PersistenceServices.h"
#endif
#ifdef NSD_SUPPORT
	#include "NSDHandler.h"
#endif

#ifdef NO_PERSISTENCE
	#include "error.h"
#endif

#ifndef AVR
#pragma pack(2)
#endif

typedef struct	
	{
	uint8	PortValue[7];
	uint8	reserved;
	uint8	PWMCount;
	uint8	PWMState;
	uint16	pwm[8];
	} GAINControllerPortState;

typedef struct
	{
	uint8	analogueCount;
	uint8	reserved;
	float	analogue[];
	} GAINControllerAnalogueValues;

#if (kMaintenanceProtocolVersion > 0)
typedef struct
	{
	uint16	version;
	uint16	capabilities;
	uint8	reserved[12];
	} protocolInfoRequestPayload;
#endif

typedef struct
	{
	uint16	partNumber;
	uint16	subPartNumber;
	uint16	powerBudget;
	uint16	standbyPowerBudget;
	char	partNumberString[16];
	char	serialNumberString[16];
	char	softwareVersionString[16];
	char	hardwareVersionString[8];
	} connectRequestPayload;

typedef struct
	{
	} stateChangedRequestPayload;

#ifdef NSD_SUPPORT
typedef struct
	{
	} networkRequestPayload;

typedef struct
	{
	uint16	powerBudget;
	uint16	standbyPowerBudget;
	char	PartNumber[15];
	char	PartNumberAmendment;
	char	SerialNumber[12];
	char	HardwareVersion[7];
	uint8	reserved;
	} NSDDataRequest;
#endif

typedef struct
	{
	tick_t	time;
	uint8	state;
	uint8	flags;

	uint8	deviceID;
	uint8	feederID;
	uint8	deviceFlags;
	uint8	networkState;
	} GAINStateNotification;
		
#if (kMaintenanceProtocolVersion == 0)
typedef struct
	{
	int16			numberOfFaults;
	int16			currentFaultIndex;
	errorLogEntry	error;
	} faultLogHeader;
#else
typedef struct
	{
	uint16			numberOfFaults;
	uint16			numberOfFaultsInMemory;
	uint16			currentFaultIndex;
	errorLogEntry	error;
	uint32			errorCounters[kGAINErrorCount];
	} faultLogHeader;

typedef struct
	{
	int16			faultIndex;
	errorLogEntry	error;
	} faultLogEntryHeader;
#endif

#ifndef AVR
#pragma pack()
#endif

MaintenanceTool::MaintenanceTool(void)
	{
	maintenanceIsConnected = false;
	nextMaintenanceSyncTime = 0;
	nextMaintenanceUsageTimerSyncTime = 0;
	}

void MaintenanceTool::setConnected(bool isConnected)
	{
	maintenanceIsConnected = isConnected;
	lastMaintenanceMessageReceivedTime = controller.ticks();
	}

bool MaintenanceTool::handlePeriodicTask(stream& io, uint8 state, uint8 flags)
	{
#ifdef E_DEVICE
	if (maintenanceIsConnected)
		corePortsClearPortBit(kPortG, kPortBit0);
	else
		corePortsSetPortBit(kPortG, kPortBit0);

	if (bSendNotifications)
		corePortsClearPortBit(kPortG, kPortBit1);
	else
		corePortsSetPortBit(kPortG, kPortBit1);
#endif
	if (maintenanceIsConnected)
		{
		if ((controller.ticks() - lastMaintenanceMessageReceivedTime) > 10000)
			{
			event	e;
	
			maintenanceIsConnected = false;
			lastMaintenanceMessageReceivedTime = forever;
			
			e.eventClass = kGAINEventClass;
			e.eventType = kGAINMaintenanceConnectionLostEvent;
			e.data.data = 0;
			sendEventSelf(e);
			}
		else if (controller.ticks() >= nextMaintenanceSyncTime)
			{
			if (bSendNotifications)
				{
				nextMaintenanceSyncTime = controller.ticks() + 500;
			
				GAINNotifyState(io, state, flags);
				GAINNotifyControllerState(io);
				
				if (state > kGAINStateReset)
					{
					GAINNotifyAnalogueValues(io);
			
					if (controller.ticks() >= nextMaintenanceUsageTimerSyncTime)
						{
						nextMaintenanceUsageTimerSyncTime = controller.ticks() + 5000;
						GAINNotifyUsageData(io);
						}
					}
				}
			}
		}

	return true;
	}

bool MaintenanceTool::handleCommunication(stream& io)
	{
	char	input;
	bool	result(true);

	io >> input;

	switch (input)
		{
		case '<':
			result = handleGAINQuery(io);
			break;

		case '>':
			result = handleGAINCommand(io);
			break;

		case '!':
			if (!maintenanceIsConnected)
				{
				event	e;

				nextMaintenanceSyncTime = 0;
				maintenanceIsConnected = true;
				bSendNotifications = true;

				e.eventClass = kGAINEventClass;
				e.eventType = kGAINMaintenanceConnectionEstablishedEvent;
				e.data.data = 0;
				sendEventSelf(e);

			#ifdef E_DEVICE
				corePortsClearPortBit(kPortG, kPortBit2);
			#endif
				}
			break;
			
		default:
			result = false;
			break;
		}

	if (result)
		lastMaintenanceMessageReceivedTime = controller.ticks();

	return result;
	}

void MaintenanceTool::updateAnalogueValue(uint8 idx, float value)
	{
	analogueValues[idx] = value;
#if (__DEBUG__ > 4) && (PARTNUMBER == 916)
	if (idx == 7)
		controller.asc0 << "updateAnalogueValue: " << value << " V" << newline;
#endif
#if (__DEBUG__ > 4) && (PARTNUMBER == 932)
	if (idx == 7)
		controller.asc0 << "updateAnalogueValue: " << value << " V" << newline;
#endif
#if (__DEBUG__ > 4) && (PARTNUMBER == 922)
	if (idx == 0)
		controller.asc0 << "updateAnalogueValue: rpm == " << value << newline;
#endif
	}

/*****

	GAIN notifications

*****/
bool MaintenanceTool::GAINNotifyState(stream& io, uint8 state, uint8 flags)
	{
#if (__DEBUG__ > 3)
	controller.asc0 << "GAINNotifyState: state == " << state << ", flags == " << kModeHex << flags << newline;
#endif
	GAINMessageHeader		header;
	GAINStateNotification	notification;
	uint16	checksum;
	
	bool	result(true);
		
	header.command = kGAINNotifyState;
	header.selector = 0;
	header.dataSize = sizeof(GAINStateNotification);
	
	notification.time = controller.ticks();
	notification.state = state;
	notification.flags = flags;

#ifdef NSD_SUPPORT
	notification.deviceID = NSD.getGAINDeviceID();
	notification.feederID = NSD.getGAINFeederID();

	notification.deviceFlags = (NSD.getGAINIsPrimary()?1:0);
	notification.deviceFlags |= (NSD.getGAINMaintenancePin()?2:0);
	notification.deviceFlags |= (NSD.getGAINParityPin()?4:0);
	notification.networkState = 0;
	
	if (NSD.MGCUIsPresent())
		notification.networkState |= (1 << 0);
	
	if (NSD.CentralizedPowerControlAvailable())
		notification.networkState |= (1 << 1);
#else
	notification.deviceFlags = 0;
	notification.networkState = 0;
#endif

	checksum = calculateChecksum(header);
	checksum = calculateChecksum(checksum, header.dataSize, &notification);

	io.writeData(&header, sizeof(header));
	io.writeData(&notification, sizeof(notification));
	io.writeData(&checksum, sizeof(checksum));

	return result;
	}

bool MaintenanceTool::GAINNotifyControllerState(stream& io)
	{
	GAINMessageHeader		header;
	GAINControllerPortState	state;
	uint16	checksum;
	uint8	i;
	bool	result(true);

	header.command = kGAINNotifyControllerState;
	header.selector = 0;
	header.dataSize = sizeof(GAINControllerPortState);

#ifdef MGS_ZST_Revision
	uint32	port(FIO1PIN);

	for (i=0; i<4; i++)
		{
		state.PortValue[3-i] = (uint8)(port & 0xFF);
		port >>= 8;
		}
#else
	for (i=0; i<7; i++)
		{
		state.PortValue[i] = corePortsGetPort((controllerPort)i);
		}

	state.PWMCount = kPWMPinCount;
	state.PWMState = 0;
	for (i=0; i<kPWMPinCount; i++)
		{
		if (corePWMGetPWMState((pwmPin)i))
			{
			state.PWMState |= (1 << i);
			}
		}

	for (i=0; i<kPWMPinCount; i++)
		{
		state.pwm[i] = corePWMGetPWMDutyCycle((pwmPin)i);
		}
#endif		
	checksum = calculateChecksum(header);
	checksum = calculateChecksum(checksum, sizeof(GAINControllerPortState), &state);

	io.writeData(&header, sizeof(header));
	io.writeData(&state, sizeof(GAINControllerPortState));
	io.writeData(&checksum, sizeof(checksum));
	
	return result;
	}

bool MaintenanceTool::GAINNotifyAnalogueValues(stream& io)
	{
	GAINMessageHeader	header;
	GAINControllerAnalogueValues	values;
	uint16	checksum;
	bool	result(true);

#ifdef IPECO_PSC_Revision
#if PARTNUMBER != 920	
#ifdef NO_RTC
	updateAnalogueValue(12, 20.1);
#else
	updateAnalogueValue(12, coreRTCGetTemperatureValue());
#endif
#endif
#endif

	header.command = kGAINNotifyAnalogueValues;
	header.selector = 0;
	header.dataSize = sizeof(GAINControllerAnalogueValues) + sizeof(analogueValues);

	values.analogueCount = 16;
	values.reserved = 0;
		
	checksum = calculateChecksum(header);
	checksum = calculateChecksum(checksum, sizeof(GAINControllerAnalogueValues), &values);
	checksum = calculateChecksum(checksum, sizeof(analogueValues), &analogueValues);

	io.writeData(&header, sizeof(header));
	io.writeData(&values, sizeof(GAINControllerAnalogueValues));
	io.writeData(analogueValues, sizeof(analogueValues));
	io.writeData(&checksum, sizeof(checksum));

	return result;
	}

bool MaintenanceTool::GAINNotifyUsageData(stream& io)
	{
	GAINMessageHeader	header;
	uint16	checksum;
	bool	result(true);
	uint8	n;

	header.command = kGAINNotifyUsageData;
	header.selector = 0;

#if PARTNUMBER == 0xBABE
	header.dataSize = sizeof(n);
	checksum = calculateChecksum(header);
	checksum = calculateChecksum(checksum, 1, &n);

	io.writeData(&header, sizeof(header));
	io.writeData(&n, sizeof(n));
	io.writeData(&checksum, sizeof(checksum));
#else
#if PARTNUMBER == 922
	uint32	usageData[kNumberOfUsageTimeCounters + 3 + kNumberOfUsageCounters];
#else
	uint32	usageData[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions + kNumberOfUsageCounters];
#endif
	n = persistent.getUsageData(usageData);

	header.dataSize = (n * sizeof(uint32)) + 1;
	checksum = calculateChecksum(header);
	checksum = calculateChecksum(checksum, 1, &n);
	checksum = calculateChecksum(checksum, n * sizeof(uint32), &usageData);

	io.writeData(&header, sizeof(header));
	io.writeData(&n, sizeof(n));

	if (n > 0)
		io.writeData(usageData, n * sizeof(uint32));

	io.writeData(&checksum, sizeof(checksum));
#endif
	
	return result;
	}

/*****

	GAIN commands

*****/
bool MaintenanceTool::handleGAINCommand(stream& io)
	{
    GAINMessageHeader	header;
	uint16	checksum;
	bool	result(true);

	io.readData(&header, sizeof(GAINMessageHeader), 500);
   	
   	if (header.dataSize == 0)
   		{
		uint16	headerChecksum;

		io.readData(&checksum, sizeof(checksum), 500);
   		headerChecksum = calculateChecksum(header);
   		result = (checksum == headerChecksum);
   		}
	
	if (result)
		{
		switch (header.command)
			{
			case kGAINCommandConnect:
				result = handleGAINCommandConnect(io, header);
				break;

			case kGAINCommandDisconnect:
				result = handleGAINCommandDisconnect(io, header);
				break;

			case kGAINCommandSetNotificationState:
				result = handleGAINCommandSetNotificationState(io, header);
				break;

			case kGAINCommandReset:
				notifyGAINCommandReceived(io, header, result);
				result = handleGAINCommandReset(io, header);
				break;

			case kGAINCommandResetNVM:
				result = handleGAINCommandResetNVM(io, header);
				break;

			case kGAINCommandSetControllerState:
				result = handleGAINCommandSetControllerState(io, header);
				break;

			case kGAINCommandSetConfiguration:
				result = handleGAINCommandSetConfiguration(io, header);
				break;

			case kGAINCommandSetCycleDictionary:
				result = handleGAINCommandSetCycleDictionary(io, header);
				break;

			case kGAINCommandSetCycleDescription:
				result = handleGAINCommandSetCycleDescription(io, header);
				break;
	
		#ifdef NSD_SUPPORT
			case kGAINCommandSetNSDData:
				result = handleGAINCommandSetNSDData(io, header);
				break;
		#endif

			default:
				result = false;
				break;
			}
		}

	notifyGAINCommandReceived(io, header, result);

	return result;
	}

void MaintenanceTool::notifyGAINCommandReceived(stream& io, GAINMessageHeader& header, bool result)
	{
	uint16	checksum;

	header.selector = header.command;
	header.command = kGAINNotifyCommandReceived;
	header.dataSize = 0;

	if (!result)
		header.selector |= 0x80;

	checksum = calculateChecksum(header);

	io.writeData(&header, sizeof(header));
	io.writeData(&checksum, sizeof(checksum));
	}

bool MaintenanceTool::handleGAINCommandConnect(stream& io, GAINMessageHeader& header)
	{
	bool	result(true);
	
	if (!maintenanceIsConnected)
		{
		event	e;

		nextMaintenanceSyncTime = 0;
		maintenanceIsConnected = true;
		bSendNotifications = true;

		e.eventClass = kGAINEventClass;
		e.eventType = kGAINMaintenanceConnectionEstablishedEvent;
		e.data.data = 0;
		sendEventSelf(e);

	#ifdef E_DEVICE
		corePortsClearPortBit(kPortG, kPortBit2);
	#endif
		}

	return result;
	}

bool MaintenanceTool::handleGAINCommandDisconnect(stream& io, GAINMessageHeader& header)
	{
	event	e;
	bool	result(true);

	maintenanceIsConnected = false;
	bSendNotifications = false;

	e.eventClass = kGAINEventClass;
	e.eventType = kGAINMaintenanceConnectionLostEvent;
	e.data.data = header.selector;
	sendEventSelf(e);

#ifdef E_DEVICE
	corePortsSetPortBit(kPortG, kPortBit2);
#endif
	return result;
	}

bool MaintenanceTool::handleGAINCommandSetNotificationState(stream& io, GAINMessageHeader& header)
	{
	event	e;
	bool	result(true);

	bSendNotifications = (header.selector == 1);

	return result;
	}

bool MaintenanceTool::handleGAINCommandReset(stream& io, GAINMessageHeader& header)
	{
	controller.reset();

	return true;
	}

bool MaintenanceTool::handleGAINCommandResetNVM(stream& io, GAINMessageHeader& header)
	{
	bool	result(true);

	switch (header.selector)
		{
		case '0':	//	Clear working state
		#if PARTNUMBER != 0xBABE
			persistent.clearWorkingStateForGAIN();
		#endif
			break;
			
		case '1':	//	Set factory defaults, clear working state and reset
		#if PARTNUMBER != 0xBABE
			persistent.clearWorkingStateForGAIN();
			GAIN.setFactoryDefaults(GAIN.configuration.commons.subPartNumber, true, false);
		#endif
			break;
			
		case '2':	//	Set factory defaults, clear working state and reset
		#if PARTNUMBER != 0xBABE
			persistent.clearWorkingStateForGAIN();
			GAIN.setFactoryDefaults(GAIN.configuration.commons.subPartNumber, false, true);
		#endif
			break;
			
		case '3':	//	Clear error log
			persistent.initializeErrorArea();
			break;
			
		case '4':	//	Clear usage counters
			persistent.initializeUsageArea();
			break;
			
		case '5':	//	Clear maintenance counters
			persistent.initializeMaintenanceArea();
			break;
			
		default:
			result = false;
			break;
		}

	return result;
	}

bool MaintenanceTool::handleGAINCommandSetControllerState(stream& io, GAINMessageHeader& header)
	{
	uint16	checksum;
	uint16	checksumProvided;
	bool	result(header.dataSize == sizeof(GAINControllerPortState));
	uint8	i;
	
	if (result)
		{
		uint8	n;
		GAINControllerPortState	state;

		n = io.readData(&state, sizeof(GAINControllerPortState), 500);
		result = (n == sizeof(GAINControllerPortState));

		n = io.readData(&checksumProvided, sizeof(checksumProvided), 100);

		if (result)
			{
			checksum = calculateChecksum(header);
			checksum = calculateChecksum(checksum, sizeof(GAINControllerPortState), &state);
			
			result = (checksum == checksumProvided);
			}

		if (result)
			{
		#ifdef MGS_ZST_Revision	
			uint32	port(0);

			for (i=0; i<4; i++)
				{
				port <<= 8;
				port |= state.PortValue[i];
				}
			
			FIO1CLR = (~port & FIO1DIR);
			FIO1SET = (port & FIO1DIR);
			//	FIO1PIN = port;
		#else
			for (i=0; i<7; i++)
				{
				corePortsSetPort((controllerPort)i, state.PortValue[i]);
				}
			for (i=0; i<kPWMPinCount; i++)
				{
				corePWMSetPWMDutyCycle((pwmPin)i, state.pwm[i]);
			#if __DEBUG__ > 4
				controller.asc0 << "corePWMSetPWMDutyCycle: channel #" << i << ", pwm == " << command.pwm[i] << newline;
			#endif
				if (state.PWMState & (1 << i))
					corePWMSetPWMOn((pwmPin)i);
				else if (corePWMGetPWMState((pwmPin)i))
					corePWMSetPWMOff((pwmPin)i);
/*
				if (state.PWMState & (1 << i))
					{
					corePWMSetPWMDutyCycle((pwmPin)i, state.pwm[i]);
				#if __DEBUG__ > 4
					controller.asc0 << "corePWMSetPWMDutyCycle: channel #" << i << ", pwm == " << command.pwm[i] << newline;
				#endif
					if (state.PWMState & (1 << i))
						corePWMSetPWMOn((pwmPin)i);
					else if (corePWMGetPWMState((pwmPin)i))
						corePWMSetPWMOff((pwmPin)i);
					}
*/
				}
		#endif
			}
		}
	else
		{
		char	ch;
		
		for (i=0; i<header.dataSize; i++)
			io >> ch;
		}
	
	return result;
	}

bool MaintenanceTool::handleGAINCommandSetConfiguration(stream& io, GAINMessageHeader& header)
	{
	uint16	checksumProvided;
	bool	result(header.dataSize == sizeof(ConfigurationData));
	
	if (result)
		{
		int16	n;
		uint16	subPartNumber(GAIN.configuration.commons.subPartNumber);
		
		n = io.readData(&GAIN.configuration, sizeof(ConfigurationData), 500);
		result = (n == sizeof(ConfigurationData));

		io.readData(&checksumProvided, sizeof(checksumProvided), 100);

		if (result)
			{
			uint16	checksum;

			checksum = calculateChecksum(header);
			checksum = calculateChecksum(checksum, header.dataSize, &GAIN.configuration);

			result = (checksum == checksumProvided);
			}
		
		if (result)
			{
			bool	resetConfiguration(subPartNumber != GAIN.configuration.commons.subPartNumber);

			if (resetConfiguration)
				{
				GAIN.setFactoryDefaults(GAIN.configuration.commons.subPartNumber, true, false);
				}
			else
				{
				persistent.writeConfigurationDataArea(GAIN.configuration);
				}
			}
		else
			{
			GAIN.setFactoryDefaults(subPartNumber, true, false);
			}

		persistent.clearWorkingStateForGAIN();
		}
	else
		{
		uint8	i;
		char	ch;
		
		for (i=0; i<header.dataSize; i++)
			io >> ch;
		}

	return result;
	}

bool MaintenanceTool::handleGAINCommandSetCycleDictionary(stream& io, GAINMessageHeader& header)
	{
	return true;
/*
	uint16	checksumProvided;
	bool	result(header.dataSize == sizeof(CycleDictionary));
	
	if (result)
		{
		int16	n;
		
		n = io.readData(&GAIN.cycleDictionary, sizeof(CycleDictionary), 500);
		result = (n == sizeof(CycleDictionary));

		io.readData(&checksumProvided, sizeof(checksumProvided), 100);

		if (result)
			{
			uint16	checksum;

			checksum = calculateChecksum(header);
			checksum = calculateChecksum(checksum, header.dataSize, &GAIN.cycleDictionary);

			result = (checksum == checksumProvided);
			}
		
		if (result)
			{
			persistent.invalidatePersistentConfigurationDataArea();
			persistent.writeCycleDictionaryArea(GAIN.cycleDictionary);
			}
		}
	else
		{
		uint8	i;
		char	ch;
		
		for (i=0; i<header.dataSize; i++)
			io >> ch;
		}

	return result;
*/
	}


bool MaintenanceTool::handleGAINCommandSetCycleDescription(stream& io, GAINMessageHeader& header)
	{
	uint16	checksumProvided;
	bool	result(header.dataSize == sizeof(CycleDescription));
	
	if (result)
		{
		CycleDescription	cycle;
		int16	n;
	
		n = io.readData(&cycle, sizeof(CycleDescription), 500);
		result = (n == sizeof(CycleDescription));

		io.readData(&checksumProvided, sizeof(checksumProvided), 100);

		if (result)
			{
			uint16	checksum;

			checksum = calculateChecksum(header);
			checksum = calculateChecksum(checksum, header.dataSize, &cycle);

			result = (checksum == checksumProvided);
			}
		
		if (result)
			{
			result = (header.selector < GAIN.cycleDictionary.numberOfCycleDescriptions);
			}
		
		if (result)
			{
		#if (PARTNUMBER == 921)||(PARTNUMBER == 928)
			if ((cycle.overshootTime > 0) && (cycle.targetTemperature > (cycle.overshootEntryTemperature - 5)))
				cycle.overshootEntryTemperature = cycle.targetTemperature + 5;							
		#endif
			persistent.writeCycleDescription(cycle, header.selector);
			GAIN.resyncWithCycleDescription(header.selector);
			}
		}
	else
		{
		uint8	i;
		char	ch;
		
		for (i=0; i<header.dataSize; i++)
			io >> ch;
		}

	return result;
	}

#ifdef NSD_SUPPORT
bool MaintenanceTool::handleGAINCommandSetNSDData(stream& io, GAINMessageHeader& header)
	{
	uint16	checksumProvided;
	bool	result(header.dataSize == sizeof(NSDDataRequest));
	
	if (result)
		{
		NSDDataRequest	request;
		//uint8	checksum(0);
		uint8	n;
	
		n = io.readData(&request, sizeof(NSDDataRequest), 1000);
		result = (n == sizeof(NSDDataRequest));

		io.readData(&checksumProvided, sizeof(checksumProvided), 100);

		if (result)
			{
			uint16	checksum;

			checksum = calculateChecksum(header);
			checksum = calculateChecksum(checksum, header.dataSize, &request);

			result = (checksum == checksumProvided);
		    }

		if (result)
			{
			GAINDataBlock05	dataBlock05;
			
			coreNSD_readGAINConfigurationDataBlock(5, &dataBlock05);
			//	Copy data
			memcpy(dataBlock05.PartNumber, request.PartNumber, sizeof(dataBlock05.PartNumber));
			dataBlock05.PartNumberAmendment = request.PartNumberAmendment;
			memcpy(dataBlock05.SerialNumber, request.SerialNumber, sizeof(dataBlock05.SerialNumber));
			memcpy(dataBlock05.HardwareVersion, request.HardwareVersion, sizeof(dataBlock05.HardwareVersion));
			coreNSD_writeGAINConfigurationDataBlock(5, &dataBlock05);
			coreNSD_flushGAINConfigurationDataBlock(5);

			GAINDataBlock03	dataBlock03;

			coreNSD_readGAINConfigurationDataBlock(3, &dataBlock03);
			dataBlock03.MaximumPowerBudget = (request.powerBudget + 49)/50;
			dataBlock03.MinimumPowerBudget = dataBlock03.MaximumPowerBudget;
			dataBlock03.StandbyPowerThreshold = (request.standbyPowerBudget + 4)/5;
			coreNSD_writeGAINConfigurationDataBlock(3, &dataBlock03);
			coreNSD_flushGAINConfigurationDataBlock(3);
			}
		}
	else
		{
		uint8	i;
		char	ch;
		
		for (i=0; i<header.dataSize; i++)
			io >> ch;
		}

	return result;
	}
/*
bool MaintenanceTool::handleGAINCommandSetNSDDataBlock(stream& io, GAINMessageHeader& header)
	{
	uint16	checksumProvided;
	bool	result(header.dataSize == sizeof(NSDDataRequest));
	
	if (result)
		{
		NSDDataRequest	request;
		//uint8	checksum(0);
		uint8	n;
	
		n = io.readData(&request, sizeof(request), 500);
		result = (n == sizeof(request));

		io.readData(&checksumProvided, sizeof(checksumProvided), 100);

		if (result)
			{
			uint16	checksum;

			checksum = calculateChecksum(header);
			checksum = calculateChecksum(checksum, header.dataSize, &request);

			result = (checksum == checksum);
		    }

		if (result)
			{
			GAINDataBlock05	data;
			
			coreNSD_readGAINConfigurationDataBlock(5, &data);
			//	Copy data
			memcpy(data.PartNumber, request.PartNumber, sizeof(data.PartNumber));
			data.PartNumberAmendment = request.PartNumberAmendment;
			memcpy(data.SerialNumber, request.SerialNumber, sizeof(data.SerialNumber));
			memcpy(data.HardwareVersion, request.HardwareVersion, sizeof(data.HardwareVersion));
			coreNSD_writeGAINConfigurationDataBlock(5, &data);
			coreNSD_flushGAINConfigurationDataBlock(5);
			}
		}
	else
		{
		uint8	i;
		char	ch;
		
		for (i=0; i<header.dataSize; i++)
			io >> ch;
		}

	return result;
	}
*/
#endif


/*****

	GAIN queries

*****/
bool MaintenanceTool::handleGAINQuery(stream& io)
	{
    GAINMessageHeader	header;
	uint16	checksum;
	uint16	checksumProvided;
	bool	result(true);

	io.readData(&header, sizeof(GAINMessageHeader), 1000);
	io.readData(&checksumProvided, sizeof(checksumProvided), 100);
   	
   	checksum = calculateChecksum(header);
   	result = (checksum == checksumProvided);

	notifyGAINQueryReceived(io, header, result);

	if (result)
		{
		switch (header.command)
			{
		#if (kMaintenanceProtocolVersion > 0)
			case kGAINQueryProtocolInfo:
				result = handleGAINQueryProtocolInfo(io, header);
				break;
		#endif
			case kGAINQueryInfo:
				result = handleGAINQueryInfo(io, header);
				break;

			case kGAINQueryConfiguration:
				result = handleGAINQueryConfiguration(io, header);
				break;

			case kGAINQueryCycleDictionary:
				result = handleGAINQueryCycleDictionary(io, header);
				break;

			case kGAINQueryCycleDescription:
				result = handleGAINQueryCycleDescription(io, header);
				break;

			case kGAINQueryFaultLog:
				result = handleGAINQueryFaultLog(io, header);
				break;

			default:
				result = false;
				break;
			}
		}

	return result;
	}

void MaintenanceTool::notifyGAINQueryReceived(stream& io, GAINMessageHeader& header, bool result)
	{
    GAINMessageHeader	reply;
	uint16	checksum;

	reply.selector = header.command;
	reply.command = kGAINNotifyQueryReceived;
	reply.dataSize = 0;
	if (!result)
		reply.selector |= 0x80;
	
	checksum = calculateChecksum(reply);
	io.writeData(&reply, sizeof(reply));
	io.writeData(&checksum, sizeof(checksum));
	}

bool MaintenanceTool::handleGAINQueryInfo(stream& io, GAINMessageHeader& header)
	{
	connectRequestPayload	connect;
	uint16	checksum;
	bool	result(true);

	header.command = kGAINQueryInfo;
	header.selector = kMaintenanceProtocolVersion;
	header.dataSize = sizeof(connectRequestPayload);

#if PARTNUMBER == 0xBABE
    connect.partNumber = 0xBABE;
    connect.commons.subPartNumber = 0;
#else
    connect.partNumber = GAIN.configuration.commons.partNumber;
    connect.subPartNumber = GAIN.configuration.commons.subPartNumber;
#endif
#ifdef NSD_SUPPORT
	strncpy(connect.partNumberString, configurationData.block05.PartNumber, sizeof(configurationData.block05.PartNumber));
	connect.partNumberString[15] = 0;
	strncpy(connect.serialNumberString, configurationData.block05.SerialNumber, sizeof(configurationData.block05.SerialNumber));
	connect.serialNumberString[12] = 0;
	strncpy(connect.hardwareVersionString, configurationData.block05.HardwareVersion, sizeof(configurationData.block05.HardwareVersion));
	connect.hardwareVersionString[7] = 0;
	connect.powerBudget = configurationData.block03.MaximumPowerBudget * 50;	
	connect.standbyPowerBudget = configurationData.block03.StandbyPowerThreshold * 5;
#else
	connect.partNumberString[0] = 0;
	connect.serialNumberString[0] = 0;
	connect.hardwareVersionString[0] = 0;
	connect.powerBudget = 0;
	connect.standbyPowerBudget = 0;
#endif
	strncpy(connect.softwareVersionString, versionString, sizeof(connect.softwareVersionString));

	checksum = calculateChecksum(header);	
	checksum = calculateChecksum(checksum, header.dataSize, &connect);	

	io.writeData(&header, sizeof(GAINMessageHeader));
	io.writeData(&connect, sizeof(connectRequestPayload));
	io.writeData(&checksum, sizeof(checksum));

	return result;
	}

#if (kMaintenanceProtocolVersion > 0)
bool MaintenanceTool::handleGAINQueryProtocolInfo(stream& io, GAINMessageHeader& header)
	{
	protocolInfoRequestPayload	protocolInfo;
	uint16	checksum;
	bool	result(true);

	header.command = kGAINQueryProtocolInfo;
	header.selector = 0;
	header.dataSize = sizeof(protocolInfoRequestPayload);
	memset(&protocolInfo, 0, sizeof(protocolInfo));
	protocolInfo.version = kMaintenanceProtocolVersion;

	checksum = calculateChecksum(header);	
	checksum = calculateChecksum(checksum, header.dataSize, &version);	

	io.writeData(&header, sizeof(GAINMessageHeader));
	io.writeData(&connect, sizeof(protocolInfoRequestPayload));
	io.writeData(&checksum, sizeof(checksum));

	return result;
	}
#endif
	
bool MaintenanceTool::handleGAINQueryConfiguration(stream& io, GAINMessageHeader& header)
	{
	uint16	checksum;
	bool	result(true);

	header.command = kGAINQueryConfiguration;
	
	if (header.selector == 0)		//	Factory defaults configuration
		{
		header.dataSize = sizeof(ConfigurationData);
		checksum = calculateChecksum(header);	
	#ifdef AVR
		ConfigurationData	configuration;

		memcpy_P(&configuration, &factoryDefaultsConfiguration, sizeof(ConfigurationData));
		checksum = calculateChecksum(checksum, header.dataSize, &configuration);	
		
		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&configuration, sizeof(ConfigurationData));
	#else
		checksum = calculateChecksum(checksum, header.dataSize, &factoryDefaultsConfiguration);	
		
		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&factoryDefaultsConfiguration, sizeof(ConfigurationData));
	#endif
		io.writeData(&checksum, sizeof(checksum));
		}
	else if (header.selector == 1)	//	Factory defaults cycle descriptions
		{
		header.dataSize = sizeof(CycleDictionary) + (kNumberOfCycleDescriptions * sizeof(CycleDescription));
		checksum = calculateChecksum(header);	
	#ifdef AVR
		uint8	i;
		
		checksum = calculateChecksum(checksum, sizeof(CycleDictionary), &GAIN.cycleDictionary);	

		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&GAIN.cycleDictionary, sizeof(CycleDictionary));
		
		for (i=0; i<kNumberOfCycleDescriptions; i++)
			{
			CycleDescription	cycle;
				
			memcpy_P(&cycle, &factoryDefaultsCycles.cycles[i], sizeof(CycleDescription));
			checksum = calculateChecksum(checksum, sizeof(CycleDescription), &cycle);	
			io.writeData(&cycle, sizeof(CycleDescription));
			}
	#else
		checksum = calculateChecksum(checksum, header.dataSize, &factoryDefaultsCycles);	
		
		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&factoryDefaultsCycles, sizeof(CycleData));
	#endif
		io.writeData(&checksum, sizeof(checksum));
		}
	else if (header.selector == 2)	//	Current configuration
		{
		header.dataSize = sizeof(ConfigurationData);
		checksum = calculateChecksum(header);	
		checksum = calculateChecksum(checksum, header.dataSize, &GAIN.configuration);	
		
		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&GAIN.configuration, sizeof(ConfigurationData));
		io.writeData(&checksum, sizeof(checksum));
		}
	else if (header.selector == 3)	//	Current cycle descriptions
		{
		uint8	i;

		header.dataSize = sizeof(CycleDictionary) + (kNumberOfCycleDescriptions * sizeof(CycleDescription));
		checksum = calculateChecksum(header);	
		checksum = calculateChecksum(checksum, sizeof(CycleDictionary), &GAIN.cycleDictionary);	

		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&GAIN.cycleDictionary, sizeof(CycleDictionary));
		
		for (i=0; i<kNumberOfCycleDescriptions; i++)
			{
			CycleDescription	cycle;
			
			persistent.readCycleDescription(cycle, i);
			checksum = calculateChecksum(checksum, sizeof(CycleDescription), &cycle);	
			io.writeData(&cycle, sizeof(CycleDescription));
			}

		io.writeData(&checksum, sizeof(checksum));
		}

	return result;
	}

bool MaintenanceTool::handleGAINQueryCycleDictionary(stream& io, GAINMessageHeader& header)
	{
	bool	result(true);
/*
	uint16	checksum;

	header.command = kGAINQueryCycleDictionary;
	header.selector = 0;
	header.dataSize = sizeof(CycleDictionary);

	checksum = calculateChecksum(header);	
	checksum = calculateChecksum(checksum, header.dataSize, &GAIN.cycleDictionary);	
		
	io.writeData(&header, sizeof(GAINMessageHeader));
	io.writeData(&GAIN.cycleDictionary, header.dataSize);
	io.writeData(&checksum, sizeof(checksum));
*/
	return result;
	}

bool MaintenanceTool::handleGAINQueryCycleDescription(stream& io, GAINMessageHeader& header)
	{
	return true;
/*
	bool	result;
	uint16	checksum;
	CycleDescription	cycle;
	
	result = (header.selector < GAIN.cycleDictionary.numberOfCycleDescriptions);
	
	if (result)
		result = persistent.readCycleDescription(cycle, header.selector);
		
	if (result)
		{
		header.dataSize = sizeof(CycleDescription);

		checksum = calculateChecksum(header);	
		checksum = calculateChecksum(checksum, header.dataSize, &cycle);	

		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&cycle, sizeof(CycleDescription));
		io.writeData(&checksum, sizeof(checksum));
		}

	return result;
*/
	}

#if (kMaintenanceProtocolVersion == 0)
bool MaintenanceTool::handleGAINQueryFaultLog(stream& io, GAINMessageHeader& header)
	{
	int16			i;
	int16			n;
	faultLogHeader	faultHeader;	
	uint16	checksum;
	bool	result(true);

#ifdef NO_PERSISTENCE
	faultHeader.numberOfFaults = (kGAINErrorCount - 1);//0x80;
	faultHeader.currentFaultIndex = kGAINErrorCount;
	faultHeader.error.errorID = 1;
	faultHeader.error.annotation = 0;
	faultHeader.error.timestamp = 0;
	faultHeader.error.totalUpTime = 0;
#else
	faultHeader.numberOfFaults = persistent.getErrorCount();
	faultHeader.currentFaultIndex = persistent.getCurrentErrorIndex();
	persistent.getError(-1, faultHeader.error);
#endif

	if (faultHeader.numberOfFaults > kPersistentErrorAreaSize)
		faultHeader.numberOfFaults = kPersistentErrorAreaSize;

	n = faultHeader.numberOfFaults;

	header.command = kGAINQueryFaultLog;
	header.dataSize = sizeof(faultLogHeader);
	checksum = calculateChecksum(header);
	checksum = calculateChecksum(checksum, sizeof(faultLogHeader), &faultHeader);

	io.writeData(&header, sizeof(header));
	io.writeData(&faultHeader, sizeof(faultHeader));
	io.writeData(&checksum, sizeof(checksum));

	header.command = kGAINQueryFaultLogEntry;
	header.selector = 0;

	for (i=0; i<n; i++)
		{
	#ifdef NO_PERSISTENCE
		errorDescription	description;
	
		memcpy_P(&description, &GAINErrorDescription[i + 1], sizeof(description));

		faultHeader.currentFaultIndex = i;
		faultHeader.error.errorID = description.errorID;
		faultHeader.error.annotation = 0;
		faultHeader.error.timestamp = (tick_t)i*1000;
		faultHeader.error.totalUpTime = (uint32)i*250;
	#else
		persistent.getError(i, faultHeader.error);
	#endif
		header.dataSize = sizeof(errorLogEntry);

		checksum = calculateChecksum(header);	
		checksum = calculateChecksum(checksum, header.dataSize, &faultHeader.error);	

		io.writeData(&header, sizeof(GAINMessageHeader));
		io.writeData(&faultHeader.error, header.dataSize);
		io.writeData(&checksum, sizeof(checksum));
		}

	return result;
	}

#else

bool MaintenanceTool::handleGAINQueryFaultLog(stream& io, GAINMessageHeader& header)
	{
	int16			i;
	int16			n;
	faultLogHeader	faultHeader;	
	uint16	checksum;
	bool	result(true);

#ifdef NO_PERSISTENCE
	faultHeader.numberOfFaults = kGAINErrorCount;//0x80;
	faultHeader.currentFaultIndex = kGAINErrorCount;
	faultHeader.error.errorID = 1;
	faultHeader.error.annotation = 0;
	faultHeader.error.timestamp = 0;
	faultHeader.error.totalUpTime = 0;
#else
	faultHeader.numberOfFaults = persistent.getErrorCount();
	faultHeader.currentFaultIndex = persistent.getCurrentErrorIndex();
	persistent.getError(-1, faultHeader.error);
	persistent.getErrorCounter(faultHeader.errorCounter);
#endif
	if (faultHeader.numberOfFaults <= kPersistentErrorAreaSize)
		n = faultHeader.numberOfFaults;
	else
		n = kPersistentErrorAreaSize;

	header.command = kGAINQueryFaultLog;
	header.dataSize = sizeof(faultLogHeader);
	checksum = calculateChecksum(header);
	checksum = calculateChecksum(checksum, sizeof(faultLogHeader), &faultHeader);

	io.writeData(&header, sizeof(header));
	io.writeData(&faultHeader, sizeof(faultHeader));
	io.writeData(&checksum, sizeof(checksum));

	return result;
	}

bool MaintenanceTool::handleGAINQueryFaultLogEntry(stream& io, GAINMessageHeader& header, int16 id)
	{
	faultLogHeader	faultHeader;	
	uint16	checksum;
	bool	result(true);

	header.command = kGAINQueryFaultLogEntry;
	header.selector = 0;

#ifdef NO_PERSISTENCE
	errorDescription	description;
	
	memcpy_P(&description, &GAINErrorDescription[id], sizeof(description));

	faultHeader.currentFaultIndex = id;
	faultHeader.error.errorID = description.errorID;
	faultHeader.error.annotation = 0;
	faultHeader.error.timestamp = (tick_t)id*1000;
	faultHeader.error.totalUpTime = (uint32)id*250;
#else
	persistent.getError(id, faultHeader.error);
#endif
	header.dataSize = sizeof(errorLogEntry);

	checksum = calculateChecksum(header);	
	checksum = calculateChecksum(checksum, header.dataSize, &faultHeader.error);	

	io.writeData(&header, sizeof(GAINMessageHeader));
	io.writeData(&faultHeader.error, header.dataSize);
	io.writeData(&checksum, sizeof(checksum));

	return result;
	}

#endif

/*	
#ifdef NSD_SUPPORT
bool MaintenanceTool::handleGAINQueryNSDDataBlock(stream& io, GAINMessageHeader& header)
	{
	bool	result(true);
	
	return result;
	}
#endif
*/
uint16 MaintenanceTool::calculateChecksum(const GAINMessageHeader& header)
	{
	uint16	checksum(0);
	
	checksum = calculateChecksum(0, sizeof(header), &header);

	return checksum;
	}

uint16 MaintenanceTool::calculateChecksum(uint16 checksum, uint16 size, const void* data)
	{
	uint16	i;
	
	for (i=0; i<(size/2); i++)
		checksum ^= ((uint16*)data)[i];
	
	if (size & 0x01)
		checksum ^= (uint16)((uint8*)data)[size - 1];

	return checksum;
	}


