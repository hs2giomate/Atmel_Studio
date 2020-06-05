/*--------------------------------------------------------------------------

NSDHandler.cc

Implementation
NSD-Protokoll

$Author: steffen $
$Date: 2018-06-20 15:20:29 +0200 (Mi, 20. Jun 2018) $
$Revision: 7932 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Network/NSDHandler.cc $

--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT

	#ifdef AVR
	//#undef __DEBUG__
	#endif

	#ifndef __DEBUG__
	#undef DEBUG_NSD
	#endif

	#if defined(AVR)||defined(MGS_ZST_Revision)
		#include "coreCAN.h"
		#include "coreNVM.h"
		#include "eDevice.h"
		#include "eDeviceEvent.h"
		#if PARTNUMBER != 0xBABE
			#include "PersistenceServices.h"
		#endif

		#ifdef DEBUG_NSD
			#define debugStream	controller.asc0
		#endif
	#endif
#include "NSDHandler.h"

NSDHandler	NSD;

uint16 nvmBaseAddress;

const uint16 daysCumulatedPerYear[4][12] =
	{
    	{   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
    	{ 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
    	{ 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
    	{1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
	};

const uint16	daysPer4Years = (365*4+1);
 
/*
https://codereview.stackexchange.com/questions/38275/convert-between-date-time-and-time-stamp-without-using-standard-library-routines

typedef struct
{
    unsigned char second; // 0-59
    unsigned char minute; // 0-59
    unsigned char hour;   // 0-23
    unsigned char day;    // 1-31
    unsigned char month;  // 1-12
    unsigned char year;   // 0-99 (representing 2000-2099)
}
date_time_t;

static unsigned short days[4][12] =
{
    {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
    { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
    { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
    {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
};


unsigned int date_time_to_epoch(date_time_t* date_time)
{
    unsigned int second = date_time->second;  // 0-59
    unsigned int minute = date_time->minute;  // 0-59
    unsigned int hour   = date_time->hour;    // 0-23
    unsigned int day    = date_time->day-1;   // 0-30
    unsigned int month  = date_time->month-1; // 0-11
    unsigned int year   = date_time->year;    // 0-99
    return (((year/4*(365*4+1)+days[year%4][month]+day)*24+hour)*60+minute)*60+second;
}


void epoch_to_date_time(date_time_t* date_time,unsigned int epoch)
{
    date_time->second = epoch%60; epoch /= 60;
    date_time->minute = epoch%60; epoch /= 60;
    date_time->hour   = epoch%24; epoch /= 24;

    unsigned int years = epoch/(365*4+1)*4; epoch %= 365*4+1;

    unsigned int year;
    for (year=3; year>0; year--)
    {
        if (epoch >= days[year][0])
            break;
    }

    unsigned int month;
    for (month=11; month>0; month--)
    {
        if (epoch >= days[year][month])
            break;
    }

    date_time->year  = years+year;
    date_time->month = month+1;
    date_time->day   = epoch-days[year][month]+1;
}
*/

extern "C" {
tick_t getControllerTicksHandler(void);
bool CANWriteMessageHandler(CANMessage*);
void readGAINConfigurationDataHandler(int16 adr, int16 n, void* destPtr);
void writeGAINConfigurationDataHandler(int16 adr, int16 n, const void* destPtr);
void networkStateChangedHandler(NetworkStateChangeBits, uint8);
void powerControlStateChangedHandler(uint16 powerBudgetMax, uint16 time);
void initiateSelftestHandler(uint8);

tick_t getControllerTicksHandler(void)
	{
#if defined(AVR)||defined(MGS_ZST_Revision)
	return controller.ticks();
#else
	return controller::ticks();
#endif
	}
	
bool CANWriteMessageHandler(CANMessage* message)
	{
	bool	result(true);
	
	result = coreCANWriteMessage(kCAN0, message);

#if DEBUG_NSD >  4
	if (!result)
		{
		debugStream.lock();
		debugStream << "CANWriteMessageHandler: " << (result?"done":"failed") << newline;
		debugStream.unlock();
		}
#endif
	return result;
	}

/*
	GAIN configuration data starts at 0x100 offset
*/	
void readGAINConfigurationDataHandler(int16 adr, int16 n, void* destPtr)
	{
#if DEBUG_NSD > 4
	debugStream.lock();
	debugStream << "readGAINConfigurationDataHandler: adr ==" << adr <<", n == " << n << ", nvbase == " << nvmBaseAddress << newline;
	debugStream.unlock();
#endif
#if defined(AVR)||defined(MGS_ZST_Revision)
#if defined(E_DEVICE_CAN)||defined(NO_PERSISTENCE)
	coreNVMRead(kNVM0, adr + nvmBaseAddress, destPtr, n);
#elif defined(MGS_ZST_Revision)
	coreNVMRead(kNVM2, adr + nvmBaseAddress, destPtr, n);
#else
	coreNVMRead(kNVM2, adr + nvmBaseAddress, destPtr, n);
#endif
#else
	coreEEPROMReadData(adr + nvmBaseAddress, destPtr, n);
#endif
	}

void writeGAINConfigurationDataHandler(int16 adr, int16 n, const void* sourcePtr)
	{
#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "writeGAINConfigurationDataHandler: adr ==" << adr <<", n == " << n << ", nvbase == " << nvmBaseAddress << newline;
	debugStream.unlock();
#endif
#if defined(AVR)||defined(MGS_ZST_Revision)
#if defined(E_DEVICE_CAN)||defined(NO_PERSISTENCE)
	coreNVMWrite(kNVM0, adr + nvmBaseAddress, sourcePtr, n);
#elif defined(MGS_ZST_Revision)
	coreNVMWrite(kNVM2, adr + nvmBaseAddress, sourcePtr, n);
#else
	coreNVMWrite(kNVM2, adr + nvmBaseAddress, sourcePtr, n);
#endif
#else
	coreEEPROMWriteData(adr + nvmBaseAddress, sourcePtr, n);
#endif
	}

void networkStateChangedHandler(NetworkStateChangeBits bits , uint8 value)
	{
#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "networkStateChangedHandler" << newline;
	debugStream.unlock();
#endif
	switch (bits)
		{
		case kMGCUPresentChanged:
			sendEventSelf(kNSDEventClass, (value?kNSDMGCUConnectionEstablished:kNSDMGCUConnectionLost), 0);
			break;

		case kCentralizedPowerAvailableChanged:
			sendEventSelf(kNSDEventClass, (value?kNSDCentralizedPowerEstablished:kNSDCentralizedPowerLost), 0);
			break;

		case kDisinfectionModeChanged:
			sendEventSelf(kNSDEventClass, (value?kNSDDisinfectionModeStarted:kNSDDisinfectionModeFinished), 0);
			break;

		case kPinCodingError:
			sendEventSelf(kNSDEventClass, kNSDPinProgrammingError, value);
			break;
		
		default:
			break;
		};
	}
	
void powerControlStateChangedHandler(uint16 powerBudgetMax, uint16 time)
	{
#if DEBUG_NSD > 0
	debugStream.lock();
	debugStream << "powerBudgetMax == " << powerBudgetMax << newline;
	debugStream << "time == " << time << newline;
	debugStream.unlock();
#endif
	sendEventSelf(kNSDEventClass, kNSDPowerBudgetChanged, eventData(powerBudgetMax, time));
	}

void selfTestRequestHandler(uint8 command)
	{
#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "initiateSelftestHandler" << newline;
	debugStream.unlock();
#endif
	sendEventSelf(kNSDEventClass, kNSDSelftestRequest, command);
	}
};
	

NSDHandler::NSDHandler(void)
	:flightInfoUTCValue(0), NSDStartupTime(forever), gainFeederID(0), gainDeviceID(0), gainFallbackType(kType_Secondary),
	currentGAINMode(kMode_Undefined), currentGAINState(kState_Undefined)
	{
	}

