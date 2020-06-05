/*--------------------------------------------------------------------------

ControlPanel.cc

Implementation
Ansteuerung Control Panel auf Protokollebene

Autor: Steffen Simon

$Date: 2018-06-27 15:54:20 +0200 (Mi, 27. Jun 2018) $
$Revision: 7962 $
$Author: steffen $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/ControlPanel/ControlPanel.cc $

--------------------------------------------------------------------------*/

#include "version.h"
#include "coreI2C.h"
#include "eDeviceEvent.h"
#include "GAINClass.h"
#ifdef NSD_SUPPORT
#include "NSDHandler.h"
#endif
#include "ControlPanel.h"
#if (PARTNUMBER == 917)
#include "PersistenceServices.h"
#endif
#ifdef AVR
#include <avr/pgmspace.h>
#endif
#include <string.h>
#ifdef __DEBUG__
#include "eDevice.h"
#endif

#ifndef AVR
#pragma pack(2)
#endif

#if (PARTNUMBER == 932)
#define UPVERSION 1
#endif

#if (PARTNUMBER == 917)
typedef struct
	{
	uint8	trayCode;
	uint8	numberOfHeatingPrograms;
	uint8	keepHotProgram;
	uint8	reserved;
	} trayChangedRequestPayload;

typedef struct
	{
	uint8	heatingProgram;
	uint8	flags;
	heatingProgramSummary	summary;
	} heatingProgramChangedRequestPayload;

typedef struct
	{
	uint16	remaining[4];
	uint16	progress;
	} cateringProgressRequestPayload;

typedef userGuidanceStep	userGuidanceStepPayload;

typedef userGuidanceHeader	userGuidanceHeaderPayload;
#elif (CP_VERSION > 0)
typedef struct
	{
	uint32	remaining;
	uint32	flags;
	} cateringProgressRequestPayload;

typedef struct
	{
	uint32	cateringTime;
	uint32	maximumTime;
	uint32	flags;
	} cateringTimeRequestPayload;

typedef struct
	{
	LRUCycles	lru;
	} lruCyclesRequestPayload;
#else
typedef struct
	{
	uint16	remaining;
	uint8	progress;
	uint8	flags;	
	} cateringProgressRequestPayload;
#endif
#if (PARTNUMBER == 916)
typedef struct
	{
	uint16	power;
	uint16	percentage;
	} powerLevelRequestPayload;
#elif (PARTNUMBER == 932)
typedef struct
	{
	uint16	power;
	uint16	percentage;
	} powerLevelRequestPayload;
#else
typedef struct
	{
	int16	targetTemperature;
	int16	currentTemperature;
	} temperatureRequestPayload;
#endif

#if CP_VERSION > 0
typedef struct
	{
	uint16	partNumber;
	uint16	subPartNumber;
	char	partNumberString[16];
	char	serialNumberString[16];
	char	softwareVersionString[16];
	uint16	version;
	uint32	flags;
	char	reserved[18];
	} connectRequestPayload;
#else
typedef struct
	{
	uint16	partNumber;
	uint16	subPartNumber;
	char	partNumberString[16];
	char	serialNumberString[16];
	char	softwareVersionString[16];
	uint8	temperatureUnit;
	uint8	displayBrightness;
	char	reserved[22];
	} connectRequestPayload;
#endif

#if CP_VERSION > 0
typedef struct
	{
	uint8	state;
	uint8	reserved[3];
	uint32	flags;
	} stateChangedRequestPayload;
#else
typedef struct
	{
	uint8	state;
	uint8	flags;
	} stateChangedRequestPayload;
#endif

#if CP_VERSION > 0
typedef struct
	{
	uint8	id;
	uint8	reserved[3];
	uint32	flags;
	CycleDescription	cycle;
	} cycleDescriptionRequestPayload;

typedef struct
	{
	uint8	mode;
	uint8	reserved[3];
	uint32	flags;
	} modeChangedRequestPayload;
#else
typedef struct
	{
	uint8	id;
#if (PARTNUMBER == 916)||(PARTNUMBER == 917)
	uint8	reserved;
#endif
	CycleDescription	cycle;
	} cycleDescriptionRequestPayload;

typedef struct
	{
	uint8	mode;
	uint8	flags;
	} modeChangedRequestPayload;
#endif

#ifdef NSD_SUPPORT
typedef struct
	{
	uint8	deviceID;
	uint8	feederID;
	uint8	deviceFlags;
	uint8	networkState;
	} networkRequestPayload;
#endif

