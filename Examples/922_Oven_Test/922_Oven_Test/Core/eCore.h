/*--------------------------------------------------------------------------

eCore.h

This file is part of e.Development

Interface
Common includes for the eCore C library

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2006,2007, 2009, 2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_E_CORE_H
#define CORE_E_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"
#include "coreADConverter.h"
#ifdef MGS_ZST_Revision
#include "coreInterrupts.h"
#endif
#include "coreController.h"
#include "corePorts.h"
#include "coreAsyncSerial.h"
#include "coreNVM.h"
#ifdef MGS_ZST_Revision
#include "coreFlashROM.h"
#endif
#include "coreI2C.h"
#include "coreSPI.h"
#if defined(MGS_ZST_Revision)||defined(IPECO_PSC_Revision)
#include "coreRTC.h"
#endif
#include "coreSystemTimer.h"
#include "corePWM.h"
#if defined(MGS_ZST_Revision)||defined(IPECO_PSC_Revision)||defined(E_DEVICE_CAN)
#include "coreCAN.h"
#endif

void eCoreInitialize(void);

#ifdef __cplusplus
	}
#endif

#endif

