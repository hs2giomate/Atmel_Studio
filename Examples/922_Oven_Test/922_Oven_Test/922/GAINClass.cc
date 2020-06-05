/*--------------------------------------------------------------------------

GAINClass.cc

Implementation
Ipeco P/N 922

Autor: Steffen Simon

$Date: 2018-03-28 16:26:12 +0200 (Mi, 28. Mrz 2018) $
$Revision: 7888 $
$Author: steffen $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/922/GAINClass.cc $

--------------------------------------------------------------------------*/
#include <avr/pgmspace.h>
#include <string.h>
#include <math.h>
#include "eDevice.h"
#include "PersistenceServices.h"
#ifdef NSD_SUPPORT
#include "NSDHandler.h"
#endif
#include "version.h"
#include "error.h"
#include "GAINClass.h"

GAINClass	GAIN;

#if defined(PINCODING_SUPPORT)
#ifdef NO_PINCODING
typedef struct	GAINPinCoding
	{
	uint16	magic;
	uint16	size;
	uint8	feederID;
	uint8	deviceID;
	uint8	pins;
	uint8	pinProgrammingError;
	} GAINPinCoding;

GAINPinCoding	currentPinCoding;
#endif
#endif

#define	kTimerClock    				kTimerClock32Hz
#define	kTimerCountdownValue		((uint16)(120 * 32))
#define kPSCFanOnAfterHeatingTime	(5L * 60L * 1000L)
#define countDownTimerValueToMilliseconds(x)	(((tick_t)x * 1000L) >> 5)
#define millisecondsToCountDownTimerValue(x)	((uint16)((x << 5)/1000L))

#define kPSCFanOnTemperatureThreshold	35	
#define kPSCFanOffTemperatureThreshold	33	
#define kPSCFanEnterSelfProtectTemperatureThreshold	86	
#define kPSCFanLeaveSelfProtectTemperatureThreshold	65	

// ADC inputs in use: Pin1 (Ambient), Pin3 (Steamer), Pin4 (Cavity), Pin5 (Vin sense), Pin7 (Water pressure)
static const uint8	kADCInputMask = 0xBA;

ConfigurationData	GAINClass::configuration;
CycleDictionary		GAINClass::cycleDictionary;
PersistentConfigurationData	GAINClass::persistentConfiguration;

void	checkCommunication(void);
void	checkEvent(void);

void checkCommunication(void)
	{
#ifdef NSD_SUPPORT
	MGCUStateBits	mgcu;
	NSD.handlePeriodicTask(mgcu);
#endif

	if (controller.asc0.dataAvailable() > 0)
		{
	#if __DEBUG__ > 4
		controller.asc0 << "asc0 data available: " << controller.asc0.dataAvailable() << " byte" << newline;
	#endif
		char	buffer[2];
		event	e;
		
        controller.asc0.readData(buffer, 1, 10);

        switch (buffer[0])
        	{
		#ifndef __DEBUG__
			case '@':
				GAIN.maintenance.handleCommunication(controller.asc0);
				break;
		#endif
			default:
				e.eventClass = kCommunicationEventClass;
				e.eventType = kUART0DataAvailableEvent;
				e.data.data = buffer[0];
  				sendEventSelf(e);
			#if (__DEBUG__ > 4) && !defined(NSD_SUPPORT)
				controller.asc0 << "UART0 data available: '" << buffer[0] << "' (" << (uint8)buffer[0] << ")" << newline;
			#endif
				break;
			}
		}

	GAIN.maintenance.handlePeriodicTask(controller.asc0, GAIN.state.currentState, GAIN.state.internalCateringState);
	}

void checkEvent(void)
	{
	event	e;
	
	coreControllerServiceWatchdog();

	if (GAIN.state.currentState >= kGAINStateStandbyOFF)
		{
		GAIN.panel.handlePeriodicTask();
	#ifndef NO_RTC
		coreRTCSetTimerValue(kTimerCountdownValue);
	#endif
		}

#if defined (PINCODING_SUPPORT)
	if ((GAIN.state.currentState == kGAINStateON) && controller.getPin(HeaterOutputPin))
		{
		GAIN.coding.handlePeriodicTask(true);
		}
	else
		{
		GAIN.coding.handlePeriodicTask(false);
		}

	if (GAIN.state.maintenanceToolConnected)
		{
		GAIN.maintenance.updateAnalogueValue(13, GAIN.coding.getPhaseValue(0));
		GAIN.maintenance.updateAnalogueValue(14, GAIN.coding.getPhaseValue(1));
		GAIN.maintenance.updateAnalogueValue(15, GAIN.coding.getPhaseValue(2));
		}
#endif
	
	e.eventClass = kGAINEventClass;

	bool	doorState(GAIN.getDoorState());
	bool	savedDoorState(GAIN.state.savedDoorState);

	if (doorState != savedDoorState)
		{
	#if (__DEBUG__ > 1)
		controller.asc0 << kModeHex << doorState << "/" << kModeHex << savedDoorState << newline;
	#endif
		GAIN.state.savedDoorState = doorState;
	#if (__DEBUG__ > 1)
		controller.asc0 << "doorState: " << (GAIN.state.savedDoorState?"true":"false") << newline;
	#endif

		if (GAIN.state.savedDoorState)
			e.eventType = kGAINDoorClosed;
		else
			e.eventType = kGAINDoorOpened;

		e.data.data = 0;

		sendEventSelf(e);
		}
	
	static	tick_t	nextSensorUpdate;
	static	tick_t	lowWaterDetectionTime;
	static	tick_t	lowFanSpeedDetectionTime;

	if ((GAIN.state.currentState > kGAINStateStandbyOFF) && (GAIN.state.currentState < kGAINStateError) && (controller.ticks() >= nextSensorUpdate))
		{
		int16	temperature;
		int16	temperatureThreshold;

		nextSensorUpdate = controller.ticks() + 500;

		temperature = GAIN.getAmbientTemperature();
		
		if (GAIN.state.currentState == kGAINStatePrepareStandbyON)
			temperatureThreshold = 71;
		else if ((GAIN.state.currentState == kGAINStateSelfProtect) && (GAIN.state.callingState == kGAINStatePrepareStandbyON))
			temperatureThreshold = 71;
		else
			temperatureThreshold = 81;
			
		if (((temperature > temperatureThreshold) || (temperature < 2)))
			{
			if (!GAIN.state.ambientTemperatureInRangeEventPending)
				{
				GAIN.state.ambientTemperatureInRangeEventPending = true;
				e.eventType = kGAINAmbientTemperatureOutOfRange;
				e.data.data = temperature;
				sendEventSelf(e);
			#if (__DEBUG__ > 2) && !defined(NSD_SUPPORT)
				controller.asc0 << coreSystemTimerTicks() << ": Sending kGAINAmbientTemperatureOutOfRange event" << newline;
			#endif
				}
			}
		else if ((temperature > 2) && (temperature < 71))
			{
			if (GAIN.state.ambientTemperatureInRangeEventPending)
				{
				GAIN.state.ambientTemperatureInRangeEventPending = false;
				e.eventType = kGAINAmbientTemperatureInRange;
				e.data.data = temperature;
				sendEventSelf(e);
			#if (__DEBUG__ > 3) && !defined(NSD_SUPPORT)
				controller.asc0 << coreSystemTimerTicks() << ": Sending kGAINAmbientTemperatureInRange event" << newline;
			#endif
				}
			}
	#ifdef OFFLINE
		if (GAIN.state.currentState == kGAINStateON)
			temperature = 48.0;
		else
			temperature = 25;
	#else
		temperature = coreRTCGetTemperatureValue();
	#endif
		//	Check PSC temperature if in states > Standby OFF or < Error
		if ((GAIN.state.currentState > kGAINStateStandbyOFF) && (GAIN.state.currentState < kGAINStateError))
			{
			if (temperature > kPSCFanOnTemperatureThreshold)
				{
				if (!controller.getPin(FanPSCOutputPin))
					{
				#if (__DEBUG__ > 2)
					controller.asc0 << "coreRTCGetTemperatureValue == " << temperature << " switching on PSC fan" << newline;
				#endif
					controller.setPin(FanPSCOutputPin);
					}
				}
			else if (temperature < kPSCFanOffTemperatureThreshold)
				{
				if (controller.getPin(FanPSCOutputPin))
					{
				#if (__DEBUG__ > 2)
					controller.asc0 << "coreRTCGetTemperatureValue == " << temperature << " switching off PSC fan" << newline;
				#endif
					controller.clearPin(FanPSCOutputPin);
					}
				}

			if (temperature > kPSCFanEnterSelfProtectTemperatureThreshold)
				{
				if (!GAIN.state.controllerTemperatureInRangeEventPending)
					{
					GAIN.state.controllerTemperatureInRangeEventPending = true;
					e.eventType = kGAINControllerTemperatureOutOfRange;
					e.data.data = temperature;
					sendEventSelf(e);
				#if (__DEBUG__ > 2)
					controller.asc0 << coreSystemTimerTicks() << ": Sending kGAINControllerTemperatureOutOfRange event" << newline;
				#endif
					}
				}
			else if (temperature < kPSCFanLeaveSelfProtectTemperatureThreshold)
				{
				//	IPECO-SRD-PSC-A350-922-275
				if (GAIN.state.controllerTemperatureInRangeEventPending)
					{
					GAIN.state.controllerTemperatureInRangeEventPending = false;
					e.eventType = kGAINControllerTemperatureInRange;
					e.data.data = temperature;
					sendEventSelf(e);
				#if (__DEBUG__ > 2)
					controller.asc0 << coreSystemTimerTicks() << ": Sending kGAINControllerTemperatureInRange event" << newline;
				#endif
					}
				}
			}
		//	Stop PSC fan if in states < Standby ON or > Self Protect
		else if (controller.getPin(FanPSCOutputPin))
			{
			controller.clearPin(FanPSCOutputPin);
			}

		if (GAIN.state.currentState > kGAINStateStandbyOFF)
			{
			//	IPECO-SES-V-25-36-922-2386
			// Only check if unit is configured as steam oven
			if (GAIN.configuration.commons.subPartNumber > 0)
				{
				int16	pressure;

				pressure = GAIN.getWaterPressure();
	
				if (pressure < 480)
					{
					if (!GAIN.state.waterPressureInRangeEventPending)
						{
						if (lowWaterDetectionTime == 0)
							lowWaterDetectionTime = controller.ticks();

					#ifdef OFFLINE
						else if ((controller.ticks() - lowWaterDetectionTime) >= 1000)
					#else
						else if ((controller.ticks() - lowWaterDetectionTime) >= 60000)
					#endif
							{
						#if (__DEBUG__ > 2)
							controller.asc0 << "Sending kGAINWaterLowPressure event, pressure is " << pressure << newline;
						#endif
							GAIN.state.waterPressureInRangeEventPending = true;
							e.eventType = kGAINWaterLowPressure;
							e.data.floatData = pressure;
							sendEventSelf(e);
							}
						}
					}
				else if (pressure > 8618)
					{
					if (!GAIN.state.waterPressureInRangeEventPending)
						{
					#if (__DEBUG__ > 3)
						controller.asc0 << "Sending kGAINWaterOverPressure event, pressure is " << pressure << newline;
					#endif
						GAIN.state.waterPressureInRangeEventPending = true;
						e.eventType = kGAINWaterOverPressure;
						e.data.floatData = pressure;
						sendEventSelf(e);
						}
					}
				else if ((pressure >= 480) && (pressure <= 8618))
					{
					if (lowWaterDetectionTime)
						lowWaterDetectionTime = 0;

					if (GAIN.state.waterPressureInRangeEventPending)
						{
					#if (__DEBUG__ > 2)
						controller.asc0 << "Sending kGAINWaterPressureInRange event, pressure is " << pressure << newline;
					#endif
						GAIN.state.waterPressureInRangeEventPending = false;
						e.eventType = kGAINWaterPressureInRange;
						e.data.floatData = pressure;
						sendEventSelf(e);
						}
					}
	   			}

			GAIN.state.cabinetTemperature = GAIN.getCabinetTemperature();

			if (GAIN.state.cavityTemperatureOutOfRange)
				{
				if (GAIN.state.cabinetTemperature >= GAIN.state.targetTemperature)
					GAIN.state.cavityTemperatureOutOfRange = false;	
				}
			else
				{
				GAIN.state.cavityTemperatureOutOfRange = (GAIN.state.cabinetTemperature <= (GAIN.state.targetTemperature - 5));
				}

			GAIN.state.steamerTemperature = GAIN.getSteamerTemperature();

			if (GAIN.state.steamerTemperatureOutOfRange)
				{
				if (GAIN.state.steamerTemperature <= GAIN.configuration.steamerOperationalTemperature)
					GAIN.state.steamerTemperatureOutOfRange = false;	
				}
			else
				{
				GAIN.state.steamerTemperatureOutOfRange = (GAIN.state.steamerTemperature >= GAIN.configuration.steamerOverheatTemperature);
				}
			}
		}
	else if (GAIN.state.maintenanceToolConnected)
		{
		GAIN.getWaterPressure();
		if (GAIN.configuration.commons.subPartNumber > 0)
			GAIN.getSteamerTemperature();
		GAIN.getAmbientTemperature();
		GAIN.getCabinetTemperature();
		}

	//!	Check if fan is powered on and running
	if (controller.getPin(FanStartOutputPin))
		{
		//!	Fan is powered on, so check for fan speed and failures
		//	Fan failure modes:
		//	- Fan Not Run
		//	- Fan Motor OT
		//	- Fan Controller OT
		uint8	fanFailureFlags(0);
	#ifndef OFFLINE		
		if (controller.getPin(FanNotRunInputPin))
			;//fanFailureFlags |= 0x01;
					
		if (controller.getPin(FanMotorOverTemperatureInputPin))
			fanFailureFlags |= 0x02;
					
		if (controller.getPin(FanControllerOverTemperatureInputPin))
			fanFailureFlags |= 0x04;

	#if __DEBUG__ > 2
		if (fanFailureFlags)
			controller.asc0 << "fanFailureFlags == " << kModeHex << fanFailureFlags << newline;
	#endif
	#endif					
		if (fanFailureFlags)
			{
			if (!GAIN.state.fanStateValidEventPending)
				{
				GAIN.state.fanStateValidEventPending = true;
				e.eventType = kGAINFanStateEvent;
				e.data.wordData[0] = fanFailureFlags;
				sendEventSelf(e);
				}
			}
		else
			{
			if (GAIN.state.fanStateValidEventPending)
				{
				GAIN.state.fanStateValidEventPending = false;
				e.eventType = kGAINFanStateEvent;
				e.data.wordData[0] = fanFailureFlags;
				sendEventSelf(e);
				}
			}
		//	Fan speed:
		if (controller.ticks() >= GAIN.nextFanStateUpdateTime)
			{
			//	Check fan speed (RPM)
			float	deltaT(controller.ticks() - GAIN.nextFanStateUpdateTime);
			float	rpm;
			uint16	counter;
		
			GAIN.nextFanStateUpdateTime = controller.ticks() + 10000;
		
			coreControllerDisableExternalInterrupt(FanTachoSignalInputPin);
			counter = externalInterruptCounter[FanTachoSignalInputPin - 4] >> 2;
			externalInterruptCounter[FanTachoSignalInputPin - 4] = 0;
			coreControllerEnableExternalInterrupt(FanTachoSignalInputPin, NULL);
			deltaT = deltaT/1000.0;
   			deltaT += 10.000;

			rpm = ((float)counter/deltaT) * 60.0;
			GAIN.maintenance.updateAnalogueValue(0, rpm);
		
		#ifdef OFFLINE
			//if (GAIN.state.currentState == kGAINStateSelfProtect)
				rpm = 3000;
		#endif	
		#if __DEBUG__ > 3
			controller.asc0 << "rpm == " << rpm << ", counter == " << counter << ", delta T == " << deltaT << newline;
		#endif
			if (rpm < 2000)			//	Value TBC
				{
				if (!GAIN.state.fanSpeedInRangeEventPending)
					{
					if (lowFanSpeedDetectionTime == 0)
						lowFanSpeedDetectionTime = controller.ticks();
				#ifdef OFFLINE
					else if ((controller.ticks() - lowFanSpeedDetectionTime) >= 1000)
				#else
					else if ((controller.ticks() - lowFanSpeedDetectionTime) >= 10000)
				#endif
						{
					#if (__DEBUG__ > 2)
						controller.asc0 << "Sending kGAINFanSpeedEvent event, rpm is " << rpm << newline;
					#endif
						GAIN.state.fanSpeedInRangeEventPending = true;
						e.eventType = kGAINFanSpeedEvent;
						e.data.wordData[0] = 1;
						e.data.wordData[1] = rpm;
						sendEventSelf(e);
						}
					}
				}
			else if (rpm > 2000)	//	Value TBC
				{
				if (lowFanSpeedDetectionTime)
					lowFanSpeedDetectionTime = 0;

			#ifdef FANSPEED_FAULT_IS_RECOVERABLE
				if (GAIN.state.fanSpeedInRangeEventPending)
					{
					GAIN.state.fanSpeedInRangeEventPending = false;
					e.eventType = kGAINFanSpeedEvent;
					e.data.wordData[0] = 0;
					e.data.wordData[1] = rpm;
					sendEventSelf(e);
					}
			#endif
				}
			}
		}

	checkCommunication();

	static	tick_t	nextUptimeUpdate;
	
	if (controller.ticks() > nextUptimeUpdate)
		{
		persistent.updateUptime(false);
		if (controller.getPin(FanStartOutputPin))
			persistent.updateFanOnTime(false);
		if (controller.getPin(HeaterOutputPin))
			persistent.updateHeaterOnTime(false);
		if (controller.getPin(SteamerOutputPin))
			persistent.updateSteamerOnTime(false);

		nextUptimeUpdate = controller.ticks() + 5000;
		}
	}
		