void NSDHandler::initializeHandler(uint16 baseAddr)
	{
	coreNSDConfigurationStruct	configureStruct;
	
	nvmBaseAddress = baseAddr;
	
	configureStruct.getControllerTicksCallback = getControllerTicksHandler;
	configureStruct.CANWriteMessageCallback = CANWriteMessageHandler;
	configureStruct.readGAINConfigurationDataCallback = readGAINConfigurationDataHandler;
	configureStruct.writeGAINConfigurationDataCallback = writeGAINConfigurationDataHandler;
	configureStruct.networkStateChangedCallback = networkStateChangedHandler;
	configureStruct.powerControlStateChangedCallback = powerControlStateChangedHandler;
	configureStruct.selfTestRequestCallback = selfTestRequestHandler;

#if DEBUG_NSD > 3
	debugStream << "initializeHandler: baseAddr == " << kModeHex << baseAddr << newline;
#endif
 	if (coreNSD_initialize(&configureStruct))
		{
		InhibitTimerDT = coreNSD_convertToSystemEndian16(configurationData.block09.InhibitTimerDT);
		SequenceTimeoutTimer = coreNSD_convertToSystemEndian16(configurationData.block09.SequenceTimeoutTimer);
		TimerSTA_GAIN = coreNSD_convertToSystemEndian16(configurationData.block09.TimerSTA_GAIN);
		TimeoutTimerSTA_MGCU = coreNSD_convertToSystemEndian16(configurationData.block09.TimeoutTimerSTA_MGCU);
		TimerPHSM_GAIN = coreNSD_convertToSystemEndian16(configurationData.block09.TimerPHSM_GAIN);
		// ???
		configurationData.block02.GAINImplementedFunctionalities |= kGAIN_CentralizedPowerControl;		
		isPowerControlCapable = (configurationData.block02.GAINImplementedFunctionalities & kGAIN_CentralizedPowerControl);		
	#if DEBUG_NSD > 3
		if (isPowerControlCapable)
			debugStream << "Power Control capable" << newline;
		else
			debugStream << "No Power Control" << newline;
	#endif
		
	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "coreNSD_initialize success:" << newline;
		debugStream << "configurationData.magic = " << configurationData.magic<< newline;
		debugStream << "configurationData.size = " << configurationData.size<< newline;
		debugStream << "InhibitTimerDT = " << InhibitTimerDT<< newline;
		debugStream << "SequenceTimeoutTimer = " << SequenceTimeoutTimer<< newline;
		debugStream << "TimerSTA_GAIN = " << TimerSTA_GAIN<< newline;
		debugStream << "TimeoutTimerSTA_MGCU = " << TimeoutTimerSTA_MGCU<< newline;
		debugStream << "TimerPHSM_GAIN = " << TimerPHSM_GAIN<< newline;

		debugStream << "Version: " << (uint8)configurationData.block05.SoftwareVersion[2] << '.' << (uint8)configurationData.block05.SoftwareVersion[3]
									 << " build " << *((uint16*)&configurationData.block05.SoftwareVersion) << newline;

		debugStream << "size of configurationData == " << sizeof(configurationData) << newline;
		debugStream << "size of block #0 == " << sizeof(GAINDataBlock00) << newline;
		debugStream << "size of block #1 == " << sizeof(GAINDataBlock01) << newline;
		debugStream << "size of block #2 == " << sizeof(GAINDataBlock02) << newline;
		debugStream << "size of block #3 == " << sizeof(GAINDataBlock03) << newline;
		debugStream << "size of block #4 == " << sizeof(GAINDataBlock04) << newline;
		debugStream << "size of block #5 == " << sizeof(GAINDataBlock05) << newline;
		debugStream << "size of block #6 == " << sizeof(GAINDataBlock06) << newline;
		debugStream << "size of block #8 == " << sizeof(GAINDataBlock08) << newline;
		debugStream << "size of block #9 == " << sizeof(GAINDataBlock09) << newline;
		debugStream << "size of block #12 == " << sizeof(GAINDataBlock12) << newline;
		debugStream.unlock();
	#endif
		}
	else
		{
	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "coreNSD_initialize failed!";
		debugStream << newline;
		debugStream.unlock();
		controller.wait(10000);
	#endif
		}

	setGetControllerTicksHandler(getControllerTicksHandler);
	setCANWriteMessageHandler(CANWriteMessageHandler);
	setReadGAINConfigurationDataHandler(readGAINConfigurationDataHandler);
	setWriteGAINConfigurationDataHandler(writeGAINConfigurationDataHandler);
	setNetworkStateChangedHandler(networkStateChangedHandler);
	setPowerControlStateChangedHandler(powerControlStateChangedHandler);
    setSelfTestRequestHandler(selfTestRequestHandler);
    
	mgcuState.bits.isPresent = false;
	mgcuState.bits.centralizedPowerAvailable = false;
	mgcuState.bits.desinfectionMode = false;

	mgcuState.timeoutCounter = configurationData.block12.MGCUCounterThreshold;
	mgcuState.lastLifepulse = 0;

	gainFallbackType = configurationData.block05.FallbackType;
	gainFeederID = configurationData.block05.FeederID;
	gainDeviceID  = configurationData.block05.DeviceID;

	for (uint8 i=kSTA_GAIN; i<kNumberOfGAINMessages; i++)
		{
		messages[i].setMessageIdentifier((GAINMessage)i);
		messages[i].setDeviceID(gainDeviceID);
		for (uint8 j=0; j<8; j++)
			messages[i].message.data[j] = 0;
		}

	messages[kSTA_GAIN].setGAINFeeder_ID(gainFeederID);
	messages[kSTA_GAIN].setGAINGAIN_Is_Primary(getGAINIsPrimary());
	messages[kSTA_GAIN].setGAINFallback_Power_Budget(50*configurationData.block03.FallbackPowerThreshold);
	messages[kGAIN_PT].setGAINPTFallbackPowerThreshold(50*configurationData.block03.FallbackPowerThreshold);

    currentGAINMode = kMode_Undefined;
	currentGAINState = kState_Undefined;
	
    sendNextPBRTime = forever;

	dataTransferState = 0;

	messages[kSTA_GAIN].setGAINImpl_Functionalities(configurationData.block02.GAINImplementedFunctionalities);
	messages[kSTA_GAIN].setGAINUsed_Power_Budget(5*configurationData.block03.StandbyPowerThreshold);

	messages[kGAIN_PT].setGAINPTStandbyPowerThreshold(5*configurationData.block03.StandbyPowerThreshold);
	messages[kGAIN_PT].setGAINPTMaximumPowerThreshold(50*configurationData.block03.MaximumPowerBudget);
	messages[kGAIN_PT].setGAINPTMinimumPowerThreshold(50*configurationData.block03.MinimumPowerBudget);

	setGAINMode(kMode_Fallback, false);
	setGAINState(kState_Initialize, false);

	//	Default second counter
	coreSystemTimerSetSeconds(persistent.getUptime());
    NSDStartupTime = getControllerTicksCallback();
    
#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "sizeof(configurationData) == " << sizeof(configurationData);
	debugStream << newline;
	debugStream.unlock();
#endif
	}	

bool NSDHandler::connectionStateDetermined(void)
	{
	return (getControllerTicksCallback() - NSDStartupTime) >= (TimeoutTimerSTA_MGCU * configurationData.block12.MGCUCounterThreshold);
	}

uint16 NSDHandler::setPinCode(uint32 pinCode)
	{
	uint16	result(0);
	uint8	pinProgrammingErrorState;

    pinProgrammingErrorState = (uint8)(pinCode & 0xFF);
	
	if (!networkStateChangedCallback)
		return kNSDDiagNoHandler;

#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "setPinCode: " << kModeHex << pinCode << ", error state: " << kModeHex << pinProgrammingErrorState << newline;
	debugStream.unlock();
#endif
	if ((pinCode != configurationData.block00.pinCode) || pinProgrammingErrorState)
		{
		uint8	i;
		
		configurationData.block00.pinCode = pinCode;		
		coreNSD_flushGAINConfigurationDataBlock(0);

		gainFeederID = (uint8)((pinCode >> 24) & 0xFF);
		gainDeviceID = (uint8)((pinCode >> 16) & 0xFF);

		if ((pinCode >> 8) & 0x01)
			{
	    	gainFallbackType = kType_Primary;
	    	if (configurationData.block03.FallbackPowerThreshold != configurationData.block03.MaximumPowerBudget)
	    		{
				configurationData.block03.FallbackPowerThreshold = configurationData.block03.MaximumPowerBudget;
				//	Save values in NVM
				coreNSD_flushGAINConfigurationDataBlock(3);
				}
	    	}
		else
			{
	    	gainFallbackType = kType_Secondary;
	    	if (configurationData.block03.FallbackPowerThreshold != ((5*configurationData.block03.StandbyPowerThreshold)+49)/50)
	    		{
				configurationData.block03.FallbackPowerThreshold = ((5*configurationData.block03.StandbyPowerThreshold)+49)/50;
				//	Save values in NVM
				coreNSD_flushGAINConfigurationDataBlock(3);
				}
	    	}

		if ((configurationData.block05.FallbackType != gainFallbackType)
			||(configurationData.block05.FeederID != gainFeederID)
			||(configurationData.block05.DeviceID != gainDeviceID))
			{
			configurationData.block05.FallbackType = gainFallbackType;
			configurationData.block05.FeederID = gainFeederID;
			configurationData.block05.DeviceID = gainDeviceID;

			//	Save values in NVM
			coreNSD_flushGAINConfigurationDataBlock(5);
			}

		if (gainDeviceID == 0)
			{
			pinProgrammingErrorState |= 0x08;
			}

	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "setPinCode: " << kModeHex << pinCode;
		debugStream << ": Feeder ID: " << gainFeederID;
		debugStream << ", Device ID: " << gainDeviceID;
		if (gainFallbackType == kType_Primary)
			debugStream << ", Primary";
		else
			debugStream << ", Secondary";
		debugStream << newline;
	#endif
		//	Pin coding error?
		//	Yes:
		if (pinProgrammingErrorState)
			{
		#if DEBUG_NSD > 3
			debugStream << ", pin programming error ";
		#endif
			if (pinProgrammingErrorState & 0x08)
				{
			#if DEBUG_NSD > 3
				debugStream << "invalid device ID ";
			#endif
				addDIAGCode(kNSDDiagInvalidDeviceID);
				result = kNSDDiagInvalidDeviceID;
				}

			if (pinProgrammingErrorState & 0x01)
				{
			#if DEBUG_NSD > 3
				debugStream << "feeder ID ";
			#endif
				gainFeederID = 0;
				addDIAGCode(kNSDDiagPinProgrammingFeederID);
				result = kNSDDiagPinProgrammingFeederID;
				}

			if (pinProgrammingErrorState & 0x02)
				{
			#if DEBUG_NSD > 3
				debugStream << "device ID ";
			#endif
				gainDeviceID = 0;
				addDIAGCode(kNSDDiagPinProgrammingDeviceID);
				result = kNSDDiagPinProgrammingDeviceID;
				}

			if (pinProgrammingErrorState & 0x04)
				{
			#if DEBUG_NSD > 3
				debugStream << "primary/secondary ";
			#endif
	    		gainFallbackType = kType_Secondary;
				configurationData.block03.FallbackPowerThreshold = ((5*configurationData.block03.StandbyPowerThreshold)+49)/50;
				addDIAGCode(kNSDDiagPinProgrammingPrimary);
				result = kNSDDiagPinProgrammingPrimary;
				}

			if (pinProgrammingErrorState & 0x10)
				{
			#if DEBUG_NSD > 3
				debugStream << "parity";
			#endif
				gainDeviceID = 0;
				gainFeederID = 0;
	    		gainFallbackType = kType_Secondary;
				configurationData.block03.FallbackPowerThreshold = ((5*configurationData.block03.StandbyPowerThreshold)+49)/50;
				addDIAGCode(kNSDDiagPinProgrammingParityError);
				result = kNSDDiagPinProgrammingParityError;
				}
			}
		//	No:
	#if DEBUG_NSD > 3
		else
			{
			debugStream << ", pin programming okay";
			}
	#endif			

	#if DEBUG_NSD > 3
		debugStream << newline;
		debugStream.unlock();
	#endif

		messages[kSTA_GAIN].setGAINFeeder_ID(gainFeederID);
		messages[kSTA_GAIN].setGAINGAIN_Is_Primary(getGAINIsPrimary());
		messages[kSTA_GAIN].setGAINFallback_Power_Budget(50*configurationData.block03.FallbackPowerThreshold);
		messages[kGAIN_PT].setGAINPTFallbackPowerThreshold(50*configurationData.block03.FallbackPowerThreshold);

		for (i=kSTA_GAIN; i<kNumberOfGAINMessages; i++)
			{
			messages[i].setDeviceID(gainDeviceID);
			}

		networkStateChangedHandler(kPinCodingError, pinProgrammingErrorState);
		}

	return result;
	}

