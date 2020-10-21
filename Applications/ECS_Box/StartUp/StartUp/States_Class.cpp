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
#include "FlapperValve_Class.h"
#include "TemperatureSensors_Class.h"

States_Class*	ptrStatesClass;
static void  HVACOperationTimeElapsed(const struct timer_task *const timer_task){
	ptrStatesClass->timeElapsed=true;
}
static void HVACTimerTicks(const struct timer_task *const timer_task)
{
	hvacTimer.ticks++;
}

static HVACState staticCurrentState;
	// default constructor
States_Class::States_Class(void)
{
	ptrStatesClass=this;
	listener.eventHandler = NULL;
	staticCurrentState=defaultState;
	isOK=false;
	hvacState=&staticCurrentState;
}

// default destructor
States_Class::~States_Class()
{
} //~States_Class

bool States_Class::Init(void){
	if (hvacTimer.Init())
	{
		hvacTimer.Start_periodic_task(FUNC_PTR(HVACTimerTicks),1);
	}
	isOK=temperatures.Init();
	if (isOK)
	{
		temperatures.StartOneConversion();
	}
	if (fv1.Init())
	{
		hvacState->hvacStatus.statusBits.flapValve1=true;
		//alu.PrepareNewTask(kALUTaskUpdateFlapperValves);
	} 
	else
	{
	}
	
	

	//return isOK;
	return true;
	
}
void States_Class::ControllerResume(uint8_t	operationMode){
	event e;
	bool done(false);
	
	
		if (operationMode==0)
		{
			if (timeElapsed)
			{
				usb<<"Air Conditioning System Operating"<<NEWLINE;
				timeElapsed=false;
				delay_ms(10);
				//alu.PrepareNewTask(kALUCheckCommunication);
				
			}
		}
		if (hvacState->currentState==kHVACStateLeaving)
		{
			prepareStateChangeEvent(hvacState->lastState);
			
		} 
		else
		{
			prepareStateChangeEvent(kHVACStateInitialize);
		}
		
		if (operationMode>0)
		{
			
			event	e;
			listener.eventHandler=&States_Class::CheckEvents;
			while ((cBit.IsOK())&&(!done))
			{
				if (listener.WaitForEvent(e, kHVACEventClass, kHVACSwitchStateEvent,8))
				{
					handleHVACEvent(e);
					if (hvacState->currentState==kHVACStateLeaving)
					{
							done=true;
							break;
					} 
						
				}
				else if (listener.WaitForEvent(e, kALUEventClass, kAnyEventType,4)){
					alu.PrepareNewEvent(kALUEventCheckPheripherals);
					done=true;
				}
			}
			
		}else{
			alu.PrepareNewEvent(kALUEventSimpleStart);
			return;
		}
		
}