GAINClass::GAINClass(void)
	{
	WaitEventCallbackHandler = NULL;
    state.build = build;
	state.currentState = kGAINStateReset;
	state.callingState = kGAINStateReset;
	state.internalCateringState = kStateStandby;
	state.stateFlags = 0;
	state.totalHeatingTime = 0;
	state.remainingTime = 0;
	state.steamCycleDuration = 0;
	state.savedDoorState = true;
	state.steamMode = true;
	}

void GAINClass::run(void)
	{
	event	e;
	bool	isValidWorkingStateForGAINAvailable;

#ifdef __DEBUG__
	controller.asc0 << "run()\r\r";
#endif
	state.currentState = kGAINStateReset;
	
	coreI2COpen(kI2C0, 400000);
	coreADConverterEnableAutoTrigger(kTimerCounter0Overflow, kADCInputMask, kOversampling64);
#if __DEBUG__ > 3
	controller.asc0 << "ADC resolution == " << coreADConverterGetResolution() << newline;
	controller.asc0 << "ADC max value  == " << coreADConverterGetMaximumValue() << newline;
#endif

	panel.setPowerSupplyState(true);

#ifdef NO_RTC
	uint16	timerValue(1);
	resumeLastStateValid = true;
#else
	uint16	timerValue;
	uint16	deltaValue;

	coreRTCEnableTimer(kTimerClock);
	coreRTCEnableTemperature();

	timerValue = coreRTCGetTimerValue();
	deltaValue = (kTimerCountdownValue - timerValue);
	powerInterruptDuration = countDownTimerValueToMilliseconds(deltaValue);

#if __DEBUG__ > 0		
	controller.asc0.lock();
	controller.asc0 << "run: kTimerCountdownValue == " << kTimerCountdownValue
						<< ", timerValue == " << timerValue
						<< ", deltaValue == " << deltaValue << newline;
	controller.asc0.unlock();	
#endif
	resumeLastStateValid = ((timerValue > 0) && (powerInterruptDuration < 5000));
#if __DEBUG__ > 0		
	controller.asc0.lock();
	controller.asc0 << "run: powerInterruptDuration == " << powerInterruptDuration << (resumeLastStateValid?" resume":" catering cycle resume") << newline;
	controller.asc0.unlock();	
#endif
#endif
	
	isValidWorkingStateForGAINAvailable = persistent.validWorkingStateForGAINAvailable(countDownTimerValueToMilliseconds(timerValue), resumeLastStateValid);

	if (!isValidWorkingStateForGAINAvailable)
		persistent.checkAndRepairNVMConsistency();

	persistent.updateUptime(true);

	if (!persistent.readConfigurationDataArea(configuration))
		{
	#if __DEBUG__ > 3
		controller.asc0 << "readConfigurationDataArea failed!" << newline;
	#endif
		setFactoryDefaults(1, true, false);
		}

	if (!persistent.readCycleDictionaryArea(cycleDictionary))
		{
	#if __DEBUG__ > 3
		controller.asc0 << "readCycleDictionaryArea failed!" << newline;
	#endif
		setFactoryDefaults(1, false, true);
		}

	if (!persistent.readPersistentConfigurationDataArea(persistentConfiguration))
		{
	#if __DEBUG__ > 3
		controller.asc0 << "readPersistentConfigurationDataArea failed!" << newline;
	#endif
		setPersistentConfigurationDefaults();
		}

#if __DEBUG__ > 3
	controller.asc0 << "total on time: " << persistent.getUptime() << newline;
#endif	
		
#ifdef PINCODING_SUPPORT
#ifdef NO_PINCODING
	coreNVMRead(kNVM0, 0x40, &currentPinCoding, sizeof(GAINPinCoding));

	if ((currentPinCoding.magic != 0x1903)||(currentPinCoding.size != sizeof(GAINPinCoding)))
		{
	#if __DEBUG__ > 3
		controller.asc0.lock();
		controller.asc0 << "GAIN initializing NVM!" << newline;
		controller.asc0.unlock();
	#endif
		initializePinCoding(true);
		}
#endif
	coding.connect(configuration.commons.phaseLossDetectionThreshold);
#endif

#ifdef NSD_SUPPORT
    coreCANOpen(kCAN0, 125000);

	NSD.initializeHandler(persistent.getNSDDataAreaAddress());

	maximumPowerBudget = configurationData.block03.MaximumPowerBudget * 50;
	minimumPowerBudget = configurationData.block03.MinimumPowerBudget * 50;
#endif

	while (analogConversionReadyMask != kADCInputMask)
		{
		coreSystemTimerWait(1);
		}

	//	Initialize ambient temperature history buffer for moving average
	getAmbientTemperature(true);
	//	Initialize steamer temperature history buffer for moving average
	if (GAIN.configuration.commons.subPartNumber > 0)
		getSteamerTemperature(true);
	//	Initialize cabinet temperature history buffer for moving average
	getCabinetTemperature(true);

	if (isValidWorkingStateForGAINAvailable)
		prepareStateChangeEvent(e, kGAINStateResume);
	else
		prepareStateChangeEvent(e, kGAINStateInitialize);

	WaitEventCallbackHandler = checkCommunication;
	sendEventSelf(e);

	while (true)
		{
		if (waitForEvent(e, kGAINEventClass, kGAINSwitchStateEvent))
			{
			handleGAINEvent(e);
			}
		}
	}

void GAINClass::setFactoryDefaults(uint16 subPartNumber, bool config, bool cycles)
	{
#if __DEBUG__ > 2
	controller.asc0 << "setFactoryDefaults " << (config?"config ":" ") << (cycles?"cycles":"") << newline;
#endif
	persistent.invalidatePersistentConfigurationDataArea();
	
	if (config)
		{
		memcpy_P(&configuration, &factoryDefaultsConfiguration, sizeof(configuration));
		configuration.commons.subPartNumber = subPartNumber;
		persistent.writeConfigurationDataArea(configuration);
		}

	if (cycles)
		{
		memcpy_P(&cycleDictionary, &factoryDefaultsCycles.dictionary, sizeof(cycleDictionary));
		persistent.writeCycleDictionaryArea(cycleDictionary);

		for (uint8 i=0; i<cycleDictionary.numberOfCycleDescriptions + kNumberOfContinuesCycleDescriptions; i++)
			{
			CycleDescription	cycle;
				
			memcpy_P(&cycle, &factoryDefaultsCycles.cycles[i], sizeof(CycleDescription));
			persistent.writeCycleDescription(cycle, i);
			}
		}
	}

void GAINClass::setPersistentConfigurationDefaults(void)
	{
#if __DEBUG__ > 2
	controller.asc0 << "setPersistentConfigurationDefaults" << newline;
#endif
	if (configuration.commons.subPartNumber == 1)
		persistentConfiguration.cycleDescriptionID = 0;
	else
		persistentConfiguration.cycleDescriptionID = 1;

	persistentConfiguration.delayTime = 0;

	persistent.readCycleDescription(persistentConfiguration.cycle, persistentConfiguration.cycleDescriptionID);
	state.targetTemperature = persistentConfiguration.cycle.targetTemperature;
	persistent.writePersistentConfigurationDataArea(persistentConfiguration);
	}

bool GAINClass::syncConfigurationWithPanel(void)
	{
	bool	result;
	
	result = panel.syncWithGAINInfo(configuration.commons.partNumber, configuration.commons.subPartNumber);
	
	for (uint8 i=0; result && (i<cycleDictionary.numberOfCycleDescriptions); i++)
		{
		if (i != persistentConfiguration.cycleDescriptionID)
			{
			CycleDescription	cycle;
				
			persistent.readCycleDescription(cycle, i);
			if  ((cycle.targetTemperature > 35)||((cycle.cycleFlags & kCycleModeMask) == kDefrostMode))
				result &= panel.syncWithCycleDescription(i, cycle);
			else
				break;
			}
		else
			result &= panel.syncWithCycleDescription(i, persistentConfiguration.cycle);
		}

	if (result)	
		result = panel.syncWithCycleDescriptionID(persistentConfiguration.cycleDescriptionID, (!getSteamSensorState()||state.insufficientWaterPressure)?kDefrostMode:kSteamMode);
	
	if (result)	
		result = panel.syncWithDelayTime(persistentConfiguration.delayTime);

	return result;
	}

void GAINClass::resyncWithCycleDescription(uint8 cycleID)
	{
	if (cycleID == persistentConfiguration.cycleDescriptionID)
		{
		CycleDescription	cycle;
		
		persistent.readCycleDescription(cycle, cycleID);
		persistentConfiguration.cycle = cycle;
		panel.syncWithCycleDescription(cycleID, cycle);	
		persistent.writePersistentConfigurationDataArea(persistentConfiguration);
		state.targetTemperature = persistentConfiguration.cycle.targetTemperature;
		}
	}

bool GAINClass::resyncWithPanel(uint8 state)
	{
	bool	result;
	
	if (state == kGAINStateReset)
		state = GAIN.state.currentState;
		
	result = syncConfigurationWithPanel();

	if ((state > kGAINStateStandbyOFF) && (state < kGAINStateError))
		{	
		if (result)
			{
			result = panel.syncWithTemperature(GAIN.state.targetTemperature, GAIN.state.cabinetTemperature);

			if (result)
				{	
				if ((state == kGAINStateDelayed)||(state == kGAINStateON))
					result = panel.syncWithRemainingTime(GAIN.state.remainingTime);
				else if (persistentConfiguration.delayTime)
					result = panel.syncWithRemainingTime(persistentConfiguration.delayTime);
				else
					result = panel.syncWithRemainingTime(persistentConfiguration.cycle.cycleDuration);
				}
			}
		}
	
#ifdef NSD_SUPPORT
	if (result)	
		result = panel.syncWithNetworkState();
#endif

	if (state > kGAINStateStandbyOFF)
		result = panel.syncWithState(state, GAINGetHMIFlags());
  	else
		result = panel.syncWithState(state, 0);
	
	return result;
	}

void GAINClass::handleGAINEvent(event& e)
	{
	if (state.currentState != kGAINStateResume)
		state.callingState = state.currentState;
	state.currentState = e.data.wordData[0];
	state.stateFlags = e.data.wordData[1];
 	state.stateEntryTime = coreSystemTimerTicks();

#if __DEBUG__ > 1
	controller.asc0.lock();
	controller.asc0 << "handleGAINEvent received: switching from ";
	controller.asc0.writeP(gainStateDescription[state.callingState]);
	controller.asc0 << " to ";
	controller.asc0.writeP(gainStateDescription[state.currentState]);
	controller.asc0 << " (flags == " << kModeHex << state.stateFlags << ')' << newline;
	controller.asc0.unlock();
#endif

	saveCurrentState();

	switch (state.currentState)
		{
		case kGAINStateInitialize:
			handleStateInitialize(state.stateFlags);
			break;

		case kGAINStateResume:
			handleStateResume(state.stateFlags);
			break;

		case kGAINStateStandbyOFF:
			handleStateStandbyOFF(state.stateFlags);
			break;

		case kGAINStatePrepareStandbyON:
			handleStatePrepareStandbyON(state.stateFlags);
			break;

		case kGAINStateStandbyON:
			handleStateStandbyON(state.stateFlags);
			break;

		case kGAINStateStandbyReady:
			handleStateStandbyReady(state.stateFlags);
			break;

		case kGAINStateDelayed:
			handleStateDelayed(state.stateFlags);
			break;

	#ifdef NSD_SUPPORT
		case kGAINStateInterrupted:
			handleStateInterrupted(state.stateFlags);
			break;
	#endif

		case kGAINStateON:
			handleStateON(state.stateFlags);
			break;

		case kGAINStateFinished:
			handleStateFinished(state.stateFlags);
			break;

		case kGAINStateStoppedProcess:
			handleStateStoppedProcess(state.stateFlags);
			break;

		case kGAINStateSelfProtect:
			handleStateSelfProtect(state.stateFlags);
			break;

		case kGAINStateError:
			handleStateError(state.stateFlags);
			break;

		case kGAINStateMaintenance:
			handleStateMaintenance(state.stateFlags);
			break;

		case kGAINStateHMIUpdate:
			handleStateHMIUpdate(state.stateFlags);
			break;

		case kGAINStateFirmwareUpdate:
			handleStateFirmwareUpdate(state.stateFlags);
			break;

		default:
			handleStateError(kGAINErrorIllegalStateTransition);
			break;
		}
	}