uint16 NSDHandler::resume(uint32 pinCode, GAINMode mode, GAINState state, uint16 usedPower, tick_t timeRemaining)
	{
	uint16	result(setPinCode(pinCode));
	
	currentGAINMode = mode;
	messages[kSTA_GAIN].setGAINMode(mode);
	
	if (mode == kMode_Centralized)
		{
		mgcuState.bits.isPresent = true;
		messages[kSTA_GAIN].setGAINMGCU_Is_Present(true);
		mgcuState.bits.centralizedPowerAvailable = true;
		mgcuState.timeoutCounter = 0;
		mgcuState.lastLifepulse = getControllerTicksCallback();
		}

	currentGAINState = state;
	messages[kSTA_GAIN].setGAINState(state);

	if (usedPower)
		{
		messages[kSTA_GAIN].setGAINUsed_Power_Budget(usedPower);
		}
	else
		{
		messages[kSTA_GAIN].setGAINUsed_Power_Budget(5 * configurationData.block03.StandbyPowerThreshold);
		}

	if (timeRemaining != forever)
		messages[kSTA_GAIN].setGAINPB_Remaining_Time((timeRemaining + 999)/1000);
	else
		messages[kSTA_GAIN].setGAINPB_Remaining_Time(0xFFFF);

	return result;
	}

void NSDHandler::handlePeriodicTask(MGCUStateBits& mgcu)
	{
	uint8	i;
	tick_t	now(coreSystemTimerTicks());
	tick_t	deltaT;

#if __DEBUG__ > 4
	static tick_t	nextLogEntry;
	static uint32	lastSR;
		
	if (nextLogEntry < coreSystemTimerTicks())
		{
		uint32	SR(coreCANGetSR(kCAN0));
			
		if (lastSR != SR)
			{
			lastSR = SR;
			controller.asc0 << stream::time2string(now, 1) << ": CAN0 status " << kModeHex << SR << newline;
			nextLogEntry = now + 1000;
			}
		}
#endif

	if (!networkStateChangedCallback)
		return;

	setPHSMValueTEC(coreCANGetTEC(kCAN0));
	setPHSMValueREC(coreCANGetREC(kCAN0));

	setPHSMValueNumberOfTransmitErrorsDetected(0);
	setPHSMValueNumberOfReceiveErrorsDetected(0);

	if (coreCANGetBusOffState(kCAN0))
		{
		static tick_t	nextCANResetTime;
		if (nextCANResetTime < now)
			{
			nextCANResetTime = now + 3000;
		#if __DEBUG__ > 1
			controller.asc0 << "CAN0 checkCAN: bus OFF" << newline;
		#endif
			coreCANResetBusOffState(kCAN0);
		#if __DEBUG__ > 1
			controller.asc0 << "CAN0 reset counter: " << coreCANGetResetCounter(kCAN0) << newline;
		#endif
			setPHSMValueNumberOfCANResets(coreCANGetResetCounter(kCAN0));
			}
		}
	else if (coreCANDataAvailable(kCAN0) > 0)
		{
		CANMessage	message;
		
	#if __DEBUG__ > 4
		controller.asc0 << "CAN0 message(s) available: " << coreCANDataAvailable(kCAN0) << " messages" << newline;
	#endif
        coreCANReadMessage(kCAN0, &message, 1);
        messageReceived(message);
		}

	if (currentGAINState > kState_Initialize)
		{
		for (i=kSTA_GAIN; i<=kPHSM; i++)
			{
			if (messages[i].message.isPending)
				{
				now = getControllerTicksCallback();
				deltaT = now - messages[i].message.timestamp;

				if (deltaT >= GAINMessageInhibitTimes[i])
					sendNSDMessage(i);
				}
			}
		}

	if (dataTransferState == 2)
		handleDataDownload();

	now = getControllerTicksCallback();
	deltaT = now - mgcuState.lastLifepulse;
	
    if (sendNextPBRTime <= now)
    	{
		sendNSDMessage(kPBR);
		sendNextPBRTime = now + (2 * TimeoutTimerSTA_MGCU);
	#if DEBUG_NSD > 0
		debugStream.lock();
		debugStream << "Resend PBR: now == " << now << ", sendNextPBRTime == " << sendNextPBRTime << newline;
		debugStream.unlock();
	#endif
		}	

	if ((dataTransferState != 0) && (now > dataTransferAbortionTime))
		{
		dataTransferState = 0;
		addDIAGCode(kNSDDiagDataTransmissionInterrupted);
		sendDIAG(false);
		removeDIAGCode(kNSDDiagDataTransmissionInterrupted);
		
	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "Data transfer aborted: timeout" << newline;
		debugStream.unlock();
	#endif
		}

	if (deltaT > TimeoutTimerSTA_MGCU)
		{
		if (mgcuState.timeoutCounter < configurationData.block12.MGCUCounterThreshold)
			mgcuState.timeoutCounter += configurationData.block12.MGCUCounterDeltaInc;

		mgcuState.lastLifepulse = now;
		}

	if (mgcuState.timeoutCounter >= configurationData.block12.MGCUCounterThreshold)
		{
		/**
			NSD_ATA2536_NM004-1
		*/
		if (mgcuState.bits.isPresent)
			{
			messages[kSTA_GAIN].setGAINMGCU_Is_Present(false);

			mgcuState.bits.isPresent = false;
            mgcuState.bits.centralizedPowerAvailable = false;
			mgcuState.timeoutCounter = configurationData.block12.MGCUCounterThreshold;

			//	Callback to Application Code
			networkStateChangedCallback(kCentralizedPowerAvailableChanged, false);
			networkStateChangedCallback(kMGCUPresentChanged, false);
			}	
		}
	else
		{
		/**
			NSD_ATA2536_NM004-2
		*/
		if (!mgcuState.bits.isPresent && (mgcuState.timeoutCounter <= 0))
			{
			messages[kSTA_GAIN].setGAINMGCU_Is_Present(true);
			mgcuState.bits.isPresent = true;
			mgcuState.timeoutCounter = 0;

			/*
				Send GAIN_PT on GNC detection (NSD Part 2 Issue 5)
				NSD350-V-25-36-639
			*/
			sendNSDMessage(kGAIN_PT);
			//	Callback to Application Code
			networkStateChangedCallback(kMGCUPresentChanged, true);	

			//	Centralized Power available!
			if (mgcuState.bits.centralizedPowerAvailable)
				{
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "Go to centralized!" << newline;
				debugStream.unlock();
			#endif
				//	Callback to Application Code
				networkStateChangedCallback(kCentralizedPowerAvailableChanged, true);	
				}
			}	
		}

	now = getControllerTicksCallback();
	deltaT = now - messages[kSTA_GAIN].message.timestamp;
	/**
		NSD_ATA2536_NM006-1
		STA_GAIN
	*/
#if DEBUG_NSD > 4
	debugStream << "handlePeriodicTask: deltaT STA_GAIN == " << deltaT << newline;
#endif
	if (deltaT >= TimerSTA_GAIN)
		{
	#if DEBUG_NSD > 4
		debugStream << "handlePeriodicTask: deltaT STA_GAIN == " << deltaT << newline;
	#endif
		sendNSDMessage(kSTA_GAIN);
		}
	
	/**
		NSD350-V-25-36-108s
		PHSC_GAIN
	*/
#if DEBUG_NSD > 4
	debugStream << "handlePeriodicTask: deltaT PHSM == " << deltaT << newline;
#endif
	deltaT = now - messages[kPHSM].message.timestamp;
	if (deltaT >= TimerPHSM_GAIN)
		{
	#if DEBUG_NSD > 4
		debugStream << "handlePeriodicTask: deltaT PHSM == " << deltaT << newline;
	#endif
		sendNSDMessage(kPHSM);
		}
	
	mgcu = mgcuState.bits;
	}

