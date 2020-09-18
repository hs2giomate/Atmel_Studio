/* 
* Timer_Class.h
*
* Created: 3/2/2020 11:38:08 AM
* Author: GMateusDP
*/


#ifndef __TIMER_CLASS_H__
#define __TIMER_CLASS_H__
#define		TIMERSERIALCLASSVERSION			0x01
#define		TIMERSERIALCLASSSUBVERSION		0x00
#include "driver_init.h"
#include "utils.h"
#include <utils_list.h>
#include <hpl_timer.h>
#define TASK_NUMBER		4


class TimerSerial_Class
{
//variables
public:
	bool	enabled;
	volatile bool	timeout;
	FUNC_PTR			handler;
	timer_task*			task;
protected:
private:
	timer_descriptor    *timer_descr;
	//Tc* timerRegister;
	TimerSerial_Class	*    prtTimer;
	uint32_t	 clockCycles;
	
	uint32_t			ticks;
	uint8_t			i,j;
//functions
public:
	TimerSerial_Class();
	TimerSerial_Class(timer_descriptor * descr);
	~TimerSerial_Class();
	
	void Init();
	void Init(timer_descriptor * descr);
	void deinit();
	void set_descriptor(timer_descriptor * descr);
	int32_t Start(uint32_t);
	int32_t Start(void);
	int32_t Stop();
	volatile void	setTimeout();
	int32_t set_clock_cycles_per_tick(uint32_t clock_cycles);
	void SetOneShotTimer(uint32_t interval);
	int32_t	Add_task(FUNC_PTR func,uint32_t interval);
	int32_t	Add_periodic_task(FUNC_PTR func,uint32_t interval);
	int32_t	Add_oneShot_task(FUNC_PTR func,uint32_t interval);
	int32_t	Start_oneShot_task(FUNC_PTR func,uint32_t interval);
	void Remove_task();
	uint32_t	Get_ticks(void);
	void	GetTaskFunction(FUNC_PTR func);
	void	ChooseAvailableTimerTask(void);
	void	Remove_task(FUNC_PTR func);
	

protected:
	int32_t	Add_task(FUNC_PTR func,uint32_t interval,timer_task_mode);
private:
	TimerSerial_Class( const TimerSerial_Class &c );
	TimerSerial_Class& operator=( const TimerSerial_Class &c );

}; //Timer_Class

extern	TimerSerial_Class usbTimer;


#endif //__TIMER_CLASS_H__