#ifdef NSD_SUPPORT
void GAINClass::handleStateInitialize(uint16 flags)
	{
	event	e;
   	uint16	selfTestResult(kGAINNoError);
	
#if __DEBUG__ > 0
	controller.asc0.lock();
	controller.asc0 << "handleStateInitialize: flags == " << kModeHex << flags << newline;
	controller.asc0.unlock();	
#endif
	
	panel.syncWithState(state.currentState);

	NSD.setGAINState(kState_Initialize, true);
#ifdef NO_PINCODING
	state.pincoding = ((uint32)currentPinCoding.feederID << 24)|((uint32)currentPinCoding.deviceID << 16)|((uint32)currentPinCoding.pins << 8)|(uint32)currentPinCoding.pinProgrammingError;
#else	//	NO_PINCODING
	state.pincoding = coding.getPinCodingValue();
#endif
#if __DEBUG__ > 0
	controller.asc0.lock();
	controller.asc0 << "pincoding == " << kModeHex << state.pincoding << newline;
	controller.asc0.unlock();	
#endif
	selfTestResult = NSD.setPinCode(state.pincoding);

	if (state.pincoding & (1 << 9))
		{
		prepareStateChangeEvent(e, kGAINStateMaintenance);
		sendEventSelf(e);
		}
	else
		{
		if (selfTestResult == kNSDDiagInvalidDeviceID)
			selfTestResult = kGAINNoError;
		else if (selfTestResult)
			{
			persistent.clearWorkingStateForGAIN();
			selfTestResult = kGAINErrorPinProgramming;
			prepareStateChangeEvent(e, kGAINStateError, kGAINErrorPinProgramming);
			sendEventSelf(e);
			}

		GAIN.state.insufficientWaterPressure = (configuration.commons.subPartNumber == 0);

		state.cabinetTemperature = 0;
	#if __DEBUG__ > 3
		controller.asc0 << "cabinetTemperatureOffset: " << configuration.cabinetTemperatureOffset << newline;
		controller.asc0 << "pressureSensorOffset: " << configuration.pressureSensorOffset << newline;
		controller.asc0 << "steamerOverheatTemperature: " << configuration.steamerOverheatTemperature << newline;
	#endif

		if (selfTestResult == kGAINNoError)
			{
			selfTestResult = handlePowerOnSelftest();
			}

		if (!NSD.MGCUIsPresent())
			{
			sendEventSelf(kNSDEventClass, kNSDMGCUConnectionLost, 0);
			sendEventSelf(kNSDEventClass, kNSDCentralizedPowerLost, 0);
			}
	
		state.centralizedPowerControl = NSD.CentralizedPowerControlAvailable();

		if (selfTestResult == kGAINNoError)
			{
			prepareStateChangeEvent(e, kGAINStateStandbyOFF);
			sendEventSelf(e);
			NSD.sendDIAG(true);
		
			syncConfigurationWithPanel();
			}
		}


	WaitEventCallbackHandler = checkEvent;
    coreControllerEnableWatchdog();
	}
#else
void GAINClass::handleStateInitialize(uint16 flags)
	{
	event	e;
   	uint16	selfTestResult(kGAINNoError);
	
#if __DEBUG__ > 0
	controller.asc0.lock();
	controller.asc0 << "handleStateInitialize" << newline;
	controller.asc0.unlock();	
#endif
	
	GAIN.state.insufficientWaterPressure = (configuration.commons.subPartNumber == 0);

	state.cabinetTemperature = 0;
#if __DEBUG__ > 3
	controller.asc0 << "cabinetTemperatureOffset: " << configuration.cabinetTemperatureOffset << newline;
	controller.asc0 << "pressureSensorOffset: " << configuration.pressureSensorOffset << newline;
	controller.asc0 << "steamerOverheatTemperature: " << configuration.steamerOverheatTemperature << newline;
#endif

	selfTestResult = handlePowerOnSelftest();

	if (selfTestResult == kGAINNoError)
		{
		prepareStateChangeEvent(e, kGAINStateStandbyOFF);
		sendEventSelf(e);
		
		syncConfigurationWithPanel();
		}

	WaitEventCallbackHandler = checkEvent;
    coreControllerEnableWatchdog();
	}
#endif


void GAINClass::handleStateResume(uint16 flags)
	{
	event	e;
	uint8	currentState(state.currentState);

#if __DEBUG__ > 0
	controller.asc0.lock();
	controller.asc0 << "handleStateResume" << newline;
	controller.asc0.unlock();	
#endif
#if __DEBUG__ > 3
	controller.asc0 << "state.currentState (before) " << state.currentState << newline;
#endif
	persistent.restoreWorkingStateForGAIN(state);
#if __DEBUG__ > 3
	controller.asc0 << "state.currentState (after) " << state.currentState << newline;
#endif
	coreSystemTimerSetTicks(state.now);
	
	//	maintenance.setConnected(state.maintenanceToolConnected);	
#ifdef NSD_SUPPORT
	NSD.resume(state.pincoding, (state.centralizedPowerControl?kMode_Centralized:kMode_Fallback), kState_Initialize, state.powerBudgetGranted, state.remainingTime);
#endif

	switch (state.currentState)
		{	
		case kGAINStateDelayed:
		case kGAINStateON:
			prepareStateChangeEvent(e, state.currentState, true);
			break;
					
		case kGAINStateStoppedProcess:
			prepareStateChangeEvent(e, kGAINStateStoppedProcess, state.stateFlags);
			break;
					
		default:
			prepareStateChangeEvent(e, state.currentState, state.stateFlags);
			break;
		}

	resyncWithPanel(state.currentState);
	state.currentState = currentState;

	WaitEventCallbackHandler = checkEvent;
    coreControllerEnableWatchdog();
	sendEventSelf(e);
	}

void GAINClass::handleStateStandbyOFF(uint16 flags)
	{
	event	e;
	bool	done(false);
   	
#ifdef __DEBUG__
	controller.asc0.lock();
	controller.asc0 << "handleStateStandbyOFF" << newline;
	controller.asc0.unlock();	
#endif

	panel.syncWithState(state.currentState);

#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_StandbyOFF, true);
#endif
	if (controller.getPin(FanPSCOutputPin))
		controller.clearPin(FanPSCOutputPin);

	state.ambientTemperatureInRangeEventPending = false;
	state.controllerTemperatureInRangeEventPending = false;
	state.cabinetTemperatureInRangeEventPending = false;
	state.steamerTemperatureInRangeEventPending = false;

    state.fanSpeedInRangeEventPending = false;
	state.fanStateValidEventPending = false;

	while (!done)
		{
		if (handleInStateEvent(e, forever, done))
			{
			switch (e.eventClass)
				{
				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							prepareStateChangeEvent(e, kGAINStatePrepareStandbyON);
							done = true;
 							break;

						default:
							break;
						}
					break;
					
				default:
					break;
				}
			}
		}

	sendEventSelf(e);
	}
		

void GAINClass::handleStatePrepareStandbyON(uint16 flags)
	{
	event	e;
	bool	done(false);
   	
#ifdef __DEBUG__
	controller.asc0.lock();
	controller.asc0 << "handleStatePrepareStandbyON" << newline;
	controller.asc0.unlock();	
#endif

	panel.syncWithState(state.currentState);

#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_StandbyOFF, true);
#endif
	tick_t	leaveDisplayTime = controller.ticks() + 2000;

	while (!done)
		{
		if (handleInStateEvent(e, 1000, done))
			{
			switch (e.eventClass)
				{
				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

						default:
							break;
						}
					break;
					
				default:
					break;
				}
			}
		
		if (!done)
			{
			done = (controller.ticks() > leaveDisplayTime);
			if (done)
				prepareStateChangeEvent(e, kGAINStateStandbyON);
			}
		}

	sendEventSelf(e);
	}

void GAINClass::handleStateStandbyON(uint16 flags)
	{
	event	e;
	bool	done(false);

#if (__DEBUG__ > 3)
	controller.asc0.lock();
	controller.asc0 << "handleStateStandbyON, flags == " << kModeHex << flags << newline;
	controller.asc0.unlock();	
#endif
	if (!GAINIsReady())
		{
		panel.syncWithState(state.currentState, GAINGetHMIFlags());
		panel.syncWithRemainingTime(persistentConfiguration.cycle.cycleDuration);
		panel.syncWithTemperature(persistentConfiguration.cycle.targetTemperature, state.cabinetTemperature);
	#ifdef NSD_SUPPORT
		NSD.setGAINState(kState_StandbyON, true);
	#endif
		}
	else
		{
		prepareStateChangeEvent(e, kGAINStateStandbyReady);
		done = true;
		}

	while (!done)
		{
		if (handleInStateEvent(e, forever, done))
			{
			switch (e.eventClass)
				{
			#ifdef NSD_SUPPORT
				case kNSDEventClass:
					switch (e.eventType)
						{
						case kNSDCentralizedPowerEstablished:
							if (GAINIsReady())
								{
								prepareStateChangeEvent(e, kGAINStateStandbyReady);
								done = true;
								}
							else
								{
								panel.syncWithState(state.currentState, GAINGetHMIFlags());
								}
	 						break;
 						
 						default:
 							break;
						}
					break;
     		#endif

				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

						default:
							break;
						}
					break;

				case kGAINEventClass:
					{
					switch (e.eventType)
						{
						case kGAINDoorClosed:
							if (GAINIsReady())
								{
								prepareStateChangeEvent(e, kGAINStateStandbyReady);
								done = true;
								}
							else
								{
								panel.syncWithState(state.currentState, GAINGetHMIFlags());
							    panel.syncWithDelayTime(persistentConfiguration.delayTime);
								}
							break;
						
						default:
							break;
						}
					}
					
				default:
					break;
				}
			}
		}

	sendEventSelf(e);
	}

void GAINClass::handleStateStandbyReady(uint16 flags)
	{
	event	e;
	bool	done(false);

#if (__DEBUG__ > 3)
	controller.asc0.lock();
	controller.asc0 << "handleStateStandbyON, flags == " << kModeHex << flags << newline;
	controller.asc0.unlock();	
#endif
	panel.syncWithState(state.currentState, GAINGetHMIFlags());

#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_StandbyON, true);
#endif
	panel.syncWithRemainingTime(persistentConfiguration.cycle.cycleDuration);
	panel.syncWithTemperature(persistentConfiguration.cycle.targetTemperature, getCabinetTemperature());
								
	while (!done)
		{
		if (handleInStateEvent(e, forever, done))
			{
			switch (e.eventClass)
				{
			#ifdef NSD_SUPPORT
				case kNSDEventClass:
					switch (e.eventType)
						{
						case kNSDCentralizedPowerLost:
							if (!NSD.getGAINIsPrimary())
								{
								prepareStateChangeEvent(e, kGAINStateStandbyON);
								done = true;
								}
	 						break;
 						
 						default:
 							break;
						}
					break;
     		#endif
				
				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

 						case kHMIStartCateringCycleEvent:
						#if (__DEBUG__ > 3)
							controller.asc0 << "handleStateStandbyReady received kHMIStartCateringCycleEvent == "
											<< kModeHex << e.data.wordData[0] << ':' << kModeHex << e.data.wordData[1] << newline;
						#endif
							if (persistentConfiguration.delayTime > 0)
								{
								prepareStateChangeEvent(e, kGAINStateDelayed, false);
								done = true;
								}
							else
								{
							#ifdef NSD_SUPPORT
								if (NSD.CentralizedPowerControlAvailable())
									{
									prepareStateChangeEvent(e, kGAINStateInterrupted, state.currentState);
									done = true;
									}
								else if (NSD.getGAINIsPrimary())
									{
									state.powerBudgetGranted = maximumPowerBudget;
									state.powerTimeGranted = persistentConfiguration.cycle.cycleDuration;
									prepareStateChangeEvent(e, kGAINStateON, false);
									done = true;
									}
							#else
								prepareStateChangeEvent(e, kGAINStateON, false);
								done = true;
							#endif
								}
 							break;

						default:
							break;
						}
					break;

				case kGAINEventClass:
					{
					switch (e.eventType)
						{
						case kGAINDoorOpened:
							prepareStateChangeEvent(e, kGAINStateStandbyON);
							done = true;
							break;
						
						default:
							break;
						}
					}
					
				default:
					break;
				}
			}
		}

	sendEventSelf(e);
	}

void GAINClass::handleStateDelayed(uint16 flags)
	{
	event	e;
	bool	done(false);
    bool	resume(flags != 0);
			
	tick_t	deltaT;
	tick_t	zeroTime;
	tick_t	delay;
	tick_t	stepDuration;
					
#if (__DEBUG__ > 2)
	controller.asc0.lock();
	controller.asc0 << "handleStateDelayed: flags == " << kModeHex << flags << newline;
	controller.asc0.unlock();	
#endif

	if (!resume)
		{
		state.totalHeatingTime = 0;
		state.remainingTime = (tick_t)persistentConfiguration.delayTime * 1000;
		panel.syncWithRemainingTime(state.remainingTime);
		}

	panel.syncWithState(state.currentState, GAINGetHMIFlags());
#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_StandbyON, true);
#endif

#ifndef NO_PERSISTENCE
	saveCurrentState();
#endif
	zeroTime = coreSystemTimerTicks();

	while (!done)
		{
		delay = 500;
			
		stepDuration = coreSystemTimerTicks();

		if (handleInStateEvent(e, delay, done))
			{
			switch (e.eventClass)
				{
			#ifdef NSD_SUPPORT
				case kNSDEventClass:
					switch (e.eventType)
						{
						case kNSDCentralizedPowerLost:
							if (!NSD.getGAINIsPrimary())
								{
 								prepareStateChangeEvent(e, kGAINStateStandbyON);
                        		done = true;
 								}
							break;

						default:
							break;
						};
					break;
     		#endif
				case kHMIEventClass:
					{
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

 						case kHMIAbortCateringCycleEvent:
							if (e.data.wordData[1] == kEventPowerButtonPressed)
								prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							else
								prepareStateChangeEvent(e, kGAINStateStandbyON);
							done = true;
 							break;

						default:
							break;
						}
					}
					break;

				case kGAINEventClass:
					switch (e.eventType)
						{
						case kGAINSwitchStateEvent:
							done = true;
							break;

						case kGAINDoorOpened:
							prepareStateChangeEvent(e, kGAINStateStoppedProcess);
							done = true;
							break;

						default:
							break;
						}
					break;

				default:
					break;
				}
			}

		if (!done)
			{
			deltaT = coreSystemTimerTicks() - zeroTime;
			zeroTime = coreSystemTimerTicks();
            state.totalHeatingTime += deltaT;

		#if __DEBUG__ > 4
			controller.asc0.lock();
			controller.asc0 << "state.totalHeatingTime == " << state.totalHeatingTime << newline;
			controller.asc0.unlock();
		#endif
			state.remainingTime = ((tick_t)persistentConfiguration.delayTime * 1000) - state.totalHeatingTime;
			if (state.remainingTime <= 0)
				{
				state.remainingTime = 0;
				done = true;
				}

			panel.syncWithRemainingTime(state.remainingTime);

			if (done)
				{
			#ifdef NSD_SUPPORT
				if (NSD.CentralizedPowerControlAvailable())
					{
					prepareStateChangeEvent(e, kGAINStateInterrupted);
					}
				else if (NSD.getGAINIsPrimary())
					{
					state.powerBudgetGranted = maximumPowerBudget;
					state.powerTimeGranted = persistentConfiguration.cycle.cycleDuration;
					prepareStateChangeEvent(e, kGAINStateON);
					}
                else
                    {
                    prepareStateChangeEvent(e, kGAINStateStandbyON);
                    }
                    
			#else
				prepareStateChangeEvent(e, kGAINStateON);
			#endif
				}
			}

		stepDuration = coreSystemTimerTicks() - stepDuration;
		
		if (delay > stepDuration)
			delay -= stepDuration;
		else if (delay > 0)
			delay = 0;

	#ifndef NO_PERSISTENCE
		saveCurrentState();
	#endif
      	}


	switch (e.data.wordData[0])
		{
	#ifdef NSD_SUPPORT
		case kGAINStateInterrupted:
			break;
	#endif
		case kGAINStateStoppedProcess:
			break;
		
		default:
			persistentConfiguration.delayTime = 0;
    		persistent.writePersistentConfigurationDataArea(persistentConfiguration);
    		panel.syncWithDelayTime(persistentConfiguration.delayTime);
			break;
		}		

	sendEventSelf(e);
	}