typedef struct
	{
    panelRequestHeader	header;

	union
		{
		connectRequestPayload				connect;
		stateChangedRequestPayload			stateChanged;
	#if (PARTNUMBER == 917)
		trayChangedRequestPayload			trayChanged;
		heatingProgramChangedRequestPayload	heatingProgram;
		cateringProgressRequestPayload		cateringProgress;
		userGuidanceHeaderPayload			userGuidanceHeader;
		userGuidanceStepPayload				userGuidanceStep;
	#else
		cateringProgressRequestPayload		cateringProgress;
		cycleDescriptionRequestPayload		modeDescription;
		modeChangedRequestPayload			modeChanged;
	#if  (PARTNUMBER == 916)
		powerLevelRequestPayload			powerLevel;
	#elif  (PARTNUMBER == 932)
		cateringTimeRequestPayload			cateringTime;
		powerLevelRequestPayload			powerLevel;
		lruCyclesRequestPayload				lruCycles;
	#elif (PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
		temperatureRequestPayload			temperatureState;
	#endif
	#endif
	#ifdef NSD_SUPPORT
		networkRequestPayload				networkState;
	#endif
		};
	} panelRequestMessage;


typedef struct
	{
	uint8	eventType;
	uint8	eventData[3];
	} panelEvent;
	
typedef struct
	{
	uint8		panelState;
	uint8		GAINState;
	panelEvent	event;
	uint8		eventCount;
	uint8		crc8;
	} panelReplyMessage;
#ifndef AVR
#pragma pack()
#endif

#ifdef AVR
#define I2C_BUFFER_ON_HEAP
#ifdef I2C_BUFFER_ON_HEAP
extern unsigned char __heap_start;
static uint8*	receiveBuffer = &__heap_start;
#else
#ifdef __DEBUG__
static uint8	receiveBuffer[0];
#else
static uint8	receiveBuffer[264];
#endif
#endif
#else
static uint8	receiveBuffer[264];
#endif

//	static uint8*	receiveBuffer(&memoryPool.receiveBuffer);

static uint8	I2CRead(uint8 n, uint8* data);

ControlPanel::ControlPanel(void)
	:lastMessageSize(0), lastMessageSent(0), retryCounter(0), disconnectCounter(0), panelHasFirmware(false), panelIsConnected(false), panelGAINStateIsSynched(false)
	{
	}

void ControlPanel::setPowerSupplyState(bool on)
	{
#if (__DEBUG__ > 3)
	controller.asc0 << "setControlPanelPowerSupplyState: " << (on?"ON":"OFF") << " (state is " << (getPowerSupplyState()?"ON":"OFF") << ')' << newline;
#endif
#ifdef AVR
#if IPECO_PSC_Revision == 0x0103
	controller.switchPin(kControlPanelPowerSupplyPin, on);
#endif
#else
	controller.switchDigitalPowerPin((controllerPortPin)kControlPanelPowerSupplyPin, on);
#endif
	}

bool ControlPanel::getPowerSupplyState(void) const
	{
#ifdef AVR
#if IPECO_PSC_Revision == 0x0103
	return controller.getPin(kControlPanelPowerSupplyPin);
#else
	return false;
#endif
#else
	return controller.getDigitalPowerPin(kControlPanelPowerSupplyPin);
#endif
	}

#if CP_VERSION > 0
bool ControlPanel::syncWithGAINInfo(uint16 partNumber, uint16 subPartNumber, uint32 flags)
	{
#if __DEBUG__ > 0
	controller.asc0 << "connect.partNumber:    " << partNumber << newline;
	controller.asc0 << "connect.subPartNumber: " << subPartNumber << newline;
	controller.asc0 << "connect.flags: " << kModeHex << flags << newline;
#endif
#else
bool ControlPanel::syncWithGAINInfo(uint16 partNumber, uint16 subPartNumber)
	{
#if __DEBUG__ > 0
	controller.asc0 << "connect.partNumber:    " << partNumber << newline;
	controller.asc0 << "connect.subPartNumber: " << subPartNumber << newline;
#endif
#endif
	panelRequestMessage	panelRequest;
	
	// Zero initial connect message
	memset(&panelRequest, 0, sizeof(panelRequest));

	panelRequest.header.command = kHMIRequestConnect;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(connectRequestPayload);
	panelRequest.connect.partNumber = partNumber;
	panelRequest.connect.subPartNumber = subPartNumber;
	strncpy(panelRequest.connect.softwareVersionString, versionString, sizeof(panelRequest.connect.softwareVersionString));

#ifdef NSD_SUPPORT
	memset(panelRequest.connect.partNumberString, 0, sizeof(panelRequest.connect.partNumberString));
	strncpy(panelRequest.connect.partNumberString, configurationData.block05.PartNumber, sizeof(configurationData.block05.PartNumber));
#elif (PARTNUMBER == 917)
	strncpy(panelRequest.connect.partNumberString, GAIN.hardware.partNumber, sizeof(panelRequest.connect.partNumberString));
#else
	panelRequest.connect.partNumberString[0] = 0;
#endif
#ifdef NSD_SUPPORT
	memset(panelRequest.connect.serialNumberString, 0, sizeof(panelRequest.connect.serialNumberString));
	strncpy(panelRequest.connect.serialNumberString, configurationData.block05.SerialNumber, sizeof(configurationData.block05.SerialNumber));
#elif (PARTNUMBER == 917)
	strncpy(panelRequest.connect.serialNumberString, GAIN.hardware.serialNumber, sizeof(panelRequest.connect.serialNumberString));
#else
	panelRequest.connect.serialNumberString[0] = 0;
#endif

#if (PARTNUMBER == 920)||(PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
	panelRequest.connect.displayBrightness = GAIN.configuration.commons.displayBrighness;
	panelRequest.connect.temperatureUnit = GAIN.configuration.commons.temperatureUnit;
#endif

#if __DEBUG__ > 0
	controller.asc0.lock();
	controller.asc0 << "connect.partNumberString:            " << panelRequest.connect.partNumberString << newline;
	controller.asc0 << "connect.serialNumberString:          " << panelRequest.connect.serialNumberString << newline;
	controller.asc0 << "connect.softwareVersionString:       " << panelRequest.connect.softwareVersionString << newline;
	controller.asc0.unlock();
#endif
	
#if CP_VERSION > 0
	panelRequest.connect.version = CP_VERSION;
	panelRequest.connect.flags = flags;
#endif
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 50);

	return panelIsConnected;
	}

