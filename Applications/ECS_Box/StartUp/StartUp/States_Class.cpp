/* 
* States_Class.cpp
*
* Created: 6/22/2020 7:24:49 PM
* Author: GMateusDP
*/


#include "States_Class.h"
#include "MemoryManagment_Class.h"
#include "Calendar_Class.h"
#include "Interfaces_Class.h"
#include "Timer_Class.h"
#include "CDC_Class.h"
#include "ALU_Class.h"
#include "CBIT_Class.h"

	// default constructor
States_Class::States_Class(void)
	{
	listener.eventHandler = NULL;
state=defaultState;
	}

// default destructor
States_Class::~States_Class()
{
} //~States_Class

bool States_Class::Init(void){
	return fv1.Init();
	
}
void States_Class::Start(uint8_t	operationMode)
	{
	event	e;
    tick_t	timerValue = 0L;
    bool	isCurrentStateValid = false;
	memory.ReadFastApplicationState(lastState);
	state.currentState = kGAINStateReset;
	if (calendar.WasInterrupted())
	{
		powerInterruptDuration = calendar.GetPowerInterrupTime();
		#ifdef OFFLINE
		resumeLastStateValid = ((powerInterruptDuration > 0) && (powerInterruptDuration < 5000L));
		#else
		resumeLastStateValid = ((powerInterruptDuration >= 0) && (powerInterruptDuration < 5000L));
		#endif
		#if __DEBUG__ > 3

		usb << "powerInterruptDuration was " << powerInterruptDuration << (resumeLastStateValid?" 200msec":" power resume") << newline;
		
		#endif
	}else{
		memory.LogResetEvent();
	}

	

	//	Check persistent memory
	if (SetInitialState()<0){
		SetDefaultState();
	}



	readResult=memory.ReadLastConfigurationData(configuration);
	if (readResult>0)
		{
	#if __DEBUG__ > 3
		usb << "readConfigurationDataArea failed!" << newline;
	#endif
		SetFactoryDefaults(0, true, false);
		}
#if __DEBUG__ > 3
	else
		{
		usb << "configuration.minimumTargetVoltage == " << configuration.minimumTargetVoltage << newline;
		usb << "configuration.maximumTargetVoltage == " << configuration.maximumTargetVoltage << newline;
		usb << "configuration.m == " << configuration.m << newline;
		usb << "configuration.zA0 == " << configuration.zA0 << newline;
		}
#endif

	if (!memory.readPersistentConfigurationDataArea(persistentConfiguration))
		{
	#if __DEBUG__ > 2
		usb << "readPersistentConfigurationDataArea failed!" << memory.getUptime() << newline;
	#endif
		setPersistentConfigurationDefaults();
		}
#if __DEBUG__ > 2
	usb << "persistentConfiguration.cycle, id == " << persistentConfiguration.cycleDescriptionID
					<< ", description == " << persistentConfiguration.cycle.description
					<< ", power == " << persistentConfiguration.cycle.power
					<< ", duration == " << persistentConfiguration.cycle.duration << ", maximum time == " << persistentConfiguration.cycle.maximumDuration
					<< ", flags == " << persistentConfiguration.cycle.cycleFlags << newline;
#endif
	

	communicator.CheckCommunication();

	if (isCurrentStateValid)
    {
		prepareStateChangeEvent(e, kGAINStateResume);
    }else{
		prepareStateChangeEvent(e, kGAINStateInitialize);
    }

	listener.SendEventSelf(e);



    while (cBit.IsOK())
		{
		if (listener.WaitForEvent(e, kGAINEventClass, kGAINSwitchStateEvent))
			{
			handleHVACEvent(e);
			}
        else{

        }
		}
		Stop(0);
	}


void States_Class::setPersistentConfigurationDefaults(void)
	{
	setPersistentConfiguration(0);
	}

void States_Class::setPersistentConfiguration(uint8 cycleID)
	{
	persistentConfiguration.cycleDescriptionID = cycleID;
	readCycleDescription(persistentConfiguration.cycle, persistentConfiguration.cycleDescriptionID);
   writeResult=memory.writePersistentConfigurationDataArea(persistentConfiguration);
	} 	