void NSDHandler::setGAINMode(GAINMode v, bool sync)
	{
	if (v != currentGAINMode)
		{
		currentGAINMode = v;
		messages[kSTA_GAIN].setGAINMode(v);
		/**
			NSD_ATA2536_NM006-2 (TBD!)
		*/
		if (sync)
			sendNSDMessage(kSTA_GAIN);
		}
	}

void NSDHandler::setGAINFallbackPowerBudget(uint16 v, bool sync)
	{
	messages[kSTA_GAIN].setGAINFallback_Power_Budget(v);

#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "setGAINFallbackPowerBudget(" << v << ")" << newline;
	debugStream.unlock();
#endif
	}

void NSDHandler::setGAINState(GAINState v, bool sync)
	{
	if (v != currentGAINState)
		{
		currentGAINState = v;
		messages[kSTA_GAIN].setGAINState(v);
		/**
			NSD_ATA2536_NM006-3 (TBD!)
		*/
		if (sync)
			sendNSDMessage(kSTA_GAIN);
			
		}
	}

void NSDHandler::setGAINPowerBudgetUsed(uint16 v, bool sync)
	{
	bool	powerBudgetChanged(false);
	
	if (v > 0)
		{
		if (messages[kSTA_GAIN].getGAINUsed_Power_Budget()/50 != ((v+49)/50))
			{
			messages[kSTA_GAIN].setGAINUsed_Power_Budget(v);
			powerBudgetChanged = true;
			}
		}
	else
		{
		if (messages[kSTA_GAIN].getGAINUsed_Power_Budget() != ((uint16)5 * configurationData.block03.StandbyPowerThreshold))
			{
			messages[kSTA_GAIN].setGAINUsed_Power_Budget(5 * configurationData.block03.StandbyPowerThreshold);
			powerBudgetChanged = true;
			}
		}
	/**
		NSD_ATA2536_NM006-4 (TBD!)
	*/
	if (powerBudgetChanged && sync)
		{
		sendNSDMessage(kSTA_GAIN);
	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "setGAINPowerBudgetUsed(" << v << ")" << newline;
		debugStream.unlock();
	#endif
		}
	}

void NSDHandler::setGAINRemainingTime(tick_t v, bool sync)
	{
	uint16	remainingTime;
	bool	remainingTimeChanged;
	
	if (v != forever)
		remainingTime = (uint16)((v + 999)/1000);
	else
		remainingTime = 0xFFFF;
		
	remainingTimeChanged = (messages[kSTA_GAIN].getGAINPB_Remaining_Time() != remainingTime);
	messages[kSTA_GAIN].setGAINPB_Remaining_Time(remainingTime);
	
	if (sync && remainingTimeChanged)
		{
		sendNSDMessage(kSTA_GAIN);
	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "setGAINRemainingTime(" << v << ")" << newline;
		debugStream.unlock();
	#endif
		}
	}

void NSDHandler::syncGAINStatus(void)
	{
	sendNSDMessage(kSTA_GAIN);
	}

void NSDHandler::setPHSMValueNumberOfReceiveErrorsDetected(uint16 v)
	{
	static uint16	numberOfReceiveErrorsDetected;
	
	if (v != numberOfReceiveErrorsDetected)
		{
	#if DEBUG_NSD > 2
		controller.asc0 << "setPHSMValueNumberOfReceiveErrorsDetected: " << v << newline;
	#endif
		numberOfReceiveErrorsDetected = v;
		messages[kPHSM].message.data[0] = v >> 8;
		messages[kPHSM].message.data[1] = v & 0xFF;
		}
	}

void NSDHandler::setPHSMValueNumberOfTransmitErrorsDetected(uint16 v)
	{
	static uint16	numberOfTransmitErrorsDetected;
	
	if (v != numberOfTransmitErrorsDetected)
		{
	#if DEBUG_NSD > 2
		controller.asc0 << "setPHSMValueNumberOfTransmitErrorsDetected: " << v << newline;
	#endif
		numberOfTransmitErrorsDetected = v;
		messages[kPHSM].message.data[2] = v >> 8;
		messages[kPHSM].message.data[3] = v & 0xFF;
		}
	}

void NSDHandler::setPHSMValueNumberOfCANResets(uint16 v)
	{
	static uint16	numberOfCANResets;
	
	if (v != numberOfCANResets)
		{
	#if DEBUG_NSD > 2
		controller.asc0 << "setPHSMValueNumberOfCANResets: " << v << newline;
	#endif
		numberOfCANResets = v;
		messages[kPHSM].message.data[4] = v >> 8;
		messages[kPHSM].message.data[5] = v & 0xFF;
		}
	}

void NSDHandler::setPHSMValueREC(uint8 v)
	{
	messages[kPHSM].message.data[6] = v;
	}

void NSDHandler::setPHSMValueTEC(uint8 v)
	{
	messages[kPHSM].message.data[7] = v;
	}

void NSDHandler::sendPBR(uint16 nominal, int16 nominalTime, uint16 minimal, int16 minimalTime)
	{
	messages[kPBR].setPBRPowerBudgetType(true);

	messages[kPBR].setPBRNominalPowerBudget(nominal);
	messages[kPBR].setPBRNeededTimeWithNominalPB(nominalTime);

	messages[kPBR].setPBRMinimalPowerBudget(minimal);
	messages[kPBR].setPBRNeededTimeWithMinimalPB(minimalTime);

	if (sendNextPBRTime == forever)
		{
		sendNSDMessage(kPBR);
		sendNextPBRTime = getControllerTicksCallback() + (2 * TimeoutTimerSTA_MGCU);
	#if DEBUG_NSD > 0
		debugStream.lock();
		debugStream << "sendPBR(" << nominal << ", " << nominalTime << ", " << minimal << ", " << minimalTime << ")" << newline;
	#if DEBUG_NSD > 1
		debugStream << "sendNextPBRTime == " << sendNextPBRTime << ", now == " << getControllerTicksCallback() << newline;
	#endif
		debugStream.unlock();
	#endif
		}
#if DEBUG_NSD > 0
	else
		{
		debugStream.lock();
		debugStream << "PBR pending" << newline;
		debugStream.unlock();
		}
#endif
	}

void NSDHandler::sendPBR(uint16 nominal, tick_t nominalTime, uint16 minimal, tick_t minimalTime)
	{
	int16	nominalSeconds;
	int16	minimalSeconds;
	
	if (nominalTime != forever)
		nominalSeconds = (nominalTime + 999)/1000;
	else
		nominalSeconds = 0xFFFF;
	
	if (minimalTime != forever)
		minimalSeconds = (minimalTime + 999)/1000;
	else
		minimalSeconds = 0xFFFF;

	sendPBR(nominal, nominalSeconds, minimal, minimalSeconds);
	}

void NSDHandler::cancelPBR(void)
	{
	sendNextPBRTime = forever;
	}

void NSDHandler::sendDIAG(bool force)
	{
#if DEBUG_NSD > 2
	debugStream.lock();
	debugStream << "sendDIAG(" << (force?"true":"false") << ")" << newline;
	debugStream.unlock();
#endif
	if (force || messages[kDIAG_FAILURE].getDIAGMessageDirtyBit())
		{
		messages[kDIAG_FAILURE].setDIAGMessageDirtyBit(false);
		sendNSDMessage(kDIAG_FAILURE);
	#if DEBUG_NSD > 2
		debugStream.lock();
		debugStream << "sendNSDMessage(kDIAG_FAILURE)" << newline;
		debugStream.unlock();
	#endif
		}

	if (messages[kDIAG_MALFUNCTION].getDIAGMessageDirtyBit())
		{
		messages[kDIAG_MALFUNCTION].setDIAGMessageDirtyBit(false);
		sendNSDMessage(kDIAG_MALFUNCTION);
	#if DEBUG_NSD > 2
		debugStream.lock();
		debugStream << "sendNSDMessage(kDIAG_MALFUNCTION)" << newline;
		debugStream.unlock();
	#endif
		}

	if (messages[kDIAG_WARNING].getDIAGMessageDirtyBit())
		{
		messages[kDIAG_WARNING].setDIAGMessageDirtyBit(false);
		sendNSDMessage(kDIAG_WARNING);
	#if DEBUG_NSD > 2
		debugStream.lock();
		debugStream << "sendNSDMessage(kDIAG_WARNING)" << newline;
		debugStream.unlock();
	#endif
		}
	}

void NSDHandler::addDIAGCode(NSDDiagCode v)
	{
#if DEBUG_NSD > 2
	debugStream.lock();
	debugStream << "addDIAGCode #" << (uint8)v << newline;
	debugStream.unlock();
#endif
	if (v > kNSDDiagNoHandler)
		{
	#ifdef AVR
		NSDDiag	error;
		memcpy_P(&error, &NSDDiagList[v], sizeof(error));
		uint8	diagBit(error.diagBit);
		uint8	diagClassification(error.diagClassification);
	#else
		uint8	diagBit(NSDDiagList[v].diagBit);
		uint8	diagClassification(NSDDiagList[v].diagClassification);
	#endif
		switch (diagClassification)
			{
			case kDIAGWarning:
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "Warning: bit #" << diagBit << newline;
				debugStream.unlock();
			#endif
				messages[kDIAG_WARNING].addDIAG_WarningBit((NSDWarningBit)diagBit);

				messages[kDIAG_MALFUNCTION].setWarningPendingBit(true);
				messages[kDIAG_FAILURE].setWarningPendingBit(true);
				break;
				
			case kDIAGMalfunction:
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "Malfunction: bit #" << diagBit << newline;
				debugStream.unlock();
			#endif
				messages[kDIAG_MALFUNCTION].addDIAG_MalfunctionBit((NSDMalfunctionBit)diagBit);

				messages[kDIAG_WARNING].setMalfunctionPendingBit(true);
				messages[kDIAG_FAILURE].setMalfunctionPendingBit(true);
				break;
				
			case kDIAGFailure:
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "Failure: bit #" << diagBit << newline;
				debugStream.unlock();
			#endif
				messages[kDIAG_FAILURE].addDIAG_FailureBit((NSDFailureBit)diagBit);

				messages[kDIAG_WARNING].setFailurePendingBit(true);
				messages[kDIAG_MALFUNCTION].setFailurePendingBit(true);
				break;
			}
		}
	}

