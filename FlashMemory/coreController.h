/*--------------------------------------------------------------------------

coreController.h

This file is part of e.Development

Interface
low level interface to the ATmega128/AT90CAN128

$Date: 2017-11-27 11:39:44 +0100 (Mo, 27 Nov 2017) $
$Revision: 1618 $

--------------------------------------------------------------------------*/
#ifndef CORE_CONTROLLER_H
#define CORE_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <coreTypes.h>

// Constants for Clock generators
#define GENERIC_CLOCK_GENERATOR_MAIN      (0u)
#define GENERIC_CLOCK_GENERATOR_XOSC32K   (1u)
#define GENERIC_CLOCK_GENERATOR_OSCULP32K (2u) // Initialized at reset for WDT
#define GENERIC_CLOCK_GENERATOR_OSC8M     (3u)
#define GENERIC_CLOCK_GENERATOR_XOSC_RTC  (4u)

/** Initialize controller.
*/
void	coreControllerInitialize(void);

/** Reset controller.
*/
void	coreControllerReset(void);

/**
Enter Idle mode
*/
void	coreControllerIdle(void);

/**
Enter Deep Sleep mode
*/
void	coreControllerSleep(void);

/** Enable Brown Out detection.
*/
void	coreControllerEnableBOD(void);

/** Disable Brown Out detection.
*/
void	coreControllerDisableBOD(void);

/** Enable the watchdog timer.
*/
void	coreControllerEnableWatchdog(void);

/** Disable the watchdog timer.
*/
void	coreControllerDisableWatchdog(void);

void	coreControllerServiceWatchdog(void);

#ifdef __cplusplus
	}
#endif

#endif