#if CP_VERSION > 0
bool ControlPanel::syncWithState(uint8 state, uint32 flags)
	{
#if (__DEBUG__ > 3)
	controller.asc0 << "syncWithState: state == "
					<< state << "("
					<< gainStateDescription[state] << ")"
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
#else
bool ControlPanel::syncWithState(uint8 state, uint16 flags)
	{
#if (__DEBUG__ > 3)
#ifdef AVR
	controller.asc0 << "syncWithState: state == ";
	controller.asc0.writeP(gainStateDescription[state]);
	controller.asc0 << ", flags == " << kModeHex << flags << newline;
#else
	controller.asc0 << "syncWithState: state == " << gainStateDescription[state]
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
#endif
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestStateChanged;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(stateChangedRequestPayload);
	panelRequest.stateChanged.state = state;
	panelRequest.stateChanged.flags = flags;
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);
	
	if (panelIsConnected)
		{
		panelGAINStateIsSynched = false;
		savedGAINState = state;
		}

	return panelIsConnected;
	}

bool ControlPanel::syncWithCateringState(uint8 state, uint8 flags)
	{
#if (__DEBUG__ > 3)
	controller.asc0 << "syncWithCateringState: state == " << state
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
	panelRequestMessage	panelRequest;

	panelRequest.header.command = kHMIRequestCateringStateChanged;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(stateChangedRequestPayload);
	panelRequest.stateChanged.state = state;
	panelRequest.stateChanged.flags = flags;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}

bool ControlPanel::syncWithNextMaintenanceTime(int16 time, uint8 flags)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithNextMaintenanceTime: time == " << time
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
	panelRequestMessage	panelRequest;

	panelRequest.header.command = kHMIRequestNextMaintenanceTimeChanged;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(cateringProgressRequestPayload);

	panelRequest.cateringProgress.remaining = time;
#if (PARTNUMBER != 932)
	panelRequest.cateringProgress.progress = 0;
#endif
	panelRequest.cateringProgress.flags = flags;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}

#if CP_VERSION > 0
bool ControlPanel::syncWithCycleDescription(uint8 id, const CycleDescription& cycle, uint32 flags)
	{
#if __DEBUG__ > 2
#if (PARTNUMBER == 932)
	controller.asc0 << "syncWithCycleDescription, id == " << id << ", description == " << cycle.description
					<< ", power == " << cycle.power
					<< ", duration == " << cycle.duration << ", maximum time == " << cycle.maximumDuration
					<< ", flags == " << cycle.cycleFlags << newline;
#else
	controller.asc0 << "syncWithCycleDescription, id == " << id << ", description == " << cycle.description
					<< ", flags == " << cycle.cycleFlags << newline;
#endif
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestModeDescription;
	panelRequest.header.target = 0;

	panelRequest.header.size = sizeof(cycleDescriptionRequestPayload);
	panelRequest.modeDescription.id = id;
	panelRequest.modeDescription.cycle = cycle;
	panelRequest.modeDescription.flags = flags;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, -1);

	return panelIsConnected;
	}

bool ControlPanel::syncWithCycleDescriptionID(uint8 id, uint32 flags)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithCycleDescriptionID, id == " << id
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestCateringMode;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(modeChangedRequestPayload);
	panelRequest.modeChanged.mode = id;
	panelRequest.modeChanged.flags = flags;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}

bool ControlPanel::syncWithLRUCycles(LRUCycles& lru, uint32 flags)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithLRUCycles, #entries == " << lru.numberOfEntries
					<< ", power == "  << lru.manualPower
					<< ", duration == " << lru.manualDuration
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestLRUCycles;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(lruCyclesRequestPayload);
	panelRequest.lruCycles.lru = lru;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}