void NSDHandler::removeDIAGCode(NSDDiagCode v)
	{
#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "removeDIAGCode(" << (uint8)v << ")";
	debugStream.unlock();
#endif
	if (v>=kNSDDiagCateringProcessQualityDamaged)
		{
	#ifdef AVR
		NSDDiag	error;
		memcpy_P(&error, &NSDDiagList[v], sizeof(error));
		uint8	diagBit(error.diagBit);
		uint8	diagClassification(error.diagClassification);
	#else
		uint8	diagBit(NSDDiagList[v].diagBit);
		uint8	diagClassification(NSDDiagList[v].diagClassification);
	#endif
		switch (diagClassification)
			{
			case kDIAGWarning:
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "Warning: bit #" << diagBit << newline;
				debugStream.unlock();
			#endif
				messages[kDIAG_WARNING].removeDIAG_WarningBit((NSDWarningBit)diagBit);

				messages[kDIAG_MALFUNCTION].setWarningPendingBit(messages[kDIAG_WARNING].getWarningPendingBit());
				messages[kDIAG_FAILURE].setWarningPendingBit(messages[kDIAG_WARNING].getWarningPendingBit());
				break;
				
			case kDIAGMalfunction:
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "Malfunction: bit #" << diagBit << newline;
				debugStream.unlock();
			#endif
				messages[kDIAG_MALFUNCTION].removeDIAG_MalfunctionBit((NSDMalfunctionBit)diagBit);

				messages[kDIAG_WARNING].setMalfunctionPendingBit(messages[kDIAG_MALFUNCTION].getMalfunctionPendingBit());
				messages[kDIAG_FAILURE].setMalfunctionPendingBit(messages[kDIAG_MALFUNCTION].getMalfunctionPendingBit());
				break;
				
			case kDIAGFailure:
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "Failure: bit #" << diagBit << newline;
				debugStream.unlock();
			#endif
				messages[kDIAG_FAILURE].removeDIAG_FailureBit((NSDFailureBit)diagBit);

				messages[kDIAG_WARNING].setFailurePendingBit(messages[kDIAG_FAILURE].getFailurePendingBit());
				messages[kDIAG_MALFUNCTION].setFailurePendingBit(messages[kDIAG_FAILURE].getFailurePendingBit());
				break;
			}
		}
	}

void NSDHandler::clearDIAG(void)
	{
#if DEBUG_NSD > 3
	debugStream.lock();
	debugStream << "clearDIAG" << newline;
	debugStream.unlock();
#endif
	messages[kDIAG_WARNING].clearDIAG();
	messages[kDIAG_MALFUNCTION].clearDIAG();
	messages[kDIAG_FAILURE].clearDIAG();
	}

void NSDHandler::messageReceived(const CANMessage& msg)
	{
#if DEBUG_NSD > 4
	debugStream.lock();
	debugStream << "messageReceived(const CANMessage& msg)" << newline;
	debugStream.unlock();
#endif
	if (lastMessage.checkAndSetCANMessage(gainDeviceID, msg))
		{
	#if DEBUG_NSD > 0
		logNSDMessage(lastMessage, false);
	#endif
		switch (lastMessage.getMessageCode())
			{
			case kMessageCode_STA_MGCU:
				handleSTA_MGCU(lastMessage);
				break;

			case kMessageCode_STA_GAIN:
				break;

			case kMessageCode_PBM:
				if (CentralizedPowerControlAvailable())
					handlePBM(lastMessage);
				break;

			case kMessageCode_FLIGHT_INFO:
				handleFLIGHT_INFO(lastMessage);
				break;

			case kMessageCode_DIAG_CMD:
				handleDIAG_CMD(lastMessage);
				break;

		#ifdef CCOTF_DEMONSTRATOR
			case kMessageCode_CAN_BRIDGE_REMOTE:
				handleCAN_BRIDGE_REMOTE(lastMessage);
				break;
		#endif

			case kMessageCode_DT:
				if (currentGAINState != kState_Error)
					{
					if (lastMessage.getPVT() && lastMessage.getSMT())
						{
						handleDataUpload(lastMessage);
						}
					else if (lastMessage.getSMT())
						{
						if (lastMessage.message.data[1] == 2)
							handleDataUploadRequest(lastMessage);
						else if (lastMessage.message.data[1] == 3)
							handleDataDownloadRequest(lastMessage);
						}
					}
				break;

			default:
				break;
			}
		}
	#if DEBUG_NSD > 3
	else
		{
		debugStream.lock();
		debugStream << "Message rejected: id == " << kModeHex << msg.identifier;
		debugStream << ",: code == " << coreNSD_getMessageCode(msg.identifier);
		debugStream << ", LCC == " << coreNSD_getLCC(msg.identifier);
		debugStream << ", FID == " << coreNSD_getSourceFID(msg.identifier) << newline;
		debugStream << ", device ID == " << coreNSD_getDeviceID(msg.identifier) << newline;
		debugStream.unlock();
		}
	#endif
	}

void NSDHandler::sendNSDMessage(uint8 idx)
	{
	if (currentGAINState > kState_Initialize)
		{
		tick_t	deltaT(getControllerTicksCallback() - messages[idx].message.timestamp);
	
		if (deltaT < GAINMessageInhibitTimes[idx])
			{
			messages[idx].message.isPending = true;
		#if DEBUG_NSD > 4
			debugStream.lock();
			debugStream << "Message pending: idx == " << idx << ", code == " << coreNSD_getMessageCode(messages[idx].message.identifier);
			debugStream << ", deltaT == " << deltaT;
			debugStream << ", GAINMessageInhibitTimes == " << GAINMessageInhibitTimes[idx];
			debugStream << ", timestamp == " << messages[idx].message.timestamp << newline;
			debugStream << "***** ";
			debugStream.unlock();
			logNSDMessage(messages[idx], true);
		#endif
			}
		else
			{
			if (CANWriteMessageCallback)
				{
				if (CANWriteMessageCallback(&messages[idx].message))
					{
					messages[idx].message.isPending = false;
					messages[idx].message.timestamp = getControllerTicksCallback();
				#if DEBUG_NSD > 2
					logNSDMessage(messages[idx], true);
					messages[idx].message.timestamp = getControllerTicksCallback();
				#endif
					}
				else
					{
					messages[idx].message.isPending = true;
				#if DEBUG_NSD > 4
					debugStream.lock();
					debugStream << "CAN transmission failed! Message pending." << newline;
					debugStream.lock();
				#endif
					}
				}
			}
		}
	else if (idx == kGAIN_PT)
		messages[idx].message.isPending = true;
	else if (idx == kDIAG_WARNING)
		messages[idx].message.isPending = true;
	else if (idx == kDIAG_MALFUNCTION)
		messages[idx].message.isPending = true;
	else if (idx == kDIAG_FAILURE)
		messages[idx].message.isPending = true;
	}

