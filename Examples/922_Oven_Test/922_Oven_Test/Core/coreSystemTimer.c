/*--------------------------------------------------------------------------

coreSystemTimer.c

This file is part of e.Core

Implementation
low level interface to the ATmega128/AT90CAN128 timer

$Author: steffen $
$Date: 2018-02-28 17:52:23 +0100 (Mi, 28. Feb 2018) $
$Revision: 7744 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/coreSystemTimer.c $

--------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "corePorts.h"
#include "coreController.h"
#include "coreSystemTimer.h"

const tick_t	forever = 0x7fffffff;

#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
#define kBootstrapButtonHoldDefaultTime	((uint16)5000)
#endif

#if (MAX_TIMER_EVENTS > 0)
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
	
static volatile	uint8	timerEventMask = 0;
static volatile	uint8	timerEventCount = 0;
static timerEventHandlerStruct	handler[MAX_TIMER_EVENTS];
#endif

#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
static volatile bool	monitorBootstrapButton = true;
static volatile uint16	bootstrapButtonHoldTime = kBootstrapButtonHoldDefaultTime;
#endif

static volatile	uint16	callbackCounter = 0;
static volatile	uint16	callbackCounterIntervall = 100;
static volatile bool	callCoreSystemTimerWaitCallbackHandler = false;
bool	inReadTicksCriticalSection = false;
volatile tick_t	coreSystemTimerTickCount = 0;
static volatile uint32	coreSystemTimerSecondCount = 0;

#if defined(__AVR_ATmega128__)
ISR(TIMER2_COMP_vect)
#elif defined(__AVR_AT90CAN128__)
ISR(TIMER0_COMP_vect)
#elif defined(__AVR_ATmega644P__)
ISR(TIMER0_COMP_vect)
#else
	#error "AVR controller not supported!"
#endif
	{
	static	uint8	i = 0;
	static	uint16	toggleCounter = 0;
	static	uint16	ticksPending = 0;

	if (inReadTicksCriticalSection)
		ticksPending++;
	else
		{
		coreSystemTimerTickCount++;
		if (ticksPending)
			{
			coreSystemTimerTickCount += ticksPending;
			ticksPending = 0;
			}
		}

    toggleCounter++;
#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
	if (monitorBootstrapButton)
		{
	#if E_DEVICE_CAN == 0x0100
		if (!(PINE & _BV(PINE6)))
	#elif E_BASIC_Revision == 0x0101
		if (!(PINE & _BV(PINE2)))
	#elif E_BASIC_Revision == 0x0103
		if (!(PINE & _BV(PINE6)))
	#elif E_BASIC_Revision == 0x0104
		if (!(PINE & _BV(PINE2)))
	#else
		if (!(PINE & _BV(PINE6)))
	#endif
			{
			if (bootstrapButtonHoldTime == 0)
				{
				coreControllerEnterBootloader();
				}
			else
				bootstrapButtonHoldTime--;	
			}
		else if (bootstrapButtonHoldTime != kBootstrapButtonHoldDefaultTime)
			bootstrapButtonHoldTime = kBootstrapButtonHoldDefaultTime;
		}
#endif

	// Onboard-LED blinken lassen
	if (toggleCounter == 500)
		{
		coreControllerToggleTimerLED();
		}
	else if (toggleCounter == 1000)
		{
		toggleCounter = 0;
		coreControllerToggleTimerLED();
		coreSystemTimerSecondCount++;
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

#if (MAX_TIMER_EVENTS > 0)
	//	handle timer tasks
	if (timerEventMask)
		{
		uint8	count = timerEventCount;

		for (i=0; i<MAX_TIMER_EVENTS; i++)
			{
			if (count && (timerEventMask & (1 << i)) && (handler[i].remaining-- <= 0))
				{
				count--;
				(handler[i].handler)(handler[i].privateData);
				handler[i].remaining = handler[i].intervall;
				if (handler[i].shots > 0)
					{
					handler[i].shots--;
					if (!handler[i].shots)
						{
						timerEventMask &= ~(1 << (i - 1));
						timerEventCount--;
						}
					}
				}
			}
		}
#endif
	}

void coreSystemTimerInitialize(void)
	{
#if (MAX_TIMER_EVENTS > 0)
	uint8	i;

	for (i=0; i<MAX_TIMER_EVENTS; i++)
		{
		handler[i].handler = NULL;
		handler[i].privateData = NULL;
		handler[i].intervall = 0;
		handler[i].remaining = 0;
		handler[i].shots = 0;
		}

	timerEventMask = 0;
	timerEventCount = 0;
#endif	
	
#if defined(__AVR_ATmega128__)
	TCCR2A = _BV(CS21)|_BV(CS20)|_BV(WGM21);	// Divisor == 64, CTC Mode
	OCR2A = 230 - 1;
	TIMSK2 |= _BV(OCIE2A);
#elif defined(__AVR_AT90CAN128__)
	TCCR0A = _BV(CS01)|_BV(CS00)|_BV(WGM01);	// Divisor == 64, CTC Mode
	OCR0A = 250 - 1;
	TIMSK0 |= _BV(OCIE0A);
#elif defined(__AVR_ATmega644P__)
	TCCR0A = _BV(CS01)|_BV(CS00)|_BV(WGM01);	// Divisor == 64, CTC Mode
	OCR0A = 125 - 1;
	TIMSK0 |= _BV(OCIE0A);
#endif	
	}

void coreSystemTimerWait(tick_t t)
	{
	tick_t	sleepTime = t;

	if (sleepTime != forever)
		sleepTime += coreSystemTimerTicks();
	
	while(coreSystemTimerTicks() < sleepTime)
		{
		coreControllerServiceWatchdog();
		sleep_mode();
		}
	}

void coreSystemTimerWaitWithCallback(tick_t t, tick_t intervall, timerWaitCallbackHandler handler)
	{
	callbackCounter = 0;
	callbackCounterIntervall = intervall;
	callCoreSystemTimerWaitCallbackHandler = false;

	tick_t	sleepTime = t;

	if (sleepTime != forever)
		sleepTime += coreSystemTimerTicks();
	
	while(coreSystemTimerTicks() < sleepTime)
		{
		if (handler && callCoreSystemTimerWaitCallbackHandler)
			{
			callCoreSystemTimerWaitCallbackHandler = false;
			handler();
			}
		coreControllerIdle();
		}

	callbackCounterIntervall = 0;
	}

void coreSystemTimerSetTicks(tick_t t)
	{
	coreControllerEnterCritical();
	coreSystemTimerTickCount = t;
	coreControllerLeaveCritical();
	}

uint32 coreSystemTimerSeconds(void)
	{
	uint32	result;

	coreControllerEnterCritical();
	result = coreSystemTimerSecondCount;
	coreControllerLeaveCritical();
	return result;
	}

void coreSystemTimerSetSeconds(uint32 t)
	{
	coreControllerEnterCritical();
	coreSystemTimerSecondCount = t;
	coreControllerLeaveCritical();
	}

#if (MAX_TIMER_EVENTS > 0)
uint8 coreSystemTimerInstallHandler(timerEventHandler evt, void* privateData, uint16 interval, int16 shots)
	{
	uint8	result;
	
	result = 0;

	if ((timerEventCount < MAX_TIMER_EVENTS) && (shots != 0) && (evt != NULL))
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
				
				coreControllerEnterCritical();
				handler[i].handler = evt;
				coreControllerLeaveCritical();
				
				timerEventMask |= (1 << i);
				timerEventCount++;
				
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
		coreControllerEnterCritical();
		timerEventMask &= ~(1 << (handle - 1));
		coreControllerLeaveCritical();
		
		if (timerEventCount)
			timerEventCount--;

		handler[handle - 1].handler = NULL;
		handler[handle - 1].privateData = NULL;
		result = true;
		}
	
	return result;
	}
#endif