#else
#if (PARTNUMBER == 916)||(PARTNUMBER == 920)||(PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
bool ControlPanel::syncWithCycleDescription(uint8 id, const CycleDescription& cycle)
	{
#if __DEBUG__ > 2
#if (PARTNUMBER == 916)
	controller.asc0 << "syncWithCycleDescription, id == " << id << ", description == " << cycle.description
					 << ", flags == " << cycle.cycleFlags
					 << ", steps == " << cycle.numberOfSteps
					<< ", time == " << cycle.cycleDuration<< ", minimum time == " << cycle.minimumCycleDuration << ", maximum time == " << cycle.maximumCycleDuration
					<< ", power == " << cycle.maximumPower << newline;
					for (int8 i=0; i<cycle.numberOfSteps; i++)
						{
						controller.asc0 << "        step #" << i << ": flags == "  << cycle.cycleSteps[i].stepFlags << ", time == " << cycle.cycleSteps[i].stepDuration
										<< ", minimum time == " << cycle.cycleSteps[i].minimumStepDuration
										<< ", maximum time == " << cycle.cycleSteps[i].maximumStepDuration
										<< ", power == " << cycle.cycleSteps[i].targetPower << newline;
						}
#elif (PARTNUMBER == 920)
	controller.asc0 << "syncWithCycleDescription, id == " << id << ", description == " << cycle.description << ", flags == " << cycle.cycleFlags
					<< ", volume == " << cycle.targetVolume << ", temperature == " << cycle.targetTemperature << newline;
#elif (PARTNUMBER == 922)
	controller.asc0 << "syncWithCycleDescription, id == " << id << ", description == " << cycle.description << ", flags == " << cycle.cycleFlags
					<< ", time == " << cycle.cycleDuration << ", temperature == " << cycle.targetTemperature << newline;
#else
	controller.asc0 << "syncWithCycleDescription, id == " << id << ", description == " << cycle.description << ", flags == " << cycle.cycleFlags
					<< ", temperature == " << cycle.targetTemperature << newline;
#endif
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestModeDescription;
	panelRequest.header.target = 0;

#if (PARTNUMBER == 916)
	panelRequest.header.size = sizeof(cycleDescriptionRequestPayload) - sizeof(cycle.cycleSteps);
#else
	panelRequest.header.size = sizeof(cycleDescriptionRequestPayload);
#endif
	panelRequest.modeDescription.id = id;
	panelRequest.modeDescription.cycle = cycle;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, -1);

	return panelIsConnected;
	}
#endif

bool ControlPanel::syncWithCycleDescriptionID(uint8 id, uint8 flags)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithCycleDescriptionID, id == " << id << ", flags == " << flags << newline;
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestCateringMode;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(modeChangedRequestPayload);
	panelRequest.modeChanged.mode = id;
	panelRequest.modeChanged.flags = flags;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}
#endif

#if (PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
bool ControlPanel::syncWithTemperature(int16 target, int16 current)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithTemperature: target == " << target << ", current == " << current << newline;
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestTemperature;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(temperatureRequestPayload);
	panelRequest.temperatureState.targetTemperature = target;
	panelRequest.temperatureState.currentTemperature = current;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}
#endif

#if (PARTNUMBER == 916)||(PARTNUMBER == 932)||(PARTNUMBER == 921)||(PARTNUMBER == 922)||(PARTNUMBER == 928)
bool ControlPanel::syncWithDelayTime(int16 delay, uint8 flags)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithDelayTime: " << delay << newline;
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestSetDelayTime;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(cateringProgressRequestPayload);

	panelRequest.cateringProgress.remaining = delay;
#if (PARTNUMBER != 932)
	panelRequest.cateringProgress.progress = 0;
#endif
	panelRequest.cateringProgress.flags = flags;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}

#endif


#if (PARTNUMBER == 920)
bool ControlPanel::syncWithProgress(uint8 progress, uint8 flags)
	{
	if (progress > 99)
		progress = 99;

	if (progress != progressSave)
		{
	#if __DEBUG__ > 4
		controller.asc0 << "syncWithProgress: " << progress << ", " << flags << newline;
	#endif
		panelRequestMessage	panelRequest;

		memset(&panelRequest, 0, sizeof(panelRequest));
		progressSave = progress;
	
		panelRequest.header.command = kHMIRequestCateringProgress;
		panelRequest.header.target = 0;
		panelRequest.header.size = sizeof(cateringProgressRequestPayload);

		panelRequest.cateringProgress.remaining = 0;
		panelRequest.cateringProgress.progress = progress;
		panelRequest.cateringProgress.flags = flags;
	
		panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);
		}

	return panelIsConnected;
	}
#endif
#if CP_VERSION > 0
bool ControlPanel::syncWithRemainingTime(int16 remaining, uint32 flags)
	{
#if __DEBUG__ > 4
	controller.asc0 << "syncWithRemainingTime: " << remaining
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestCateringProgress;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(cateringProgressRequestPayload);

	panelRequest.cateringProgress.remaining = remaining;
	panelRequest.cateringProgress.flags = flags;
		
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}