#if DEBUG_NSD
void NSDHandler::logNSDMessage(const NSDMessage& message, bool direction)
	{
#if DEBUG_NSD > 4
	if (currentGAINState == kState_Maintenance)
		return;

	const char*	prolog;
	
	if (direction)
		prolog = "<< ";
	else
		prolog = ">> ";
		
	debugStream.lock();
	debugStream << prolog << stream::time2string(message.message.timestamp, true) << " ";

#if DEBUG_NSD > 4
	debugStream << prolog << "message code == " << message.getMessageCode() << ", LCC == " << coreNSD_getLCC(message.message.identifier);
	debugStream << ", FID == " << coreNSD_getSourceFID(message.message.identifier);
	debugStream << ", Device ID == " <<  message.getDeviceID() << newline;
	debugStream << prolog;
	debugStream << "Message: " << kModeHex << message.message.identifier << newline;
	debugStream << prolog;
    debugStream << "DLC: " << message.message.dataLengthCode << ", data:";
    for (uint8 i=0; i<message.message.dataLengthCode; i++)
    	debugStream << ' ' << kModeHex << message.message.data[i];
	debugStream << newline << prolog;
#endif

	switch (message.getMessageCode())
		{
		case kMessageCode_STA_MGCU:
			debugStream << "STA_MGCU";
		#if DEBUG_NSD > 4
			debugStream << ", state: " << message.getMGCUState();
			debugStream << ", disinfection: " << (message.getMGCUDisinfection_Mode()?"on":"off");
			debugStream << ", implemented: " << message.getMGCUImplemented_Functionalities();
			if  (message.getMGCUImplemented_Functionalities() & kMGCU_CentralizedPowerControl)
				debugStream << ", centralized power control";
			else
				debugStream << ", NO centralized power control";
			debugStream << ", feeder: " << message.getMGCUPower_Info_Feeder();
			debugStream << ", centralized " << (message.getMGCUCentral_Power_Valid_On_Feeder()?"valid":"invalid");
		#endif
			break;
			
		case kMessageCode_STA_GAIN:
				debugStream << "STA_GAIN";
		#if DEBUG_NSD > 4
				debugStream << ", Device ID == " <<  message.getDeviceID();
				debugStream << ", Feeder ID == " <<  message.getGAINFeeder_ID();
				debugStream << ", " << (message.getGAINGAIN_Is_Primary()?"primary":"secondary");
				debugStream << ", " << (message.getGAINMGCU_Is_Present()?"MGCU":"--");
				debugStream << ", mode: " << message.getGAINMode();
				debugStream << ", state: " << message.getGAINState();
				debugStream << ", implemented: " << kModeHex << message.getGAINImpl_Functionalities();
				debugStream << ": " << ((message.getGAINImpl_Functionalities() & kGAIN_CentralizedPowerControl)?"CPC":"No CPC");
				debugStream << " " << ((message.getGAINImpl_Functionalities() & kGAIN_DecentralizedPowerControl)?"DCPC":"No DCPC");
				debugStream << " " << ((message.getGAINImpl_Functionalities() & kGAIN_Diagnostics)?"DIAG":"No DIAG");
				debugStream << " " << ((message.getGAINImpl_Functionalities() & kGAIN_FallbackPowerMode)?"Fallback":"No Fallback");
				debugStream << " " << ((message.getGAINImpl_Functionalities() & kGAIN_DataTranfer)?"DT":"No DT");

				if (message.getGAINState() == kState_ON)
					{
					debugStream << ", remaining: " << message.getGAINPB_Remaining_Time();
					debugStream << ", fallback: " << message.getGAINFallback_Power_Budget() << " VA";
					debugStream << ", used: " << message.getGAINUsed_Power_Budget() << " VA";
					}
		#endif
			break;
			
		case kMessageCode_PBR:
			debugStream << "PBR";
			debugStream << ", nominal: " << message.getPBRNominalPowerBudget();
			debugStream << ", time: " << message.getPBRNeededTimeWithNominalPB();
			debugStream << ", minimal: " << message.getPBRMinimalPowerBudget();
			debugStream << ", time: " << message.getPBRNeededTimeWithMinimalPB();
			break;
			
		case kMessageCode_PBM:
			debugStream << "PBM";
			debugStream << ", Device ID == " <<  message.getDeviceID();
			debugStream << ", budget: " << message.getPBMPowerBudgetMax();
			debugStream << ", time: " << message.getPBMTime();
			break;
			
		case kMessageCode_DIAG_FAILURE:
			debugStream << "DIAG_FAILURE";
			break;
			
		case kMessageCode_DIAG_MALFUNCTION:
			debugStream << "DIAG_MALFUNCTION";
			break;
			
		case kMessageCode_DIAG_WARNING:
			debugStream << "DIAG_WARNING";
			break;
			
		case kMessageCode_SES_GAIN:
			debugStream << "SES_GAIN";
			break;
			
		case kMessageCode_FLIGHT_INFO:
			debugStream << "FLIGHT_INFO";
			break;
			
		case kMessageCode_GAIN_PT:
			debugStream << "GAIN_PT";
			break;
			
		case kMessageCode_DIAG_CMD:
			debugStream << "DIAG_CMD";
			break;
			
		case kMessageCode_PHSM:
			debugStream << "PHSM";
			break;
			
		case kMessageCode_DT:
		#if DEBUG_NSD > 3
			if (message.getPVT())
				{
				debugStream << "Data Transfer: Data Packet";
				}
			else if (message.getSMT())
				{
				if (message.message.data[1] == 2)
					{
					if (message.message.data[2] == 1)
						debugStream << "Data Transfer: Data Upload Request #2";
					else
						debugStream << "Data Transfer: Data Upload Request";
					}
				else if (message.message.data[1] == 3)
					{
					if (message.message.data[2] == 1)
						{
						if (message.message.dataLengthCode == 3)
							debugStream << "Data Transfer: Data Download Acknowledge";
						else
							debugStream << "Data Transfer: Data Download Request #2";
						}
					else
						debugStream << "Data Transfer: Data Download Request";
					}
				else
					debugStream << "Data Transfer: PVT: ??????";
				}
			else
				{
				if (message.message.data[1] == 2)
					{
					if (message.message.dataLengthCode == 3)
						debugStream << "Data Transfer: Data Upload Response #2";
					else
						debugStream << "Data Transfer: Data Upload Response";
					}
				else if (message.message.data[1] == 3)
					{
					if (message.message.data[3] == 1)
						debugStream << "Data Transfer: Data Download Response";
					else
						debugStream << "Data Transfer: Data Download Response #2";
					}
				else
					debugStream << "Data Transfer: No PVT: ??????";
				}
			#endif
			break;
		
		default:
			debugStream << "Unknown message code!" << newline;
			debugStream << prolog << "message code == " << message.getMessageCode() << ", LCC == " << coreNSD_getLCC(message.message.identifier);
			debugStream << ", FID == " << coreNSD_getSourceFID(message.message.identifier);
			debugStream << ", Device ID == " <<  message.getDeviceID() << newline;
			debugStream << prolog;
			debugStream << "Message: " << kModeHex << message.message.identifier << newline;
			debugStream << prolog;
		    debugStream << "DLC: " << message.message.dataLengthCode << ", data:";
		    for (uint8 i=0; i<message.message.dataLengthCode; i++)
		    	debugStream << ' ' << kModeHex << message.message.data[i];
			break;
		}

	debugStream << newline;
	debugStream.unlock();
#endif
	}
#endif

void NSDHandler::handleSTA_MGCU(const NSDMessage& message)
	{
	if (!networkStateChangedCallback)
    	return;

	mgcuState.lastLifepulse = getControllerTicksCallback();
	if (mgcuState.timeoutCounter > 0)
		mgcuState.timeoutCounter -= configurationData.block12.MGCUCounterDeltaDec;


	if (message.getMGCUDisinfection_Mode())
		{
		if (!mgcuState.bits.desinfectionMode)
			{
			mgcuState.bits.desinfectionMode = true;
			networkStateChangedCallback(kDisinfectionModeChanged, true);	
			}
		}
	else
		{
		if (mgcuState.bits.desinfectionMode)
			{
			mgcuState.bits.desinfectionMode = false;
			networkStateChangedCallback(kDisinfectionModeChanged, false);	
			}
		}
	
	if (message.getMGCUImplemented_Functionalities() & kMGCU_CentralizedPowerControl)
		{
		if (message.getMGCUPower_Info_Feeder() == gainFeederID)
			{
			if (message.getMGCUCentral_Power_Valid_On_Feeder())
				{
				if (isPowerControlCapable && !mgcuState.bits.centralizedPowerAvailable)
					{
					mgcuState.bits.centralizedPowerAvailable = true;
					//	Centralized Power available!
					if (mgcuState.bits.isPresent)
						{
						// Callback
						networkStateChangedCallback(kCentralizedPowerAvailableChanged, true);	
						}
					}
				}
			else
				{
				if (mgcuState.bits.centralizedPowerAvailable)
					{
					mgcuState.bits.centralizedPowerAvailable = false;
					//	Centralized Power lost!
					if (mgcuState.bits.isPresent)
						{
						networkStateChangedCallback(kCentralizedPowerAvailableChanged, false);
						cancelPBR();	
						}
					}
				}
			}
		}
	else if (mgcuState.bits.centralizedPowerAvailable)
		{
		mgcuState.bits.centralizedPowerAvailable = false;
		//	Centralized Power lost!
		if (mgcuState.bits.isPresent)
			{
			networkStateChangedCallback(kCentralizedPowerAvailableChanged, false);	
			}
		}
	}

void NSDHandler::handlePBM(const NSDMessage& message)
	{
	if (!powerControlStateChangedCallback)
		return;
		
#if DEBUG_NSD > 0
	debugStream.lock();
	debugStream << "handlePBM: time == " << message.getPBMTime() << ", power == " << message.getPBMPowerBudgetMax() << ", GAIN state == " << currentGAINState << newline;
	debugStream.unlock();
#endif
	if (message.getPBMPowerBudgetMax() == 0)
		{
		if ((currentGAINState == kState_ON)||(currentGAINState == kState_StoppedProcess))
			{
			if (message.getPBMTime() == 0)
				{
			#if DEBUG_NSD > 0
				debugStream.lock();
				debugStream << "handlePBM: catering state aborted. Will wait for " << message.getPBMTime() << " seconds until next PBR" << newline;
				debugStream.unlock();
			#endif
				powerControlStateChangedCallback(0, message.getPBMTime());
				sendNextPBRTime = getControllerTicksCallback() + ((tick_t)message.getPBMTime() * 1000L);
				}
		#if DEBUG_NSD > 0
			else
				{
				debugStream.lock();
				debugStream << "handlePBM: malformed PBM 0 received: PBM time ==  " << message.getPBMTime() << " seconds. Ignoring." << newline;
				debugStream.unlock();
				}
		#endif
			}
		else if (currentGAINState == kState_Interrupted)
			{
			powerControlStateChangedCallback(0, message.getPBMTime());
			sendNextPBRTime = getControllerTicksCallback() + ((tick_t)message.getPBMTime() * 1000L);
			}
		else if (currentGAINState == kState_Waiting)
			{
			sendNextPBRTime = getControllerTicksCallback() + ((tick_t)message.getPBMTime() * 1000L);
			}
		}
	else
		{
		if (((currentGAINState == kState_Interrupted)||(currentGAINState == kState_Waiting))
			&& (message.getPBMPowerBudgetMax() >= messages[kPBR].getPBRMinimalPowerBudget()))
			{
			sendNextPBRTime = forever;
			powerControlStateChangedCallback(message.getPBMPowerBudgetMax(), message.getPBMTime());
			}
		}
	}

