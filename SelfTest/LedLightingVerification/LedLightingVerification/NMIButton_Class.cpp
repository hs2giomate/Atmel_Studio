/* 
* NMIButton_Class.cpp
*
* Created: 7/15/2020 2:47:38 PM
* Author: GMateusDP
*/


#include "NMIButton_Class.h"
#include "LedHandler_Class.h"
#include "Timer_Class.h"

static void IncrementDutyCycle(const struct timer_task *const timer_task)
{
	if (nmi.pressed)
	{
		if (!nmi.direction)
		{
				if (lighter.stepTime<POWERON_SINGLE_TIME_MAX)
				{
					lighter.stepTime+=10;
				}
				else
				{
					lighter.stepTime-=10;
					nmi.direction=true;
				}
		} 
		else
		{
				if (lighter.stepTime<10)
				{
					lighter.stepTime+=10;
					nmi.direction=false;
				}
				else
				{
					lighter.stepTime-=10;
					
				}
		}
	} 
	else
	{
		nmi.modulate=false;
	}
	
}

static void ChangeModeTimer(const struct timer_task *const timer_task)
{
	if (nmi.pressed)
	{
		nmi.pressCounter++;
		//lighter.changeModeRequest=false;
		//nmi.modulate=true;
		if (!nmi.direction)
		{
			if (lighter.stepTime<POWERON_SINGLE_TIME_MAX)
			{
				lighter.stepTime+=10;
			}
			else
			{
				lighter.stepTime-=10;
				nmi.direction=true;
			}
		}
		else
		{
			if (lighter.stepTime<10)
			{
				lighter.stepTime+=10;
				nmi.direction=false;
			}
			else
			{
				lighter.stepTime-=10;
				
			}
		}
	}
	else
	{
		nmi.released=true;
		if (nmi.pressCounter<100)
		{
			lighter.changeModeRequest=true;
			nmi.modulate=false;
		} 
		else
		{
			lighter.changeModeRequest=false;
			nmi.modulate=true;
		}
		//lighter.changeModeRequest=true;
	}
	
}

// default constructor
NMIButton_Class::NMIButton_Class()
{
	pressedTimerStarted=false;
	pressCounter=0;
} //NMIButton_Class

// default destructor
NMIButton_Class::~NMIButton_Class()
{
} //~NMIButton_Class

bool	NMIButton_Class::IsButtonPressed(void){
	if (!lighter.poweron)
	{
		gpio_set_pin_direction(NMI,GPIO_DIRECTION_IN);
		gpio_set_pin_pull_mode(NMI,GPIO_PULL_UP);
		pressed=gpio_get_pin_level(NMI)==false;
		
		
		gpio_set_pin_pull_mode(NMI,GPIO_PULL_DOWN);
		gpio_set_pin_direction(NMI, GPIO_DIRECTION_OUT);
		gpio_set_pin_function(NMI, GPIO_PIN_FUNCTION_OFF);
		gpio_set_pin_level(NMI,false);
		return	pressed;
	} 
	else
	{
		return false;
	}
	return false;
}
uint32_t	NMIButton_Class::OnPressed(void){
// 	if (!pressedTimerStarted)
// 	{
// 		butttonTimer.stop();
// 		pressCounter=0;
// 		direction=false;
// 		butttonTimer.add_periodic_task((FUNC_PTR)ChangeModeTimer,POWERON_SINGLE_TIME);
// 		butttonTimer.start();
// 		stepTime=lighter.stepTime;
// 		nmi.released=false;
// 		pressedTimerStarted=true;
// 	}else{
// 		if (IsButtonPressed()){
// 			ModulateStepTime();
// 		}
// 		else
// 		{
// 			if (pressCounter<COUNTER_TIME)
// 			{
// 				lighter.changeModeRequest=true;
// 			
// 			} 
// 			else
// 			{
// 				lighter.changeModeRequest=false;
// 				
// 			}
// 			OnRelease();
// 			
// 		}
// 	}
		
	lighter.changeModeRequest=true;
	return	stepTime;
}
uint32_t	NMIButton_Class::ModulateStepTime(void){
// 	pressCounter++;
// 	
// 	if (!direction)
// 	{
// 		if (lighter.stepTime<POWERON_SINGLE_TIME_MAX)
// 		{
// 			lighter.stepTime+=COUNTER_TIME;
// 		}
// 		else
// 		{
// 			lighter.stepTime-=COUNTER_TIME;
// 			direction=true;
// 		}
// 	}
// 	else
// 	{
// 		if (lighter.stepTime<COUNTER_TIME)
// 		{
// 			lighter.stepTime+=COUNTER_TIME;
// 			direction=false;
// 		}
// 		else
// 		{
// 			lighter.stepTime-=COUNTER_TIME;
// 			
// 		}
// 	}
	return	lighter.stepTime;
}
	
uint32_t NMIButton_Class::OnRelease(void){
	if (IsButtonPressed()){
			if ((modulate))
			{
				//butttonTimer.stop();
				//butttonTimer.Start_oneShot_task((FUNC_PTR)IncrementDutyCycle,COUNTER_TIME);
				
			}
	}else{
// 				butttonTimer.stop();
// 				butttonTimer.remove_task();
				stepTime=lighter.stepTime;
			/*	pressedTimerStarted=false;*/
				
	}
	return	stepTime;
}

NMIButton_Class		nmi;


