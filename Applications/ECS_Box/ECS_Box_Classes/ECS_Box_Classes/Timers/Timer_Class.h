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

#define TASK_NUMBER		32

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
	uint8_t		lastTaskIndex;
	

//functions
public:
	Timer_Class();
	Timer_Class(timer_descriptor * descr);
	~Timer_Class();
	
	bool Init(void);
	void Init(timer_descriptor * descr);
	void deinit();
	void Set_descriptor(timer_descriptor * descr);
	int32_t Start(uint32_t);
	int32_t Start(void);
	int32_t Stop();
	volatile void	setTimeout();
	int32_t set_clock_cycles_per_tick(uint32_t clock_cycles);
	void setOneShotTimer(uint32_t interval);
	int32_t	Add_task(FUNC_PTR func,uint32_t interval);
	int32_t	Add_periodic_task(FUNC_PTR func,uint32_t interval);
	int32_t	add_oneShot_task(FUNC_PTR func,uint32_t interval);
	int32_t Start_oneShot_task(FUNC_PTR func,uint32_t interval);
	int32_t Start_periodic_task(FUNC_PTR func,uint32_t interval);
	int32_t StartCountingTicks(uint32_t interval);
	

	
	void	Remove_task(void);
	void	Remove_task(FUNC_PTR func);
	uint32_t	Get_ticks(void);
	

protected:
	int32_t	Add_task(FUNC_PTR func,uint32_t interval,timer_task_mode);
private:
	Timer_Class( const Timer_Class &c );
	Timer_Class& operator=( const Timer_Class &c );
	void	ChooseAvailableTimerTask(FUNC_PTR func);
	void	GetTaskFunction(FUNC_PTR func);
	void	ChooseAvailableTimerTask(void);

}; //Timer_Class
// 
// extern	Timer_Class eventTimer;
static Timer_Class temperatureTimer(&TIMER_TEMPERATURES);
static Timer_Class eventTimer(&TIMER_EVENT);
static Timer_Class hvacTimer(&TIMER_HVAC);
static Timer_Class connectionTimer(&TIMER_INTERFACE);
static Timer_Class arincTimer(&TIMER_ARINC);
static Timer_Class usbTerminalTimer(&TIMER_USB);
static Timer_Class interfaceTimer(&TIMER_MAINTENANCE);
static Timer_Class canoTimer(&TIMER_CCU);

//static	Timer_Class hvacTimer;
// extern	Timer_Class	connectionTimer;
// extern	Timer_Class	arincTimer;
// extern	Timer_Class	temperatureTimer;
// extern	Timer_Class usbTerminalTimer;
// extern	Timer_Class interfaceTimer;

#endif //__TIMER_CLASS_H__
