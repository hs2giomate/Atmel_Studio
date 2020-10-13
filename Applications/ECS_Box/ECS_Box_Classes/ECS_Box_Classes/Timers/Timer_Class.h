/* 
* Timer_Class.h
*
* Created: 3/2/2020 11:38:08 AM
* Author: GMateusDP
*/


#ifndef __TIMER_CLASS_H__
#define __TIMER_CLASS_H__
#include "driver_init.h"
//#include "utils.h"

#include <hal_timer.h>

#define TASK_NUMBER		8

class Timer_Class
{
//variables
public:
	bool	enabled;
	volatile bool	timeout,isOK;
	FUNC_PTR			handler;
	volatile uint32_t	ticks;

protected:
private:
	timer_descriptor    *timer_descr;
	//Tc* timerRegister;
	Timer_Class	*    prtTimer;
	uint32_t	 clockCycles;
	timer_task*			task;
	

//functions
public:
	Timer_Class();
	Timer_Class(timer_descriptor * descr);
	~Timer_Class();
	
	bool Init(void);
	void Init(timer_descriptor * descr);
	void deinit();
	void Set_descriptor(timer_descriptor * descr);
	int32_t start(uint32_t);
	int32_t start(void);
	int32_t stop();
	volatile void	setTimeout();
	int32_t set_clock_cycles_per_tick(uint32_t clock_cycles);
	void setOneShotTimer(uint32_t interval);
	int32_t	Add_task(FUNC_PTR func,uint32_t interval);
	int32_t	Add_periodic_task(FUNC_PTR func,uint32_t interval);
	int32_t	add_oneShot_task(FUNC_PTR func,uint32_t interval);
	int32_t Start_oneShot_task(FUNC_PTR func,uint32_t interval);
	int32_t Start_periodic_task(FUNC_PTR func,uint32_t interval);
	

	void	GetTaskFunction(FUNC_PTR func);
	void	ChooseAvailableTimerTask(void);
	void	Remove_task(void);
	void	Remove_task(FUNC_PTR func);
	uint32_t	Get_ticks(void);
	

protected:
	int32_t	Add_task(FUNC_PTR func,uint32_t interval,timer_task_mode);
private:
	Timer_Class( const Timer_Class &c );
	Timer_Class& operator=( const Timer_Class &c );

}; //Timer_Class

extern	Timer_Class eventTimer;
extern	Timer_Class hvacTimer;
extern	Timer_Class	connectionTimer;
extern	Timer_Class	arincTimer;
extern	Timer_Class	temperatureTimer;

#endif //__TIMER_CLASS_H__