#ifdef NSD_SUPPORT	
void GAINClass::handleStateInterrupted(uint16 flags)
	{
	event	e;
	bool	done(false);
	bool	resume(flags & 0x0001);		
	bool	receivedPBM(flags & 0x0002);		
	bool	syncPanelWithStateInterrupted(false);
	tick_t	zeroTime(coreSystemTimerTicks());
	tick_t	remainingTime(0);
	

#if (__DEBUG__ > 1)
	controller.asc0.lock();
	controller.asc0 << "handleStateInterrupted, flags == " << kModeHex << flags << newline;
	controller.asc0.unlock();	
#endif
	if (resume)
		{
		if (receivedPBM && (state.callingState == kGAINStateStoppedProcess))
			{
#if (__DEBUG__ > 1)
	controller.asc0.lock();
	controller.asc0 << "handleStateInterrupted, receivedPBM set and resuming from kGAINStateStoppedProcess" << newline;
	controller.asc0.unlock();	
#endif
			prepareStateChangeEvent(e, kGAINStateON, state.inCycle?0xFFFF:0x0000);
			done = true;
			}
		else
			{
#if (__DEBUG__ > 1)
	controller.asc0.lock();
	controller.asc0 << "handleStateInterrupted, resuming from " << kModeHex << state.callingState << newline;
	controller.asc0.unlock();	
#endif
			// panel.syncWithState(state.currentState);
			NSD.setGAINState(kState_Interrupted, true);
			}
		}
	else
		{
		// panel.syncWithState(state.currentState);

		NSD.setGAINPowerBudgetUsed(0, false);
		NSD.setGAINState(kState_Interrupted, true);

	    state.powerBudgetRequested = maximumPowerBudget;
	    //	state.powerBudgetRequested = persistentConfiguration.cycle.maximumPower;

		if ((state.callingState == kGAINStateON)||(state.callingState == kGAINStateStoppedProcess))
			{
			state.powerTimeRequested = state.remainingTime/1000;
			}
		else
			{
			state.powerTimeRequested = persistentConfiguration.cycle.cycleDuration;
			}

		NSD.sendPBR(state.powerBudgetRequested, state.powerTimeRequested,
				state.powerBudgetRequested, state.powerTimeRequested);
		}

	while (!done)
		{
		if (!syncPanelWithStateInterrupted)
			{
			syncPanelWithStateInterrupted = ((coreSystemTimerTicks() - state.stateEntryTime) > 1000);
			syncPanelWithStateInterrupted |= (remainingTime > 0);
			if (syncPanelWithStateInterrupted)
				panel.syncWithState(state.currentState, GAINGetHMIFlags());
			}

		if (handleInStateEvent(e, 1000, done))
			{
			switch (e.eventClass)
				{
				case kNSDEventClass:
					switch (e.eventType)
						{
						case kNSDCentralizedPowerLost:
							NSD.cancelPBR();
 							if (NSD.getGAINIsPrimary())
								{
								prepareStateChangeEvent(e, kGAINStateON);
								state.powerBudgetGranted = state.powerBudgetRequested;
								state.powerTimeGranted = state.powerTimeRequested;
                        		done = true;
 								}
							else
								{
 								prepareStateChangeEvent(e, kGAINStateStandbyON);
                        		done = true;
 								}
							break;

						case kNSDPowerBudgetChanged:
					#if __DEBUG__
						controller.asc0.lock();
						controller.asc0 << "handleStateInterrupted: kNSDPowerBudgetChanged received" << newline;
						controller.asc0.unlock();
					#endif
							if (!done)
								{
								if (e.data.wordData[0] == 0)	//PBM (0,x)
									{
									zeroTime = coreSystemTimerTicks();
									remainingTime = e.data.wordData[1] * 1000L;
									}
								else
									{
									if (state.callingState == kGAINStateON)
										prepareStateChangeEvent(e, kGAINStateON, true);
									else if (state.callingState == kGAINStateStoppedProcess)
										prepareStateChangeEvent(e, kGAINStateStoppedProcess, 0x0002);
									else if (GAINIsReady())
										prepareStateChangeEvent(e, kGAINStateON, 0);
									else
										prepareStateChangeEvent(e, kGAINStateStoppedProcess, 0x0002);

									done = true;
									}
								}
							break;

						default:
							break;
						}
					break;
					
				case kHMIEventClass:
					{
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
#if (__DEBUG__ > 3)
	controller.asc0.lock();
	controller.asc0 << "handleStateInterrupted, received kEventPowerButtonPressed" << newline;
	controller.asc0.unlock();	
#endif
							abortCateringCycle();
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

 						case kHMIAbortCateringCycleEvent:
#if (__DEBUG__ > 3)
	controller.asc0.lock();
	controller.asc0 << "handleStateInterrupted, received kHMIAbortCateringCycleEvent" << newline;
	controller.asc0.unlock();	
#endif
 							abortCateringCycle();
							if (e.data.wordData[1] == kEventPowerButtonPressed)
								prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							else
								prepareStateChangeEvent(e, kGAINStateStandbyON);
							done = true;
 							break;

						default:
							break;
						}
					}
					break;

				case kGAINEventClass:
					switch (e.eventType)
						{
						case kGAINDoorOpened:
							prepareStateChangeEvent(e, kGAINStateStoppedProcess);
							done = true;
							break;
						
						default:
      						break;
						}
					break;

				default:
					break;
				}
			}
		
		if (remainingTime > 0)
			{
			tick_t	deltaT;

			deltaT = coreSystemTimerTicks() - zeroTime;
			zeroTime = coreSystemTimerTicks();
			remainingTime -= deltaT;

			if (remainingTime > 0)
            	{
				panel.syncWithRemainingTime(remainingTime);
            	}
  			}
   		}

	sendEventSelf(e);
	}
#endif

