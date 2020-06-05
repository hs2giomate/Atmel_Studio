/*--------------------------------------------------------------------------

coreScheduler.h

This file is part of e.Lib

Interface
low level interface to the e.Lib scheduler

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2007,2008,2009,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_SCHEDULER_H
#define CORE_SCHEDULER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Core/coreTypes.h"

void	coreSchedulerInitialize(void);

/** Switch to next waiting thread.
*/
void	coreSchedulerYield(void);

/** Wait for t milliseconds.
*/
void	coreSchedulerWait(time_t t);

#ifdef __cplusplus
	}
#endif

#endif