void States_Class::Start(uint8_t	operationMode)
	{
	hvacState->currentState = kHVACStateStart;
	bool done(false);
	if (operationMode==0)
	{
		hvacTimer.Start_periodic_task(FUNC_PTR(HVACOperationTimeElapsed),100);
		usb<<"Air Conditioning System Started"<<NEWLINE;
		alu.PrepareNewTask(kALUTaskCheckCommunication);
	
		return;
	}
		
		

    tick_t	timerValue = 0L;
    bool	isCurrentStateValid = false;
	memory.ReadFastApplicationState(lastHVACState);
	
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
	//if (SetInitialState()<0){
	//	SetDefaultState();
	//}



	readResult=memory.ReadLastConfigurationData(configuration);
	if (readResult>0)
		{
		#if __DEBUG__ > 3
			usb << "readConfigurationDataArea failed!" << newline;
		#endif
		SetFactoryDefaults(0, true, false);
		}

	else
		{
		#if __DEBUG__ > 3
		
		usb << "configuration.minimumTargetVoltage == " << configuration.minimumTargetVoltage << newline;
		usb << "configuration.maximumTargetVoltage == " << configuration.maximumTargetVoltage << newline;
		usb << "configuration.m == " << configuration.m << newline;
		usb << "configuration.zA0 == " << configuration.zA0 << newline;
		
		#endif
		}


// 	if (!memory.readPersistentConfigurationDataArea(persistentConfiguration))
// 		{
// 		#if __DEBUG__ > 2
// 			usb << "readPersistentConfigurationDataArea failed!" << memory.getUptime() << newline;
// 		#endif
// 		setPersistentConfigurationDefaults();
// 		}
// #if __DEBUG__ > 2
// 	usb << "persistentConfiguration.cycle, id == " << persistentConfiguration.cycleDescriptionID
// 					<< ", description == " << persistentConfiguration.cycle.description
// 					<< ", power == " << persistentConfiguration.cycle.power
// 					<< ", duration == " << persistentConfiguration.cycle.duration << ", maximum time == " << persistentConfiguration.cycle.maximumDuration
// 					<< ", flags == " << persistentConfiguration.cycle.cycleFlags << newline;
// #endif
	

	//interfaces.CheckInternalCommunication();

	if (readResult==0)
    {
		prepareStateChangeEvent(kHVACStateInitialize);
		event	e;
		listener.eventHandler=&States_Class::CheckEvents;
		while ((cBit.IsOK())&&(!done))
		{
			if (listener.WaitForEvent(e, kHVACEventClass, kHVACSwitchStateEvent))
			{
		
				handleHVACEvent(e);
				if (hvacState->currentState==kHVACStateLeaving)
				{
						done=true;
						break;
				} 
				else
				{
					
					
				}
			
			}
			else{
				if (listener.WaitForEvent(e, kALUEventClass, kHVACEventDoPendingTasks)){
					alu.PrepareNewEvent(kALUEventSimpleStart);
					done=true;
					break;
				}

			}
		}
			
    }else{
		alu.PrepareNewEvent(kALUEventSimpleStart);
		return;
    }



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
		hvacState->callingState = hvacState->currentState;
	if (hvacState->currentState != kHVACStateResume){
		hvacState->currentState = e.data.wordData[0];
		hvacState->stateFlags = e.data.wordData[1];
		
	}else{
		hvacState->currentState=hvacState->lastState;
	}
	hvacState->stateEntryTime = hvacTimer.Get_ticks();

#if __DEBUG__ > 0
	
	usb << "handleGAINEvent received: switching from ";
	//usb.write(gainStateDescription[hvacState->callingState]);
	usb << " to ";
	//usb.write(gainStateDescription[hvacState->currentState]);
	//usb << " (flags == " << hvacState->stateFlags << ')' << NEWLINE;
	
#endif

	saveCurrentState();

	switch (hvacState->currentState)
		{
		case kHVACStateInitialize:
			StateInitialize(hvacState->stateFlags);
			break;

		case kHVACStateResume:
			StateResume(hvacState->stateFlags);
			break;

		case kHVACStateStandbyOFF:
			StateStandbyOFF(hvacState->stateFlags);
			break;

		case kHVACStatePrepareStandbyON:
			StatePrepareStandbyON(hvacState->stateFlags);
			break;

		case kHVACStateStandbyON:
			handleStateStandbyON(hvacState->stateFlags);
			break;
		case kHVACStateStandbyAUTO:
			StateStandbyAUTO(hvacState->stateFlags);
			break;
		case kHVACStateStandbyVENT:
			StateStandbyVENT(hvacState->stateFlags);
			break;

		case kHVACStateStandbyReady:
			handleStateStandbyReady(hvacState->stateFlags);
			break;
		case kHVACStatePrepareStandbyOFF:
			StatePrepareStandbyOFF(hvacState->stateFlags);
			break;

		case kHVACStateON:
			handleStateON(hvacState->stateFlags);
			break;

		case kHVACStateFinished:
			handleStateFinished(hvacState->stateFlags);
			break;

		case kHVACStateStoppedProcess:
			StateStoppedProcess(hvacState->stateFlags);
			break;

		case kHVACStateSelfProtect:
			handleStateSelfProtect(hvacState->stateFlags);
			break;

		case kHVACStateError:
			handleStateError(hvacState->stateFlags);
			break;

		case kHVACStateMaintenance:
			handleStateMaintenance(hvacState->stateFlags);
			break;
		case kHVACStateLeaving:
			StateLeaving(hvacState->stateFlags);
		break;

		default:
			handleStateError(kHVACErrorIllegalStateTransition);
			break;
		}
	}


void States_Class::StateInitialize(uint32 flags)
	{
	event	e;
	hvacState->currentState=kHVACStateInitialize;
   	uint16	selfTestResult(kHVACNoError);
	
#if __DEBUG__ > 0
	
	usb << "handleStateInitialize: flags == 0x" << flags << NEWLINE;
		
#endif
	
	
	//listener.CheckEvent();
	if (pBit.isOK)
	{
		#if __DEBUG__ > 0
		
		usb << "handleStateInitialize" << NEWLINE;
		
		#endif
		


		#ifdef _OFFLINE
			prepareStateChangeEvent( kHVACStateInterrupted);
			listener.SendEventSelf(e);
		#else
		selfTestResult = handlePowerOnSelftest();
		if (selfTestResult == kHVACNoError)
		{
			prepareStateChangeEvent( kHVACStateStandbyOFF);

		}
		#endif

		//listener.CheckEvent();
	} 
	else
	{
		Stop(0);
	}



	
	}


