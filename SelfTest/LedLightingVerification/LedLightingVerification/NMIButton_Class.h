/* 
* NMIButton_Class.h
*
* Created: 7/15/2020 2:47:39 PM
* Author: GMateusDP
*/


#ifndef __NMIBUTTON_CLASS_H__
#define __NMIBUTTON_CLASS_H__
#define		COUNTER_TIME	4
#define		POWERON_SINGLE_TIME_MAX	4000
#include "driver_init.h"
class NMIButton_Class
{
//variables
public:
	volatile	bool pressed,pressedTimerStarted,modulate, incremented,direction,released;
	volatile	uint32_t pressCounter;
protected:
private:
		uint32_t	stepTime;

//functions
public:
	NMIButton_Class();
	~NMIButton_Class();
//	friend void NonMaskableInt_Handler(void);
	bool	IsButtonPressed(void);
	uint32_t	OnPressed(void);
	uint32_t	OnRelease(void);
	uint32_t	ModulateStepTime(void);
	
protected:
private:
	NMIButton_Class( const NMIButton_Class &c );
	NMIButton_Class& operator=( const NMIButton_Class &c );
	

}; //NMIButton_Class
extern	NMIButton_Class		nmi;
#endif //__NMIBUTTON_CLASS_H__