/*
(
	(
		(
		year/4*daysPer4Years+daysCumulatedPerYear[year%4][month]+day
		)*24+hour
	)*60+minute
)*60+second;
*/
void NSDHandler::handleFLIGHT_INFO(const NSDMessage& message)
	{
	uint32	utc;
	uint8	second = (message.message.data[5] >> 2);
	uint8	minute = (message.message.data[4] >> 2);
	uint8	hour = (message.message.data[3] >> 3);
	uint8	day = (message.message.data[2] >> 3);
	uint8	month = (message.message.data[1] >> 4);
	uint8	year = message.message.data[0];

    utc = (uint32)(year/4) * daysPer4Years;
    utc += daysCumulatedPerYear[year%4][month - 1];
    utc += day - 1;
    utc *= 24UL;
    utc += hour;
    utc *= 60UL;
    utc += minute;
    utc *= 60UL;
    utc += second;
	utc += 946684800;	// 1.1.2000 00:00:00
	coreSystemTimerSetSeconds(utc);
/*

    return (((year/4*daysPer4Years+daysCumulatedPerYear[year%4][month]+day)*24+hour)*60+minute)*60+second;
}
	uint16	dateValue;
	uint16	timeValue;
	
	dateValue = message.message.data[0];			//	Year
	dateValue <<= 7;
	dateValue |= message.message.data[1];			//	Month
	dateValue <<= 4;
	dateValue |= message.message.data[2];			//	Day
	
	timeValue = message.message.data[3];			//	Hour
	timeValue <<= 5;
	timeValue |= message.message.data[4];			//	Minute
	timeValue <<= 6;
	timeValue |= (message.message.data[5] >> 1);	//	Seconds
	
	flightInfoUTCValue = dateValue;
	flightInfoUTCValue <<= 16;
	flightInfoUTCValue |= timeValue;
*/	
#if DEBUG_NSD > 1
	char	timeString[] = "28.12.2000 14:30:55";

	timeString[0] = day/10 + '0';
	timeString[1] = day%10 + '0';
	timeString[3] = month/10 + '0';
	timeString[4] = month%10 + '0';
	timeString[8] = year/10 + '0';
	timeString[9] = year%10 + '0';
	timeString[11] = hour/10 + '0';
	timeString[12] = hour%10 + '0';
	timeString[14] = minute/10 + '0';
	timeString[15] = minute%10 + '0';
	timeString[17] = second/10 + '0';
	timeString[18] = second%10 + '0';

	debugStream.lock();
	debugStream << "handleFLIGHT_INFO: " << timeString << ", UTC == " << utc << newline;
	debugStream.unlock();
#endif
	}

void NSDHandler::handleDIAG_CMD(const NSDMessage& message)
	{
	if (currentGAINState != kState_Initialize)
		{
		if (!selfTestRequestCallback)
			return;
	
		switch (message.message.data[0])
			{
			case 1:
			case 2:
				selfTestRequestCallback(message.message.data[0]);
				break;
		
			default:
				break;
			}
		}
	}

#ifdef CCOTF_DEMONSTRATOR
void NSDHandler::handleCAN_BRIDGE_REMOTE(const NSDMessage& message)
	{
#if DEBUG_NSD > 2
	debugStream.lock();
	debugStream << "handleCAN_BRIDGE_REMOTE" << newline;
	debugStream.unlock();
#endif
	}
#endif

void NSDHandler::handleDataUploadRequest(const NSDMessage& message)
	{
	dataTransferAbortionTime = getControllerTicksCallback() + SequenceTimeoutTimer;	
	messages[kDT_RES].message.data[1] = 0x02;	//	Service Function Code 2
	if (message.message.data[2] == 1)	//	Data upload request #2
		{
		messages[kDT_RES].message.dataLengthCode = 3;
		//	Data Upload Resonse #2
		if (dataTransferState == -1)
			{
			//	Data upload request #2 in order = ACK
			messages[kDT_RES].message.data[2] = 1;
			dataTransferState = -2;
		#if DEBUG_NSD > 2
			debugStream.lock();
			debugStream << "handleDataUploadRequest: data upload request #2: ACK" << newline;
			debugStream.unlock();
		#endif
			}
		else
			{
			//	Data upload request #2 out of order => NACK
			messages[kDT_RES].message.data[2] = (uint8)-1;
			dataTransferState = 0;
		#if DEBUG_NSD > 2
			debugStream.lock();
			debugStream << "handleDataUploadRequest: data upload request #2: NACK" << newline;
			debugStream.unlock();
		#endif
			}						
		}	
	else	//	Data upload request
		{
		messages[kDT_RES].message.dataLengthCode = 8;
		if (dataTransferState == 0)
			{
			//	Data upload request in order
			dataBlockNumber = (message.message.data[2] & 0x7F);
			dataBlockSize = 0;
			//	dataBlockSize = (message.message.data[4]) << 24;
            //	dataBlockSize |= (uint32)message.message.data[5] << 16;
            dataBlockSize |= (uint32)message.message.data[6] << 8;
            dataBlockSize |= (uint32)message.message.data[7];

            dataBlockSize -= 4;

			if ((dataBlockNumber == 8) && (GAINConfigurationDataBlockOffsetTable[dataBlockNumber] != 0xFFFF))
				{
			#if DEBUG_NSD > 2
				debugStream.lock();
				debugStream << "handleDataUploadRequest:" << newline;
				debugStream << "requested == " << dataBlockSize << " byte" << newline;
				debugStream << "expected  == " << GAINConfigurationDataBlockSizeTable[dataBlockNumber] + 4 << " byte" << newline;
				debugStream.unlock();
			#endif
				if ((GAINConfigurationDataBlockSizeTable[dataBlockNumber]) == dataBlockSize)
					{
					//	Valid data block number: Data Upload Resonse
					messages[kDT_RES].message.data[2] = 1;
					messages[kDT_RES].message.data[3] = 50;
					messages[kDT_RES].message.data[4] = (dataBlockSize + 4) >> 8;	//	sizeof(dataUploadReceiveBuffer) >> 8;
					messages[kDT_RES].message.data[5] = (dataBlockSize + 4) & 0xFF;	//	sizeof(dataUploadReceiveBuffer) & 0xFF;
					messages[kDT_RES].message.data[6] = 0x02;	//	MSB 512 msec block seperation time
					messages[kDT_RES].message.data[7] = 0x00;	//	LSB 512 msec block seperation time
					dataTransferState = -1;
					dataBlockDataIndex = 0;
					//	dataBlockOffset = GAINConfigurationDataBlockOffsetTable[dataBlockNumber];
					dataTransferBuffer = (uint8*)coreNSD_getGAINConfigurationDataBlockAdress(dataBlockNumber);
					}
				else
					{
					/*
						NSD350-V-25-36-810
						Invalid data block size
					*/
					messages[kDT_RES].message.data[2] = (uint8)-3;
					dataTransferState = 0;
				#if DEBUG_NSD > 3
					debugStream.lock();
					debugStream << "handleDataUploadRequest: data upload request: invalid data block size" << newline;
					debugStream.unlock();
				#endif
					}
				}
			else
				{
				/*
					NSD350-V-25-36-809
					Invalid data block number != 8
				*/
				messages[kDT_RES].message.data[2] = (uint8)-2;
				dataTransferState = 0;
			#if DEBUG_NSD > 3
				debugStream.lock();
				debugStream << "handleDataUploadRequest: data upload request: invalid data block" << newline;
				debugStream.unlock();
			#endif
				}
			}
		else
			{
			//	Data upload request out of order => NACK
			messages[kDT_RES].message.data[2] = (uint8)-1;
			dataTransferState = 0;
		#if DEBUG_NSD > 3
			debugStream.lock();
			debugStream << "handleDataUploadRequest: data upload request: out of order" << newline;
			debugStream.unlock();
		#endif
			}						
		}

	sendNSDMessage(kDT_RES);	
	}

void NSDHandler::handleDataUpload(const NSDMessage& message)
	{
	uint8	i;

	dataTransferAbortionTime = getControllerTicksCallback() + SequenceTimeoutTimer;	
	if (dataTransferState == -2)
		{
		for (i=0; (i<8) && (dataBlockDataIndex < dataBlockSize); i++)
			dataTransferBuffer[dataBlockDataIndex++] = message.message.data[i];	//	copy data

		for (; (i<8) && (dataBlockDataIndex < (dataBlockSize + 4)); i++)
			CRCBuffer[dataBlockDataIndex++ - dataBlockSize] = message.message.data[i];			//	copy CRC
			
	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "handleDataUpload: received " << dataBlockDataIndex << " of " << dataBlockSize + 4 << " byte" << newline;
		debugStream << newline;
		debugStream.unlock();
	#endif
		if (dataBlockDataIndex == (dataBlockSize + 4))
			{
			uint32	crcIn = (uint32)CRCBuffer[0] << 24|(uint32)CRCBuffer[1] << 16|(uint32)CRCBuffer[2] << 8|(uint32)CRCBuffer[3];
			uint32	crc = coreNSD_calculateCRC32(dataTransferBuffer, dataBlockSize);

		#if DEBUG_NSD > 3
			debugStream.lock();
			debugStream << "handleDataUpload: CRC == " << kModeHex << crc << ", CRC in == " << kModeHex << crcIn << newline;
			debugStream.unlock();
		#endif
			messages[kDT_RES].message.dataLengthCode = 3;
			messages[kDT_RES].message.data[1] = 0x02;	//	Service Function Code 2

			if (crc == crcIn)	//	Check CRC!
				{
			#if DEBUG_NSD > 3
				debugStream.lock();
				debugStream << "handleDataUpload: done, CRC okay" << newline;
				debugStream.unlock();
			#endif
				coreNSD_flushGAINConfigurationDataBlock(dataBlockNumber);
				//	coreNSD_writeGAINConfigurationData(dataBlockOffset, dataBlockSize, dataUploadReceiveBuffer);
				messages[kDT_RES].message.data[2] = 1;
				sendNSDMessage(kDT_RES);
				}
			else
				{
				coreNSD_refreshGAINConfigurationDataBlock(dataBlockNumber);
			#if DEBUG_NSD > 3
				debugStream.lock();
				debugStream << "handleDataUpload: done, CRC error" << newline;
				debugStream.unlock();
			#endif
				messages[kDT_RES].message.data[2] = (uint8)-1;
				sendNSDMessage(kDT_RES);
				addDIAGCode(kNSDDiagDataTransmissionError);
				sendDIAG(false);
				clearDIAG();
				}

			dataTransferState = 0;	
			}
		}
	}