bool ControlPanel::syncWithRemainingTime(tick_t remaining, uint32 flags)
	{
	bool	result;
	int16	remainingSeconds;

	remainingSeconds = (int16)((remaining + 999)/1000);
   	remainingSecondsSave = remainingSeconds;
	result = syncWithRemainingTime(remainingSeconds, flags);
/*
    tick_t	offset;
    
    offset = (remainingMillisecondsSave - remaining);

	if (offset > 500)
		{
	   	remainingMillisecondsSave = remaining;
		result = syncWithRemainingTime((int16)((remaining + 999)/1000), flags);
		}
	else if (offset < 0)
		{
 		result = syncWithRemainingTime((int16)((remaining + 999)/1000), flags);
		}
	else
		result = true;
*/

/*	
	int16	remainingSeconds;

	remainingSeconds = (int16)((remaining + 999)/1000);
	if (remainingSeconds != remainingSecondsSave)
		{
    	remainingSecondsSave = remainingSeconds;
		result = syncWithRemainingTime(remainingSeconds, flags);
		}
	else
		result = true;
*/	
	return result;
	}

bool ControlPanel::syncWithCateringTime(tick_t cateringTime, tick_t maximumTime, uint32 flags)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithCateringTime: " << cateringTime
					<< ", " << "maximumTime: " << maximumTime
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
	panelRequestMessage	panelRequest;

	memset(&panelRequest, 0, sizeof(panelRequest));
	panelRequest.header.command = kHMIRequestCateringTime;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(cateringTimeRequestPayload);

	panelRequest.cateringTime.cateringTime = cateringTime;
	panelRequest.cateringTime.maximumTime = maximumTime;
	panelRequest.cateringTime.flags = flags;
		
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}
#elif (PARTNUMBER == 922)||(PARTNUMBER == 916)
bool ControlPanel::syncWithRemainingTime(int16 remaining, uint8 flags)
	{
	panelRequestMessage	panelRequest;

#if __DEBUG__ > 2
	controller.asc0 << "syncWithRemainingTime: " << remaining
					<< ", flags == " << kModeHex << flags
					<< newline;
#endif
	panelRequest.header.command = kHMIRequestCateringProgress;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(cateringProgressRequestPayload);

	panelRequest.cateringProgress.remaining = remaining;
	panelRequest.cateringProgress.progress = 0;
	panelRequest.cateringProgress.flags = flags;
		
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}

bool ControlPanel::syncWithRemainingTime(tick_t remaining, uint8 flags)
	{
/*
	int16	remainingSeconds;
	bool	result;
	
	remainingSeconds = (int16)((remaining + 999)/1000);
   	remainingSecondsSave = remainingSeconds;
	result = syncWithRemainingTime(remainingSeconds, flags);
*/	
	int16	remainingSeconds;
	bool	result;
	
	remainingSeconds = (int16)((remaining + 999)/1000);
	if (remainingSeconds != remainingSecondsSave)
		{
    	remainingSecondsSave = remainingSeconds;
		result = syncWithRemainingTime(remainingSeconds, flags);
		}
	else
		result = true;

	return result;
	}
#endif

#if ((PARTNUMBER == 916)||(PARTNUMBER == 932))
bool ControlPanel::syncWithPowerLevel(uint16 power, uint16 percentage)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncPanelWithPowerLevel: power == " << power << ", percentage == " << percentage << newline;
#endif
	panelRequestMessage	panelRequest;
/*
	int16	n;
	int16	size(sizeof(panelRequestHeader) + sizeof(powerLevelRequestPayload));
	
    panelIsConnected = true;
*/	
	panelRequest.header.command = kHMIRequestPowerLevel;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(powerLevelRequestPayload);

	panelRequest.powerLevel.power = power;
	panelRequest.powerLevel.percentage = percentage;
	
		
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}
#endif


#ifdef NSD_SUPPORT
bool ControlPanel::syncWithNetworkState(void)
	{
#if __DEBUG__ > 2
	controller.asc0 << "syncWithNetworkState" << newline;
#endif
	panelRequestMessage	panelRequest;

	panelRequest.header.command = kHMIRequestNetworkState;
	panelRequest.header.target = 0;
	panelRequest.header.size = sizeof(networkRequestPayload);
	panelRequest.networkState.deviceID = NSD.getGAINDeviceID();
	panelRequest.networkState.feederID = NSD.getGAINFeederID();
	panelRequest.networkState.deviceFlags = (NSD.getGAINIsPrimary()?1:0);
	panelRequest.networkState.networkState = 0;
	
	if (NSD.MGCUIsPresent())
		panelRequest.networkState.networkState |= (1 << 0);
	
	if (NSD.CentralizedPowerControlAvailable())
		panelRequest.networkState.networkState |= (1 << 1);
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}
#endif

bool ControlPanel::syncWithFirmwareUpdate(void)
	{
	panelRequestMessage	panelRequest;

	panelRequest.header.command = kHMIRequestFirmwareUpdate;
	panelRequest.header.target = 0;
	panelRequest.header.size = 0;
	
	panelIsConnected = sendRequest(panelRequest.header, &panelRequest, 10);

	return panelIsConnected;
	}

