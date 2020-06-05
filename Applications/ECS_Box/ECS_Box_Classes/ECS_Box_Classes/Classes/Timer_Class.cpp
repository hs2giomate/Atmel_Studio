/* 
* Timer_Class.cpp
*
* Created: 3/2/2020 11:38:07 AM
* Author: GMateusDP
*/


#include "Timer_Class.h"

Timer_Class*	ptrTimerClass;
static void extern_task_cb(const struct timer_task *const timer_task)
{
	ptrTimerClass->handler();
}
static void task_cb(const struct timer_task *const timer_task)
{
	ptrTimerClass->setTimeout();
}
// default constructor
Timer_Class::Timer_Class()
{
	timer_descr=&TIMER_0;
	prtTimer=this;
	ptrTimerClass=this;
	clockCycles=1024;
	task.cb=task_cb;
} //Timer_Class

// default constructor
Timer_Class::Timer_Class( timer_descriptor *const descr)
{
	timer_descr=descr;
	ptrTimerClass=this;
	clockCycles=1024;
	task.cb=task_cb;
} //Timer_Class

// default destructor
Timer_Class::~Timer_Class()
{
	timeout=0;
} //~Timer_Class



void Timer_Class::Init(void){
	ticks=0;
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
	if (task.cb==task_cb)
	{
		setOneShotTimer(timeoutsec);
	} 
	else
	{
		tempTask=task;
		remove_task();
		task=tempTask;
		task.interval=timeoutsec;
		timer_add_task(timer_descr, &task);
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
	task.interval=interval;
	task.cb=task_cb;
	task.mode=TIMER_TASK_ONE_SHOT;
	timer_add_task(timer_descr, &task);
}

volatile void Timer_Class::setTimeout(){
	timeout=1;
}
void	Timer_Class::deinit(){
	timer_deinit(timer_descr);
}

int32_t Timer_Class::add_task(FUNC_PTR func,uint32_t interval){
	int32_t	status;

	handler=func;
	task.interval = interval;
	task.cb       = extern_task_cb;
	task.mode     = TIMER_TASK_REPEAT;
	status=timer_add_task(timer_descr, &task);
	
	return status;
}

void	Timer_Class::remove_task(void){
	timer_remove_task(timer_descr, &task);
}

void	Timer_Class::set_descriptor(timer_descriptor *const descr){
	timer_descr=descr;
}

uint32_t	Timer_Class::get_ticks(void){
	
	timer_get_clock_cycles_in_tick(timer_descr,&ticks);
	return ticks;
}

