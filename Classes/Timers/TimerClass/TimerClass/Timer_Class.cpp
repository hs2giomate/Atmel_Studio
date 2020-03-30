/* 
* Timer_Class.cpp
*
* Created: 3/2/2020 11:38:07 AM
* Author: GMateusDP
*/


#include "Timer_Class.h"

Timer_Class*	ptrTimerClass;
static void task_cb(const struct timer_task *const timer_task)
{
	ptrTimerClass->setTimeout();
}
// default constructor
Timer_Class::Timer_Class()
{
	TIMER0=TIMER_0;
	prtTimer=this;
	ptrTimerClass=this;
	clockCycles=1024;
} //Timer_Class

// default destructor
Timer_Class::~Timer_Class()
{
	timeout=0;
} //~Timer_Class



void Timer_Class::Init(void){
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
	timer_init(&TIMER0, RTC, _rtc_get_timer());
	set_clock_cycles_per_tick(clockCycles);
}
int32_t Timer_Class::set_clock_cycles_per_tick(uint32_t clock_cycles){
	int32_t	status;
	status=timer_set_clock_cycles_per_tick(&TIMER0,clock_cycles);
	return status;
}

int32_t Timer_Class::start(uint32_t timeoutsec){
	int32_t	status;
	timeout=0;
	//set_clock_cycles_per_tick(timeoutsec/10);
	setOneShotTimer(timeoutsec);
	status=timer_start(&TIMER0);
	return status;
	
}

int32_t Timer_Class::stop( void){
	int32_t status;	
	status=timer_stop(&TIMER0);
	timeout=0;
	return status;
}



void	Timer_Class::setOneShotTimer(uint32_t interval){
	task.interval=interval;
	task.cb=task_cb;
	task.mode=TIMER_TASK_ONE_SHOT;
	timer_add_task(&TIMER0, &task);
}

void Timer_Class::setTimeout(){
	timeout=1;
}
void	Timer_Class::deinit(){
	timer_deinit(&TIMER0);
}