void States_Class::StateResume(uint32 flags)
	{
	event	e;
	uint8	currentState(hvacState->currentState);
    uint16	selfTestResult(kHVACNoError);

#if __DEBUG__ > 3
	usb << "handleStateResume" << newline;
#endif

#if __DEBUG__ > 3
	usb << "state.currentState (before) " << hvacState->currentState << newline;
#endif
	//memory.restoreWorkingStateForGAIN(hvacState);
#if __DEBUG__ > 3
	usb << "state.currentState (after) " << hvacState->currentState << newline;
#endif


    //Switch into current state if no error occured
    if(selfTestResult == kHVACNoError)
    {
        switch (hvacState->currentState)
        {
        case kHVACStateON:
            prepareStateChangeEvent( hvacState->currentState, true);
            break;

        case kHVACStateStoppedProcess:
            prepareStateChangeEvent( kHVACStateStoppedProcess);
            break;

        default:
            prepareStateChangeEvent( hvacState->currentState);
            break;
        }
    }

	hvacState->currentState = currentState;

	listener.CheckEvent();
	listener.SendEventSelf(e);
	}
				
void States_Class::StateStandbyOFF(uint32 flags)
{
	event	e;
	hvacState->currentState=kHVACStateStandbyOFF;
	hvacState->arincStatus.statusArinc=OFF;
	bool	done(false);

#ifdef __DEBUG__
	
	usb << "handleStateStandbyOFF" << NEWLINE;
		
#endif




	listener.eventHandler=&States_Class::CheckEvents;
	while(!done){
		if (InStateEvent(e,CHECK_EVENT_STATE_TIMEOUT))
		{
			switch(e.eventClass){
				case kALUEventClass:
					switch (e.eventType){
						case kHVACEventDoPendingTasks:
							prepareStateChangeEvent(kHVACStateLeaving);
							done=true;
						break;
						default:
						break;
					}
				break;
				case kHVACEventClass:
					switch (e.eventType){
							case kHVACEventDoPendingTasks:
								prepareStateChangeEvent(kHVACStateLeaving);
								done=true;
							break;
							default:
							break;
					}
		
				break;
				case kInterfaceEventClass:
					switch (e.eventType){
						case k_AC_CMD_ENABLE_Event:
							prepareStateChangeEvent(kHVACStatePrepareStandbyON);
							done=true;
						break;
		
						default:
						break;
					}
		
				break;
				default:
				break;
					
			}
		
		
		
		
			
		} 
		else
		{
			done=false;			
		}
	}



}
	
void States_Class::StateStandbyAUTO(uint32 flags)
{
	event	e;
	hvacState->currentState=kHVACStateStandbyAUTO;
	hvacState->arincStatus.statusArinc=AUTO;
	bool	done(false);
	gotAluEvent=false;
	gotHVACEvent=false;
	
	#ifdef __DEBUG__
	
	usb << "handleStateStandbyOFF" << NEWLINE;
	
	#endif




	listener.eventHandler=&States_Class::CheckEvents;
	while(!done){
		do
		{
				fv1.WriteSetpoint(GetFlapperValveSetpoint());
		}while(IsNewFlapperValveSetpoint());
	
		if (InStateEvent(e,CHECK_EVENT_STATE_TIMEOUT))
		{
			switch(e.eventClass){
				case kALUEventClass:
					switch (e.eventType){
						case kHVACEventDoPendingTasks:
							saveCurrentState();
							prepareStateChangeEvent(kHVACStateLeaving);
							done=true;
						break;
				
						default:
						break;
					}
				//done=gotAnyEvent;
				break;
				case kHVACEventClass:
					switch (e.eventType){
						case kHVACEventDoPendingTasks:
							saveCurrentState();
							prepareStateChangeEvent(kHVACStateLeaving);
							done=true;
						break;
						default:
						break;
					}
				
				break;
				case kInterfaceEventClass:
					switch (e.eventType){
			
						case k_AC_CMD_DISABLE_Event:
							prepareStateChangeEvent(kHVACStatePrepareStandbyOFF);
							done=true;
						break;
						default:
						break;
					}
				
				break;
				default:
				break;
				
			}
			
			
			
			
			
		}
		else
		{
			done=false;
		}
	}

	prepareStateChangeEvent(kHVACStateLeaving);

}

