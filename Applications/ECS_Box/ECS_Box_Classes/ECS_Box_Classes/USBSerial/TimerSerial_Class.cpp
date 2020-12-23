/* 
* Timer_Class.cpp
*
* Created: 3/2/2020 11:38:07 AM
* Author: GMateusDP
*/


#include "TimerSerial_Class.h"

TimerSerial_Class*	ptrTimerSerialClass;
static  timer_task taskSerialArray[SERIAL_TASK_NUMBER];



static void extern_task_cb(const struct timer_task *const timer_task)
{
	ptrTimerSerialClass->handler();
}
static void task_cb(const struct timer_task *const timer_task)
{
	ptrTimerSerialClass->setTimeout();
}
// default constructor
TimerSerial_Class::TimerSerial_Class()
{
	prtTimer=this;
	ptrTimerSerialClass=this;
	clockCycles=1024;
	task->cb=task_cb;
} //Timer_Class

// default constructor
TimerSerial_Class::TimerSerial_Class( timer_descriptor * descr)
{
	timer_descr=descr;
	ptrTimerSerialClass=this;
	clockCycles=1024;
	task->cb=task_cb;
} //Timer_Class
void	TimerSerial_Class::set_descriptor(timer_descriptor * descr){
		timer_descr=descr;
		ptrTimerSerialClass=this;
}

// default destructor
TimerSerial_Class::~TimerSerial_Class()
{
	timeout=0;
} //~Timer_Class



void TimerSerial_Class::Init(timer_descriptor * descr){
	set_descriptor(descr);
	ticks=0;
}
bool TimerSerial_Class::Init(void){
	
	ticks=0;
	isOK=Start()==0;
	return isOK;
}
int32_t TimerSerial_Class::set_clock_cycles_per_tick(uint32_t clock_cycles){
	int32_t	status;
	status=timer_set_clock_cycles_per_tick(timer_descr,clock_cycles);
	return status;
}

int32_t TimerSerial_Class::Start(uint32_t timeoutsec){
	int32_t	status;
	timer_task tempTask;
	timeout=0;
	//set_clock_cycles_per_tick(timeoutsec/10);
	if (task->cb==task_cb)
	{
		SetOneShotTimer(timeoutsec);
	} 
	else
	{
		task->interval=timeoutsec;
		timer_add_task(timer_descr, task);
	}
	
	status=timer_start(timer_descr);
	return status;
	
}
int32_t TimerSerial_Class::Start(void){
	int32_t	status;
		
	status=timer_start(timer_descr);
	return status;
	
}

int32_t TimerSerial_Class::Stop( void){
	int32_t status;	
	status=timer_stop(timer_descr);
	timeout=0;
	return status;
}
int32_t TimerSerial_Class::Start_periodic_task(FUNC_PTR func,uint32_t interval){
	Add_periodic_task(func,interval);
	return timer_start(timer_descr);
}



void	TimerSerial_Class::SetOneShotTimer(uint32_t interval){
	ChooseAvailableTimerTask();
	task->interval=interval;
	task->cb=task_cb;
	task->mode=TIMER_TASK_ONE_SHOT;
	timer_add_task(timer_descr, task);
}

volatile void TimerSerial_Class::setTimeout(){
	timeout=1;
}
void	TimerSerial_Class::deinit(){
	timer_deinit(timer_descr);
}

int32_t TimerSerial_Class::Add_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;

	ChooseAvailableTimerTask();
	task->interval = interval;
	task->cb       = (timer_cb_t)func;
	task->mode     = TIMER_TASK_REPEAT;
	status=timer_add_task(timer_descr, task);
	
	return status;
}
int32_t TimerSerial_Class::Add_task(FUNC_PTR func,uint32_t interval,timer_task_mode modeTask){
	int32_t	status;
	ChooseAvailableTimerTask();
	task->interval = interval;
	task->cb       = (timer_cb_t)func;
	task->mode     = modeTask;
	status=timer_add_task(timer_descr, task);
	
	return status;
}
int32_t TimerSerial_Class::Add_periodic_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;
	status=Add_task(func,interval,TIMER_TASK_REPEAT);
		return status;
}
int32_t TimerSerial_Class::Add_oneShot_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;
	status=Add_task(func,interval,TIMER_TASK_ONE_SHOT);
	return status;
}
int32_t TimerSerial_Class::Start_oneShot_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;
	status=Add_task(func,interval,TIMER_TASK_ONE_SHOT);
	status=timer_start(timer_descr);
	return status;
}

void	TimerSerial_Class::Remove_task(void){
	
	timer_remove_task(timer_descr, task);
	task->cb=NULL;
}

void	TimerSerial_Class::Remove_task(FUNC_PTR func){
	GetTaskFunction(func);
	
	if (is_list_element(&timer_descr->tasks, task))
	{
		timer_remove_task(timer_descr, task);
	} 
	else
	{
		
	}

	task->cb=NULL;
}


uint32_t	TimerSerial_Class::Get_ticks(void){
	
	return ticks;
}

void	TimerSerial_Class::ChooseAvailableTimerTask(void){
	for ( uint8_t ii = 0; ii < SERIAL_TASK_NUMBER; ii++)
	{
			if (taskSerialArray[ii].cb==NULL)
			{
				
				task=&taskSerialArray[ii];
			return;
				
			}
	}
	task=&taskSerialArray[0];

}
void	TimerSerial_Class::GetTaskFunction(FUNC_PTR func){
	for (uint8_t ii = 0; ii < SERIAL_TASK_NUMBER; ii++)
	{
		if (taskSerialArray[ii].cb==(timer_cb_t)func)
		{
			
			task=&taskSerialArray[ii];
			return;
			
		}
	}


}


//TimerSerial_Class usbTerminalTimer(&TIMER_USB);
//TimerSerial_Class interfaceTimer(&TIMER_MAINTENANCE);

