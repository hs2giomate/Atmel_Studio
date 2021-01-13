/* 
* Maintenance_Tool.cpp
*
* Created: 9/15/2020 1:29:17 PM
* Author: GMateusDP
*/


#include "Maintenance_Tool.h"
#include "FlashMemoryClass.h"
//#include "FRAM_Memory_Class.h"

#include "TimerSerial_Class.h"
#include "string.h"
#include "SingleHeater_Class.h"
#include "TemperatureSensors_Class.h"
#include "FlapperValveController.h"
#include "MemoryFlash_Class.h"

static bool periodicTask;

static uint8_t		staticBuffer[MAINTENANCE_TOOL_BUFFER_SIZE];


Maintenance_Tool	*ptrMaintenanceTool;

static void MaintenaceToolTimming( const struct timer_task *const timer_task)
{
		
	ptrMaintenanceTool->ticks++;		

}

static void MaintenaceToolPCBeaming( const struct timer_task *const timer_task)
{
	
	ptrMaintenanceTool->is_MTPC_Beaming=false;

}




// default constructor
Maintenance_Tool::Maintenance_Tool()
{
	ptrMaintenanceTool=this;
	maintenanceIsConnected = false;
	nextMaintenanceSyncTime = 0;
	nextMaintenanceUsageTimerSyncTime = 0;
	ticks=0;
	localBuffer=staticBuffer;
} //Maintenance_Tool

// default destructor
Maintenance_Tool::~Maintenance_Tool()
{
	ticks=0;
	interfaceTimer.Stop();
	interfaceTimer.Remove_task();
	
} //~Maintenance_Tool

bool	Maintenance_Tool::Init(void){
	ticks=0;
	interfaceTimer.Init();
	interfaceTimer.Add_periodic_task((FUNC_PTR)MaintenaceToolTimming,1000);
	InitCommandHandler(localBuffer);
	GetCPUSerialNumber(cpuSerial);
	memcpy(localBuffer,cpuSerial,16);
	gotAccess=false;
	return result;
}