/*
bool ControlPanel::handlePeriodicTask(void)
	{
	bool	result(false);
	static tick_t	nextReconnectionTime(0);
	
	if (isConnected())
		{
		if (lastReceivedGAINState != savedGAINState)
			{
			if (panelGAINStateIsSynched)
				{
			#if __DEBUG__ > 3
				controller.asc0 << "handlePeriodicTask: received GAIN state is " << lastReceivedGAINState << ", expected " << savedGAINState << ", time == " << nextReconnectionTime << newline;
			#endif
				if (nextReconnectionTime == 0)
					nextReconnectionTime = coreSystemTimerTicks();
				else
					panelIsConnected = ((coreSystemTimerTicks() - nextReconnectionTime) < 250);
				}
			}
		else if (nextReconnectionTime != 0L)
			{
		#if __DEBUG__ > 3
			controller.asc0 << "handlePeriodicTask: resetting nextReconnectionTime" << newline;
		#endif
			nextReconnectionTime = 0;
			}
		}
	else if (coreSystemTimerTicks() > nextReconnectionTime)
		{
	#if __DEBUG__ > 3
		controller.asc0 << "Panel not connected!" << newline;
	#endif
		if (!getPowerSupplyState())
			{
			setPowerSupplyState(true);
			}
		else
			{
		#if (PARTNUMBER == 932)
			remainingSecondsSave = -1;
		#elif (PARTNUMBER == 922)||(PARTNUMBER == 916)
			remainingSecondsSave = -1;
		#endif
			GAIN.resyncWithPanel();

			if (!isConnected())
				{
				if (retryCounter++ > 5)
					{
					coreI2CAbortOperation(kI2C0);
					setPowerSupplyState(false);
					retryCounter = 0;
					}
				}
			}

		if (isConnected())
			nextReconnectionTime = 0L;
		else
			nextReconnectionTime = coreSystemTimerTicks() + 500;
		}

	if (isConnected())
		{
		checkEvents();
		}
	
	return result;
	}
*/

bool ControlPanel::handlePeriodicTask(void)
	{
	bool	result(false);

	if (isConnected())
		{
		checkEvents();
		}
	
	return result;
	}

