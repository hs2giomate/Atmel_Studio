/*--------------------------------------------------------------------------

coreNVM.h

This file is part of Core

Interface
low level interface to NVM

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

--------------------------------------------------------------------------*/
#ifndef CORE_NVM_H
#define CORE_NVM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/**
	Selectors for on-board NVM
*/
enum
	{
	kNVM0 = 0,	//!	internal EEPROM
#if defined(IPECO_PSC_Revision)
	kNVM1 = 1,	//!	EEPROM
	kNVM2 = 2,	//!	FRAM
#ifdef RTC_RAM_SUPPORT
	kNVM3 = 3,	//!	RTC RAM
#endif
#endif
	kNumberOfNVMS
	};

/** \addtogroup NVM
*/
/** Initialize NVM subsystem
*/
int8 coreNVMInitialize(void);

/** Get NVM capacity
	\param nvm NVM selector
	\return NVM capacity in bytes
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

