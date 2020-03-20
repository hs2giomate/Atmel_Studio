/* 
* Timer_Class.h
*
* Created: 3/2/2020 11:38:08 AM
* Author: GMateusDP
*/


#ifndef __TIMER_CLASS_H__
#define __TIMER_CLASS_H__


class Timer_Class
{
//variables
public:
	bool	enabled;
protected:
private:
	int8	timerNumber;
	Tc* timerRegister;
//functions
public:
	Timer_Class();
	Timer_Class(Tc*);
	~Timer_Class();
	void  startTimer(time_t timeoutsec);
	void stopTimer(void);
	bool timerTimeout(void);
	friend	void TC7_Handler(void);
	friend	void TC6_Handler(void);

	
	void setUpTimer(void);
protected:
private:
	Timer_Class( const Timer_Class &c );
	Timer_Class& operator=( const Timer_Class &c );

}; //Timer_Class
void TC7_Handler(void);
void TC6_Handler(void);

#endif //__TIMER_CLASS_H__