void States_Class::StateStandbyVENT(uint32 flags)
{
	event	e;
	hvacState->currentState=kHVACStateStandbyVENT;
	hvacState->arincStatus.statusArinc=VENT;
	bool	done(false);
	float cokspitTemperature;
	uint8_t	coolingRequired;

	
	#ifdef __DEBUG__
	
	usb << "handleStateStandbyOFF" << NEWLINE;
	
	#endif




	listener.eventHandler=&States_Class::CheckEvents;
	while(!done){
		cokspitTemperature=temperatures.values[0][1];
		coolingRequired=0x80+(uint8_t)(((uint32_t)((cokspitTemperature*10)-200))&0xff);
		fv1.WriteSetpoint(coolingRequired);
		
		
		if (InStateEvent(e,CHECK_EVENT_STATE_TIMEOUT))
		{
			switch(e.eventClass){
				case kALUEventClass:
				switch (e.eventType){
					case kHVACEventDoPendingTasks:
					saveCurrentState();
					prepareStateChangeEvent(kHVACStateLeaving);
					done=true;
					break;
					
					default:
					break;
				}
				//done=gotAnyEvent;
				break;
				case kHVACEventClass:
				switch (e.eventType){
					case kHVACEventDoPendingTasks:
					saveCurrentState();
					prepareStateChangeEvent(kHVACStateLeaving);
					done=true;
					break;
					default:
					break;
				}
				
				break;
				case kInterfaceEventClass:
				switch (e.eventType){
					
					case k_AC_CMD_DISABLE_Event:
					prepareStateChangeEvent(kHVACStatePrepareStandbyOFF);
					done=true;
					break;
					default:
					break;
				}
				
				break;
				default:
				break;
				
			}
			
			
			
			
			
		}
		else
		{
			done=false;
		}
	}

	prepareStateChangeEvent(kHVACStateLeaving);

}

void States_Class::StatePrepareStandbyON(uint32 flags)
	{
	hvacState->lastState=hvacState->currentState;
	hvacState->currentState=kHVACStatePrepareStandbyON;
	//uint32	connectFlags(hvacState->alarmState?kConnectFlagsAlarmOn:0);
	   	
#ifdef __DEBUG__
	
	//usb << "handleStatePrepareStandbyON" << NEWLINE;
		
#endif

	//tick_t	leaveDisplayTime = hvacTimer.Get_ticks() + 4000;
		switch (commando270.statusArinc)
		{
			case OFF:
				prepareStateChangeEvent(kHVACStatePrepareStandbyOFF);
			/* Your code here */
			break;
			case VENT:
				prepareStateChangeEvent(kHVACStateStandbyVENT);
			/* Your code here */
			break;
			case AUTO :
				prepareStateChangeEvent(kHVACStateStandbyAUTO);
			/* Your code here */
			break;
			default:
				prepareStateChangeEvent(kHVACStatePrepareStandbyOFF);
			/* Your code here */
			break;
		}
	


	}
	
void States_Class::StatePrepareStandbyOFF(uint32 flags)
{
	
	hvacState->currentState=kHVACStatePrepareStandbyOFF;
	//uint32	connectFlags(hvacState->alarmState?kConnectFlagsAlarmOn:0);
	
	#ifdef __DEBUG__
	
	//usb << "handleStatePrepareStandbyON" << NEWLINE;
	
	#endif

	//tick_t	leaveDisplayTime = hvacTimer.Get_ticks() + 4000;
	switch (hvacState->lastState)
	{
		case kHVACStateStandbyOFF:
		
		/* Your code here */
		break;
		case kHVACStateStandbyVENT:
			fv1.SetVentilatePosition();

		/* Your code here */
		break;
		case kHVACStateStandbyAUTO :
			fv1.SetOffsetPosition();

		/* Your code here */
		break;
		default:
	
		/* Your code here */
		break;
	}
	prepareStateChangeEvent(kHVACStateStandbyOFF);


}

