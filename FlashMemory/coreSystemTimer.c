/*--------------------------------------------------------------------------

coreSystemTimer.c

This file is part of e.Lib

Implementation
low level interface to the SAMD21 timer

$Author: steffen $
$Date: 2016-11-07 09:07:35 +0100 (Mo, 07 Nov 2016) $
$Revision: 1080 $
$HeadURL: https://svn.s2embedded.at/customers/hs2-engineering/trunk/HiLo/Firmware/Libraries/Cortex/e.Core/coreSystemTimer.c $

--------------------------------------------------------------------------*/
#include "sam.h"
//#include "coreInterrupts.h"
#include "coreController.h"
#include "coreSystemTimer.h"


#define TIMER_GCLK		8000000UL
#ifndef TIMER_GCLK
#define TIMER_GCLK		F_CPU
#endif

#if (TIMER_GCLK == 1000000UL)
	#define TIMER_PRESCALER_VALUE	0x02	//	DIV 8
	#define TIMER_RELOAD_VALUE		125
#endif
#if (TIMER_GCLK == 8000000UL)
	#define TIMER_PRESCALER_VALUE	0x05	//	DIV 64
	#define TIMER_RELOAD_VALUE		125
#endif
#if (TIMER_GCLK == 48000000UL)
	#define TIMER_PRESCALER_VALUE	0x05	//	DIV 64
	#define TIMER_RELOAD_VALUE		750
#endif

const time_t	forever = 0x7fffffff;

volatile time_t	coreSystemTimerTickCount = 0;
static volatile	uint16	callbackCounter = 0;
static volatile	uint16	callbackCounterIntervall = 100;
static volatile bool	callCoreSystemTimerWaitCallbackHandler = false;

#define MAX_TIMER_EVENTS	8
#define kBootstrapButtonHoldDefaultTime	((uint16)1000)
/**
*/
typedef struct timerEventHandlerStruct
	{
	/** the timer event handler */
	timerEventHandler	handler;		
	/** the timer event handler */
	void*				privateData;		
	/** Timer Intervall in msecs */
	uint16				intervall;		
	/** Timer Intervall in msecs */
	uint16				remaining;		
	/** number of shots, < 0 == forever */
	int16				shots;			
	} timerEventHandlerStruct;
	
static timerEventHandlerStruct	handler[MAX_TIMER_EVENTS];

#if defined(__SAMD21G18A__)||defined(__SAMD21J18A__)
void TC3_Handler(void)
#else
	#error "AVR controller not supported!"
#endif
	{
	static	uint16_t	toggleCounter = 0;
	static	uint8_t		saved_INTFLAG;
	//	Read all flags
	saved_INTFLAG = TC3->COUNT16.INTFLAG.reg;
	
	coreSystemTimerTickCount++;
    toggleCounter++;

	// Onboard-LED blinken lassen
	if (toggleCounter == 500)
		{
		toggleCounter = 0;
		PORT->Group[1].OUTTGL.reg = (1UL << 15);
		//coreControllerToggleTimerLED();
		}
 	
  	if (callbackCounterIntervall)
  		{
    	callbackCounter++;
		if (callbackCounter == callbackCounterIntervall)
			{
			callbackCounter = 0;
			callCoreSystemTimerWaitCallbackHandler = true;
			}
		}

	//	handle timer tasks
	for (uint8_t i=0; i<MAX_TIMER_EVENTS; i++)
		{
		if (handler[i].handler && (--handler[i].remaining <= 0))
			{
			(handler[i].handler)(handler[i].privateData);
			handler[i].remaining = handler[i].intervall;
			if (handler[i].shots > 0)
				{
				handler[i].shots--;
				if (!handler[i].shots)
					{
					handler[i].handler = NULL;
					handler[i].privateData = NULL;
					}
				}
			}
		}

#if defined(__SAMD21G18A__)||defined(__SAMD21J18A__)
	TC3->COUNT16.COUNT.reg = TIMER_RELOAD_VALUE;	// Count down
#endif

	//	Clear interupt flags
	TC3->COUNT16.INTFLAG.reg = saved_INTFLAG;
	}


void coreSystemTimerInitialize(void)
	{
#if defined(__SAMD21G18A__)||defined(__SAMD21J18A__)
	PM->APBCMASK.reg |= (1u << 11);			//	Enable clock for TC3
	//GCLK->CLKCTRL.reg = 0x401B;				//	Configure GCLK0 for TCC2/TC3
	GCLK->CLKCTRL.reg = 0x431B;				//	Configure GCLK3 for TCC2/TC3

	TC3->COUNT16.CTRLA.bit.MODE = 0;		//	16 Bit
	TC3->COUNT16.CTRLA.bit.RUNSTDBY = 1;	//	16 Bit
	TC3->COUNT16.CTRLA.bit.PRESCALER = TIMER_PRESCALER_VALUE;
	TC3->COUNT16.COUNT.reg = TIMER_RELOAD_VALUE;			// Set counter value
	TC3->COUNT16.CTRLBSET.bit.DIR = 1;		//	Count Down
	//	Wait for synchronization
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
		;
#endif
	}

