/* 
* Timer_Class.cpp
*
* Created: 3/2/2020 11:38:07 AM
* Author: GMateusDP
*/


#include "Timer_Class.h"

Timer_Class*	ptrTimerClass;
static  timer_task taskArray[TASK_NUMBER];
static  timer_task countTicks;
static void extern_task_cb(const struct timer_task *const timer_task)
{
	ptrTimerClass->handler();
}
static void CountTicks(const struct timer_task *const timer_task)
{
	ptrTimerClass->ticks++;
}
static void task_cb(const struct timer_task *const timer_task)
{
	ptrTimerClass->setTimeout();
}
// default constructor
Timer_Class::Timer_Class()
{
	prtTimer=this;
	ptrTimerClass=this;
	clockCycles=1024;
	task->cb=task_cb;
} //Timer_Class

// default constructor
Timer_Class::Timer_Class( timer_descriptor * descr)
{
	timer_descr=descr;
	ptrTimerClass=this;
	clockCycles=1024;
	task=&countTicks;
	//task->cb=task_cb;
} //Timer_Class
void	Timer_Class::set_descriptor(timer_descriptor * descr){
		timer_descr=descr;
		ptrTimerClass=this;
}

// default destructor
Timer_Class::~Timer_Class()
{
	timeout=0;
} //~Timer_Class



void Timer_Class::Init(timer_descriptor * descr){
	set_descriptor(descr);
	ticks=0;
}
void Timer_Class::Init(void){
	ticks=0;
	add_periodic_task(FUNC_PTR(CountTicks),1);
	start();
}
int32_t Timer_Class::set_clock_cycles_per_tick(uint32_t clock_cycles){
	int32_t	status;
	status=timer_set_clock_cycles_per_tick(timer_descr,clock_cycles);
	return status;
}

int32_t Timer_Class::start(uint32_t timeoutsec){
	int32_t	status;
	timer_task tempTask;
	timeout=0;
	//set_clock_cycles_per_tick(timeoutsec/10);
	if (task->cb==task_cb)
	{
		setOneShotTimer(timeoutsec);
	} 
	else
	{

		task->interval=timeoutsec;
		timer_add_task(timer_descr, task);
	}
	
	status=timer_start(timer_descr);
	return status;
	
}
int32_t Timer_Class::start(void){
	int32_t	status;
		
	status=timer_start(timer_descr);
	return status;
	
}

int32_t Timer_Class::stop( void){
	int32_t status;	
	status=timer_stop(timer_descr);
	timeout=0;
	return status;
}




void	Timer_Class::setOneShotTimer(uint32_t interval){
	ChooseAvailableTimerTask();
	task->interval=interval;
	task->cb=task_cb;
	task->mode=TIMER_TASK_ONE_SHOT;
	timer_add_task(timer_descr, task);
}

volatile void Timer_Class::setTimeout(){
	timeout=1;
}
void	Timer_Class::deinit(){
	timer_deinit(timer_descr);
}

int32_t Timer_Class::Add_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;

	ChooseAvailableTimerTask();
	task->interval = interval;
	task->cb       = (timer_cb_t)func;
	task->mode     = TIMER_TASK_REPEAT;
	status=timer_add_task(timer_descr, task);
	
	return status;
}
int32_t Timer_Class::Add_task(FUNC_PTR func,uint32_t interval,timer_task_mode modeTask){
	int32_t	status;
	ChooseAvailableTimerTask();
	task->interval = interval;
	task->cb       = (timer_cb_t)func;
	task->mode     = modeTask;
	status=timer_add_task(timer_descr, task);
	
	return status;
}
int32_t Timer_Class::Start_periodic_task(FUNC_PTR func,uint32_t interval){
	 add_periodic_task(func,interval);
	 return timer_start(timer_descr);
}
int32_t Timer_Class::add_periodic_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;
	status=Add_task(func,interval,TIMER_TASK_REPEAT);
		return status;
}
int32_t Timer_Class::add_oneShot_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;
	status=Add_task(func,interval,TIMER_TASK_ONE_SHOT);
	status=timer_start(timer_descr);
	return status;
}

void	Timer_Class::Remove_task(void){
	
	timer_remove_task(timer_descr, task);
	task->cb=NULL;
}

void	Timer_Class::Remove_task(FUNC_PTR func){
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



uint32_t	Timer_Class::Get_ticks(void){
	
		return ticks;
}
int32_t Timer_Class::Start_oneShot_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;
	status=Add_task(func,interval,TIMER_TASK_ONE_SHOT);
	status=timer_start(timer_descr);
	return status;
}
void	Timer_Class::ChooseAvailableTimerTask(void){
	for (i = 0; i < TASK_NUMBER; i++)
	{
		if (taskArray[i].cb==NULL)
		{
			
			task=&taskArray[i];
			return;
			
		}
	}
	task=&taskArray[0];

}
void	Timer_Class::GetTaskFunction(FUNC_PTR func){
	for (i = 0; i < TASK_NUMBER; i++)
	{
		if (taskArray[i].cb==(timer_cb_t)func)
		{
			
			task=&taskArray[i];
			return;
			
		}
	}


}

Timer_Class eventTimer(&TIMER_EVENT);
Timer_Class hvacTimer(&TIMER_HVAC);
Timer_Class connectionTimer(&TIMER_INTERFACE);
Timer_Class arincTimer(&TIMER_ARINC);