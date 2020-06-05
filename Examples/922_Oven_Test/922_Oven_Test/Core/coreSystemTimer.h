/*--------------------------------------------------------------------------

coreSystemTimer.h

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Interface
low level interface to the ATmega128 timer

$Date: 2018-03-01 11:07:47 +0100 (Do, 01. Mrz 2018) $
$Revision: 7750 $

--------------------------------------------------------------------------*/
#ifndef CORE_SYSTEM_TIMER_H
#define CORE_SYSTEM_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

#ifdef IPECO_PSC_Revision
#define MAX_TIMER_EVENTS	0
#else
#define MAX_TIMER_EVENTS	4
#endif

#if (MAX_TIMER_EVENTS > 0)
typedef void (*timerEventHandler)(void*);
#endif
typedef void (*timerWaitCallbackHandler)(void);

/**
	Konstante, die eine "unendliche" Zeit repräsentiert.
*/
extern const tick_t	forever;
extern bool	inReadTicksCriticalSection;
extern volatile tick_t	coreSystemTimerTickCount;


void coreSystemTimerInitialize(void);

/** Gibt die Zeit in Millisekunden seit der letzten Initialisierung an.
	Initialisiert wird das Board entweder durch Anlegen einer Spannung (USB oder
	Ã¼ber Erweiterungsport) oder durch Aufruf der Methode \ref reset().
	\sa reset(), eDeviceController()
*/
inline tick_t	coreSystemTimerTicks(void) __attribute__((always_inline));

/** Set the core timers millisecond counter to a given value.
*/
void	coreSystemTimerSetTicks(tick_t t);

/** Gibt die Zeit in Sekunden seit dem letzten Reset an.
	\sa reset(), eDeviceController()
*/
uint32	coreSystemTimerSeconds(void);

/** Set the core timers second counter to a given value.
*/
void	coreSystemTimerSetSeconds(uint32 t);

/** Wait for t milliseconds.
*/
void	coreSystemTimerWait(tick_t t);

void	coreSystemTimerWaitWithCallback(tick_t t, tick_t intervall, timerWaitCallbackHandler handler);
	
#if (MAX_TIMER_EVENTS > 0)
/** sucht freien Handler (NULL) und initialisiert interval und shots
*/
uint8	coreSystemTimerInstallHandler(timerEventHandler, void* privateData, uint16 interval, int16 shots);
	
/** setzt Handler auf NULL
*/
bool	coreSystemTimerRemoveHandler(uint8);
#endif

inline tick_t coreSystemTimerTicks(void)
	{
	tick_t	result;

	inReadTicksCriticalSection = true;	//	coreControllerEnterCritical();
	result = coreSystemTimerTickCount;
	inReadTicksCriticalSection = false;	//	coreControllerLeaveCritical();
	return result;
	}

#ifdef __cplusplus
	}
#endif

#endif