void coreSystemTimerOpen(void)
	{
	//TC3->CTRLA.bit.RUNSTDBY = 1;
	TC3->COUNT16.COUNT.reg = TIMER_RELOAD_VALUE;	//	Set counter value
	TC3->COUNT16.CTRLBSET.bit.DIR = 1;				//	Count Down
	//	Wait for synchronization
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
		;
	TC3->COUNT16.CTRLA.bit.ENABLE = 1;
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
		;

	NVIC_ClearPendingIRQ((IRQn_Type)TC3_IRQn);
	NVIC_EnableIRQ((IRQn_Type)TC3_IRQn);

	TC3->COUNT16.INTENSET.bit.OVF = 1;
	}

void coreSystemTimerClose(void)
	{
	TC3->COUNT16.CTRLA.bit.ENABLE = 0;
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
		;

	TC3->COUNT16.INTENCLR.bit.OVF = 1;

	NVIC_DisableIRQ((IRQn_Type)TC3_IRQn);
	NVIC_ClearPendingIRQ((IRQn_Type)TC3_IRQn);
	}

/*	
void coreSystemTimerSleep(time_t t)
	{
	uint8_t	saveRUNSTDBY;
	
	saveRUNSTDBY = SYSCTRL->OSC8M.bit.RUNSTDBY;
	SYSCTRL->OSC8M.bit.RUNSTDBY = 1;
	TC3->COUNT16.COUNT.reg = TIMER_RELOAD_VALUE * t;			// Set counter value
	//	Wait for synchronization
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY)
		;

	SCB->SCR = SCB_SCR_SLEEPDEEP_Msk;
	PM->SLEEP.bit.IDLE = 0x02;
	__WFI();
	coreSystemTimerTickCount += t;
	SYSCTRL->OSC8M.bit.RUNSTDBY = saveRUNSTDBY;
	}
*/	
void coreSystemTimerWait(time_t t)
	{
	time_t	sleepTime = t;
	//uint8_t	saveRUNSTDBY;
	
	//saveRUNSTDBY = SYSCTRL->OSC8M.bit.RUNSTDBY;	
	//SYSCTRL->OSC8M.bit.RUNSTDBY = 1;
	//saveRUNSTDBY = SYSCTRL->OSC8M.bit.RUNSTDBY;	
	//SYSCTRL->OSC8M.bit.RUNSTDBY = 1;

	if (sleepTime != forever)
		sleepTime += coreSystemTimerTickCount;
	
	while(coreSystemTimerTicks() < sleepTime)
		{
		//coreControllerServiceWatchdog();
		//	coreControllerIdle();
		//SCB->SCR = SCB->SCR & ~SCB_SCR_SLEEPDEEP_Msk;
		//PM->SLEEP.bit.IDLE = 0x02;
		__WFI();
		}

	//SYSCTRL->OSC8M.bit.RUNSTDBY = saveRUNSTDBY;
	}

void coreSystemTimerWaitWithCallback(time_t t, time_t intervall, timerWaitCallbackHandler handler)
	{
	time_t	sleepTime = t;
	//uint8_t	saveRUNSTDBY;
	
	//saveRUNSTDBY = SYSCTRL->OSC8M.bit.RUNSTDBY;	
	//SYSCTRL->OSC8M.bit.RUNSTDBY = 1;

	if (sleepTime != forever)
		sleepTime += coreSystemTimerTickCount;
	
	callbackCounter = 0;
	callbackCounterIntervall = intervall;
	callCoreSystemTimerWaitCallbackHandler = false;

	while(coreSystemTimerTicks() < sleepTime)
		{
		if (handler && callCoreSystemTimerWaitCallbackHandler)
			{
			callCoreSystemTimerWaitCallbackHandler = false;
			handler();
			}

		//	coreControllerServiceWatchdog();
		//SCB->SCR = SCB->SCR & ~SCB_SCR_SLEEPDEEP_Msk;
		//PM->SLEEP.bit.IDLE = 0x02;
		__WFI();
		}

	callbackCounterIntervall = 0;
	//SYSCTRL->OSC8M.bit.RUNSTDBY = saveRUNSTDBY;
	}

void coreSystemTimerSetTicks(time_t t)
	{
	//coreControllerEnterCritical();
	coreSystemTimerTickCount = t;
	//coreControllerLeaveCritical();
	}

uint8 coreSystemTimerInstallHandler(timerEventHandler evt, void* privateData, uint16 interval, int16 shots)
{
	uint8	result;
	
	result = 0;

	if ((shots != 0) && (evt != NULL))
		{
		uint8	i;
		
		for (i=0; i<MAX_TIMER_EVENTS; i++)
			{
			if (handler[i].handler == NULL)
				{
				handler[i].intervall = interval;
				handler[i].remaining = interval;
				handler[i].shots = shots;
				handler[i].privateData = privateData;
				
				//coreControllerEnterCritical();
				handler[i].handler = evt;
				//coreControllerLeaveCritical();
				
				result = i+1;
				break;
				}
			}
		}

	return result;
	}

bool coreSystemTimerRemoveHandler(uint8 handle)
	{
	bool	result = false;
	
	if ((handle>0) && (handle<=MAX_TIMER_EVENTS))
		{
		//coreControllerEnterCritical();
		handler[handle - 1].handler = NULL;
		//coreControllerLeaveCritical();
		
		handler[handle - 1].privateData = NULL;
		result = true;
		}
	
	return result;
	}