void GAINClass::handleStateON(uint16 flags)
	{
	event	e;
	bool	done(false);
	bool	resume(flags != 0);
			
	tick_t	deltaT;
	tick_t	zeroTime;
	tick_t	delay(0);
	tick_t	stepDuration;
	tick_t	relaxationTime;
	uint8	cycleMode(persistentConfiguration.cycle.cycleFlags & kCycleModeMask);
	uint8	internalCateringStateSave = 0;
					
#if (__DEBUG__ > 1)
	controller.asc0.lock();
	controller.asc0 << "handleStateON: flags == " << kModeHex << flags << newline;
	controller.asc0.unlock();	
#endif
	coreControllerEnableExternalInterrupt(FanTachoSignalInputPin, NULL);
	setSafetyRelayState(true);

	persistent.updateCateringCycleTime(cycleMode, true);
	state.waterPressureInRangeEventPending = false;

	if (resume)
		{
		internalCateringStateSave = state.internalCateringState;
		state.internalCateringState = kStateOperationDelay;
		state.leaveStateTime = controller.ticks() + configuration.resumeOperationDelayTime;
		}
	else
		{
		state.inCycle = true;
		state.totalHeatingTime = 0;
		
		state.cycleDuration = (tick_t)persistentConfiguration.cycle.cycleDuration * 1000;
		state.steamCycleDuration = 0;
		state.remainingTime = state.cycleDuration;
		state.steamerOn = false;
		state.forcedConvectionMode = !getSteamSensorState();
		state.cavityTemperatureOutOfRange = false;
		state.targetTemperature = persistentConfiguration.cycle.targetTemperature;
		state.internalCateringState = kStateStandby;

	#if (__DEBUG__ > 1)
		controller.asc0 << "handleStateON: id == " << persistentConfiguration.cycleDescriptionID << ", description == " << persistentConfiguration.cycle.description
						<< ", flags == " << persistentConfiguration.cycle.cycleFlags
					<< ", time == " << persistentConfiguration.cycle.cycleDuration << ", temperature == " << persistentConfiguration.cycle.targetTemperature << newline;
	#endif
		setFanState(true);	//	Switch on fan
		}

	//	SES IPECO-SES-V-25-36-922-1439/IPECO-SES-V-25-36-922-1440
	relaxationTime = state.cycleDuration/5;
		
	panel.syncWithState(state.currentState, GAINGetHMIFlags());
	saveCurrentState();

#ifdef NSD_SUPPORT
	NSD.setGAINPowerBudgetUsed(state.powerBudgetGranted, false);
	NSD.setGAINRemainingTime(state.remainingTime, true);
	if (NSD.getGAINState() != kState_Waiting)
		NSD.setGAINState(kState_ON, true);
#endif

	zeroTime = coreSystemTimerTicks();
	steamCycleStartTime = 0;

	while (!done)
		{
		delay = 100;
			
		stepDuration = coreSystemTimerTicks();

		panel.syncWithRemainingTime(state.remainingTime);
	#ifdef NSD_SUPPORT
		NSD.setGAINRemainingTime(state.remainingTime, true);
	#endif

		state.cabinetTemperature = getCabinetTemperature();
		
		if ((state.cabinetTemperature < -50)||(state.cabinetTemperature > 300))
			{
			prepareStateChangeEvent(e, kGAINStateError, kGAINCabinetTemperatureSensorFailure);
			sendEventSelf(e);
			done = true;
			}
		else if (handleInStateEvent(e, delay, done))
			{
			switch (e.eventClass)
				{
			#ifdef NSD_SUPPORT
				case kNSDEventClass:
					switch (e.eventType)
						{
						case kNSDCentralizedPowerLost:
							if (!NSD.getGAINIsPrimary())
								{
 								prepareStateChangeEvent(e, kGAINStateStandbyON);
                        		done = true;
 								}
							break;

						case kNSDPowerBudgetChanged:
							if (!done && (NSD.getGAINState() == kState_Waiting))
								{
								NSD.setGAINState(kState_ON, true);
								NSD.setGAINPowerBudgetUsed(state.powerBudgetGranted, false);
								}
							break;

						default:
							break;
						};
					break;
     		#endif
				case kHMIEventClass:
					{
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

 						case kHMIAbortCateringCycleEvent:
							if (e.data.wordData[1] == kEventPowerButtonPressed)
								prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							else
								prepareStateChangeEvent(e, kGAINStateStandbyON);
							done = true;
 							break;
							
						default:
							break;
						}
					}
					break;

				case kGAINEventClass:
					switch (e.eventType)
						{
						case kGAINSwitchStateEvent:
							done = true;
							break;

						case kGAINDoorOpened:
							prepareStateChangeEvent(e, kGAINStateStoppedProcess);
							done = true;
							break;

						case kGAINFanSpeedEvent:
						#if (__DEBUG__ > 2)
							controller.asc0 << "received kGAINFanSpeedEvent: flags = " << kModeHex << e.data.wordData[0] << newline;
						#endif
							if (e.data.wordData[0])
								{
							#ifdef FANSPEED_FAULT_IS_RECOVERABLE
								prepareStateChangeEvent(e, kGAINStateSelfProtect, kGAINErrorFanSpeedOutOfRange);
							#else
								prepareStateChangeEvent(e, kGAINStateError, kGAINErrorFanSpeedOutOfRange);
							#endif
								done = true;
								}
							break;

						case kGAINFanStateEvent:
						#if (__DEBUG__ > 2)
							controller.asc0 << "received kGAINFanStateEvent: flags = " << kModeHex << e.data.wordData[0] << newline;
						#endif
							if (e.data.wordData[0])
								{
								uint16	flags(e.data.wordData[0]);
								
								done = false;

								if (flags & 0x04)
									{
									prepareStateChangeEvent(e, kGAINStateSelfProtect, kGAINErrorFanControllerOverTemperature);
									done = true;
									}

								if (flags & 0x02)
									{
									prepareStateChangeEvent(e, kGAINStateSelfProtect, kGAINErrorFanMotorOverTemperature);
									done = true;
									}

								if (flags & 0x01)
									{
									prepareStateChangeEvent(e, kGAINStateSelfProtect, kGAINErrorFanFailure);
									done = true;
									}
								}
							break;


						default:
							break;
						}
					break;

				default:
					break;
				}
			}

		if (!done)
			{
			deltaT = coreSystemTimerTicks() - zeroTime;
			zeroTime = coreSystemTimerTicks();
			state.totalHeatingTime += deltaT;
            
		#if __DEBUG__ > 4
			controller.asc0.lock();
			controller.asc0 << "state.totalHeatingTime == " << state.totalHeatingTime << newline;
			controller.asc0.unlock();
		#endif
			state.remainingTime = state.cycleDuration - state.totalHeatingTime;
				
			if (state.remainingTime <= 0)
				{
				state.remainingTime = 0;
				done = true;
				}

			if (done)
				{
				prepareStateChangeEvent(e, kGAINStateFinished);				
				}
			else
				{
 				switch (state.internalCateringState)
					{
					case kStateStandby:
						if (cycleMode == kSteamMode)
							{
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateStandby: is steam cycle. Entering kStatePreheat\n"));
						#endif
							state.internalCateringState = kStatePreheat;
							state.leaveStateTime = state.totalHeatingTime + configuration.steamGenerationDelayTime;
							}
						else
							{
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateStandby: is convection cycle. Entering kStateConvectionStandby\n"));
						#endif
							state.internalCateringState = kStateConvectionStandby;
							}
						break;
						
					case kStateOperationDelay:
						if (controller.ticks() > state.leaveStateTime)
							{
							setFanState(true);	//	Switch on fan
							state.internalCateringState = internalCateringStateSave;
							if (internalCateringStateSave == kStateRefill)
								{
								setValveState(true);
								}
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateOperationDelay done. Continue regular operation\n"));
						#endif
							}
						break;

					case kStatePreheat:
						setHeaterState(state.cavityTemperatureOutOfRange, false);

						if (state.totalHeatingTime > state.leaveStateTime)
							{
							state.leaveStateTime = 0;
							state.internalCateringState = kStateSteamStandby;
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStatePreheat timeout. Entering kStateSteamStandby\n"));
						#endif
							}
						break;

					case kStateSteamStandby:
						if ((state.remainingTime <= relaxationTime) || (state.totalHeatingTime >= (35L * 60000L)))
							{
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateSteamStandby timeout. Entering kStateConvectionStandby\n"));
						#endif
							state.internalCateringState = kStateConvectionStandby;
							}
						else
							{
							if (state.cavityTemperatureOutOfRange)
								{
							#if (__DEBUG__ > 2)
								controller.asc0 << "kStateSteamStandby target temperature out of range: " << getCabinetTemperature() << "°C. Entering kStateSteamON" << newline;
							#endif
								state.leaveStateTime = controller.ticks() + configuration.steamGenerationFailureTime;
								state.internalCateringState = kStateSteamON;
								}
							else
								setHeaterState(false, false);
							}
						break;

					case kStateSteamON:
						if (!getSteamSensorState())
							{
							setHeaterState(state.cavityTemperatureOutOfRange, false);
							state.internalCateringState = kStatePrepareConvection;
							NSD.addDIAGCode(kNSDDiagMalfunction);
							NSD.sendDIAG(false);
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateSteamON: steamer temperature sensor failure. Entering kStatePrepareConvection\n"));
						#endif
							}
						else if (state.steamerTemperatureOutOfRange)
							{
							if (steamCycleStartTime != 0)
								{
								state.steamCycleDuration = (controller.ticks() - steamCycleStartTime);
								steamCycleStartTime = 0;
								}
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateSteamON done. Steam cycle duration == "));
							controller.asc0 << state.steamCycleDuration << newline;
						#endif

							if ((state.remainingTime <= (relaxationTime + state.steamCycleDuration)) || (state.totalHeatingTime >= (35L * 60000L)))
								{
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("kStateSteamON done. Entering kStateConvectionStandby\n"));
							#endif
								state.internalCateringState = kStateConvectionStandby;
								}
							else
								{
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("kStateSteamON done. Entering kStatePrepareRefill\n"));
							#endif
								setHeaterState(state.cavityTemperatureOutOfRange, false);
								state.internalCateringState = kStatePrepareRefill;
								}
	
							refillCounter = 0;
							}
						else if (controller.ticks() > state.leaveStateTime)
							{
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateSteamON: valve failure detected\n"));
							controller.asc0.writeP(PSTR("kStateSteamON done. Entering kStatePrepareConvection\n"));
						#endif
							setHeaterState(state.cavityTemperatureOutOfRange, false);
							state.internalCateringState = kStatePrepareConvection;
							}
						else
							{
							setHeaterState(state.cavityTemperatureOutOfRange, state.cavityTemperatureOutOfRange);
							if (!state.cavityTemperatureOutOfRange)
								{
								state.internalCateringState = kStateSteamStandby;
								}
							}
						break;

					case kStatePrepareRefill:
						{
						state.valveOpenedTime = controller.ticks();
						setValveState(true);
						delay = 0;						
						state.internalCateringState = kStateRefill;
						state.leaveStateTime = controller.ticks() + 500L;
						steamCycleStartTime = controller.ticks();
					#if (__DEBUG__ > 2)
						controller.asc0.writeP(PSTR("kStatePrepareRefill done. Entering kStateRefill\n"));
					#endif
						}
						break;

					case kStateRefill:
						{
						tick_t	remainingValveOpenTime;
						float	pressure(getWaterPressure(false));
						float	totalOpenTime;
							
					#if (__DEBUG__ > 2)
						controller.asc0.writeP(PSTR("kStateRefill: pressure == "));
						controller.asc0 << pressure << newline;
					#endif
						//	hPa => PSI
						pressure = pressure * 0.0145038;
						if (pressure < 7.0)
							pressure = 7.0;
						else if (pressure > 125.0)
							pressure = 125.0;
						//x^y = e^(y*ln(x))
						totalOpenTime = (15943.0 * exp(-0.687 * log(pressure)));
                        state.leaveStateTime = state.valveOpenedTime + (tick_t)totalOpenTime;
                        
					#if (__DEBUG__ > 2)
						controller.asc0.writeP(PSTR("kStatePrepareRefill: valve open time == "));
						controller.asc0 << totalOpenTime << newline;
					#endif
                        remainingValveOpenTime = (state.leaveStateTime - controller.ticks());
						if (remainingValveOpenTime > 100)
							{
							if (state.insufficientWaterPressure)
								{
								setValveState(false);
								state.internalCateringState = kStatePrepareConvection;
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("kStateRefill insufficient water pressure. Entering kStatePrepareConvection\n"));
							#endif
								delay = 0;
								}
							}
						else
							{
							if (remainingValveOpenTime > 0)
								coreSystemTimerWait(remainingValveOpenTime);
						#if (__DEBUG__ > 2)
							tick_t	additionalValveOpenTime(controller.ticks() - state.leaveStateTime);
								
							controller.asc0.writeP(PSTR("kStateRefill timeout. Additional valve open time was "));
							controller.asc0 << additionalValveOpenTime << newline;
						#endif
							setValveState(false);
							refillCounter++;
							state.leaveStateTime = controller.ticks() + configuration.steamGenerationOffTime;
							state.internalCateringState = kStateRelaxSteamer;
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateRefill timeout. Entering kStateRelaxSteamer\n"));
						#endif
							}
						}
						break;

					case kStateRelaxSteamer:
						if (controller.ticks() > state.leaveStateTime)
							{
						#if (__DEBUG__ > 2)
							controller.asc0 << "kStateRelaxSteamer steamer temperature: " << state.steamerTemperature << "°C." << newline;
						#endif
							if (state.steamerTemperatureOutOfRange)
								{
								if (refillCounter >= 3)
									{
									state.internalCateringState = kStatePrepareConvection;
								#if (__DEBUG__ > 2)
									controller.asc0.writeP(PSTR("kStateRelaxSteamer refill failure. Entering kStatePrepareConvection\n"));
								#endif
									}
								else
									{
									state.internalCateringState = kStatePrepareRefill;
								#if (__DEBUG__ > 2)
									controller.asc0.writeP(PSTR("kStateRelaxSteamer timeout. Entering kStatePrepareRefill\n"));
								#endif
									}
								}
							else
								{
								state.internalCateringState = kStateSteamStandby;
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("kStateRelaxSteamer timeout. Entering kStateSteamStandby\n"));
							#endif
								}
							}
						else
							{
							setHeaterState(state.cavityTemperatureOutOfRange, false);
							if (!getSteamSensorState())
								{
								state.internalCateringState = kStatePrepareConvection;
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("kStateRelaxSteamer: steamer temperature sensor failure. Entering kStatePrepareConvection\n"));
							#endif
								}
							}
						break;
						
					case kStatePrepareConvection:
						if ((state.remainingTime >= relaxationTime) && (state.totalHeatingTime < 35L * 60000L))
							{
							tick_t	prolongation;
								
							state.forcedConvectionMode = true;
							panel.syncWithState(state.currentState, GAINGetHMIFlags());

							state.targetTemperature = (int16)((float)persistentConfiguration.cycle.targetTemperature * 1.9);

				           	if (state.targetTemperature > 190)
				           		state.targetTemperature = 190;

                            panel.syncWithTemperature(state.targetTemperature, state.cabinetTemperature);

							prolongation = ((tick_t)((float)(state.cycleDuration - state.totalHeatingTime) * 0.14) + 30000);
						#ifdef NSD_SUPPORT
				    		if (NSD.CentralizedPowerControlAvailable())
				    			{
								tick_t	nominalTime(state.remainingTime + prolongation);

								NSD.setGAINState(kState_Waiting, true);
								NSD.sendPBR(maximumPowerBudget, nominalTime,
											maximumPowerBudget, nominalTime);
								}
						#endif
						#if (__DEBUG__ > 2)
							controller.asc0 << "target temperature == " << state.targetTemperature << ", was " << persistentConfiguration.cycle.targetTemperature << newline;
							controller.asc0 << "prolongation == " << prolongation << ", cycle time is " << state.cycleDuration << newline;
						#endif
							state.cycleDuration += prolongation;
						#if (__DEBUG__ > 2)
							controller.asc0 << "cycle time now ==  " << state.cycleDuration << newline;
						#endif
							}

						state.internalCateringState = kStateConvectionStandby;
					#if (__DEBUG__ > 2)
						controller.asc0.writeP(PSTR("kStatePrepareConvection done. Entering kStateConvectionStandby\n"));
					#endif
						break;

					case kStateConvectionStandby:
						if (state.remainingTime > 30000)
							{
							setHeaterState(state.cavityTemperatureOutOfRange, false);
							if (state.cavityTemperatureOutOfRange)
								{
								state.internalCateringState = kStateConvectionON;
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("kStateConvectionStandby done. Entering kStateConvectionON\n"));
							#endif
								}
							}
						else
							{
							setHeaterState(false, false);
							state.internalCateringState = kStateRelax;
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateConvectionStandby done. Entering kStateRelax\n"));
						#endif
							}
						break;

					case kStateConvectionON:
						setHeaterState(state.cavityTemperatureOutOfRange, false);
						if (state.remainingTime > 30000)
							{
							if (!state.cavityTemperatureOutOfRange)
								{
								state.internalCateringState = kStateConvectionStandby;
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("kStateConvectionON done. Entering kStateConvectionStandby\n"));
							#endif
								}
							}
						else
							{
							state.internalCateringState = kStateConvectionStandby;
						#if (__DEBUG__ > 2)
							controller.asc0.writeP(PSTR("kStateConvectionON done. Entering kStateConvectionStandby\n"));
						#endif
							}
						break;

					case kStateRelax:
						break;

					default:
						break;
					}

				if (controller.getPin(HeaterOutputPin))
				//	Check heating elements
					{
					if (coding.thermalFuseFailureDetected())
						{
					#if (__DEBUG__ > 2)
						controller.asc0.writeP(PSTR("kStateSteamON: non resetable thermal fuse failure detected\n"));
					#endif
						setValveState(false);
						prepareStateChangeEvent(e, kGAINStateError, kGAINErrorNonResetableThermalFuseFailure);
						done = true;
						}
					else
						{
						uint8	elementID;
						
						if (coding.elementFailureDetected(elementID))
							{
							if ((state.internalCateringState == kStateSteamON) && (elementID == 3))
								{
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("Secondary heating element failure detected. Entering kStatePrepareConvection\n"));
							#endif
								state.internalCateringState = kStatePrepareConvection;
								elementID = 0;
								}
						
							if (elementID)
								{
							#if (__DEBUG__ > 2)
								controller.asc0.writeP(PSTR("Primary heating element failure detected: #"));
								controller.asc0 << elementID << newline;
							#endif
								setValveState(false);
								prepareStateChangeEvent(e, kGAINStateError, kGAINPrimaryHeaterFailure);
								done = true;
								}
							}
						}
					}
				}
			}

		persistent.updateCateringCycleTime(cycleMode, false);
		stepDuration = coreSystemTimerTicks() - stepDuration;
		
		if (delay > stepDuration)
			delay -= stepDuration;
		else if (delay > 0)
			delay = 0;
	#ifndef NO_PERSISTENCE
		saveCurrentState();
	#endif
      	}

    setValveState(false);			//	Close valves
	setHeaterState(false, false);	//	Switch off all heaters
	setFanState(false);             //	Switch off fan
	setSafetyRelayState(false);		//	Switch off safety relay
	
	coreControllerDisableExternalInterrupt(FanTachoSignalInputPin);

	switch (e.data.wordData[0])
		{
	#ifdef NSD_SUPPORT
		case kGAINStateInterrupted:
			break;
	#endif
		case kGAINStateStoppedProcess:
			break;
		
		default:
			abortCateringCycle();
			if ((state.insufficientWaterPressure || !getSteamSensorState()) && ((persistentConfiguration.cycle.cycleFlags & kCycleModeMask) == kSteamMode))
				{
				persistentConfiguration.cycleDescriptionID = 1;
				persistent.readCycleDescription(persistentConfiguration.cycle, persistentConfiguration.cycleDescriptionID);
				state.targetTemperature = persistentConfiguration.cycle.targetTemperature;
				persistent.writePersistentConfigurationDataArea(persistentConfiguration);
				panel.syncWithCycleDescriptionID(persistentConfiguration.cycleDescriptionID, kDefrostMode);
				}
			break;
		}		

	sendEventSelf(e);
	}

void GAINClass::handleStateFinished(uint16 flags)
	{
	event	e;
	bool	done(false);

#if (__DEBUG__ > 3)
	controller.asc0.lock();
	controller.asc0 << "handleStateFinished" << newline;
	controller.asc0.unlock();	
#endif
	panel.syncWithState(state.currentState, GAINGetHMIFlags());
#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_Finished, true);
#endif

	while (!done)
		{
		if (handleInStateEvent(e, 1000, done))
			{
			switch (e.eventClass)
				{
				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

 						case kHMIAbortCateringCycleEvent:
							prepareStateChangeEvent(e, kGAINStateStandbyON);
							done = true;
 							break;

						default:
							break;
						}
					break;

				case kGAINEventClass:
					{
					switch (e.eventType)
						{
						case kGAINDoorOpened:
							prepareStateChangeEvent(e, kGAINStateStandbyON);
							done = true;
							break;
							
						default:
							break;
						}
					}

				default:
					break;
				}
			}
		}

	sendEventSelf(e);
	}

