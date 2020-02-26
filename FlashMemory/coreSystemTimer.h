/*--------------------------------------------------------------------------

coreSystemTimer.h

This file is part of e.Development

Interface
low level interface to the ATmega128 timer

$Date: 2016-11-07 09:07:35 +0100 (Mo, 07 Nov 2016) $
$Revision: 1080 $

--------------------------------------------------------------------------*/
#ifndef CORE_SYSTEM_TIMER_H
#define CORE_SYSTEM_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

typedef void (*timerEventHandler)(void*);
typedef void (*timerWaitCallbackHandler)(void);

/**
	Konstante, die eine "unendliche" Zeit repräsentiert.
*/
extern const time_t	forever;


void coreSystemTimerInitialize(void);

void coreSystemTimerOpen(void);

/** Gibt die Zeit in Millisekunden seit der letzten Initialisierung an.
	Initialisiert wird das Board entweder durch Anlegen einer Spannung (USB oder
	über Erweiterungsport) oder durch Aufruf der Methode \ref reset().
	\sa reset(), eDeviceController()
*/
inline time_t	coreSystemTimerTicks(void) __attribute__((always_inline));

/** Set the core timers millisecond counter to a given value.
*/
void	coreSystemTimerSetTicks(time_t t);

/** Wait for t milliseconds.
*/
void	coreSystemTimerWait(time_t t);
void	coreSystemTimerWaitWithCallback(time_t t, time_t intervall, timerWaitCallbackHandler handler);
void	coreSystemTimerSleep(time_t t);


/** sucht leeren Handler (NULL) und initialisiert interval und shots
*/
uint8	coreSystemTimerInstallHandler(timerEventHandler, void* privateData, uint16 interval, int16 shots);

/** setzt Handler auf NULL
*/
bool	coreSystemTimerRemoveHandler(uint8);

extern volatile time_t	coreSystemTimerTickCount;
inline time_t coreSystemTimerTicks(void)
	{
	return coreSystemTimerTickCount;
	}

#ifdef __cplusplus
	}
#endif

#endif

