/*--------------------------------------------------------------------------

coreController.h

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Interface
low level interface to the ATmega128/AT90CAN128

$Date: 2018-03-01 11:22:52 +0100 (Do, 01. Mrz 2018) $
$Revision: 7755 $

--------------------------------------------------------------------------*/
#ifndef CORE_CONTROLLER_H
#define CORE_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <coreTypes.h>

/**
	Callback function for external interrupts
*/
typedef void (*externalInterruptHandler)(uint8);
extern volatile uint16	externalInterruptCounter[4];

/** Initialize controller.
*/
void	coreControllerInitialize(void);

/** Release controller.
*/
void	coreControllerRelease(void);

/** Reset controller.
*/
void	coreControllerReset(void);

/**
Enter Idle mode
*/
inline void	coreControllerIdle(void) __attribute__((always_inline));

/**
Enable global interrupts
*/
void	coreControllerEnableInterrupts(void);

/**
Disable global interrupts
*/
void	coreControllerDisableInterrupts(void);

/**
Enable external interrupt on pin <pin>
*/
void	coreControllerEnableExternalInterrupt(uint8 pin, externalInterruptHandler handler);

/**
Disable external interrupt on pin <pin>
*/
void	coreControllerDisableExternalInterrupt(uint8 pin);

/**
Enter critical section
*/
inline void	coreControllerEnterCritical(void) __attribute__((always_inline));

/**
Leave critical section
*/
inline void	coreControllerLeaveCritical(void) __attribute__((always_inline));

/** Enable the watchdog timer.
*/
void	coreControllerEnableWatchdog(void);

/** Disable the watchdog timer.
*/
void	coreControllerDisableWatchdog(void);

/** Get watchdog timer state.
*/
bool	coreControllerGetWatchdogState(void);

/** Set watchdog timer state.
*/
void	coreControllerSetWatchdogState(bool);

/** Resets the watchdog timer.
*/
inline void	coreControllerServiceWatchdog(void) __attribute__((always_inline));

/** Enter bootloader.
*/
void	coreControllerEnterBootloader(void);

/** Toggle timer LED.
 */
inline void	coreControllerToggleTimerLED(void) __attribute__((always_inline));

extern volatile uint8	SREGSave;

inline void coreControllerToggleTimerLED(void)
	{
	#if E_DEVICE_CAN == 0x0100
		PORTB = PINB ^ _BV(PB7);
	#elif E_BASIC_Revision == 0x0101
		PORTB = PINB ^ _BV(PB7);
	#elif E_BASIC_Revision == 0x0103
		PORTB = PINB ^ _BV(PB7);
	#elif E_BASIC_Revision == 0x0104
		PORTB = PINB ^ _BV(PB7);
	#elif defined(IPECO_PSC_Revision)
		#ifdef E_DEVICE
			PORTB = PINB ^ _BV(PB7);
		#else
			PORTB = PINB ^ _BV(PB6);
		#endif
	#endif
	}

inline void coreControllerEnterCritical(void)
	{
	SREGSave = SREG;
	cli();
	}

inline void coreControllerLeaveCritical(void)
	{
	SREG = SREGSave;
	}

inline void coreControllerServiceWatchdog(void)
	{
	asm("wdr");
	}

inline void	coreControllerIdle(void)
	{
	sleep_mode();
	}

#ifdef __cplusplus
	}
#endif

#endif