void GAINClass::handleStateStoppedProcess(uint16 flags)
	{
	event	e;
 	bool	done(false);
	uint16	resumeFlags(flags);

	panel.syncWithState(state.currentState, GAINGetHMIFlags());
#ifdef NSD_SUPPORT
	if ((NSD.getGAINState() != kState_Waiting) && (NSD.getGAINState() != kState_Interrupted))
		NSD.setGAINState(kState_StoppedProcess, true);
#endif
#if (__DEBUG__ > 3)
	controller.asc0.lock();
	controller.asc0 << "handleStateStoppedProcess" << newline;
	controller.asc0.unlock();	
#endif
	while (!done)
		{
		if (handleInStateEvent(e, 1000, done))
			{
			switch (e.eventClass)
				{
			#ifdef NSD_SUPPORT
				case kNSDEventClass:
					switch (e.eventType)
						{
						case kNSDCentralizedPowerLost:
							if (!NSD.getGAINIsPrimary())
								{
							#if __DEBUG__ > 1
								controller.asc0.lock();
								controller.asc0 << "Secondary and Centralized Power Lost" << newline;
								controller.asc0.unlock();	
							#endif
 								prepareStateChangeEvent(e, kGAINStateStandbyON);
                        		done = true;
 								}
							break;

						case kNSDPowerBudgetChanged:
							if (!done)
								{
								//	Received a valid PBM => Resume to interrupted but remind PBM is already received
								if (NSD.getGAINState() == kState_Interrupted)
									{
								#if __DEBUG__ > 1
									controller.asc0.lock();
									controller.asc0 << "Stopped/Interrupted: Power budget changed" << newline;
									controller.asc0.unlock();	
								#endif
									resumeFlags |= 0x0002;
									saveCurrentState();
									}
								else
									{
								#if __DEBUG__ > 1
									controller.asc0.lock();
									controller.asc0 << "Stopped/Waiting: Power budget changed" << newline;
									controller.asc0.unlock();	
								#endif
									}

								NSD.setGAINPowerBudgetUsed(state.powerBudgetGranted, false);
								NSD.setGAINState(kState_StoppedProcess, true);
								}
							break;

						default:
							break;
						};
					break;
     		#endif

				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
 							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

 						case kHMIAbortCateringCycleEvent:
							prepareStateChangeEvent(e, kGAINStateStandbyON);
							done = true;
 							break;

						default:
							break;
						}
					break;

				case kGAINEventClass:
					{
					switch (e.eventType)
						{
						case kGAINDoorOpened:
							panel.syncWithState(state.currentState, GAINGetHMIFlags());
							break;

						case kGAINDoorClosed:
							if (GAINIsReady())
								{
								resumeFlags |= 0x0001;
								prepareStateChangeEvent(e, state.callingState, resumeFlags);
								done = true;
    							}
 							else
 								{
								panel.syncWithState(state.currentState, GAINGetWaitingFlags());
 								}
							break;
						
						default:
							break;
						}
					}

				default:
					break;
				}
			}
		}
	
	if (!resumeFlags)
		{
		abortCateringCycle();
		}

	sendEventSelf(e);
	}

void GAINClass::handleStateSelfProtect(uint16 error)
	{
	event	e;
	bool	done(false);
	bool	circulationFanMalfunction(0);
	bool	circulationFanOnPending(true);
	tick_t	nextCirculationFanToggleTime(controller.ticks() + (5L * 60L * 1000L));
	
	errorDescription	description;
	
	memcpy_P(&description, &GAINErrorDescription[error], sizeof(description));
	state.error = error;

#if (__DEBUG__ > 0)
	controller.asc0.lock();
	controller.asc0 << "handleStateSelfProtect:" << newline;
	controller.asc0 << "error ID " << kModeHex << description.errorID << ", annotation " << kModeHex << description.annotation << newline;
	controller.asc0.unlock();	
#endif

	panel.syncWithState(state.currentState, error);
#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_StandbyON, true);
	NSD.addDIAGCode((NSDDiagCode)description.annotation);
	NSD.sendDIAG(false);
#endif
	persistent.logError(description.errorID, description.annotation);

	state.selfProtectSourceMask |= (1UL << (error &0x0FFF));
	saveCurrentState();

#ifdef FANSPEED_FAULT_IS_RECOVERABLE
	circulationFanMalfunction |= ((state.selfProtectSourceMask & (1UL << kGAINErrorFanSpeedOutOfRange)) != 0UL);
#endif
	circulationFanMalfunction |= ((state.selfProtectSourceMask & (1UL << kGAINErrorFanFailure)) != 0UL);
	circulationFanMalfunction |= ((state.selfProtectSourceMask & (1UL << kGAINErrorFanMotorOverTemperature)) != 0UL);
	circulationFanMalfunction |= ((state.selfProtectSourceMask & (1UL << kGAINErrorFanControllerOverTemperature)) != 0UL);

#if (__DEBUG__ > 2)
	controller.asc0 << "selfProtectSourceMask (on entry) = " << kModeHex << state.selfProtectSourceMask << newline;
	controller.asc0 << "circulationFanMalfunction (on entry) = " << (circulationFanMalfunction?"true":"false") << newline;
#endif
	
	while (!done)
		{
		if (handleInStateEvent(e, 1000, done))
			{
		#if (__DEBUG__ > 2)
			controller.asc0 << "selfProtectSourceMask (before) = " << kModeHex << state.selfProtectSourceMask << newline;
		#endif
			switch (e.eventClass)
				{
				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							state.selfProtectSourceMask = 0;
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
 							break;

						default:
							break;
						}
					break;

				case kGAINEventClass:
					switch (e.eventType)
						{
						case kGAINAmbientTemperatureInRange:
						#if (__DEBUG__ > 3) && !defined(NSD_SUPPORT)
							controller.asc0 << "received kGAINAmbientTemperatureInRange" << newline;
						#endif
							state.selfProtectSourceMask &= ~(1 << kGAINErrorAmbientTemperatureOutOfRange);
							break;

						case kGAINControllerTemperatureInRange:
						#if (__DEBUG__ > 3) && !defined(NSD_SUPPORT)
							controller.asc0 << "received kGAINControllerTemperatureInRange" << newline;
						#endif
							state.selfProtectSourceMask &= ~(1 << kGAINErrorControllerTemperatureOutOfRange);
							break;
					#ifdef FANSPEED_FAULT_IS_RECOVERABLE
						case kGAINFanSpeedEvent:
						#if (__DEBUG__ > 2)
							controller.asc0 << "received kGAINFanSpeedEvent: flags = " << kModeHex << e.data.wordData[0] << newline;
						#endif
							if (e.data.wordData[0] == 0)
								{
								state.selfProtectSourceMask &= ~(1UL << kGAINErrorFanSpeedOutOfRange);
								}
							else
								{
								state.selfProtectSourceMask |= (1UL << kGAINErrorFanSpeedOutOfRange);
								}
							break;
       				#endif
						case kGAINFanStateEvent:
						#if (__DEBUG__ > 2)
							controller.asc0 << "received kGAINFanStateEvent: flags = " << kModeHex << e.data.wordData[0] << newline;
						#endif
							if ((e.data.wordData[0] & 0x04) == 0)
								{
								state.selfProtectSourceMask &= ~(1UL << kGAINErrorFanFailure);
								}
							else
								{
								state.selfProtectSourceMask |= (1UL << kGAINErrorFanFailure);
								}

							if ((e.data.wordData[0] & 0x02) == 0)
								{
								state.selfProtectSourceMask &= ~(1UL << kGAINErrorFanMotorOverTemperature);
								}
							else
								{
								state.selfProtectSourceMask |= (1UL << kGAINErrorFanMotorOverTemperature);
								}

							if ((e.data.wordData[0] & 0x01) == 0)
								{
								state.selfProtectSourceMask &= ~(1UL << kGAINErrorFanControllerOverTemperature);
								}
							else
								{
								state.selfProtectSourceMask |= (1UL << kGAINErrorFanControllerOverTemperature);
								}
							break;

						default:
							break;
						}
					
					saveCurrentState();
				
					if (state.selfProtectSourceMask == 0)
						{
						prepareStateChangeEvent(e, kGAINStateStandbyON);
						done = true;
						}
					else
						{
						circulationFanMalfunction = ((state.selfProtectSourceMask & (1UL << kGAINErrorFanFailure)) != 0UL);
						circulationFanMalfunction |= ((state.selfProtectSourceMask & (1UL << kGAINErrorFanMotorOverTemperature)) != 0UL);
						circulationFanMalfunction |= ((state.selfProtectSourceMask & (1UL << kGAINErrorFanControllerOverTemperature)) != 0UL);
					#ifdef FANSPEED_FAULT_IS_RECOVERABLE
						circulationFanMalfunction = ((state.selfProtectSourceMask & (1UL << kGAINErrorFanSpeedOutOfRange)) != 0UL);
					#endif
						}

					break;

				default:
					break;
				}
		#if (__DEBUG__ > 2)
			controller.asc0 << "selfProtectSourceMask (after) = " << kModeHex << state.selfProtectSourceMask << newline;
		#endif
			}
		
		if (circulationFanMalfunction && getDoorState())
			{
			if (controller.ticks() >= nextCirculationFanToggleTime)
				{
				if (circulationFanOnPending)
					{
					coreControllerEnableExternalInterrupt(FanTachoSignalInputPin, NULL);
					setFanState(true);
					circulationFanOnPending = false;
					nextCirculationFanToggleTime = (controller.ticks() + (30L * 1000L));
					}
				else
					{
					coreControllerDisableExternalInterrupt(FanTachoSignalInputPin);
					setFanState(false);
					circulationFanOnPending = true;
					nextCirculationFanToggleTime = (controller.ticks() + (5L * 60L * 1000L));
					}
				}
			}
		}

	setFanState(false);
	state.error = kGAINNoError;

#ifdef NSD_SUPPORT
	NSD.removeDIAGCode(kNSDDiagTemporaryFault);
	NSD.sendDIAG(false);
#endif

	sendEventSelf(e);
	}

void GAINClass::handleStateError(uint16 error)
	{
	event	e;
	bool	done(false);
	errorDescription	description;
	
	memcpy_P(&description, &GAINErrorDescription[error], sizeof(description));
	state.error = error;

#if (__DEBUG__ > 0)
	controller.asc0.lock();
	controller.asc0 << "handleStateError:" << newline;
	controller.asc0 << "error ID " << kModeHex << description.errorID << ", annotation " << kModeHex << description.annotation << newline;
	controller.asc0.unlock();	
#endif
	state.error = error;
	panel.syncWithState(state.currentState, GAINGetHMIFlags());

#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_Error, true);
	NSD.addDIAGCode((NSDDiagCode)description.annotation);
	NSD.sendDIAG(false);
#endif
	persistent.logError(description.errorID, description.annotation);

	while (!done)
		{
		if (handleInStateEvent(e, forever, done))
			{
			switch (e.eventClass)
				{
				case kHMIEventClass:
					switch (e.eventType)
						{
 						case kEventPowerButtonPressed:
							persistent.clearWorkingStateForGAIN();
							controller.reset();
 							break;

						default:
							break;
						}
					break;

				default:
					break;
				}
			}
		}

	sendEventSelf(e);
	}


void GAINClass::handleStateMaintenance(uint16 flags)
	{
	event	e;
	bool	done(false);
	
#if (__DEBUG__ > 3)
	controller.asc0.lock();
	controller.asc0 << "handleStateMaintenance" << newline;
	controller.asc0.unlock();	
#endif
	panel.syncWithState(state.currentState, GAINGetHMIFlags());
#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_Maintenance, true);
#endif

	while (!done)
		{
		if (handleInStateEvent(e, forever, done))
			{
			switch (e.eventClass)
				{			
				case kHMIEventClass:
					switch (e.eventType)
						{
						case kEventPowerButtonPressed:
							if (!state.disinfectionOngoing)
								{
								coreControllerReset();
								}
 							break;

						default:
							break;
						}
					break;

				default:
					break;
				}
			}
		}

	sendEventSelf(e);
	}
	
void GAINClass::handleStateHMIUpdate(uint16 flags)
	{
	uint8	buffer[2];
	
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "handleStateHMIUpdate" << newline;
	controller.asc0.unlock();	
#endif
#ifdef NSD_SUPPORT
	NSD.setGAINState(kState_Maintenance, true);
#endif
	controller.disableWatchdog();

	coreAsyncSerialRead(kUART0, buffer, 1, 100);
	buffer[0] = 0x14;
	buffer[1] = 0x10;
	controller.asc0.writeData(buffer, 2);

	panel.update(state.nextGenerationHMIAvailable);

	coreControllerReset();
	}

void GAINClass::handleStateFirmwareUpdate(uint16 flags)
	{
	uint8	buffer[2];
	
#if __DEBUG__ > 4
	controller.asc0.lock();
	controller.asc0 << "handleStateFirmwareUpdate" << newline;
	controller.asc0.unlock();	
#endif
	panel.syncWithState(state.currentState);

	coreAsyncSerialRead(kUART0, buffer, 1, 100);
	buffer[0] = 0x14;
	buffer[1] = 0x10;
	controller.asc0.writeData(buffer, 2);

	controller.update();
	}

bool GAINClass::abortCateringCycle(void)
	{
 	uint8	result(0);

#if __DEBUG__ > 1
	controller.asc0.lock();
	controller.asc0 << "abortCateringCycle" << newline;
	controller.asc0.unlock();	
#endif
	state.inCycle = false;
	state.forcedConvectionMode = !getSteamSensorState();
#ifdef NSD_SUPPORT
	state.powerTimeGranted = 0;
	state.powerBudgetGranted = 0;
	NSD.cancelPBR();
	NSD.setGAINRemainingTime(0, false);
	NSD.setGAINPowerBudgetUsed(0, false);
#endif
	return (result == 0);
	}

