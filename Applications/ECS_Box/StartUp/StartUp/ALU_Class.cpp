/* 
* ALU_Class.cpp
*
* Created: 6/19/2020 4:21:20 PM
* Author: GMateusDP
*/


#include "ALU_Class.h"
#include "AMMC_Class.h"
#include "MemoryManagment_Class.h"
#include "CDC_Class.h"
#include "DateTime_Class.h"
#include "N25Q256_Class.h"
#include "CBIT_Class.h"
#include "EventHandler_class.h"
ALU_Class	*ptrALUClass;

//static TaskHandler_Class*	taskList(NULL);

static void CheckPeriodicTask(void);
static void ARINCTimeUp(const struct timer_task *const timer_task){
	ptrALUClass->arincTXTimeUP=true;
}


// default constructor
ALU_Class::ALU_Class()
{
	
	//taskList=(list_descriptor*)taskStorage;
	ptrALUClass=this;
	clk_rate       = 1000;
	timeout_period = 4096*1024;
} //ALU_Class

// default destructor
ALU_Class::~ALU_Class()
{
} //~ALU_Class

uint32_t	ALU_Class::Init(void){
	uint32_t	s;
	event	e;
	ptrPbit=&pBit;
	StartLivePulse();
		
	if (hvac.Init())
	{
		cBit.isOK=true;
		hvac.SetCRCConfigData();
		SetInitialConfiguration(configuration);
		memory.WriteDefaultState();
		hvac.SetInitialState();
		uhr.Init();
		hvac.saveCurrentState();
		listener.Init();
		
		if (!(interfaces.Init()))
		{
			NotifyError(kARINCINnterfaceError,s);
			return s;
		}
		else
		{
			arincTimer.Start_periodic_task(FUNC_PTR(ARINCTimeUp),500);
			
			s=pBit.CheckCurrentStatus(status);
			if (s>0)
			{
				NotifyError(kpBITError,s);
				return s;
			}
			else
			{
				InitTaskArray();
				EnableWatchDog();
				PrepareNewEvent(kALUEventSimpleStart);
				
			}
		}
		
		
	} 
	else
	{
	}
	//while(1);

	

	  
	return s;
}

uint32_t	ALU_Class::RunController(void){
	event e;
	
	
	   while (pBit)
	   {
		   listener.eventHandler=&ALU_Class::CheckPeriodicTask;
		   while (!arincTXTimeUP)
		   	{
				   if (listener.WaitForEvent(e, kALUEventClass, kALUControllerEvent,8))
				   {
					   HandleControllerEvent(e);
					   
				   }else if (listener.WaitForEvent(e, kHVACEventClass, kHVACEventDoPendingTasks,8))
				   {
					break;
				   }
			 }
			//arinc.TrasmitSingleLabel();
			ExecutePendingTask();
			arincTXTimeUP=false;
	   }
// 	while (1)
// 	{
// 		while (!arincTXTimeUP)
// 		{
// 			if (!pBit)
// 			{
// 				hvac.Stop(0);
// 			}
// 			else
// 			{
// 				hvac.Resume(0);
// 			}
// 			
// 		}
// 		interfaces.CheckCommunication();
// 	}
	
}



void ALU_Class::HandleControllerEvent(event& e)
{

	alu.callingTask = alu.currentTask;
	alu.currentTask = e.data.wordData[0];
	alu.taskEntryTime= hvacTimer.Get_ticks();




	switch (alu.currentTask)
	{
		case kALUEventSimpleStart:
			hvac.Start(1);
		break;

		case kALUEventSimpleResume:
			hvac.ControllerResume(1);
		break;

		case kALUEventSimpleStop:
			hvac.Stop(0);
		break;
		case kALUTaskCheckCommunication:
			interfaces.CheckCommunication();
		break;
		case kALUEventCheckPheripherals:
			interfaces.CheckCommunication();
		break;

		default:
			hvac.Stop(0);
		break;
	}
}

uint8_t	ALU_Class::GetSelectedAMMC(void){
	activeLine=arinc.GetActiveLine();
		switch (activeLine)
		{
			case 1:
			ammc=&ammc1;
			/* Your code here */
			break;
			
			case 2:
			ammc=&ammc2;
			/* Your code here */
			break;
			default:
			ammc=&ammc1;
			/* Your code here */
			break;
		}
	return activeLine;	
}