bool	Maintenance_Tool::IsAppConnected(void){
	result=false;
	magic=0;
	if (gotAccess==true)
	{
		result=gotAccess;
	} 
	else
	{
		if (usb.rxReady)
		{
			for (int i = 0; i < 4; i++)
			{
				magic=magic<<8;
				magic|= localBuffer[i];
			}
			xorResult=magic^MAINTENANCE_TOOL_KEYWORD;
			if (xorResult==0)
			{
				gotAccess=true;
				result=true;
				is_MTPC_Beaming=true;
				setConnected(true);
				return	result;
			}
			else
			{
				//delay_ms(1000);
				result=false;
			}
		}
		else
		{
			result=false;
		}
		
		usb.readDataAsyn(localBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
	}
	
	return	result;
}

void Maintenance_Tool::setConnected(bool isConnected)
	{
		maintenanceIsConnected = isConnected;
		if (isConnected)
		{
			
			interfaceTimer.Start_oneShot_task((FUNC_PTR)MaintenaceToolPCBeaming,60*1000);
			//usbTimer.Start();
				
				
		} 
		else
		{
			interfaceTimer.Stop();
				
		}

	//lastMaintenanceMessageReceivedTime = controller.ticks();
	}

/*!
 * \brief	Synchronizes data (notifications) with Maintenance tool.
 *
 * \details	If connected and a timeout for synch occurred notifications like NotifyState,
 * -ControllerState and -Analoge values are updated.
 * 	If also a usage data timeout occurred NotifyUsageData update is done.
 *
 * \author	ss	Stefan Simone
 *
 * \param	io  Hardware interface \n
 *          state  \n
 *          flages
 *
 * \return
*/

/*!
 * \brief   Detects what kind of data received form m.tool (query, command or note that m.tool is connected)
 *
 * \details If data is received over communication interface this function is called to detect
 * what kind of data is received. It could be a query, a command or a note that m.tool is connected.
 *
 *
 * \param	io  Hardware interface
 *
 * \return Returns true if data was valid. Means query, command or connection note. Otherwise
 * false is returned.
*/
bool Maintenance_Tool::handleCommunication(void)
	{
	
	result=false;

	if (gotAccess)
	{
		if (usb.rxReady)
		{
			int sizeMessage=sizeof(HVACMessageHeader);
			memcpy(&header,localBuffer,sizeMessage);
			char	input=(char)(header.command);
			switch (input){
				case '<':
					gotCommand=true;
					result = handleHVACTask();
				break;
				case '>':
					result = handleHVACTask();
				break;

				case '=': //This is something like an alive beacon from MTool. Received every 10 sec
					is_MTPC_Beaming=true;
					interfaceTimer.Stop();
					interfaceTimer.Remove_task((FUNC_PTR)MaintenaceToolPCBeaming);
					interfaceTimer.Start_oneShot_task((FUNC_PTR)MaintenaceToolPCBeaming,10*1000);
				
					nextMaintenanceSyncTime = 0;
					maintenanceIsConnected = true;
					bSendNotifications = true;
					result=is_MTPC_Beaming;
				break;
				
				
				default:
					gotCommand=false;
				
				break;
				
			}
			ClearLocalBuffer();
		}
		else
		{
			gotCommand=false;
			
			
		}
		usb.readDataAsyn(localBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
		
	}else{
		gotCommand=false;
		
	}
	

	//if (result)
		//lastMaintenanceMessageReceivedTime = controller.ticks();

	return result;
	}
	
uint8_t	Maintenance_Tool::ClearLocalBuffer(void){
	uint8_t i;
	for (i = 0; i < MAINTENANCE_TOOL_BUFFER_SIZE; i++)
	{
		localBuffer[i]=0;
	}
	return	i;
}
/*!
 * \brief	Used for updating an analog value that is transmitted frequently to mainenance tool later.
 *
 * \details Analogue values are transmitted with the notification kGAINNotifiyAnalogueValues to m.tool
 *          Values are transmitted frequentlly by handlePeriodicTask of maintenance class.
 *          Frequence defined by nextMaintenanceSyncTime.
 *
 * \param	idx     Tells which value to update \n
 *          value   Contains new value
*/
void Maintenance_Tool::updateAnalogueValue(uint8 idx, float value)
	{
	analogueValues[idx] = value;
// #if __DEBUG__ > 4
// 	if (idx == 7)
// 		controller.asc0 << "updateAnalogueValue: " << value << " V" << newline;
// #endif
	}

/*****

GAIN notifications to maintenance tool.

Done frequentlly by handlePeriodicTask.

Each notification has a unique identifier kGAINNotify... so that
    m.tool knows how to handle each one.

Notifications are build out of 3 or 4 messages (for example: header, notification data and checksum)
    with individuell sizes.

*****/
bool Maintenance_Tool::GAINNotifyState( uint8 state, uint8 flags)
	{
// #if (__DEBUG__ > 3)
// 	controller.asc0 << "GAINNotifyState: state == " << state << ", flags == " << kModeHex << flags << newline;
// #endif

	

	
	result=true;
		
// 	header.command = kGAINNotifyState;
// 	header.selector = 0;
// 	header.dataSize = sizeof(GAINStateNotification);
// 	
// 	notification.time = controller.ticks();
// 	notification.state = state;
// 	notification.flags = flags;
// 
// #ifdef NSD_SUPPORT
// 	notification.deviceID = NSD.getGAINDeviceID();
// 	notification.feederID = NSD.getGAINFeederID();
// 
// 	notification.deviceFlags = (NSD.getGAINIsPrimary()?1:0);
// 	notification.deviceFlags |= (NSD.getGAINMaintenancePin()?2:0);
// 	notification.deviceFlags |= (NSD.getGAINParityPin()?4:0);
// 	notification.networkState = 0;
// 	
// 	if (NSD.MGCUIsPresent())
// 		notification.networkState |= (1 << 0);
// 	
// 	if (NSD.CentralizedPowerControlAvailable())
// 		notification.networkState |= (1 << 1);
// #else
// 	notification.deviceFlags = 0;
// 	notification.networkState = 0;
// #endif
// 
// 	checksum = calculateChecksum(header);
// 	checksum = calculateChecksum(checksum, header.dataSize, &notification);
// 
// 	io.writeData(&header, sizeof(header));
// 	io.writeData(&notification, sizeof(notification));
// 	io.writeData(&checksum, sizeof(checksum));

	return result;
	}
	
bool	Maintenance_Tool::NotifyConnectionAcknowledge(void){
	uint8_t i;
	memcpy(&deviceID.cpuSerialNumber,cpuSerial,sizeof(cpuSerial));
	//deviceID.cpuSerialNumber=cpuSerial;
	delay_us(100);
	usb<<"CPU Serial: ";
	for (i = 0; i < sizeof(cpuSerial); i++)
	{
		usb.print(cpuSerial[i],HEX);
	}

	usb.println();
	result=(bool)(i>0);
	return result;
}

bool Maintenance_Tool::GAINNotifyControllerState(void)
	{
	


	header.command = kGAINNotifyControllerState;
// 	header.selector = 0;
// 	header.dataSize = sizeof(GAINControllerPortState);

// #ifdef MGS_ZST_Revision
// 	uint32	port(FIO1PIN);
// 
// 	for (i=0; i<4; i++)
// 		{
// 		state.PortValue[3-i] = (uint8)(port & 0xFF);
// 		port >>= 8;
// 		}
// #else
// 	for (i=0; i<7; i++)
// 		{
// 		state.PortValue[i] = corePortsGetPort((controllerPort)i);
// 		}
// 
// 	state.PWMCount = kPWMPinCount;
// 	state.PWMState = 0;
// 	for (i=0; i<kPWMPinCount; i++)
// 		{
// 		if (corePWMGetPWMState((pwmPin)i))
// 			{
// 			state.PWMState |= (1 << i);
// 			}
// 		}
// 
// 	for (i=0; i<kPWMPinCount; i++)
// 		{
// 		state.pwm[i] = corePWMGetPWMDutyCycle((pwmPin)i);
// 		}
// #endif		
// 	checksum = calculateChecksum(header);
// 	checksum = calculateChecksum(checksum, sizeof(GAINControllerPortState), &state);
// 
// 	io.writeData(&header, sizeof(header));
// 	io.writeData(&state, sizeof(GAINControllerPortState));
// 	io.writeData(&checksum, sizeof(checksum));
	
	return result;
	}

bool Maintenance_Tool::GAINNotifyAnalogueValues()
	{

	GAINControllerAnalogueValues	values;


// 
// #ifdef IPECO_PSC_Revision
// #if PARTNUMBER != 920	
// #ifdef NO_RTC
// 	updateAnalogueValue(12, 20.1);
// #else
// 	updateAnalogueValue(12, coreRTCGetTemperatureValue());
// #endif
// #endif
// #endif
// 
// 	header.command = kGAINNotifyAnalogueValues;
// 	header.selector = 0;
// 	header.dataSize = sizeof(GAINControllerAnalogueValues) + sizeof(analogueValues);
// 
// 	values.analogueCount = 16;
// 	values.reserved = 0;
// 		
// 	checksum = calculateChecksum(header);
// 	checksum = calculateChecksum(checksum, sizeof(GAINControllerAnalogueValues), &values);
// 	checksum = calculateChecksum(checksum, sizeof(analogueValues), &analogueValues);
// 
// 	io.writeData(&header, sizeof(header));
// 	io.writeData(&values, sizeof(GAINControllerAnalogueValues));
// 	io.writeData(analogueValues, sizeof(analogueValues));
// 	io.writeData(&checksum, sizeof(checksum));

	return result;
	}

bool Maintenance_Tool::GAINNotifyUsageData(void)
	{

// 	uint8	n;
// 
// 	header.command = kGAINNotifyUsageData;
// 	header.selector = 0;
// 
// #if PARTNUMBER == 0xBABE
// 	header.dataSize = sizeof(n);
// 	checksum = calculateChecksum(header);
// 	checksum = calculateChecksum(checksum, 1, &n);
// 
// 	io.writeData(&header, sizeof(header));
// 	io.writeData(&n, sizeof(n));
// 	io.writeData(&checksum, sizeof(checksum));
// #else
// #if PARTNUMBER == 922
// 	uint32	usageData[kNumberOfUsageTimeCounters + 3 + kNumberOfUsageCounters];
// #else
// 	uint32	usageData[kNumberOfUsageTimeCounters + kNumberOfCycleDescriptions + kNumberOfUsageCounters];
// #endif
// 	n = persistent.getUsageData(usageData);
// 
// 	header.dataSize = (n * sizeof(uint32)) + 1;
// 	checksum = calculateChecksum(header);
// 	checksum = calculateChecksum(checksum, 1, &n);
// 	checksum = calculateChecksum(checksum, n * sizeof(uint32), &usageData);
// 
// 	io.writeData(&header, sizeof(header));
// 	io.writeData(&n, sizeof(n));
// 
// 	if (n > 0)
// 		io.writeData(usageData, n * sizeof(uint32));
// 
// 	io.writeData(&checksum, sizeof(checksum));
// #endif
	
	return result;
	}

bool Maintenance_Tool::GAINNotifyTemperatureData(void)
{

//     uint8	n;
// 
//     header.command = kGAINNotifyTemperaturOutOfRangeCounterReceived;
//     header.selector = 0;
// 
//     uint32	usageData[3] = {4,5,6};
// 
//     n = persistent.getTemperatureOutOfRangeData(usageData);
// 
//     header.dataSize = (n * sizeof(uint32)) + 1;
//     checksum = calculateChecksum(header);
//     checksum = calculateChecksum(checksum, 1, &n);
//     checksum = calculateChecksum(checksum, n * sizeof(uint32), &usageData);
// 
//     io.writeData(&header, sizeof(header));
//     io.writeData(&n, sizeof(n));
// 
//     if (n > 0)
//         io.writeData(usageData, n * sizeof(uint32));
// 
//     io.writeData(&checksum, sizeof(checksum));

    return result;
}

/*****

	GAIN commands

*****/
bool Maintenance_Tool::handleHVACTask(void){
	int sizeMessage=sizeof(SingleTaskMessage);
	memcpy(&header,localBuffer,sizeof(HVACMessageHeader));

	if ((header.task>=kGAINCommandConnect)
	&& (header.task<kGAINNumberOfCommands))
	{
		gotTask=true;
		result=gotTask;
	} 
	else
	{
		gotTask=false;
		usb.readDataAsyn(&localBuffer, MAINTENANCE_TOOL_BUFFER_SIZE);
		result=usb.rxReady;
	}


	
	if (result)
		{
		switch (header.task)
			{
			case kGAINCommandConnect:
				result = handleGAINCommandConnect( header);
				break;

			case kGAINCommandDisconnect:
				result = handleGAINCommandDisconnect( header);
				break;

			case kGAINCommandSetNotificationState:
				result = handleGAINCommandSetNotificationState( header);
				break;

			case kGAINCommandReset:
				notifyGAINCommandReceived( header, result);
				result = handleGAINCommandReset(header);
				break;

			case kGAINCommandResetNVM:
				result = handleGAINCommandResetNVM( header);
				break;

			case kGAINCommandSetControllerState:
				result = handleGAINCommandSetControllerState( header);
				break;

			case kGAINCommandSetConfiguration:
				result = handleGAINCommandSetConfiguration( header);
				break;
			case kGAINCommandWriteParameters:
				result = CommandWriteParameters();
				break;
			case kGAINCommandReadParameters:
				result = CommandReadParameters();
				break;
			case kHVACCommandSetHeaters:
				result = CommandSetHeaters();
				break;
				
			case kHVACCommandReadHeaterStatus:
				result = CommandReadHeaterStatus();
				break;
			case kHVACCommandSetEnableFans:
				result = CommandSetEnableFans();
				break;
			
			case kHVACCommandSetPWMFans:
				result = CommandSetPWMFans();
				break;
			case kHVACCommandRedStatusFans:
				result = CommandFansStatus();
				break;
			case kHVACCommandReadTemperatures:
				result = CommandReadTemmperatures();
				break;
			case kHVACReadPositionFlapperValve:
				result = CommandReadFlapperData();
				break;
				
			case kHVACCommandFlapperValve:
				result = CommandSetFlapperValve();
				break;
			case kHVACWriteSetpointFlapperValve:
				result = CommandSetFlapperPosition();
				break;
			case kGAINCommandSetCycleDictionary:
				result = handleGAINCommandSetCycleDictionary( header);
				break;

			case kGAINCommandSetCycleDescription:
				result = handleGAINCommandSetCycleDescription( header);
				break;
	
		#ifdef NSD_SUPPORT
			case kGAINCommandSetNSDData:
				result = handleGAINCommandSetNSDData( header);
				break;
		#endif
			case kGAINCommandSetView:
				result = handleGAINCommandEnableSecondView( header);
				break;
			default:
				result = false;
				break;
			}
		}

	//notifyGAINCommandReceived(header, result);

	return result;
	}

void Maintenance_Tool::notifyGAINCommandReceived( HVACMessageHeader& header, bool result)
	{

// 
// 	header.selector = header.command;
// 	header.command = kGAINNotifyCommandReceived;
// 	header.dataSize = 0;

	if (!result){
		//	header.selector |= 0x80;
	}

	checksum = calculateChecksum(header);

	usb.writeData(&header, sizeof(header));
	/*io.writeData(&checksum, sizeof(checksum));*/
	}

bool Maintenance_Tool::handleGAINCommandConnect( HVACMessageHeader& header)
	{
	
	if (!maintenanceIsConnected)
		{
		//event	e;

		nextMaintenanceSyncTime = 0;
		maintenanceIsConnected = true;
		bSendNotifications = true;
// 
// 		e.eventClass = kGAINEventClass;
// 		e.eventType = kGAINMaintenanceConnectionEstablishedEvent;
// 		e.data.data = 0;
// 		sendEventSelf(e);
// 
// 	#ifdef E_DEVICE
// 		corePortsClearPortBit(kPortG, kPortBit2);
// 	#endif
 		}

	return result;
	}

bool Maintenance_Tool::handleGAINCommandDisconnect( HVACMessageHeader& header)
	{



	maintenanceIsConnected = false;
	bSendNotifications = false;

// 	e.eventClass = kGAINEventClass;
// 	e.eventType = kGAINMaintenanceConnectionLostEvent;
// 	e.data.data = header.selector;
// 	sendEventSelf(e);
// 
// #ifdef E_DEVICE
// 	corePortsSetPortBit(kPortG, kPortBit2);
// #endif
	return result;
	}

bool Maintenance_Tool::handleGAINCommandSetNotificationState( HVACMessageHeader& header)
	{


	//bSendNotifications = (header.selector == 1);

	return result;
	}

bool Maintenance_Tool::handleGAINCommandReset( HVACMessageHeader& header)
	{
//	controller.reset();
	return true;
	}

bool Maintenance_Tool::handleGAINCommandResetNVM( HVACMessageHeader& header)
	{
	bool	result(true);

// 	switch (header.selector)
// 		{
// 		case '0':	//	Clear working state
// 		#if PARTNUMBER != 0xBABE
// 			persistent.clearWorkingStateForGAIN();
// 		#endif
// 			break;
// 			
// 		case '1':	//	Set factory defaults, clear working state and reset
// 		#if PARTNUMBER != 0xBABE
// 			persistent.clearWorkingStateForGAIN();
// 			GAIN.setFactoryDefaults(GAIN.configuration.commons.subPartNumber, true, false);
// 		#endif
// 			break;
// 			
// 		case '2':	//	Set factory defaults, clear working state and reset
// 		#if PARTNUMBER != 0xBABE
// 			persistent.clearWorkingStateForGAIN();
// 			GAIN.setFactoryDefaults(GAIN.configuration.commons.subPartNumber, false, true);
// 		#endif
// 			break;
// 			
// 		case '3':	//	Clear error log
// 			persistent.initializeErrorArea();
// 			break;
// 			
// 		case '4':	//	Clear usage counters
// 			persistent.initializeUsageArea();
// 			break;
// 			
// 		case '5':	//	Clear maintenance counters
// 			persistent.initializeMaintenanceArea();
// 			break;
// 			
// 		default:
// 			result = false;
// 			break;
// 		}

	return result;
	}

bool Maintenance_Tool::handleGAINCommandSetControllerState( HVACMessageHeader& header)
	{


/*	bool	result(header.dataSize == sizeof(GAINControllerPortState));*/
// 	uint8	i;
// 	
// 	if (result)
// 		{
// 		uint8	n;
// 		GAINControllerPortState	state;
// 
// 		n = io.readData(&state, sizeof(GAINControllerPortState), 500);
// 		result = (n == sizeof(GAINControllerPortState));
// 
// 		n = io.readData(&checksumProvided, sizeof(checksumProvided), 100);
// 
// 		if (result)
// 			{
// 			checksum = calculateChecksum(header);
// 			checksum = calculateChecksum(checksum, sizeof(GAINControllerPortState), &state);
// 			
// 			result = (checksum == checksumProvided);
// 			}
// 
// 		if (result)
// 			{
// 		#ifdef MGS_ZST_Revision	
// 			uint32	port(0);
// 
// 			for (i=0; i<4; i++)
// 				{
// 				port <<= 8;
// 				port |= state.PortValue[i];
// 				}
// 			
// 			FIO1CLR = (~port & FIO1DIR);
// 			FIO1SET = (port & FIO1DIR);
// 			//	FIO1PIN = port;
// 		#else
// 			for (i=0; i<7; i++)
// 				{
// 				corePortsSetPort((controllerPort)i, state.PortValue[i]);
// 				}
// 			for (i=0; i<kPWMPinCount; i++)
// 				{
// 				corePWMSetPWMDutyCycle((pwmPin)i, state.pwm[i]);
// 			#if __DEBUG__ > 4
// 				controller.asc0 << "corePWMSetPWMDutyCycle: channel #" << i << ", pwm == " << command.pwm[i] << newline;
// 			#endif
// 				if (state.PWMState & (1 << i))
// 					corePWMSetPWMOn((pwmPin)i);
// 				else if (corePWMGetPWMState((pwmPin)i))
// 					corePWMSetPWMOff((pwmPin)i);
// /*
// 				if (state.PWMState & (1 << i))
// 					{
// 					corePWMSetPWMDutyCycle((pwmPin)i, state.pwm[i]);
// 				#if __DEBUG__ > 4
// 					controller.asc0 << "corePWMSetPWMDutyCycle: channel #" << i << ", pwm == " << command.pwm[i] << newline;
// 				#endif
// 					if (state.PWMState & (1 << i))
// 						corePWMSetPWMOn((pwmPin)i);
// 					else if (corePWMGetPWMState((pwmPin)i))
// 						corePWMSetPWMOff((pwmPin)i);
// 					}
// */
// 				}
// 		#endif
// 			}
// 		}
// 	else
// 		{
// 		char	ch;
// 		
// 		for (i=0; i<header.dataSize; i++)
// 			io >> ch;
// 		}
	
	return result;
	}

bool Maintenance_Tool::handleGAINCommandSetConfiguration( HVACMessageHeader& header)	{
	
// 	bool	result(header.dataSize == sizeof(ConfigurationData));
// 	
// 	if (result)
// 		{
// 		int16	n;
// 		uint16	subPartNumber(GAIN.configuration.commons.subPartNumber);
// 		
// 		n = io.readData(&GAIN.configuration, sizeof(ConfigurationData), 500);
// 		result = (n == sizeof(ConfigurationData));
// 
// 		io.readData(&checksumProvided, sizeof(checksumProvided), 100);
// 
// 		if (result)
// 			{
// 			uint16	checksum;
// 
// 			checksum = calculateChecksum(header);
// 			checksum = calculateChecksum(checksum, header.dataSize, &GAIN.configuration);
// 
// 			result = (checksum == checksumProvided);
// 			}
// 		
// 		if (result)
// 			{
// 			bool	resetConfiguration(subPartNumber != GAIN.configuration.commons.subPartNumber);
// 
// 			if (resetConfiguration)
// 				{
// 				GAIN.setFactoryDefaults(GAIN.configuration.commons.subPartNumber, true, false);
// 				}
// 			else
// 				{
// 				persistent.writeConfigurationDataArea(GAIN.configuration);
// 				}
// 			}
// 		else
// 			{
// 			GAIN.setFactoryDefaults(subPartNumber, true, false);
// 			}
// 
// 		persistent.clearWorkingStateForGAIN();
// 		}
// 	else
// 		{
// 		uint8	i;
// 		char	ch;
// 		
// 		for (i=0; i<header.dataSize; i++)
// 			io >> ch;
// 		}
// 
// 	return result;
	return true;
	}

bool Maintenance_Tool::CommandReadParameters(){
	int n=sizeof(HVACMessageHeader);
	HVACMessageHeader hm;
	uint32_t	w,r;
	
	 	bool	result(header.task == kGAINCommandReadParameters);
		if (result){
		// 		
			uint32_t add=(uint32_t)&flashLayout->parameters;
			r=memory.ReadParameters(parameters);
			 memcpy(localBuffer,(void*)&hm,n);
			memcpy((uint8_t*)&localBuffer[n],(void*)&parameters,sizeof(UserParameters));
			if (fvc.dataStruct.controlOutputs.iAlcFvStandAloneOut)
			{
				localBuffer[n+sizeof(UserParameters)]=parameters.flapperValveStandAloneMinimumPosition;
			} 
			else
			{
				localBuffer[n+sizeof(UserParameters)]=parameters.flapperValveMinimumPosition;
			}
			w=usb.write(localBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);

	
			 
	 	}else{

		 }
		
		 return result;
}

bool Maintenance_Tool::CommandReadHeaterStatus(){
	int n=sizeof(SingleTaskMessage);
	SingleTaskMessage	singleTask;
	uint8_t	heatersEnabled;
	
	bool	result(header.task == kHVACCommandReadHeaterStatus);
	if (result){
	
			singleTask.description=heater.ReadStatus();
			
			singleTask.header.task=kHVACCommandReadHeaterStatus;
			memcpy(localBuffer,(void*)&singleTask,n);
			heatersEnabled=heater.ReadEnableGIPO();
			localBuffer[n]=heatersEnabled;
			//delay_us(1);
			if (!heater.heaterStatusChanged)
			{
				usb.write(localBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
			}
			singleTaskMessage=singleTask;
			
	}
		return result;
}


bool Maintenance_Tool::CommandReadTemmperatures(){
	int n=sizeof(SingleTaskMessage);
	
	SingleTaskMessage	singleTask;
	
	bool	result(header.task == kHVACCommandReadTemperatures);
	if (result){
		
		singleTask.description=temperatures.faultData;
		singleTask.header.task=kHVACCommandReadTemperatures;
		memcpy(localBuffer,(void*)&singleTask,n);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				memcpy(&localBuffer[n+4*((4*i)+j)],(void*)&temperatures.values[i][j],4);
			}
			
		}
		
		usb.write(localBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
		
		singleTaskMessage=singleTask;
		
	}
	
	return result;
}

bool Maintenance_Tool::CommandWriteParameters(void)	{

	uint32_t	w,r;
		int n=sizeof(HVACMessageHeader);
		HVACMessageHeader hm;
	
	bool	result(header.task == kGAINCommandWriteParameters);
	if (result){
		
			
	
			
		memcpy((uint8_t*)&parameters,&localBuffer[n],sizeof(UserParameters));
	
		uint32_t add=(uint32_t)&flashLayout->parameters;
		r=memory.SaveParameters(parameters);
			result=(bool)(r==0);
	
		
	}else{

	}
	
	return result;
}

bool Maintenance_Tool::CommandSetHeaters(void)	{

	uint32_t	w,r;
	uint8_t	data=0;
	bool powerOn;
	
		memcpy(&singleTaskMessage,localBuffer,sizeof(SingleTaskMessage));
		
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(header.task == kHVACCommandSetHeaters);
	if (result){
		lastEnableHeaters=enableHeaters;
		enableHeaters=singleTaskMessage.description;
		if (enableHeaters!=lastEnableHeaters)
		{
			for (uint8_t i = 0; i < 4; i++)
			{
				powerOn=enableHeaters&(0x01<<i);

				heater.SetRelay(i,powerOn);
	
				

			}
		}
	}
	
	return result;
}


	






bool Maintenance_Tool::handleGAINCommandSetCycleDictionary(HVACMessageHeader& header)
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

bool Maintenance_Tool::handleGAINCommandSetCycleDescription( HVACMessageHeader& header)
	{

// 	bool	result(header.dataSize == sizeof(CycleDescription));
// 	
// 	if (result)
// 		{
// 		CycleDescription	cycle;
// 		int16	n;
// 	
// 		n = io.readData(&cycle, sizeof(CycleDescription), 500);
// 		result = (n == sizeof(CycleDescription));
// 
// 		io.readData(&checksumProvided, sizeof(checksumProvided), 100);
// 
// 		if (result)
// 			{
// 			uint16	checksum;
// 
// 			checksum = calculateChecksum(header);
// 			checksum = calculateChecksum(checksum, header.dataSize, &cycle);
// 
// 			result = (checksum == checksumProvided);
// 			}
// 		
// 		if (result)
// 			{
// 			result = (header.selector < GAIN.cycleDictionary.numberOfCycleDescriptions);
// 			}
// 		
// 		if (result)
// 			{
// 			persistent.writeCycleDescription(cycle, header.selector);
// 			GAIN.resyncWithCycleDescription(header.selector);
// 			}
// 		}
// 	else
// 		{
// 		uint8	i;
// 		char	ch;
// 		
// 		for (i=0; i<header.dataSize; i++)
// 			io >> ch;
// 		}
// 
// 	return result;
return	true;
	}

#ifdef NSD_SUPPORT
bool Maintenance_Tool::handleGAINCommandSetNSDData( HVACMessageHeader& header)
	{

// 	bool	result(header.dataSize == sizeof(NSDDataRequest));
// 	
// 	if (result)
// 		{
// 		NSDDataRequest	request;
// 		//uint8	checksum(0);
// 		uint8	n;
// 	
// 		n = io.readData(&request, sizeof(NSDDataRequest), 1000);
// 		result = (n == sizeof(NSDDataRequest));
// 
// 		io.readData(&checksumProvided, sizeof(checksumProvided), 100);
// 
// 		if (result)
// 			{
// 			uint16	checksum;
// 
// 			checksum = calculateChecksum(header);
// 			checksum = calculateChecksum(checksum, header.dataSize, &request);
// 
// 			result = (checksum == checksumProvided);
// 		    }
// 
// 		if (result)
// 			{
// 			GAINDataBlock05	dataBlock05;
// 			
// 			coreNSD_readGAINConfigurationDataBlock(5, &dataBlock05);
// 			//	Copy data
// 			memcpy(dataBlock05.PartNumber, request.PartNumber, sizeof(dataBlock05.PartNumber));
// 			dataBlock05.PartNumberAmendment = request.PartNumberAmendment;
// 			memcpy(dataBlock05.SerialNumber, request.SerialNumber, sizeof(dataBlock05.SerialNumber));
// 			memcpy(dataBlock05.HardwareVersion, request.HardwareVersion, sizeof(dataBlock05.HardwareVersion));
// 			coreNSD_writeGAINConfigurationDataBlock(5, &dataBlock05);
// 			coreNSD_flushGAINConfigurationDataBlock(5);
// 
// 			GAINDataBlock03	dataBlock03;
// 
// 			coreNSD_readGAINConfigurationDataBlock(3, &dataBlock03);
// 			dataBlock03.MaximumPowerBudget = (request.powerBudget + 49)/50;
// 			dataBlock03.MinimumPowerBudget = dataBlock03.MaximumPowerBudget;
// 			dataBlock03.StandbyPowerThreshold = (request.standbyPowerBudget + 4)/5;
// 			coreNSD_writeGAINConfigurationDataBlock(3, &dataBlock03);
// 			coreNSD_flushGAINConfigurationDataBlock(3);
// 			}
// 		}
// 	else
// 		{
// 		uint8	i;
// 		char	ch;
// 		
// 		for (i=0; i<header.dataSize; i++)
// 			io >> ch;
// 		}
// 
// 	return result;
	return	true;
	}
#endif

/*!
 * \brief	HMI has two view options and this function allowes to select between thoes view over maintenance tool
 *
 * \details	how...
 *
 * \author	oz	Olga Zitterell	oz@fabmation.de
 *
 * \param
 *
 * \return
 *
 * \todo
*/
bool Maintenance_Tool::handleGAINCommandEnableSecondView( HVACMessageHeader& header)
{
// 		io.readData(&GAIN.configuration.commons.hmiView, 1, 1000);
// 		persistent.writeConfigurationDataArea(GAIN.configuration);
		return true;
}

/*****

	GAIN queries

*****/
bool Maintenance_Tool::handleGAINQuery(void)
	{


	usb.readData(&header, sizeof(HVACMessageHeader));
	//usb.readData(&checksumProvided, sizeof(checksumProvided));
   	
//    	checksum = calculateChecksum(header);
//    	result = (checksum == checksumProvided);

	notifyGAINQueryReceived( header, result);

	if (result)
		{
		switch (header.command)
			{
// 		#if (kMaintenanceProtocolVersion > 0)
// 			case kGAINQueryProtocolInfo:
// 				result = handleGAINQueryProtocolInfo( header);
// 				break;
// 		#endif
			case kGAINQueryInfo:
				result = handleGAINQueryInfo( header);
				break;

			case kGAINQueryConfiguration:
				result = handleGAINQueryConfiguration( header);
				break;
			case kGAINQueryParameters:
				result = handleGAINQueryConfiguration( header);
				break;

			case kGAINQueryCycleDictionary:
				result = handleGAINQueryCycleDictionary( header);
				break;

			case kGAINQueryCycleDescription:
				result = handleGAINQueryCycleDescription( header);
				break;

			case kGAINQueryFaultLog:
				result = handleGAINQueryFaultLog( header);
				break;

			default:
				result = false;
				break;
			}
		}

	return result;
	}

void Maintenance_Tool::notifyGAINQueryReceived(HVACMessageHeader& header, bool result)
	{
    HVACMessageHeader	reply;


// 	reply.selector = header.command;
// 	reply.command = kGAINNotifyQueryReceived;
// 	reply.dataSize = 0;
	if (!result){
		//reply.selector |= 0x80;
	}
	//checksum = calculateChecksum(reply);
	usb.writeData(&reply, sizeof(reply));
	/*io.writeData(&checksum, sizeof(checksum));*/
	}

bool Maintenance_Tool::handleGAINQueryInfo( HVACMessageHeader& header)
	{
	DeviceInformation	connect;


	header.command = kGAINQueryInfo;
// 	header.selector = kMaintenanceProtocolVersion;
// 	header.dataSize = sizeof(DeviceInformation);

#if PARTNUMBER == 0xBABE
    connect.partNumber = 0xBABE;
   // connect.commons.subPartNumber = 0;
#else
//     connect.partNumber = GAIN.configuration.commons.partNumber;
//     connect.subPartNumber = GAIN.configuration.commons.subPartNumber;
#endif
#ifdef NSD_SUPPORT
// 	strncpy(connect.partNumberString, configurationData.block05.PartNumber, sizeof(configurationData.block05.PartNumber));
// 	connect.partNumberString[15] = 0;
// 	strncpy(connect.serialNumberString, configurationData.block05.SerialNumber, sizeof(configurationData.block05.SerialNumber));
// 	connect.serialNumberString[12] = 0;
// 	strncpy(connect.hardwareVersionString, configurationData.block05.HardwareVersion, sizeof(configurationData.block05.HardwareVersion));
// 	connect.hardwareVersionString[7] = 0;
// 	connect.powerBudget = configurationData.block03.MaximumPowerBudget * 50;	
// 	connect.standbyPowerBudget = configurationData.block03.StandbyPowerThreshold * 5;
#else
	connect.partNumberString[0] = 0;
	connect.serialNumberString[0] = 0;
	connect.hardwareVersionString[0] = 0;
	connect.powerBudget = 0;
	connect.standbyPowerBudget = 0;
#endif
   // strncpy(connect.softwareVersionString, getVersionAsString(), sizeof(connect.softwareVersionString));

// 	checksum = calculateChecksum(header);	
// 	checksum = calculateChecksum(checksum, header.dataSize, &connect);	

	usb.writeData(&header, sizeof(HVACMessageHeader));
	usb.writeData(&connect, sizeof(DeviceInformation));
/*	io.writeData(&checksum, sizeof(checksum));*/

	return result;
	}

#if (kMaintenanceProtocolVersion > 0)
bool Maintenance_Tool::handleGAINQueryProtocolInfo( HVACMessageHeader& header)
	{
// 	protocolInfoRequestPayload	protocolInfo;
// 	uint16	checksum;


// 	header.command = kGAINQueryProtocolInfo;
// 	header.selector = 0;
// 	header.dataSize = sizeof(protocolInfoRequestPayload);
// 	memset(&protocolInfo, 0, sizeof(protocolInfo));
// 	protocolInfo.version = kMaintenanceProtocolVersion;
// 
// 	checksum = calculateChecksum(header);	
// 	checksum = calculateChecksum(checksum, header.dataSize, &version);	
// 
// 	io.writeData(&header, sizeof(GAINMessageHeader));
// 	io.writeData(&connect, sizeof(protocolInfoRequestPayload));
// 	io.writeData(&checksum, sizeof(checksum));

	return result;
	}
#endif
	
bool Maintenance_Tool::handleGAINQueryConfiguration( HVACMessageHeader& header)
	{

// 
// 	header.command = kGAINQueryConfiguration;
// 	
// 	if (header.selector == 0)		//	Factory defaults configuration
// 		{
// 		header.dataSize = sizeof(ConfigurationData);
// 		checksum = calculateChecksum(header);	
// 	#ifdef AVR
// 		ConfigurationData	configuration;
// 
// 		memcpy_P(&configuration, &factoryDefaultsConfiguration, sizeof(ConfigurationData));
// 		checksum = calculateChecksum(checksum, header.dataSize, &configuration);	
// 		
// 		io.writeData(&header, sizeof(GAINMessageHeader));
// 		io.writeData(&configuration, sizeof(ConfigurationData));
// 	#else
// 		checksum = calculateChecksum(checksum, header.dataSize, &factoryDefaultsConfiguration);	
// 		
// 		io.writeData(&header, sizeof(GAINMessageHeader));
// 		io.writeData(&factoryDefaultsConfiguration, sizeof(ConfigurationData));
// 	#endif
// 		io.writeData(&checksum, sizeof(checksum));
// 		}
// 	else if (header.selector == 1)	//	Factory defaults cycle descriptions
// 		{
// 		header.dataSize = sizeof(CycleDictionary) + (kNumberOfCycleDescriptions * sizeof(CycleDescription));
// 		checksum = calculateChecksum(header);	
// 	#ifdef AVR
// 		uint8	i;
// 		
// 		checksum = calculateChecksum(checksum, sizeof(CycleDictionary), &GAIN.cycleDictionary);	
// 
// 		io.writeData(&header, sizeof(GAINMessageHeader));
// 		io.writeData(&GAIN.cycleDictionary, sizeof(CycleDictionary));
// 		
// 		for (i=0; i<kNumberOfCycleDescriptions; i++)
// 			{
// 			CycleDescription	cycle;
// 				
// 			memcpy_P(&cycle, &factoryDefaultsCycles.cycles[i], sizeof(CycleDescription));
// 			checksum = calculateChecksum(checksum, sizeof(CycleDescription), &cycle);	
// 			io.writeData(&cycle, sizeof(CycleDescription));
// 			}
// 	#else
// 		checksum = calculateChecksum(checksum, header.dataSize, &factoryDefaultsCycles);	
// 		
// 		io.writeData(&header, sizeof(GAINMessageHeader));
// 		io.writeData(&factoryDefaultsCycles, sizeof(CycleData));
// 	#endif
// 		io.writeData(&checksum, sizeof(checksum));
// 		}
// 	else if (header.selector == 2)	//	Current configuration
// 		{
// 		header.dataSize = sizeof(ConfigurationData);
// 		checksum = calculateChecksum(header);	
// 		checksum = calculateChecksum(checksum, header.dataSize, &GAIN.configuration);	
// 		
// 		io.writeData(&header, sizeof(GAINMessageHeader));
// 		io.writeData(&GAIN.configuration, sizeof(ConfigurationData));
// 		io.writeData(&checksum, sizeof(checksum));
// 		}
// 	else if (header.selector == 3)	//	Current cycle descriptions
// 		{
// 		uint8	i;
// 
// 		header.dataSize = sizeof(CycleDictionary) + (kNumberOfCycleDescriptions * sizeof(CycleDescription));
// 		checksum = calculateChecksum(header);	
// 		checksum = calculateChecksum(checksum, sizeof(CycleDictionary), &GAIN.cycleDictionary);	
// 
// 		io.writeData(&header, sizeof(GAINMessageHeader));
// 		io.writeData(&GAIN.cycleDictionary, sizeof(CycleDictionary));
// 		
// 		for (i=0; i<kNumberOfCycleDescriptions; i++)
// 			{
// 			CycleDescription	cycle;
// 			
// 			persistent.readCycleDescription(cycle, i);
// 			checksum = calculateChecksum(checksum, sizeof(CycleDescription), &cycle);	
// 			io.writeData(&cycle, sizeof(CycleDescription));
// 			}
// 
// 		io.writeData(&checksum, sizeof(checksum));
// 		}

	return result;
	}
	
bool Maintenance_Tool::handleGAINQueryParameters( HVACMessageHeader& header)
{

	
		 	header.command = kGAINQueryConfiguration;
		//
		// 	if (header.selector == 0)		//	Factory defaults configuration
		// 		{
		// 		header.dataSize = sizeof(ConfigurationData);
		// 		checksum = calculateChecksum(header);
		// 	#ifdef AVR
		// 		ConfigurationData	configuration;
		//
		// 		memcpy_P(&configuration, &factoryDefaultsConfiguration, sizeof(ConfigurationData));
		// 		checksum = calculateChecksum(checksum, header.dataSize, &configuration);
		//
		// 		io.writeData(&header, sizeof(GAINMessageHeader));
		// 		io.writeData(&configuration, sizeof(ConfigurationData));
		// 	#else
		// 		checksum = calculateChecksum(checksum, header.dataSize, &factoryDefaultsConfiguration);
		//
		// 		io.writeData(&header, sizeof(GAINMessageHeader));
		// 		io.writeData(&factoryDefaultsConfiguration, sizeof(ConfigurationData));
		// 	#endif
		// 		io.writeData(&checksum, sizeof(checksum));
		// 		}
		// 	else if (header.selector == 1)	//	Factory defaults cycle descriptions
		// 		{
		// 		header.dataSize = sizeof(CycleDictionary) + (kNumberOfCycleDescriptions * sizeof(CycleDescription));
		// 		checksum = calculateChecksum(header);
		// 	#ifdef AVR
		// 		uint8	i;
		//
		// 		checksum = calculateChecksum(checksum, sizeof(CycleDictionary), &GAIN.cycleDictionary);
		//
		// 		io.writeData(&header, sizeof(GAINMessageHeader));
		// 		io.writeData(&GAIN.cycleDictionary, sizeof(CycleDictionary));
		//
		// 		for (i=0; i<kNumberOfCycleDescriptions; i++)
		// 			{
		// 			CycleDescription	cycle;
		//
		// 			memcpy_P(&cycle, &factoryDefaultsCycles.cycles[i], sizeof(CycleDescription));
		// 			checksum = calculateChecksum(checksum, sizeof(CycleDescription), &cycle);
		// 			io.writeData(&cycle, sizeof(CycleDescription));
		// 			}
		// 	#else
		// 		checksum = calculateChecksum(checksum, header.dataSize, &factoryDefaultsCycles);
		//
		// 		io.writeData(&header, sizeof(GAINMessageHeader));
		// 		io.writeData(&factoryDefaultsCycles, sizeof(CycleData));
		// 	#endif
		// 		io.writeData(&checksum, sizeof(checksum));
		// 		}
		// 	else if (header.selector == 2)	//	Current configuration
		// 		{
		// 		header.dataSize = sizeof(ConfigurationData);
		// 		checksum = calculateChecksum(header);
		// 		checksum = calculateChecksum(checksum, header.dataSize, &GAIN.configuration);
		//
		// 		io.writeData(&header, sizeof(GAINMessageHeader));
		// 		io.writeData(&GAIN.configuration, sizeof(ConfigurationData));
		// 		io.writeData(&checksum, sizeof(checksum));
		// 		}
		// 	else if (header.selector == 3)	//	Current cycle descriptions
		// 		{
		// 		uint8	i;
		//
		// 		header.dataSize = sizeof(CycleDictionary) + (kNumberOfCycleDescriptions * sizeof(CycleDescription));
		// 		checksum = calculateChecksum(header);
		// 		checksum = calculateChecksum(checksum, sizeof(CycleDictionary), &GAIN.cycleDictionary);
		//
		// 		io.writeData(&header, sizeof(GAINMessageHeader));
		// 		io.writeData(&GAIN.cycleDictionary, sizeof(CycleDictionary));
		//
		// 		for (i=0; i<kNumberOfCycleDescriptions; i++)
		// 			{
		// 			CycleDescription	cycle;
		//
		// 			persistent.readCycleDescription(cycle, i);
		// 			checksum = calculateChecksum(checksum, sizeof(CycleDescription), &cycle);
		// 			io.writeData(&cycle, sizeof(CycleDescription));
		// 			}
		//
		// 		io.writeData(&checksum, sizeof(checksum));
		// 		}

	return result;
}

bool Maintenance_Tool::handleGAINQueryCycleDictionary( HVACMessageHeader& header)
	{

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

bool Maintenance_Tool::handleGAINQueryCycleDescription( HVACMessageHeader& header)
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
bool Maintenance_Tool::handleGAINQueryFaultLog( HVACMessageHeader& header)
	{
	int16			i;
	int16			n;
	//faultLogHeader	faultHeader;	

// 
// #ifdef NO_PERSISTENCE
// 	faultHeader.numberOfFaults = (kGAINErrorCount - 1);//0x80;
// 	faultHeader.currentFaultIndex = kGAINErrorCount;
// 	faultHeader.error.errorID = 1;
// 	faultHeader.error.annotation = 0;
// 	faultHeader.error.timestamp = 0;
// 	faultHeader.error.totalUpTime = 0;
// #else
// 	faultHeader.numberOfFaults = persistent.getErrorCount();
// 	faultHeader.currentFaultIndex = persistent.getCurrentErrorIndex();
// 	persistent.getError(-1, faultHeader.error);
// #endif
// 
// 	if (faultHeader.numberOfFaults > kPersistentErrorAreaSize)
// 		faultHeader.numberOfFaults = kPersistentErrorAreaSize;
// 
// 	n = faultHeader.numberOfFaults;
// 
//     //Send header first
// 	header.command = kGAINQueryFaultLog;
// 	header.dataSize = sizeof(faultLogHeader);
// 	checksum = calculateChecksum(header);
// 	checksum = calculateChecksum(checksum, sizeof(faultLogHeader), &faultHeader);
// 
// 	io.writeData(&header, sizeof(header));
// 	io.writeData(&faultHeader, sizeof(faultHeader));
// 	io.writeData(&checksum, sizeof(checksum));
// 
//     //Send error entries
// 	header.command = kGAINQueryFaultLogEntry;
// 	header.selector = 0;
// 
// 	for (i=0; i<n; i++)
// 		{
// 	#ifdef NO_PERSISTENCE
// 		errorDescription	description;
// 	
// 		memcpy_P(&description, &GAINErrorDescription[i + 1], sizeof(description));
// 
// 		faultHeader.currentFaultIndex = i;
// 		faultHeader.error.errorID = description.errorID;
// 		faultHeader.error.annotation = 0;
// 		faultHeader.error.timestamp = (tick_t)i*1000;
// 		faultHeader.error.totalUpTime = (uint32)i*250;
// 	#else
// 		persistent.getError(i, faultHeader.error);
// 	#endif
// 		header.dataSize = sizeof(errorLogEntry);
// 
// 		checksum = calculateChecksum(header);	
// 		checksum = calculateChecksum(checksum, header.dataSize, &faultHeader.error);	
// 
// 		io.writeData(&header, sizeof(GAINMessageHeader));
// 		io.writeData(&faultHeader.error, header.dataSize);
// 		io.writeData(&checksum, sizeof(checksum));
// 		}

	return result;
	}

#else

bool Maintenance_Tool::handleGAINQueryFaultLog( HVACMessageHeader& header)
	{
	int16			i;
	int16			n;
/*	faultLogHeader	faultHeader;	*/

// 
// #ifdef NO_PERSISTENCE
// 	faultHeader.numberOfFaults = kGAINErrorCount;//0x80;
// 	faultHeader.currentFaultIndex = kGAINErrorCount;
// 	faultHeader.error.errorID = 1;
// 	faultHeader.error.annotation = 0;
// 	faultHeader.error.timestamp = 0;
// 	faultHeader.error.totalUpTime = 0;
// #else
// 	faultHeader.numberOfFaults = persistent.getErrorCount();
// 	faultHeader.currentFaultIndex = persistent.getCurrentErrorIndex();
// 	persistent.getError(-1, faultHeader.error);
// 	persistent.getErrorCounter(faultHeader.errorCounter);
// #endif
// 	if (faultHeader.numberOfFaults <= kPersistentErrorAreaSize)
// 		n = faultHeader.numberOfFaults;
// 	else
// 		n = kPersistentErrorAreaSize;
// 
// 	header.command = kGAINQueryFaultLog;
// 	header.dataSize = sizeof(faultLogHeader);
// 	checksum = calculateChecksum(header);
// 	checksum = calculateChecksum(checksum, sizeof(faultLogHeader), &faultHeader);
// 
// 	io.writeData(&header, sizeof(header));
// 	io.writeData(&faultHeader, sizeof(faultHeader));
// 	io.writeData(&checksum, sizeof(checksum));
// 
// 	return result;
// 	}
// 
// bool Maintenance_Tool::handleGAINQueryFaultLogEntry(stream& io, GAINMessageHeader& header, int16 id)
// 	{
// 	faultLogHeader	faultHeader;	
// 	uint16	checksum;
// 	bool	result(true);
// 
// 	header.command = kGAINQueryFaultLogEntry;
// 	header.selector = 0;
// 
// #ifdef NO_PERSISTENCE
// 	errorDescription	description;
// 	
// 	memcpy_P(&description, &GAINErrorDescription[id], sizeof(description));
// 
// 	faultHeader.currentFaultIndex = id;
// 	faultHeader.error.errorID = description.errorID;
// 	faultHeader.error.annotation = 0;
// 	faultHeader.error.timestamp = (tick_t)id*1000;
// 	faultHeader.error.totalUpTime = (uint32)id*250;
// #else
// 	persistent.getError(id, faultHeader.error);
// #endif
// 	header.dataSize = sizeof(errorLogEntry);
// 
// 	checksum = calculateChecksum(header);	
// 	checksum = calculateChecksum(checksum, header.dataSize, &faultHeader.error);	
// 
// 	io.writeData(&header, sizeof(GAINMessageHeader));
// 	io.writeData(&faultHeader.error, header.dataSize);
// 	io.writeData(&checksum, sizeof(checksum));

	return result;
	}

#endif

uint16 Maintenance_Tool::calculateChecksum(const HVACMessageHeader& header)
	{
	checksum=0;
	
	checksum = calculateChecksum(0, sizeof(header), &header);

	return checksum;
	}

uint16 Maintenance_Tool::calculateChecksum(uint16 checksum, uint16 size, const void* data)
	{
	uint16	i;
	
	for (i=0; i<(size/2); i++)
		checksum ^= ((uint16*)data)[i];
	
	if (size & 0x01)
		checksum ^= (uint16)((uint8*)data)[size - 1];

	return checksum;
	}
	
uint32_t	Maintenance_Tool::SendPeriodicNotifications(void){
	if (ptrMaintenanceTool->bSendNotifications)
	{
		nextMaintenanceSyncTime = ticks + 500;
		
		//GAINNotifyState(uint8_t state=0, flags);
		GAINNotifyControllerState();
		
		if (controllerState > 0)
		{
			GAINNotifyAnalogueValues();
			
			if (ticks >= (uint32_t)nextMaintenanceUsageTimerSyncTime)
			{
				nextMaintenanceUsageTimerSyncTime = ticks + 5000;
// 				GAINNotifyUsageData();
// 				GAINNotifyTemperatureData();
			}
		}
	}
	return	ticks;
}

void Maintenance_Tool::GetCPUSerialNumber(uint8_t* buffer)
{
	uint8_t i;
	uint32_t add=0x41002018;
	for (i = 0; i < 4; i++)
	{
		memcpy(buffer+4*i, (uint8_t*)(add+4*i), 4);
	}

}
 Maintenance_Tool toolApp;