bool States_Class::readCycleDescription(CycleDescription& cycle, uint8 id)
	{
	bool	result(memory.readCycleDescription(cycle, id));

	return result;
	}


void States_Class::handleHVACEvent(event& e)
	{
	if (state.currentState != kGAINStateResume)
		state.callingState = state.currentState;
	state.currentState = e.data.wordData[0];
	state.stateFlags = e.data.wordData[1];
	state.stateEntryTime = hvacTimer.get_ticks();

#if __DEBUG__ > 0
	
	usb << "handleGAINEvent received: switching from ";
	usb.write(gainStateDescription[state.callingState]);
	usb << " to ";
	usb.write(gainStateDescription[state.currentState]);
	usb << " (flags == " << state.stateFlags << ')' << NEWLINE;
	
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

		default:
			handleStateError(kGAINErrorIllegalStateTransition);
			break;
		}
	}


void States_Class::handleStateInitialize(uint32 flags)
	{
	event	e;
   	uint16	selfTestResult(kGAINNoError);
	
#if __DEBUG__ > 0
	
	usb << "handleStateInitialize: flags == 0x" << flags << NEWLINE;
		
#endif
	
	
	listener.CheckEvent();
	if (pBIT.isOK)
	{
		#if __DEBUG__ > 0
		
		usb << "handleStateInitialize" << NEWLINE;
		
		#endif
		


		#ifdef _OFFLINE
		prepareStateChangeEvent(e, kGAINStateInterrupted);
		listener.SendEventSelf(e);
		#else
		selfTestResult = handlePowerOnSelftest();
		if (selfTestResult == kGAINNoError)
		{
			prepareStateChangeEvent(e, kGAINStateStandbyOFF);
			listener.SendEventSelf(e);
		}
		#endif

		listener.CheckEvent();
	} 
	else
	{
		Stop();
	}



	
	}


void States_Class::handleStateResume(uint32 flags)
	{
	event	e;
	uint8	currentState(state.currentState);
    uint16	selfTestResult(kGAINNoError);

#if __DEBUG__ > 3
	usb << "handleStateResume" << newline;
#endif

#if __DEBUG__ > 3
	usb << "state.currentState (before) " << state.currentState << newline;
#endif
	memory.restoreWorkingStateForGAIN(state);
#if __DEBUG__ > 3
	usb << "state.currentState (after) " << state.currentState << newline;
#endif


    //Switch into current state if no error occured
    if(selfTestResult == kGAINNoError)
    {
        switch (state.currentState)
        {
        case kGAINStateON:
            prepareStateChangeEvent(e, state.currentState, true);
            break;

        case kGAINStateStoppedProcess:
            prepareStateChangeEvent(e, kGAINStateStoppedProcess);
            break;

        default:
            prepareStateChangeEvent(e, state.currentState);
            break;
        }
    }

	state.currentState = currentState;

	listener.CheckEvent();
	listener.SendEventSelf(e);
	}
				
void States_Class::handleStateStandbyOFF(uint32 flags)
	{
	event	e;
	bool	done(false);
   	
#ifdef __DEBUG__
	
	usb << "handleStateStandbyOFF" << NEWLINE;
		
#endif

	state.smpsTemperatureOutOfRangeEventSent = false;
	state.magnetronTemperatureOutOfRangeEventSent = false;
	state.exhaustTemperatureOutOfRangeEventSent = false;

	state.magnetronFanFailureDetectionTime = forever;




	listener.SendEventSelf(e);
	}

void States_Class::handleStatePrepareStandbyON(uint32 flags)
	{
	event	e;
	bool	done(false);
	uint32	connectFlags(state.alarmState?kConnectFlagsAlarmOn:0);
	   	
#ifdef __DEBUG__
	
	usb << "handleStatePrepareStandbyON" << NEWLINE;
		
#endif

	
	tick_t	leaveDisplayTime = hvacTimer.get_ticks() + 4000;

	

	listener.SendEventSelf(e);
	}

void States_Class::handleStateStandbyON(uint32 flags)
	{
	event	e;
	bool	done(false);

#if __DEBUG__ > 1
	
	usb << "handleStateStandbyON" << newline;
		
#endif

	state.latchClosedConfirmPending = false;
    state.remainingTime = (tick_t)persistentConfiguration.cycle.duration * 1000L;

	
	state.reedRelayOpen = false;
	
	listener.SendEventSelf(e);
	}