bool ControlPanel::checkEvents(void)
	{
	int16	n;
	int16	nDrops(0);
	int16	nEvents(0);
	bool	done = false;
	event	e;
	panelReplyMessage	panelResult;

	//	wait(0);
	
	while (!done)
		{
		n = coreI2CRead(kI2C0, 0x42, &panelResult, sizeof(panelResult), 1);

		if (n == sizeof(panelResult))
			{
			done = (panelResult.event.eventType == kHMINoEvent);
			if (!panelHasFirmware)
				panelHasFirmware = (panelResult.event.eventType != kHMINoFirmwareEvent);
			
			lastReceivedGAINState = panelResult.GAINState;

		#if __DEBUG__ > 3
			if (!panelGAINStateIsSynched)
				controller.asc0 << "checkEvents: received GAIN state is " << lastReceivedGAINState << ", expected " << savedGAINState << newline;
		#endif
			panelGAINStateIsSynched = (lastReceivedGAINState == savedGAINState);

			if (!done)
				{
				e.eventType = panelResult.event.eventType;
				e.data.data = 0;

				switch (panelResult.event.eventType)
					{
					case kHMISendConfigurationEvent:
						e.data = eventData(panelResult.event.eventData[0], panelResult.event.eventData[1]);
						break;
						
					case kHMISwitchStateEvent:
						e.data = eventData(panelResult.event.eventData[0], panelResult.event.eventData[1]);
						break;
						
					case kHMIButtonPressedEvent:
						if (panelResult.event.eventData[0] != 0xFF)
							{
						#if __DEBUG__ > 1
							controller.asc0 << "Button pressed: " << panelResult.event.eventData[0] << newline;
						#endif
							e.eventType = panelResult.event.eventData[0];
							e.data.data = 0;
							}
						break;

					case kHMIStartCateringCycleEvent:
					#if (__DEBUG__ > 3)
						controller.asc0 << "Start catering cycle, flags = " << kModeHex << panelResult.event.eventData[1] << newline;
					#endif
						e.data = eventData(panelResult.event.eventData[0], panelResult.event.eventData[1]);
						break;
		
					case kHMIPauseCateringCycleEvent:
					#if (__DEBUG__ > 3)
						controller.asc0 << "Pause catering cycle, flags = " << kModeHex << panelResult.event.eventData[1] << newline;
					#endif
						e.data = eventData(panelResult.event.eventData[0], panelResult.event.eventData[1]);
						break;
		
					case kHMIResumeCateringCycleEvent:
					#if (__DEBUG__ > 3)
						controller.asc0 << "Resume catering cycle" << panelResult.event.eventData[1] << newline;
					#endif
						e.data = eventData(panelResult.event.eventData[0], panelResult.event.eventData[1]);
						break;
		
					case kHMIAbortCateringCycleEvent:
					#if (__DEBUG__ > 3)
						controller.asc0 << "Abort catering cycle" << panelResult.event.eventData[1] << newline;
					#endif
						e.data = eventData(panelResult.event.eventData[0], panelResult.event.eventData[1]);
						break;

				#if ((PARTNUMBER == 916)||(PARTNUMBER == 932))
					case kHMIChangeTimeEvent:
						e.data = eventData(panelResult.event.eventData[0] * 256 + panelResult.event.eventData[1]);
					#if (__DEBUG__ > 2)
						controller.asc0 << "Change time event: " << e.data.data << newline;
					#endif
						break;

					case kHMIIncreaseTimeEvent:
						e.data = eventData(panelResult.event.eventData[0] * 256 + panelResult.event.eventData[1]);
					#if (__DEBUG__ > 2)
						controller.asc0 << "Increase time event: " << e.data.data << newline;
					#endif
						break;

					case kHMIDecreaseTimeEvent:
						e.data = eventData(panelResult.event.eventData[0] * 256 + panelResult.event.eventData[1]);
					#if (__DEBUG__ > 2)
						controller.asc0 << "Decrease time event: " << e.data.data << newline;
					#endif
						break;

					case kHMIChangePowerEvent:
						e.data = eventData(panelResult.event.eventData[0] * 256 + panelResult.event.eventData[1]);
					#if (__DEBUG__ > 2)
						controller.asc0 << "Change power event: " << e.data.data << newline;
					#endif
						break;
				#else
					case kHMIChangeTimeEvent:
						e.data = eventData(panelResult.event.eventData[0] * 256 + panelResult.event.eventData[1]);
					#if (__DEBUG__ > 2)
						controller.asc0 << "Change time event: " << e.data.data << newline;
					#endif
						break;
				#endif
					case kHMIChangeTemperatureEvent:
						e.data = eventData(panelResult.event.eventData[0] * 256 + panelResult.event.eventData[1]);
					#if (__DEBUG__ > 3)
						controller.asc0 << "Change temperature event: " << e.data.data << newline;
					#endif
						break;

					case kHMIChangePresetEvent:
						e.data = eventData(panelResult.event.eventData[0]);
					#if (__DEBUG__ > 3)
						controller.asc0 << "Change preset event: " << e.data.data << newline;
					#endif
						break;

					case kHMIChangeModeEvent:
						e.data = eventData(panelResult.event.eventData[0]);
					#if (__DEBUG__ > 3)
						controller.asc0 << "Change mode event: " << e.data.data << newline;
					#endif
						break;

					case kHMIChangeDelayTimeEvent:
						e.data = eventData(panelResult.event.eventData[0] * 256 + panelResult.event.eventData[1]);
					#if (__DEBUG__ > 3)
						controller.asc0 << "Change delay time event: " << e.data.data << newline;
					#endif
						break;

					case kHMIChangeAlarmStateEvent:
					#if (__DEBUG__ > 3)
						controller.asc0 << "Change alarm state event, flags = " << kModeHex << panelResult.event.eventData[1] << newline;
					#endif
						e.data = eventData(panelResult.event.eventData[0], panelResult.event.eventData[1]);
						break;

					case kHMINoFirmwareEvent:								
					#if (__DEBUG__ > 2)
						controller.asc0 << "kHMINoFirmwareEvent" << newline;
					#endif
						done = true;
						//	Panel firmware was detected: ignore
						if (panelHasFirmware)
							e.eventType = kAnyEventType;
						break;

					default:
					#if (__DEBUG__ > 3)
						controller.asc0 << "Unknown panel event: eventType == " << panelResult.event.eventType << newline;
					#endif
						e.eventType = kAnyEventType;
						done = true;
						break;
					}
					
				if (e.eventType != kAnyEventType)
					{
					e.eventClass = kHMIEventClass;
					sendEventSelf(e);
					}
				}	// if (!done)

			//	panelResult.eventCount denotes the number of panel events pending, thus '0' means "no further event"
			done |= ((panelResult.eventCount == 0)||(++nEvents == 8));
			nDrops = 0;
			}			
		else
			{
		#if (__DEBUG__ >= 3)
			controller.asc0 << "ControlPanel::checkEvent(): read fault, " << " n == " << n << ", drops == "  << nDrops << newline;
		#endif
			nDrops++;
			if (nDrops > 10)
				{
			#if (__DEBUG__)
				controller.asc0 << "*** ControlPanel::checkEvent(): panel assumed absent!" << newline;
			#endif
				panelIsConnected = false;
				done = true;
				}
			else
				coreSystemTimerWait(1);
			}
		}	// while (!done)

	return panelIsConnected;
	}

