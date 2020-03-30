/* 
* Timer_Class.h
*
* Created: 3/2/2020 11:38:08 AM
* Author: GMateusDP
*/


#ifndef __TIMER_CLASS_H__
#define __TIMER_CLASS_H__
#include "driver_init.h"
#include "utils.h"
#include "hal_timer.h"
#include <hpl_rtc_base.h>


class Timer_Class
{
//variables
public:
	bool	enabled;
	bool	timeout;
	timer_descriptor    TIMER0;
	timer_task			task;
	
protected:
private:
	
	//Tc* timerRegister;
	Timer_Class	*    prtTimer;
	uint32_t	 clockCycles;
//functions
public:
	Timer_Class();
	
	~Timer_Class();
	void  startTimer(uint32_t timeoutsec);
	
	void Init();
	void deinit();
	int32_t start(uint32_t);
	int32_t stop();
	void	setTimeout();
	int32_t set_clock_cycles_per_tick(uint32_t clock_cycles);
	void setOneShotTimer(uint32_t interval);


	

protected:
private:
	Timer_Class( const Timer_Class &c );
	Timer_Class& operator=( const Timer_Class &c );

}; //Timer_Class


#endif //__TIMER_CLASS_H__