void States_Class::handleStateStandbyReady(uint32 flags)
	{
	event	e;
	bool	done(false);

	
	listener.SendEventSelf(e);
	}


void States_Class::handleStateInterrupted(uint32 flags)
	{
	event	e;
	bool	done(false);
	bool	resume(flags & 0x0001);		
	bool	receivedPBM(flags & 0x0002);
	bool	syncPanelWithStateInterrupted(false);
	tick_t	zeroTime(hvacTimer.get_ticks());
	tick_t	remainingTime(0);


	listener.SendEventSelf(e);
	}

void States_Class::handleStateON(uint32 flags)
	{
	event	e;
	bool	done(false);
	bool	checkForEmptyCavity(true);
	bool	checkForSMPSTemperature(true);
	bool	resume(flags != 0);		
	tick_t	zeroTime;
	tick_t	magnetronTemperatureSaveTime;
	float	magnetronTemperatureSave;
	float	smpsTemperatureSave;
	float	Tmec;
						
#if __DEBUG__ > 1
	
	usb << "handleStateON: resume == " << (resume?"true":"false") << newline;
		
#endif


	listener.SendEventSelf(e);
	}

void States_Class::handleStateFinished(uint32 flags)
	{
	event	e;
	bool	done(false);
	listener.SendEventSelf(e);
	}

void States_Class::handleStateStoppedProcess(uint32 flags)
	{
	event	e;
 	bool	done(false);
 	bool	abortCycle(true);
    uint16	resumeFlags(flags);
	listener.SendEventSelf(e);
	}

void States_Class::handleStateSelfProtect(uint32_t error)
	{
	event	e;
	bool	done(false);
	listener.SendEventSelf(e);
	}

void States_Class::handleStateError(uint32_t error)
	{
	event	e;
	bool	done(false);
	listener.SendEventSelf(e);
	}

void States_Class::handleStateMaintenance(uint32 flags)
	{
	event	e;
	bool	done(false);
	listener.SendEventSelf(e);
	}
	
bool States_Class::handleInStateEvent(event& e, tick_t t, bool& done)
	{
	bool	result;
	done = false;
	return result;
	}

void States_Class::prepareStateChangeEvent(event& e, uint16 newState, uint16 data)
	{
	e.eventClass =(EventClass)kGAINEventClass;
	e.eventType = (EventType)kGAINSwitchStateEvent;
	e.data.wordData[0] = newState;
	e.data.wordData[1] = data;
	}

uint32_t States_Class::handlePowerOnSelftest(void)
	{
	event	e;
	uint16	selfTestResult(kGAINNoError);
	bool	moduleAvailable;
	int16	temperature;
		
	return selfTestResult;
	}


void States_Class::saveCurrentState(void){
		calendar_date_time	cdt;
		calendar.GetDateTime(&cdt);
		state.dateTime=cdt;
		state.now = calendar.convert_datetime_to_timestamp(&cdt);
	if (state.currentState!=kGAINStateMaintenance){
		memory.WriteFastCurrentState(state);
		//memory.ReadFastApplicationState(lastState);
		if (state.currentState==0)
		{
			memory.SaveApplicationState(state);
			//delay_ms(100);	
		}
		
	}else{
// 		 if (state.currentState > kGAINStateResume){
// 			 
// 			memory.clearWorkingStateForGAIN();
// 		 }
	}
}


uint8_t	States_Class::Stop(uint8_t	layer){
	return	layer;
}

void	States_Class::GetCurrentConfiguration(ConfigurationData *cd){
	cd=&configuration;
}
void	States_Class::GetCurrentState(HVACState& hs){
	hs=state;
}
void	States_Class::SetConfigurationData(ConfigurationData& cd){
	configuration=cd;
}
void	States_Class::SetCurrentState(HVACState& hs){
	state=hs;
}
void	States_Class::SetCRCConfigData(void){
	CRC32=memory.CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&factoryDefaultsConfiguration),sizeof(factoryDefaultsConfiguration));
}
uint32_t	States_Class::GetStatus(HVACStatus& s){
	s=state.status;
	return	state.status.rawStatus;
}


States_Class	hvac;