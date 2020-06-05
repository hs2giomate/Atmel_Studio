/*--------------------------------------------------------------------------

coreI2C.h

This file is part of e.Development

Interface
low level interface to the ATMega128 I2C

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $

Copyright (c) 2006, 2007, 2013 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_I2C_H
#define CORE_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/**
	*\defgroup I2C I2C-Bus
*/
/*@{*/
/**
	Constants for on-chip I2C channels
*/
enum
	{
	kI2C0 = 0,	//!	I2C0
	kI2C1 = 1,	//!	I2C1
	kI2C2 = 2,	//!	I2C2
	kNumberOfI2Cs
	};

/** Zustände des I2C
*/
typedef enum
	{
	kI2CStateIdle = 0,
	kI2CStateBusy = 1,
	kI2CStateMasterTransmit = 2,
	kI2CStateMasterReceive = 3,
	kI2CStateSlaveTransmit = 4,
	kI2CStateSlaveReceive = 5
	} i2cStates;
/*@}*/

int8 coreI2CInitialize(void);

int8 coreI2COpen(uint8, uint32 bitrate);
int8 coreI2CClose(uint8);

void coreI2CRelease(void);

void coreI2CAbortOperation(uint8 i2c);

void coreI2CSetLocalDeviceAddress(uint8 deviceAddr, uint8 genCallEn);

void coreI2CSetSlaveReceiveHandler(uint8* (*I2CSlaveReceiveFunction)(int16* receiveDataLength));
void coreI2CSetSlaveTransmitHandler(uint8* (*I2CSlaveTransmitFunction)(int16* transmitDataLength));

int16 coreI2CRead(uint8, uint8 addr, void* p, int16 n, tick_t timeout);
int16 coreI2CReadFromRegister(uint8, uint8 addr, uint8 reg, void* p, int16 n, tick_t timeout);

int16 coreI2CWrite(uint8, uint8 addr, const void* p, int16 n, tick_t timeout);
int16 coreI2CWriteToRegister(uint8, uint8 addr, uint8 reg, const void* p, int16 n, tick_t timeout);

#ifdef __cplusplus
	}
#endif

#endif

