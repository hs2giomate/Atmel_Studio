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



class Timer_Class
{
//variables
public:
	bool	enabled;
	volatile bool	timeout;
	FUNC_PTR			handler;
	
protected:
private:
	timer_descriptor    *timer_descr;
	//Tc* timerRegister;
	Timer_Class	*    prtTimer;
	uint32_t	 clockCycles;
	timer_task			task;
	uint32_t			ticks;
//functions
public:
	Timer_Class();
	Timer_Class(timer_descriptor * descr);
	~Timer_Class();
	
	void Init();
	void Init(timer_descriptor * descr);
	void deinit();
	void set_descriptor(timer_descriptor * descr);
	int32_t start(uint32_t);
	int32_t start(void);
	int32_t stop();
	volatile void	setTimeout();
	int32_t set_clock_cycles_per_tick(uint32_t clock_cycles);
	void setOneShotTimer(uint32_t interval);
	int32_t	add_task(FUNC_PTR func,uint32_t interval);
	int32_t	add_periodic_task(FUNC_PTR func,uint32_t interval);
	int32_t	add_oneShot_task(FUNC_PTR func,uint32_t interval);
	int32_t	Start_oneShot_task(FUNC_PTR func,uint32_t interval);
	void remove_task();
	uint32_t	get_ticks(void);
	

protected:
	int32_t	add_task(FUNC_PTR func,uint32_t interval,timer_task_mode);
private:
	Timer_Class( const Timer_Class &c );
	Timer_Class& operator=( const Timer_Class &c );

}; //Timer_Class

extern	Timer_Class butttonTimer;
extern	Timer_Class	ledTimer;
extern	Timer_Class	pwmTimer;

#endif //__TIMER_CLASS_H__
