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
#include <utils_list.h>
#include <hpl_timer.h>
#include "Stream.h"


class Timer_Class
{
//variables
public:
	bool	enabled;
	volatile bool	timeout;
	timer_descriptor    *timer_descr;
	timer_task			task;
	FUNC_PTR			handler;
	uint32_t			ticks;
protected:
private:
	
	//Tc* timerRegister;
	Timer_Class	*    prtTimer;
	uint32_t	 clockCycles;
//functions
public:
	Timer_Class();
	Timer_Class(timer_descriptor *const descr);
	~Timer_Class();
	void  startTimer(uint32_t timeoutsec);
	
	void Init();
	void deinit();
	void set_descriptor(timer_descriptor *const descr);
	int32_t start(uint32_t);
	int32_t start(void);
	int32_t stop();
	volatile void	setTimeout();
	int32_t set_clock_cycles_per_tick(uint32_t clock_cycles);
	void setOneShotTimer(uint32_t interval);
	int32_t	add_task(FUNC_PTR func,uint32_t interval);
	
	void remove_task();
	uint32_t	get_ticks(void);


	

protected:
private:
	Timer_Class( const Timer_Class &c );
	Timer_Class& operator=( const Timer_Class &c );

}; //Timer_Class


#endif //__TIMER_CLASS_H__