bool GAINClass::handleInStateEvent(event& e, tick_t t, bool& done)
	{
	bool	result;
	
	done = false;

	result = waitForEvent(e, kAnyEventClass, kAnyEventType, t);
	
	if (result)
		{
	#if __DEBUG__
		controller.asc0.lock();
	#endif

	#if __DEBUG__ > 3
		controller.asc0 << "handleInStateEvent got event: ";
		controller.asc0 << '(' << kModeHex << e.eventClass << '/' << kModeHex << e.eventType << ")" << newline;
	#endif
		switch (e.eventClass)
			{
		#ifdef NSD_SUPPORT
			case kNSDEventClass:
				switch (e.eventType)
					{
					case kNSDMGCUConnectionLost:
						panel.syncWithNetworkState();
					#if __DEBUG__ > 1
						controller.asc0 << "kNSDMGCUConnectionLost" << newline;
					#endif
 						break;

					case kNSDMGCUConnectionEstablished:
						panel.syncWithNetworkState();
					#if __DEBUG__ > 1
						controller.asc0 << "kNSDMGCUConnectionEstablished" << newline;
					#endif
   						break;

					case kNSDCentralizedPowerLost:
						NSD.setGAINMode(kMode_Fallback, true);
						panel.syncWithNetworkState();
					#if __DEBUG__ > 1
						controller.asc0 << "kNSDCentralizedPowerLost" << newline;
					#endif
						state.centralizedPowerControl = NSD.CentralizedPowerControlAvailable();
						saveCurrentState();
 						break;

					case kNSDCentralizedPowerEstablished:
						NSD.setGAINMode(kMode_Centralized, true);
						panel.syncWithNetworkState();
					#if __DEBUG__ > 1
						controller.asc0 << "kNSDCentralizedPowerEstablished" << newline;
					#endif
						state.centralizedPowerControl = NSD.CentralizedPowerControlAvailable();
						saveCurrentState();
   						break;

					case kNSDPowerBudgetChanged:
					#if __DEBUG__
						controller.asc0.lock();
						controller.asc0 << "handleInState Event: kNSDPowerBudgetChanged received: powerBudgetMax == "
										<< e.data.wordData[0] << ", time == " << e.data.wordData[1] << newline;
						controller.asc0.unlock();
					#endif
						if ((state.currentState >= kGAINStateInterrupted) && (state.currentState <= kGAINStateStoppedProcess))
							{
							//	PBM (0,X) in ON state
							if ((state.currentState > kGAINStateInterrupted) && (e.data.wordData[0] == 0))
								{
							#if __DEBUG__
								controller.asc0.lock();
								controller.asc0 << "handleInState Event: abort catering cycle received." << newline;
								controller.asc0.unlock();
							#endif
								prepareStateChangeEvent(e, kGAINStateInterrupted);
								done = true;
								}
							// PBM(0, xxx) is handled by NSDHandler class	
							else
								{
								if (e.data.wordData[0] > state.powerBudgetRequested)
									state.powerBudgetGranted = state.powerBudgetRequested;
								else
									state.powerBudgetGranted = e.data.wordData[0];

								if (e.data.wordData[1] > 0)
									state.powerTimeGranted = e.data.wordData[1];
								else
									state.powerTimeGranted = state.powerTimeRequested;

								NSD.setGAINPowerBudgetUsed(state.powerBudgetGranted, false);
								NSD.setGAINRemainingTime(state.remainingTime, false);
								}
							}
						break;
						
					case kNSDDisinfectionModeStarted:
					#if __DEBUG__  > 3
						controller.asc0.lock();
						controller.asc0 << "Disinfection Mode started" << newline;
						controller.asc0.unlock();
					#endif
						state.disinfectionOngoing = true;
						if (configuration.commons.subPartNumber == 1)
							{
							prepareStateChangeEvent(e, kGAINStateMaintenance, kDisinfectionOngoing);
							done = true;
							}
   						break;

					case kNSDDisinfectionModeFinished:
					#if __DEBUG__  > 3
						controller.asc0.lock();
						controller.asc0 << "Disinfection Mode finished" << newline;
						controller.asc0.unlock();
					#endif
						state.disinfectionOngoing = false;
						if (configuration.commons.subPartNumber == 1)
							{
							prepareStateChangeEvent(e, kGAINStateStandbyOFF);
							done = true;
							}
   						break;

					case kNSDPinProgrammingError:
					#if __DEBUG__ > 1
						controller.asc0 << "kNSDPinProgrammingError: " << kModeHex << e.data.data << newline;
					#endif
  						break;

					case kNSDSelftestRequest:
					#if __DEBUG__ > 1
						controller.asc0 << "kNSDSelftestRequest: " << kModeHex << e.data.data << newline;
					#endif
						if (!stateIsCateringCycleState(state.currentState) && (e.data.data == kStartSelfTest))
							{
							persistent.clearWorkingStateForGAIN();
							controller.reset();
							}
						else
							NSD.sendDIAG(true);
   						break;
					
					default:
						break;
					}
				break;
		#endif					

			case kHMIEventClass:
				switch (e.eventType)
					{
					case kHMISendConfigurationEvent:
						state.nextGenerationHMIAvailable = (e.data.byteData[0] != 0);
						saveCurrentState();
						break;

					default:
						break;
					}

				if ((state.currentState > kGAINStateStandbyOFF) && (state.currentState < kGAINStateError))
					{
					switch (e.eventType)
						{
						case kHMIChangeTimeEvent:
							if (state.currentState != kGAINStateInterrupted)
								{
								persistentConfiguration.cycle.cycleDuration = (int16)e.data.data;
								if ((state.currentState == kGAINStateON) || (state.currentState == kGAINStateStoppedProcess))
									{
									state.cycleDuration = (tick_t)persistentConfiguration.cycle.cycleDuration * 1000;
								#ifdef NSD_SUPPORT
						    		if (NSD.CentralizedPowerControlAvailable())
							    		{
										tick_t	nominalTime(state.cycleDuration - state.totalHeatingTime);
        						
        								if (nominalTime > state.remainingTime)
        									{
											NSD.setGAINState(kState_Waiting, true);
											NSD.sendPBR(state.powerBudgetGranted, nominalTime,
														state.powerBudgetGranted, nominalTime);
											}
										}
			                    #endif
									}
								else
									{
									panel.syncWithRemainingTime(persistentConfiguration.cycle.cycleDuration, (persistentConfiguration.delayTime > 0)?1:0);
									}							

								persistent.writePersistentConfigurationDataArea(persistentConfiguration);
								panel.syncWithCycleDescription(persistentConfiguration.cycleDescriptionID, persistentConfiguration.cycle);
							#if (__DEBUG__ > 3)
								controller.asc0 << "heatingCycleDuration changed: " << persistentConfiguration.cycle.cycleDuration << newline;
							#endif
								}
							break;
							
						case kHMIChangeTemperatureEvent:
						#if (__DEBUG__ > 3)
							controller.asc0 << "received kHMIChangeTemperatureEvent" << newline;
						#endif
							persistentConfiguration.cycle.targetTemperature = e.data.data;
							state.targetTemperature = persistentConfiguration.cycle.targetTemperature;
							persistent.writePersistentConfigurationDataArea(persistentConfiguration);
							panel.syncWithCycleDescription(persistentConfiguration.cycleDescriptionID, persistentConfiguration.cycle);
							panel.syncWithTemperature(state.targetTemperature, state.cabinetTemperature);
							break;
							
						case kHMIChangeDelayTimeEvent:
						#if (__DEBUG__ > 3)
							controller.asc0 << "received kHMIChangeDelayTimeEvent" << newline;
						#endif
							persistentConfiguration.delayTime = (int16)e.data.data;
							persistent.writePersistentConfigurationDataArea(persistentConfiguration);
							panel.syncWithCycleDescription(persistentConfiguration.cycleDescriptionID, persistentConfiguration.cycle);
						#if (__DEBUG__ > 3)
							controller.asc0 << "delay time changed: " << persistentConfiguration.delayTime << newline;
						#endif
							break;
							
						case kHMIChangeModeEvent:
						#if (__DEBUG__ > 3)
							controller.asc0 << "received kHMIChangeModeEvent: id == " << e.data.data << newline;
						#endif
							persistentConfiguration.cycleDescriptionID = e.data.data;
							persistent.readCycleDescription(persistentConfiguration.cycle, persistentConfiguration.cycleDescriptionID);
							persistent.writePersistentConfigurationDataArea(persistentConfiguration);
							state.targetTemperature = persistentConfiguration.cycle.targetTemperature;
							panel.syncWithCycleDescription(persistentConfiguration.cycleDescriptionID, persistentConfiguration.cycle);
							panel.syncWithRemainingTime(persistentConfiguration.cycle.cycleDuration, (persistentConfiguration.delayTime > 0)?1:0);
							break;
							
						default:
							break;
						}
					}
				break;

			case kCommunicationEventClass:
				switch (e.eventType)
					{
					case kUART0DataAvailableEvent:
						switch (e.data.data)
							{
							case '0':		//	Firmware update
								prepareStateChangeEvent(e, kGAINStateFirmwareUpdate);
								done = true;
								break;

							case 'R':	//	Panel Firmware update
								prepareStateChangeEvent(e, kGAINStateHMIUpdate);
								done = true;
								break;

							default:
								break;
							}
						break;

					default:
						break;
					}
				break;
				
			case kGAINEventClass:
				{
				switch (e.eventType)
					{
					case kGAINSwitchStateEvent:
						done = true;
						break;

					case kGAINResetEvent:
					#if __DEBUG__ > 3
						controller.asc0 << "GAIN: controller reset!" << newline;
					#endif
						persistent.clearWorkingStateForGAIN();
						controller.reset();
						break;
					
					case kGAINMaintenanceConnectionEstablishedEvent:
						state.maintenanceToolConnected = true;
						panel.syncWithState(state.currentState, GAINGetHMIFlags());
						break;
						
					case kGAINMaintenanceConnectionLostEvent:
						if (state.maintenanceToolConnected)
							{
							state.maintenanceToolConnected = false;
                            
							panel.syncWithState(state.currentState, GAINGetHMIFlags());

							if (e.data.data)
								{
								persistent.clearWorkingStateForGAIN();
								setPersistentConfigurationDefaults();
								controller.reset();
								}
							}
						break;
						
                    case kGAINPhaseLossEvent:
						prepareStateChangeEvent(e, kGAINStateError, kGAINErrorPhaseLost);
						done = true;
    	 				break;

					case kGAINDoorOpened:
					#if __DEBUG__ > 3
						controller.asc0.lock();
						controller.asc0 << "door opened\r";
						controller.asc0.unlock();
					#endif
						persistent.increaseDoorOpenCounter();
					#if defined(NSD_SUPPORT)
				    	NSD.removeDIAGCode(kNSDDiagFoodNotRemoved);
				    	//NSD.addDIAGCode(kNSDDiagDoorOpen);
						NSD.sendDIAG(false);
					#endif
						break;

					case kGAINDoorClosed:
					#if __DEBUG__ > 3
						controller.asc0.lock();
						controller.asc0 << "door closed\r";
						controller.asc0.unlock();
					#endif
					#if defined(NSD_SUPPORT)
				    	//NSD.removeDIAGCode(kNSDDiagDoorOpen);
						//NSD.sendDIAG(false);
					#endif
						break;

					case kGAINAmbientTemperatureOutOfRange:
					#if (__DEBUG__ > 0)
						controller.asc0 << coreSystemTimerTicks() << ": Received kGAINAmbientTemperatureOutOfRange" << newline;
					#endif
						prepareStateChangeEvent(e, kGAINStateSelfProtect, kGAINErrorAmbientTemperatureOutOfRange);
						done = true;
						break;

					case kGAINControllerTemperatureOutOfRange:
					#if (__DEBUG__ > 0)
						controller.asc0 << coreSystemTimerTicks() << ": Received kGAINControllerTemperatureOutOfRange" << newline;
					#endif
						prepareStateChangeEvent(e, kGAINStateSelfProtect, kGAINErrorControllerTemperatureOutOfRange);
						done = true;
						break;

					case kGAINWaterLowPressure:
					case kGAINWaterOverPressure:
					#if (__DEBUG__ > 3)
						if (e.eventType == kGAINWaterLowPressure)
							controller.asc0 << "received kGAINWaterLowPressure" << newline;
						else
							controller.asc0 << "received kGAINWaterOverPressure" << newline;
					#endif
						state.insufficientWaterPressure = true;
						state.overPressure = (e.eventType == kGAINWaterOverPressure);
						
						if ((state.currentState != kGAINStateON) && ((persistentConfiguration.cycle.cycleFlags & kCycleModeMask) == kSteamMode))
							{
							persistentConfiguration.cycleDescriptionID = 1;
							panel.syncWithCycleDescriptionID(persistentConfiguration.cycleDescriptionID, kDefrostMode);
							
							persistent.readCycleDescription(persistentConfiguration.cycle, persistentConfiguration.cycleDescriptionID);
							persistent.writePersistentConfigurationDataArea(persistentConfiguration);
							state.targetTemperature = persistentConfiguration.cycle.targetTemperature;
							panel.syncWithCycleDescription(persistentConfiguration.cycleDescriptionID, persistentConfiguration.cycle);
							panel.syncWithRemainingTime(persistentConfiguration.cycle.cycleDuration, (persistentConfiguration.delayTime > 0)?1:0);
							//	sendEventSelf(kHMIEventClass, kHMIChangeModeEvent, 1);
							}
						else
							panel.syncWithCycleDescriptionID(persistentConfiguration.cycleDescriptionID, kDefrostMode);
					#if defined(NSD_SUPPORT)
						if (e.eventType == kGAINWaterLowPressure)
					    	NSD.addDIAGCode(kNSDDiagLowPressure);
	    				else
				    		NSD.addDIAGCode(kNSDDiagOverPressure);
						NSD.sendDIAG(false);
					#endif
						break;

					case kGAINWaterPressureInRange:
					#if (__DEBUG__ > 3)
						controller.asc0 << "received kGAINWaterPressureInRange" << newline;
					#endif
						state.insufficientWaterPressure = false;
						state.overPressure = false;
						result = panel.syncWithCycleDescriptionID(persistentConfiguration.cycleDescriptionID, kSteamMode);
					#if defined(NSD_SUPPORT)
				    	NSD.removeDIAGCode(kNSDDiagLowPressure);
				    	NSD.removeDIAGCode(kNSDDiagOverPressure);
						NSD.sendDIAG(false);
					#endif
						break;
					
					default:
						break;
					}
				}
				break;

			case kErrorEventClass:
			#if (__DEBUG__ > 3)
				controller.asc0 << "oven::handleInStateEvent(): kErrorEventClass event\r";
			#endif	
				if (e.eventType == kGAINNoError)
					prepareStateChangeEvent(e, kGAINStateStandbyON);
				else
					prepareStateChangeEvent(e, kGAINStateError, e.eventType);
				done = true;
				break;
			
			default:
				break;
			}
	#if __DEBUG__
		controller.asc0.unlock();
	#endif
		}

	return result;
	}

void GAINClass::prepareStateChangeEvent(event& e, uint16 newState, uint16 data)
	{
	e.eventClass = kGAINEventClass;
	e.eventType = kGAINSwitchStateEvent;
	e.data.wordData[0] = newState;
	e.data.wordData[1] = data;
	}

uint16 GAINClass::handlePowerOnSelftest(void)
	{
	event	e;
	uint16	selfTestResult = kGAINNoError;
	int16	temperature;
		
	controller.wait(500, 50, WaitEventCallbackHandler);

	temperature = getAmbientTemperature();
	if ((temperature < -50)||(temperature > 300))
		{
		selfTestResult = kGAINErrorAmbientSensorFailure;
		prepareStateChangeEvent(e, kGAINStateError, selfTestResult);
		sendEventSelf(e);
		}
		
	if (GAIN.configuration.commons.subPartNumber > 0)
		{
		temperature = getSteamerTemperature();
		if ((temperature < -50)||(temperature > 300))
			{
			selfTestResult = kGAINSteamerTemperatureSensorFailure;
			prepareStateChangeEvent(e, kGAINStateError, selfTestResult);
			sendEventSelf(e);
			}
		}
		
	temperature = getCabinetTemperature();
	if ((temperature < -50)||(temperature > 300))
		{
		selfTestResult = kGAINCabinetTemperatureSensorFailure;
		prepareStateChangeEvent(e, kGAINStateError, selfTestResult);
		sendEventSelf(e);
		}

	for (uint8 i=0; (i<10) && !panel.syncWithState(state.currentState); i++)
		controller.wait(100, 50, WaitEventCallbackHandler);
		
	if (!panel.isConnected())
		{
		selfTestResult = kGAINErrorHMIFailure;
		prepareStateChangeEvent(e, kGAINStateError, selfTestResult);
		sendEventSelf(e);
		}
	

#ifdef NSD_SUPPORT
	if (selfTestResult == kGAINNoError)
		{
		while (!NSD.connectionStateDetermined())
			controller.wait(100, 50, WaitEventCallbackHandler);
		}
#endif

	return selfTestResult;
	}