uint8_t	ALU_Class::Run(void){
	uint8_t line= GetSelectedAMMC();
	#ifdef __DEBUG__
		usb<<"Selected line:"<<line<<NEWLINE;
	#endif
	if (ptrPbit->GetStatus()>0)
	{
			#ifdef __DEBUG__
			usb<<"Selected line:"<<line<<NEWLINE;
			#endif
	} 
	else
	{
	}
	return line;
}
int32_t	ALU_Class::EnableWatchDog(void){

	wdt_set_timeout_period(&WATCHDOG, clk_rate, timeout_period);
	return wdt_enable(&WATCHDOG);

	
}
int32_t	ALU_Class::FeedWatchDog(void){
	return wdt_feed(&WATCHDOG);
}
uint8_t	ALU_Class::StartLivePulse(void){
	usb<<"**** Life Pulse Activated****"<<NEWLINE;
	//pwm_set_parameters(&LIVE_PULSE, 1000000, 500);
	uint32_t p=pwm_enable(&LIVE_PULSE);
	return p;
}
bool	ALU_Class::ValidateCyclus(void){
	tick_t	nextUptimeUpdate;
	
	if (hvacTimer.Get_ticks() > nextUptimeUpdate)
	{
		persistent.updateUptime(false);
		nextUptimeUpdate = hvacTimer.Get_ticks()+ 1000;
	}
	
	return (alu.FeedWatchDog()>0);
}
uint32_t ALU_Class::SetInitialConfiguration(ConfigurationData& cd){
	uint32_t	w,crc,refCRC,e;
	w=0;
	uint32_t	r=memory.ReadLastConfigurationData(cd);
	if (memory.IsConfigurationDataUpdated())
	{
		r=memory.ReadUpdatedConfigurationData(configuration);
		
	}
	else
	{
		crc=memory.CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&cd),sizeof(configuration));
		refCRC=memory.ReadCRCConfigurationData();
		if (crc==refCRC)
		{
			configuration=cd;
		
		} 
		else
		{
		
			r=memory.ReadDefaultConfigurationData(configuration);
			crc=memory.CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&configuration),sizeof(configuration));
			if (crc==hvac.CRC32)
			{
				w=memory.WriteCRCConfigurationData(crc);
				
			}
			else
			{
							
				configuration=factoryDefaultsConfiguration;
				e=qspiFlash.Erase((uint32_t)&flashMap->configurationSector);
				w=memory.WriteDefaultConfiguration();
				
				w=memory.WriteCRCConfigurationData(hvac.CRC32);
				
				e=qspiFlash.Erase((uint32_t)&flashMap->parametersSector);
				w=memory.WriteDefaultParameters();
				memory.WriteFastDefaultParameters();
				
			}
			
		}
		
		
	}
	hvac.SetConfigurationData(configuration);
	return r;
};

void ALU_Class::NotifyError(Internal_Fault_List ifl,const eventData& data)
{
	listener.SendErrorSelf(uint16_t(ifl),data);
}
void ALU_Class::NotifyError(Fault_List fl,const eventData& data)
{
	listener.SendErrorSelf(uint16_t(fl),data);
}

void ALU_Class::PrepareNewTask(ALUTaskEnum newTask, uint32_t data)
{
	
	task.id=newTask;
	task.prio=(uint8_t)newTask;
	AddTask(task);

}

void ALU_Class::PrepareNewEvent( uint16 newState, uint16 data)
{
	event	e;
	e.eventClass =(EventClass)kALUEventClass;
	e.eventType = (EventType)kALUControllerEvent;
	e.data.wordData[0] = newState;
	e.data.wordData[1] = data;
	listener.SendEventSelf(e);

}

bool	ALU_Class::ExecutePendingTask(void){
	allTasksDone=false;
	list_element *it;
	ControllerTask tk;
	uint8_t		highPrio=kALUNumberTasks;
	tk=GetHighPrioTask();
	if (tk.id!=kALUTaskArincTXMessage)
	{
		PrepareNewTask(kALUTaskArincTXMessage);
	}
	while (taskList->head){
		tk=GetHighPrioTask();
		HandleTasks(tk);
		RemoveTask(tk);
	
	}
	RunPeriodicTasks();
	allTasksDone=~((bool)taskList->head);
	if (allTasksDone)
	{
		
		PrepareNewEvent(kALUEventSimpleResume);
	}
	return allTasksDone; 
}
void ALU_Class::HandleTasks(ControllerTask& ct)
{

	taskEntryTime= hvacTimer.Get_ticks();
	switch (ct.id)
	{
		case kALUEventSimpleStart:
			hvac.Start(0);
		break;
		case kALUSimpleResume:
			hvac.ControllerResume(0);
		break;
		case kALUSimpleStop:
			hvac.Stop(0);
		break;
		case kALUTaskCheckCommunication:
			interfaces.CheckCommunication();
		break;
		case kALUTaskReadARINCR1:
			arinc.ReadRXBuffer(1);			
		break;
		case kALUTaskArincTXMessage:
			arinc.TransmitTXBuffer();
		break;
		case kALUTaskUpdateTemperatures:
			arinc.SaveTemperature();		
		break;
		case kALUTaskUpdateFlapperValves:
			arinc.SaveFlapperValveAngle();
		break;

		default:
		hvac.Stop(0);
		break;
	}
}

void ALU_Class::CheckPeriodicTask(void){
	interfaces.CheckCommunication();
}

void	ALU_Class::RunPeriodicTasks(void){
	FeedWatchDog();

}




ALU_Class	alu;