void NSDHandler::handleDataDownloadRequest(const NSDMessage& message)
	{
	dataTransferAbortionTime = getControllerTicksCallback() + SequenceTimeoutTimer;	
	messages[kDT_RES].message.data[1] = 0x03;	//	Service Function Code 3
	if ((message.message.data[2] == 1) && (message.message.dataLengthCode > 3))	//	Data download request #2
		{
		messages[kDT_RES].message.dataLengthCode = 8;
		if (dataTransferState == 1)
			{
			minimumMessageSeperationTime = message.message.data[3];
			transferBlockSize = ((uint16)message.message.data[4]) << 8;
			transferBlockSize |= (uint16)message.message.data[5];
			minimumBlockSeperationTime = ((uint16)message.message.data[6]) << 8;
			minimumBlockSeperationTime |= (uint16)message.message.data[7];

			if (minimumMessageSeperationTime < GAINMessageInhibitTimes[kDT_DDL])
				minimumMessageSeperationTime = GAINMessageInhibitTimes[kDT_DDL];

			if (minimumBlockSeperationTime < GAINMessageInhibitTimes[kDT_DDL])
				minimumBlockSeperationTime = GAINMessageInhibitTimes[kDT_DDL];
			
			if (transferBlockSize > (dataBlockSize + 4))
				transferBlockSize = (dataBlockSize + 4);
	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "handleDataDownloadRequest: minimumMessageSeperationTime " << minimumMessageSeperationTime << newline;
		debugStream << "handleDataDownloadRequest: minimumBlockSeperationTime " << minimumBlockSeperationTime << newline;
		debugStream << "handleDataDownloadRequest: transferBlockSize " << transferBlockSize << newline;
		debugStream.unlock();
	#endif
			//	Data download request #2 in order = ACK
			messages[kDT_RES].message.data[2] = 1;
			messages[kDT_RES].message.data[3] = (uint8)minimumMessageSeperationTime;
			messages[kDT_RES].message.data[4] = transferBlockSize >> 8;
			messages[kDT_RES].message.data[5] = (uint8)transferBlockSize & 0xFF;
			messages[kDT_RES].message.data[6] = (uint8)(minimumBlockSeperationTime >> 8);
			messages[kDT_RES].message.data[7] = (uint8)minimumBlockSeperationTime & 0xFF;
			dataTransferState = 2;
			}
		else
			{
			//	Data download request #2 out of order => NACK
			messages[kDT_RES].message.data[2] = (uint8)-1;
			dataTransferState = 0;
			}						

		sendNSDMessage(kDT_RES);	
		}	
	else	//	Data download request
		{
		messages[kDT_RES].message.dataLengthCode = 8;
		if (dataTransferState == 0)
			{
			//	Data download request in order
			dataBlockNumber = (message.message.data[2] & 0x7F);

			messages[kDT_RES].message.data[3] = 1;
			if ((dataBlockNumber > 0) && (dataBlockNumber <= 12) && (GAINConfigurationDataBlockOffsetTable[dataBlockNumber] != 0xFFFF))
				{
				//	Valid data block number
				dataBlockDataIndex = 0;
				transferBlockDataIndex = 0;
				//	dataBlockOffset = GAINConfigurationDataBlockOffsetTable[dataBlockNumber];
				dataBlockSize = GAINConfigurationDataBlockSizeTable[dataBlockNumber];
							
				messages[kDT_RES].message.data[2] = 1;
				messages[kDT_RES].message.data[4] = 0x00;
				messages[kDT_RES].message.data[5] = 0x00;
				messages[kDT_RES].message.data[6] = 0x00;
				messages[kDT_RES].message.data[7] = dataBlockSize + 4;

                dataTransferBuffer = (uint8*)coreNSD_getGAINConfigurationDataBlockAdress(dataBlockNumber);
				//	coreNSD_copyGAINConfigurationDataBlock(dataBlockNumber, dataUploadReceiveBuffer);
				//	coreNSD_readGAINConfigurationData(dataBlockOffset, dataBlockSize, dataUploadReceiveBuffer);

                uint32	crc = coreNSD_calculateCRC32(dataTransferBuffer, dataBlockSize);
                
                CRCBuffer[0] = (uint8)(crc >> 24);
                CRCBuffer[1] = (uint8)(crc >> 16);
                CRCBuffer[2] = (uint8)(crc >> 8);
                CRCBuffer[3] = (uint8)(crc);
			#if DEBUG_NSD > 3
				debugStream.lock();
				debugStream << "handleDataDownloadRequest: data block == " << dataBlockNumber << ", size == " << dataBlockSize << ", CRC == " << kModeHex << crc << newline;
				debugStream.unlock();
			#endif
				dataTransferState = 1;
				}
			else
				{
				//	Invalid data block number
				messages[kDT_RES].message.data[2] = (uint8)-2;
				dataTransferState = 0;
				}

			sendNSDMessage(kDT_RES);	
			}
		else if (dataTransferState == 3)	//	Ack/Nack
			{
		#if DEBUG_NSD > 2
			debugStream.lock();
			if (messages[kDT_RES].message.data[2] == 1)
				debugStream << "handleDataDownload: ACK received" << newline;
			else
				debugStream << "handleDataDownload: NACK received" << newline;
			debugStream.unlock();
		#endif
			dataTransferState = 0;
			}
		else
			{
			//	Data upload request out of order => NACK
			messages[kDT_RES].message.data[2] = (uint8)-1;
			dataTransferState = 0;
			sendNSDMessage(kDT_RES);	
			}						
		}
	}

void NSDHandler::handleDataDownload(void)
	{
	tick_t	now(getControllerTicksCallback());
#if PARTNUMBER == 916	
	coreSystemTimerWait(50);
#endif
#if PARTNUMBER == 932	
	coreSystemTimerWait(50);
#endif
#if DEBUG_NSD > 4
	if (now >= nextDataTransfer)
		{
		debugStream.lock();
		debugStream << "handleDataDownload: ready: now == " << now
 					<< ", delta time is == " << (now - nextDataTransfer) << newline;
		debugStream.unlock();
		}
	else
		{
		debugStream.lock();
		debugStream << "handleDataDownload: still waiting: now == " << now
 					<< ", delta time is == " << (now - nextDataTransfer) << newline;
		debugStream.unlock();
		}
#endif
	if (!messages[kDT_DDL].message.isPending && (now >= nextDataTransfer))
		{
		uint8	i;
	
	#if DEBUG_NSD > 4
		debugStream.lock();
		debugStream << "handleDataDownload: delta time is == " << (now - nextDataTransfer) << newline;
		debugStream << newline;
		debugStream.unlock();
	#endif
		for (i=0; (i<8) && (dataBlockDataIndex < dataBlockSize) && (transferBlockDataIndex < transferBlockSize); i++)		//	copy data
			{
			messages[kDT_DDL].message.data[i] = dataTransferBuffer[dataBlockDataIndex++];
			transferBlockDataIndex++;
			}

		for (; (i<8) && (dataBlockDataIndex < (dataBlockSize + 4)) && (transferBlockDataIndex < transferBlockSize); i++)	//	copy CRC
			{
			messages[kDT_DDL].message.data[i] = CRCBuffer[dataBlockDataIndex++ - dataBlockSize];
			transferBlockDataIndex++;
			}

		for (; (i<8); i++)
			messages[kDT_DDL].message.data[i] = 0x00;			//	clear unused bytes
			
		sendNSDMessage(kDT_DDL);
		
		if (transferBlockDataIndex >= transferBlockSize)
			{
			nextDataTransfer = now + minimumBlockSeperationTime;
			transferBlockDataIndex = 0;
			}
		else
			nextDataTransfer = now + minimumMessageSeperationTime;

		dataTransferAbortionTime = now + SequenceTimeoutTimer;	

	#if DEBUG_NSD > 3
		debugStream.lock();
		debugStream << "handleDataDownload: sent " << dataBlockDataIndex << " of " << dataBlockSize + 4 << " byte" << newline;
		debugStream << "nextDataTransfer == " << nextDataTransfer << newline;
		debugStream.unlock();
	#endif
		if (dataBlockDataIndex == (dataBlockSize + 4))
			{
		#if DEBUG_NSD > 3
			debugStream.lock();
			debugStream << "handleDataDownload: transfer done" << newline;
			debugStream.unlock();
		#endif
			dataTransferState = 3;
			}	
		}
	}

#endif	//	NSD_SUPPORT
