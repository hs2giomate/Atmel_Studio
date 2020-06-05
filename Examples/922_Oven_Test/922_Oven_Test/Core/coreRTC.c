/*--------------------------------------------------------------------------

coreRTC.h

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to Real Time Clock

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

--------------------------------------------------------------------------*/

#ifdef MGS_ZST_Revision
#include "lpc24xx.h"
#endif
#ifdef IPECO_PSC_Revision
#include "coreI2C.h"
#endif

#include "coreRTC.h"
#include "coreSystemTimer.h"

#if (IPECO_PSC_Revision >= 0x0103)
#define rtcAddress	0xAC
#endif

int8 coreRTCInitialize(void)
	{
#ifdef MGS_ZST_Revision
	RTC_AMR = 0;
	RTC_CIIR = 0;
	RTC_CCR = (1 << 4)|(1 << 0);	//	external oszilator == bit 4, enable clock == bit 0
#elif (IPECO_PSC_Revision >= 0x0103)
#ifndef NO_RTC
	uint8	controlStatus;
	
	coreRTCReadRegister(0x03, 1, &controlStatus);
	if (controlStatus & (1 << 5))
		{
		controlStatus &= ~(1 << 5);
		coreRTCWriteRegister(0x03, 1, &controlStatus);
		coreRTCSetTimerValue(0);
		}
#endif
#endif
	return 0;
	}

void coreRTCEnableTimer(uint8 clock)
	{
	uint8	controlStatus;
    //	Enable count down timer
	coreI2CReadFromRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
	if (controlStatus & 0x02)
		{
    	controlStatus &= ~0x02;
		coreI2CWriteToRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
		}
	//	Clear Bit 4, 5, 2, 1
    controlStatus &= 0x99;
    
    //	Bit 6	Bit 5
    //		0		0	32 Hz
    //		0		1	 8 Hz
    //		1		0	 1 Hz
    //		1		1	0.5 Hz
    if (clock & (1 << 1))
    	controlStatus |= (1 << 6);
    	
    if (clock & (1 << 0))
    	controlStatus |= (1 << 5);
    	
    controlStatus |= 0x02;
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
	}

void coreRTCEnableTimerWithValue(uint8 clock, uint16 value)
	{
	uint8	controlStatus;
    //	Enable count down timer
	coreI2CReadFromRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
	if (controlStatus & 0x02)
		{
    	controlStatus &= ~0x02;
		coreI2CWriteToRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
		}

	//	Clear Bit 4, 5, 2, 1
    controlStatus &= 0x99;
    //	Bit 6	Bit 5
    //		0		0	32 Hz
    //		0		1	 8 Hz
    //		1		0	 1 Hz
    //		1		1	0.5 Hz
    if (clock & (1 << 1))
    	controlStatus |= (1 << 6);
    	
    if (clock & (1 << 0))
    	controlStatus |= (1 << 5);

    controlStatus |= 0x02;
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x18, &value, 2, forever);
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
/*    	
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
    controlStatus |= 0x02;
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);

	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x18, &value, 2, forever);
*/
	}

void coreRTCDisableTimer(void)
	{
	uint8	controlStatus;
    //	Disable count down timer
	coreI2CReadFromRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
	if (controlStatus & 0x02)
		{
    	controlStatus &= ~0x02;
		coreI2CWriteToRegister(kI2C0, rtcAddress, 0x00, &controlStatus, 1, forever);
		}
	}

void coreRTCSetTimerValue(uint16 value)
	{
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x18, &value, 2, forever);
	}

uint16 coreRTCGetTimerValue(void)
	{
	uint16	value;
	
	value = 0;
	coreI2CReadFromRegister(kI2C0, rtcAddress, 0x18, &value, 2, forever);

	return value;
	}

void coreRTCEnableTemperature(void)
	{
	uint8	controlStatus;
	
	coreI2CReadFromRegister(kI2C0, rtcAddress, 0x30, &controlStatus, 1, forever);
    controlStatus &= 0x03;
    controlStatus |= 0x02;
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x30, &controlStatus, 1, forever);
	}

void coreRTCDisableTemperature(void)
	{
	uint8	controlStatus;
	
	coreI2CReadFromRegister(kI2C0, rtcAddress, 0x30, &controlStatus, 1, forever);
    controlStatus &= 0x03;
	coreI2CWriteToRegister(kI2C0, rtcAddress, 0x30, &controlStatus, 1, forever);
	}

int16 coreRTCGetTemperatureValue(void)
	{
	uint8	controlStatus;
	
	coreI2CReadFromRegister(kI2C0, rtcAddress, 0x20, &controlStatus, 1, forever);
	return (int16)((int16)controlStatus - 60);
	}

#ifdef RTC_RAM_SUPPORT
uint8	coreRTCReadRAM(uint8 adr, void* p, uint8 n)
	{
	uint8	result;
#ifdef MGS_ZST_Revision
#elif (IPECO_PSC_Revision >= 0x0103)
	result = coreI2CReadFromRegister(kI2C0, rtcAddress, 0x38 + adr, p, n, forever);
#endif
	return result;
	}

uint8	coreRTCWriteRAM(uint8 adr, const void* p, uint8 n)
	{
	int8	result = 0;
#ifdef MGS_ZST_Revision
#elif (IPECO_PSC_Revision >= 0x0103)
	result = coreI2CWriteToRegister(kI2C0, rtcAddress, 0x38 + adr, p, n, forever);
#endif
	return result;
	}
#endif

int8 coreRTCReadRegister(uint8 address, uint8 n, void* data)
	{
	int8	result = 0;
#ifdef MGS_ZST_Revision
#elif (IPECO_PSC_Revision >= 0x0103)
	coreI2CReadFromRegister(kI2C0, rtcAddress, address, data, n, forever);
#endif
	return result;
	}

int8 coreRTCWriteRegister(uint8 address, uint8 n, const void* data)
	{
	int8	result = 0;
#ifdef MGS_ZST_Revision
#elif (IPECO_PSC_Revision >= 0x0103)
	coreI2CWriteToRegister(kI2C0, rtcAddress, address, data, n, forever);
#endif
	return result;
	}

