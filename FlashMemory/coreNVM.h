/*--------------------------------------------------------------------------

coreNVM.h

This file is part of e.Development

Interface
low level interface to NVM

$Date: 2016-09-28 14:13:45 +0200 (Mi, 28 Sep 2016) $
$Revision: 967 $

Copyright (c) 2006,2007,2009,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_NVM_H
#define CORE_NVM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/**
	Constants for on-chip NVM
*/
#if defined(MGS_ZST_Revision)
enum
	{
	kNVM0 = 0,	//!	RTC RAM
	kNVM1 = 1,	//!	EEPROM
	kNVM2 = 2,	//!	FRAM
	kNumberOfNVMS
	};
#elif defined(ICB_Revision)
enum
	{
	kNVM0 = 0,	//!	RTC RAM
	kNVM1 = 1,	//!	FRAM
	kNumberOfNVMS
	};
#else
	enum
	{
		kNVM0 = 0,	//!	RTC RAM
		kNVM1 = 1,	//!	FRAM
		kNumberOfNVMS
	};
#endif

/** \addtogroup NVM
*/
/** Initialize NVM subsystem
*/
int8 coreNVMInitialize(void);

/** Get NVM capacity
	\param nvm NVM selector
	\return NVM capacity in byte
*/
int32 coreNVMGetNVMCapacity(uint8 nvm);

/** Read from NVM
	\param nvm NVM selector
	\param adr Offset into NVM
	\param p Pointer to receive data buffer
	\param n Number of bytes to read
	\return Number of bytes actually read
*/
int32	coreNVMRead(uint8 nvm, int32 adr, void* p, int32 n);

/** Write to NVM
	\param nvm NVM selector
	\param adr Offset into NVM
	\param p Pointer to write data buffer
	\param n Number of bytes to write
	\return Number of bytes actually written
*/
int32	coreNVMWrite(uint8 nvm, int32 adr, const void* p, int32 n);

#ifdef __cplusplus
	}
#endif

#endif

