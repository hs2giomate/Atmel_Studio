/*--------------------------------------------------------------------------

coreNVM.h

This file is part of e.Development

Interface
low level interface to RTC

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2006,2007,2009,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_RTC_H
#define CORE_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/** \addtogroup RTC
*/
enum
	{
	kTimerClock32Hz = 0,
	kTimerClock8Hz = 1,
	kTimerClock1Hz = 2,
	kTimerClock05Hz = 3,
	};
	
/** Initialize RTC subsystem
*/
int8 coreRTCInitialize(void);

void	coreRTCEnableTimer(uint8 clock);
void	coreRTCEnableTimerWithValue(uint8 clock, uint16 value);
void	coreRTCDisableTimer(void);
void	coreRTCSetTimerValue(uint16);
uint16	coreRTCGetTimerValue(void);

void	coreRTCEnableTemperature(void);
void	coreRTCDisableTemperature(void);
int16	coreRTCGetTemperatureValue(void);

#ifdef RTC_RAM_SUPPORT
uint8	coreRTCReadRAM(uint8 adr, void* p, uint8 n);
uint8	coreRTCWriteRAM(uint8 adr, const void* p, uint8 n);
#endif
int8	coreRTCReadRegister(uint8 address, uint8 n, void* data);
int8	coreRTCWriteRegister(uint8 address, uint8 n, const void* data);

#ifdef __cplusplus
	}
#endif

#endif