static uint8 I2CRead(uint8 n, uint8* data)
	{
	uint8	i;
	uint8	nAvailable;

	receiveBuffer[0] = 0;
	coreI2CRead(kI2C0, 0x42, receiveBuffer, n + 1, 0);	
	nAvailable = receiveBuffer[0];

	if (nAvailable == 0xFF)
		controller.reset();
		
	for (i=0; i<n && i<nAvailable; i++)
		data[i] = receiveBuffer[i+1];

#if __DEBUG__ > 4
	if (nAvailable)
		controller.asc0 << "I2CRead: received " << (uint16)nAvailable << ", expected " << (uint16)n
						<< ' ' << (uint16)receiveBuffer[0]
						<< ' ' << (uint16)receiveBuffer[1] << '\r';
	else
		controller.asc0 << "I2CRead --" << '\r';
#endif

	return nAvailable;
	}

bool ControlPanel::update(bool address32)
	{
	bool	done(false);
	uint16	n(0);
	uint16	expected(0);

#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "ControlPanel::update" << newline;
	controller.asc0.unlock();	
#endif
	if (!getPowerSupplyState())
		{
		setPowerSupplyState(true);
		controller.wait(500);
		}

	syncWithFirmwareUpdate();
	panelIsConnected = false;

	while (!done)
		{
		if (expected)
			n = I2CRead(expected, receiveBuffer);

		if (n)
			{
			controller.asc0.writeData(receiveBuffer, n);
			expected = 0;
			n = 0;
			}
		else if (controller.asc0.dataAvailable())
			{
			controller.asc0.readData(receiveBuffer, 2, 500);
			if (receiveBuffer[0] == 'R')
				{
				expected = 0;
				receiveBuffer[0] = 0x14;
				receiveBuffer[1] = 0x42;
				controller.asc0.writeData(receiveBuffer, 2);
				receiveBuffer[0] = 0x00;
				controller.asc0.writeData(receiveBuffer, 1);
				}
			else if (receiveBuffer[0] == '0')
				{
				expected = 2;
  				coreI2CWrite(kI2C0, 0x42, receiveBuffer, 2, 10);
				}
			else if (receiveBuffer[0] == '1')
				{
				expected = 10;
				coreI2CWrite(kI2C0, 0x42, receiveBuffer, 2, 10);
				}
			else if (receiveBuffer[0] == '?')
				{
				expected = 64;
				coreI2CWrite(kI2C0, 0x42, receiveBuffer, 2, 10);
				}
			else if (receiveBuffer[0] == 'P')
				{
				if (address32)
					expected = 4;
				else
					expected = 2;
				coreI2CWrite(kI2C0, 0x42, receiveBuffer, 2, 10);
				}
			else if (receiveBuffer[0] == 'U')
				{
				if (address32)
					{
					controller.asc0.readData(&receiveBuffer[2], 4, 1000);
					coreI2CWrite(kI2C0, 0x42, receiveBuffer, 6, 10);
					}
				else
					{
					controller.asc0.readData(&receiveBuffer[2], 2, 1000);
					coreI2CWrite(kI2C0, 0x42, receiveBuffer, 4, 10);
					}
				expected = 2;
				}
			//	Send device signature
			else if (receiveBuffer[0] == 'u')
				{
				expected = 5;
				coreI2CWrite(kI2C0, 0x42, receiveBuffer, 2, 10);
				}
			//	Program data block
			else if (receiveBuffer[0] == 'd')
				{
				expected = 2;
				controller.asc0.readData(&receiveBuffer[2], 2, 1000);
   				//	Write command sequence
				coreI2CWrite(kI2C0, 0x42, receiveBuffer, 4, 10);
			
				n = receiveBuffer[1] * 0x100 + receiveBuffer[2];

				controller.asc0.readData(receiveBuffer, n + 2, 1000);
				
   				//	Write command sequence
				coreI2CWrite(kI2C0, 0x42, receiveBuffer, n + 2, 10);
				}
			else if (receiveBuffer[0] == 'Q')
				{
				expected = 0;
				coreI2CWrite(kI2C0, 0x42, receiveBuffer, 2, 10);
				receiveBuffer[0] = 0x14;
				receiveBuffer[1] = 0x10;
				controller.asc0.writeData(receiveBuffer, 2);

				setPowerSupplyState(false);
				controller.wait(500);
		
				done = true;
				}
			else
				expected = 0;
			}
		else
			controller.wait(1);
		}
	
	return done;
	}

bool ControlPanel::sendRequest(const panelRequestHeader& header, const void* request, int8 delay)
	{
	int16	n;
	int16	size(header.size + sizeof(panelRequestHeader));
	bool	result;

	if (delay < 0)
		wait(size);
	else
		wait(delay);
	
	n = coreI2CWrite(kI2C0, 0x42, request, size, 1000);
	
	result = (n == size);

#if __DEBUG__ > 2
	if (!result)
		controller.asc0 << "command " << kModeHex <<  header.command << " failed: wrote " << (uint16)n  << " byte, expected " << size << newline;
#endif
	
	return result;
	}

void ControlPanel::wait(int16 size)
	{
	tick_t	deltaT(controller.ticks() - lastMessageSent);
	
	if (deltaT < lastMessageSize)
		controller.wait(lastMessageSize - deltaT);

	lastMessageSize = size;

	lastMessageSent = controller.ticks();
	}