void States_Class::handleStateStandbyON(uint32 flags)
	{
	event	e;
	bool	done(false);

#if __DEBUG__ > 1
	
	usb << "handleStateStandbyON" << newline;
		
#endif

	hvacState->latchClosedConfirmPending = false;
    hvacState->remainingTime = (tick_t)persistentConfiguration.cycle.duration * 1000L;

	
	hvacState->reedRelayOpen = false;
	
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
	tick_t	zeroTime(hvacTimer.Get_ticks());
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
void States_Class::StateLeaving(uint32 flags)
{
	event	e;
	hvacState->lastState=hvacState->callingState;
	hvacState->currentState=kHVACStateLeaving;
	
	saveCurrentState();
	//prepareStateChangeEvent(kHVACStateLeaving,(uint16)hvacState->lastState);
	//bool	done(false);
	//listener.SendEventSelf(e);
}

void States_Class::StateStoppedProcess(uint32 flags)
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
	
bool States_Class::InStateEvent(event& e, tick_t t, bool& done)
	{
	bool	result;
	done = false;
	result=listener.WaitForEvent(e,kAnyEventClass,kAnyEventType,t);
	
	return result;
	}
	
bool States_Class::InStateEvent(event& e, tick_t t){

	bool result=listener.WaitForEvent(e,kAnyEventClass,kAnyEventType,t);
	return result;

}

void States_Class::prepareStateChangeEvent(uint16 newState, uint16 data)
	{
	event e;
	e.eventClass =(EventClass)kHVACEventClass;
	e.eventType = (EventType)kHVACSwitchStateEvent;
	e.data.wordData[0] = newState;
	e.data.wordData[1] = data;
	listener.SendEventSelf(e);
	}

uint32_t States_Class::handlePowerOnSelftest(void)
	{
	event	e;
	uint16	selfTestResult(kHVACNoError);
	bool	moduleAvailable;
	int16	temperature;
		
	return selfTestResult;
	}


void States_Class::saveCurrentState(void){
		calendar_date_time	cdt;
		calendar.GetDateTime(&cdt);
		hvacState->dateTime=cdt;
		hvacState->now = calendar.convert_datetime_to_timestamp(&cdt);
	if (hvacState->currentState!=kHVACStateMaintenance){
		memory.WriteFastCurrentState(staticCurrentState);
		//memory.ReadFastApplicationState(lastState);
		if (hvacState->currentState==0)
		{
			memory.SaveApplicationState(staticCurrentState);
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
		if (layer==0)
		{
			usb<<"Air Conditioning System Stopped"<<NEWLINE;
			delay_ms(100);
			return layer;
		}
	return	layer;
}

void	States_Class::GetCurrentConfiguration(ConfigurationData *cd){
	cd=&configuration;
}
void	States_Class::GetCurrentState(HVACState& hs){
	hs=*hvacState;
}
void	States_Class::SetConfigurationData(ConfigurationData& cd){
	configuration=cd;
}
void	States_Class::SetCurrentState(HVACState& hs){
	staticCurrentState=hs;
}
void	States_Class::SetCRCConfigData(void){
	CRC32=memory.CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&factoryDefaultsConfiguration),sizeof(factoryDefaultsConfiguration));
}
uint32_t	States_Class::GetStatus(HVACStatus& s){
	s=hvacState->hvacStatus;
	return	hvacState->hvacStatus.rawStatus;
}
void States_Class::ExecutePeriodicTask(void){
	if (IsCommando270Changed())
	{
		if (commando270.AC_CMD)
		{
			
			prepareStateChangeEvent(kHVACStatePrepareStandbyON);
			listener.SendEventSelf(kInterfaceEventClass,k_AC_CMD_ENABLE_Event);
		} 
		else
		{
			prepareStateChangeEvent(kHVACStatePrepareStandbyOFF);
			listener.SendEventSelf(kInterfaceEventClass,k_AC_CMD_DISABLE_Event);
		}
		

	}
	if (fv1.IsPositionChanged())
	{
		alu.PrepareNewTask(kALUTaskUpdateFlapperValves);
		hvacState->arincStatus.statusFV=fv1.statusFlapperValve;
		
	}
	if (IsStatusArinclabelChanged())
	{
		arinc.SaveStatus(GetAcknowledgeStatus());
	}
		
}

void States_Class::CheckEvents(void){
	event e;
	hvac.ExecutePeriodicTask();
	if (interfaces.IsCommunicationRequest())
	{
		//e.eventClass=kALUEventClass;
	//	e.eventType=(EventType)kALUEventCheckCommunication;
		
	}
	if ((hvac.hvacState->currentState==kHVACStateStandbyOFF)&&(alu.taskList->head)){
		fv1.ReadActualPosition();
		alu.PrepareNewTask(kALUTaskUpdateFlapperValves);
		listener.SendEventSelf(kHVACEventClass,kHVACEventDoPendingTasks);

	}
	if (alu.arincTXTimeUP){
		if ((!alu.IsListedTask(kALUTaskArincTXMessage)))
		{
			alu.PrepareNewTask(kALUTaskArincTXMessage);
		} 
		else
		{
			
		}
		listener.SendEventSelf(kALUEventClass,kHVACEventDoPendingTasks);
	}
		

}


States_Class	hvac;