#ifdef NO_PINCODING
void GAINClass::initializePinCoding(bool write)
	{
	currentPinCoding.magic = 0x1903;
	currentPinCoding.size = sizeof(GAINPinCoding);
	currentPinCoding.feederID = 0x01;
	currentPinCoding.deviceID = 0x01;
	currentPinCoding.pins = 0x01;
	currentPinCoding.pinProgrammingError = 0x00;

#ifdef NSD_SUPPORT
	GAINDataBlock02	buffer;
	
    coreNSD_readGAINConfigurationDataBlock(2, &buffer);
   	if (buffer.GAINImplementedFunctionalities & kGAIN_CentralizedPowerControl)
   		currentPinCoding.pins |= kGAIN_CentralizedPowerControl;
#endif

	if (write)
		coreNVMWrite(kNVM0, 0x40, &currentPinCoding, sizeof(GAINPinCoding));
	}

void GAINClass::updatePinCoding(bool reset)
	{
	currentPinCoding.magic = 0x1903;
	currentPinCoding.size = sizeof(GAINPinCoding);
	coreNVMWrite(kNVM0, 0x40, &currentPinCoding, sizeof(GAINPinCoding));

#ifdef NSD_SUPPORT
	GAINDataBlock02	buffer;
	
    coreNSD_readGAINConfigurationDataBlock(2, &buffer);
   	if (currentPinCoding.pins & kGAIN_CentralizedPowerControl)
   		buffer.GAINImplementedFunctionalities |= kGAIN_CentralizedPowerControl;
 	else
   		buffer.GAINImplementedFunctionalities &= ~kGAIN_CentralizedPowerControl;
	
	coreNSD_writeGAINConfigurationDataBlock(2, &buffer);
#endif

	if (reset)
		controller.reset();
	}
#endif
void GAINClass::setFanState(bool on)
	{
#if (__DEBUG__ > 2)
	controller.asc0 << "setFanState: " << (on?"ON":"OFF") << newline;
#endif
	if (on)
		{
		controller.switchPin(FanSpeedTimerLongOutputPin, state.steamerOn);
		controller.switchPin(FanSpeedOutputPin, state.steamerOn);
		nextFanStateUpdateTime = controller.ticks() + 10000;
		}
	else
		{
		controller.switchPin(FanSpeedTimerLongOutputPin, on);
		controller.switchPin(FanSpeedOutputPin, on);
		GAIN.maintenance.updateAnalogueValue(0, 0);
		}

	controller.switchPin(CirculationFanOutputPin, on);
	if (controller.getPin(FanStartOutputPin) != on)
		persistent.updateFanOnTime(on);
	controller.switchPin(FanStartOutputPin, on);
	}
	
void GAINClass::setValveState(bool on)
	{
#if (__DEBUG__ > 2)
	controller.asc0 << "setValveState: " << (on?"ON":"OFF") << newline;
#endif	
	controller.switchPin(FanSpeedTimerLongOutputPin, on);
	controller.switchPin(FanSpeedOutputPin, on);
	if (on)
		controller.wait(100);
	controller.switchPin(SolenoidValve2OutputPin, on);	//	Switch valve #2
	controller.switchPin(SolenoidValve1OutputPin, on);	//	Switch valve #1
	}	

void GAINClass::setHeaterState(bool on, bool steam)
	{
#if (__DEBUG__ > 2)
	if ((controller.getPin(HeaterOutputPin) != on) || (controller.getPin(SteamerOutputPin) != steam))
		controller.asc0 << "setHeaterState: " << "state == " << state.internalCateringState << ", heater == " << (on?"ON":"OFF") << ", steamer: " << (steam?"ON":"OFF") << newline;
#endif	
	if (controller.getPin(HeaterOutputPin) != on)
		persistent.updateHeaterOnTime(on);

	if (controller.getPin(SteamerOutputPin) != steam)
		persistent.updateSteamerOnTime(steam);

	controller.switchPin(HeaterOutputPin, on);
	controller.switchPin(SteamerOutputPin, steam);

#ifdef OFFLINE
	if (on)
		{
		heaterOnTime = controller.ticks();
		heaterState |= 0x01;
		}
	else
		heaterState &= ~0x01;

	if (steam)
		{
		steamerOnTime = controller.ticks();
		heaterState |= 0x02;
		}
	else
		heaterState &= ~0x02;
#endif
	state.steamerOn = steam;	
	}

void GAINClass::setSafetyRelayState(bool on)
	{
#if (__DEBUG__ > 2)
	controller.asc0 << "setSafetyRelayState: " << (on?"ON":"OFF") << newline;
#endif
	controller.switchPin(SafetyRelayOutputPin, on);
	}

void GAINClass::saveCurrentState(void)
	{
	if (stateShallResume(state.currentState))
		{
		state.now = coreSystemTimerTicks();
	#ifdef E_DEVICE
		coreControllerDisableWatchdog();
	#endif
		persistent.saveWorkingStateForGAIN(state);
	#ifdef E_DEVICE
		coreControllerEnableWatchdog();
	#endif
		}
	else if (state.currentState > kGAINStateResume)
		persistent.clearWorkingStateForGAIN();
	}

bool GAINClass::stateIsCateringCycleState(uint8 state) const
	{
	return ((state >= kGAINStateDelayed) && (state < kGAINStateFinished));
	}

bool GAINClass::stateShallResume(uint8 state) const
	{
#ifdef OFFLINE
	return ((state > kGAINStateStandbyOFF) && (state < kGAINStateError));
#else
	return ((state >= kGAINStateStandbyOFF) && (state < kGAINStateError));
#endif
	}

bool GAINClass::GAINIsReady(void)
	{
	bool	result(GAINGetWaitingFlags() == 0);

#if (__DEBUG__ > 2)
	controller.asc0 << "GAINIsReady: " << (result?"true":"false") << newline;
#endif
	return result;
	}

uint16 GAINClass::GAINGetWaitingFlags(void)
	{
	uint16	result;
	
	if (getDoorState())
		result = 0;
	else
		result = kDoorOpen;

#ifdef NSD_SUPPORT
	if (!NSD.getGAINIsPrimary() && !NSD.CentralizedPowerControlAvailable())
		result |= kNoNetwork;
#endif

#if (__DEBUG__ > 3)
	if (result)
		controller.asc0 << "GAINGetWaitingFlags: " << kModeHex << result << newline;
#endif

	return result;
	}

uint16 GAINClass::GAINGetHMIFlags(void)
	{
	uint16	result(GAINGetWaitingFlags());

	result |= (state.forcedConvectionMode?kForcedConvectionMode:0);
	result |= (state.insufficientWaterPressure?kWaterPressureOutOfRange:0);
	result |= (state.disinfectionOngoing?kDisinfectionOngoing:0);
	result |= (state.maintenanceToolConnected?kDevelopmentToolConnected:0);

	return result;
	}

bool GAINClass::getDoorState(void) const
	{
#ifdef OFFLINE
	return controller.getPin(DoorSwitchInputPin);
#else
	return controller.getPin(DoorSwitchInputPin);
#endif
	}

bool GAINClass::getSteamSensorState(void) const
	{
	return((state.steamerTemperature >= -50) && (state.steamerTemperature <= 300));
	}

/*
PF7	ADC7	PressureSensor
PF6	ADC6	Reserved2
PF5	ADC5	VinSense
PF4	ADC4	FTHTemperature
PF3	ADC3	OverflowTemperature
PF2	ADC2	InletTemperature
PF1	ADC1	Ambient
PF0	ADC0	OutletTemperature
*/
float calculateTemperaturePt1000(int16 rawAnalogue);

float calculateTemperaturePt1000(int16 rawAnalogue)
//P(x)=400.3958438445212-0.6197385321734857x^1+0.00025220892172610546x^2-4.309352334210947e-8x^3+3.083947326105646e-12x^4
	{
	float	temperature(400.3958438445212);
	float	x(rawAnalogue);
	float	x2, x3, x4;
	
	temperature -= (0.6197385321734857 * x);
	x2 = x*x;
	temperature += (0.00025220892172610546 * x2);
	x3 = x2*x;
	temperature -= (4.309352334210947e-8 * x3);
	x4 = x3*x;
	temperature += (3.083947326105646e-12 * x4);
	
	return temperature;
	}
	
#define	kTemperatureHistoryDepth	16

#if __DEBUG__ > 3
bool updateTemperature(int8 type, int16 reading, float& lastTemperature, float& averageTemperature, bool initial, const char* caller);
#else
bool updateTemperature(int8 type, int16 reading, float& lastTemperature, float& averageTemperature, bool initial);
#endif

#if __DEBUG__ > 3
bool updateTemperature(int8 type, int16 reading, float& lastTemperature, float& averageTemperature, bool initial, const char* caller)
#else
bool updateTemperature(int8 type, int16 reading, float& lastTemperature, float& averageTemperature, bool initial)
#endif
	{
	bool	result(true);
	float	temperature;
	
	if (type == 1)
		temperature = ((float)reading)/81.91;
	else
		temperature = calculateTemperaturePt1000(reading);
	
	if (initial)
		averageTemperature = temperature;
	else if ((temperature - lastTemperature) < 5.0)
		{
		float	averageTemperatureTemp;
		
		averageTemperatureTemp = averageTemperature;
		averageTemperatureTemp *= (kTemperatureHistoryDepth - 1);
		averageTemperatureTemp += temperature;
		averageTemperature = averageTemperatureTemp/kTemperatureHistoryDepth;
		}
	else
		{
	#if __DEBUG__ > 3
		controller.asc0 << caller << " == " << temperature  << "  °C, reading == " << kModeHex << reading << ", lastTemperature == "
						<< lastTemperature << " °C, average == " << averageTemperature << "°C" << newline;
	#endif
		result = false;
		}
	
#if	__DEBUG__ > 4
	controller.asc0 << caller << " == " << temperature  << "  °C, average == " << averageTemperature << "°C" << newline;
#endif
	lastTemperature = temperature;

	return result;
	}
	
int16 GAINClass::getAmbientTemperature(bool initial)
	{
	static float	averageTemperature;
	static float	lastTemperature;
	
#ifdef OFFLINE
	averageTemperature = 25.0;
/*
	if (state.currentState == kGAINStateON)
		averageTemperature = 88.0;
*/
/*
	if (controller.getPin(DoorSwitchInputPin))
		averageTemperature = 75.0;
*/
#else
#if __DEBUG__ > 3
	updateTemperature(2, coreADConverterReadSingle(1), lastTemperature, averageTemperature, initial, "getAmbientTemperature");
#else
	updateTemperature(2, coreADConverterReadSingle(1), lastTemperature, averageTemperature, initial);
#endif
#endif
	maintenance.updateAnalogueValue(1, averageTemperature);

	return (int16)(averageTemperature + 0.5);
	}

int16 GAINClass::getCabinetTemperature(bool initial)
	{
	static float	averageTemperature;
	static float	lastTemperature;
	float	result;
	
#ifdef OFFLINE
	if (heaterState & 0x02)
		{
		temperature += 0.5;
		}
	else if (heaterState & 0x01)
		{
		temperature += 0.1;
		}
	else
		{
		if (temperature > 22.0)
			temperature -= 0.05;
		}
			
	if (initial)
		{
		averageTemperature = 20.0;
		temperature = 20.0;
		}
	else
		{
		float	averageTemperatureTemp;
		
		averageTemperatureTemp = averageTemperature;
		averageTemperatureTemp *= (kTemperatureHistoryDepth - 1);
		averageTemperatureTemp += temperature;
		averageTemperature = averageTemperatureTemp/kTemperatureHistoryDepth;
		}
#else
#if __DEBUG__ > 3
	updateTemperature(2, coreADConverterReadSingle(4), lastTemperature, averageTemperature, initial, "getCabinetTemperature");
#else
	updateTemperature(2, coreADConverterReadSingle(4), lastTemperature, averageTemperature, initial);
#endif
#endif
	result = averageTemperature + configuration.cabinetTemperatureOffset;

	maintenance.updateAnalogueValue(4, result);

	return  (int16)(result + 0.5);
	}

int16 GAINClass::getSteamerTemperature(bool initial)
	{
	static float	averageTemperature;
	static float	lastTemperature;
	
#ifdef OFFLINE
	if (heaterState & 0x02)
		{
		if (steamerTemperature < 250.0)
			steamerTemperature += 5.0;
		}
	else
		{
		if (state.internalCateringState == kStateRefill)
			{
			if (steamerTemperature > 160.0)
				steamerTemperature = 160.0;
			}
		else if (state.internalCateringState == kStateRelaxSteamer)
			{
			if (steamerTemperature > 50.0)
				steamerTemperature -= 0.5;
			}
		else
			{
			if (steamerTemperature > temperature)
				steamerTemperature -= 0.05;
			}
		}
			
	if (initial)
		{
		averageTemperature = 20.0;
		steamerTemperature = 20.0;
		}
	else
		{
		float	averageTemperatureTemp;
		
		averageTemperatureTemp = averageTemperature;
		averageTemperatureTemp *= (kTemperatureHistoryDepth - 1);
		averageTemperatureTemp += steamerTemperature;
		averageTemperature = averageTemperatureTemp/kTemperatureHistoryDepth;
		}
#else
#if __DEBUG__ > 3
	updateTemperature(2, coreADConverterReadSingle(3), lastTemperature, averageTemperature, initial, "getSteamerTemperature");
#else
	updateTemperature(2, coreADConverterReadSingle(3), lastTemperature, averageTemperature, initial);
#endif
#endif
	maintenance.updateAnalogueValue(3, averageTemperature);

	return  (int16)(averageTemperature + 0.5);
	}

#define	kPressureHistoryDepth	4

float GAINClass::getWaterPressure(bool initial)
	{
	static float	averagePressure;
	float	pressure;
	float	result;

#ifdef OFFLINE
#ifdef E_DEVICE
	if (corePortsGetPortPin(kPortC, kPortPin7))
		pressure = 2000.0;
	else
		pressure = 400.0;	
#else
	if ((state.internalCateringState == kStatePrepareRefill)||(state.internalCateringState == kStateRefill))
		pressure = 600.0;	
	else
		pressure = 1000.0;
#endif	
#else
	int16	value = coreADConverterReadSingle(7);
	
	value -= 978;
	if (value > 0)
		pressure = (float)value * 1.232087;	// 2.462413;
	else
		pressure = 0;
#endif
		
	if (initial)
		averagePressure = pressure;
	else
		{
		float	averagePressureTemp;
		
		averagePressureTemp = averagePressure;
		averagePressureTemp *= (kPressureHistoryDepth - 1);
		averagePressureTemp += pressure;
		averagePressure = averagePressureTemp/kPressureHistoryDepth;
		}
	
#if	__DEBUG__ > 3
	controller.asc0 << "average pressure == " << averagePressure << " hPa" << newline;
#endif
	result = averagePressure + configuration.pressureSensorOffset;
	maintenance.updateAnalogueValue(7, result);

	return result;
	}

