/*--------------------------------------------------------------------------

eCore.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
Library initialization code

$Author: steffen $
$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/eCore.c $

--------------------------------------------------------------------------*/
#include "eCore.h"

void eCoreInitialize(void)
	{
	corePortsInitialize();
#ifdef MGS_ZST_Revision
	coreInterruptsInitialize();
#endif
	coreControllerInitialize();
	// Interrupts an
#ifdef MGS_ZST_Revision
	coreInterruptsEnableInterrupts();
#else
	coreControllerEnableInterrupts();
#endif
   	coreAsyncSerialInitialize();
	coreSystemTimerInitialize();
#ifdef MGS_ZST_Revision
	coreFlashROMInitialize();
	//coreControllerConfigureSDRAM();
#endif
   	coreSPIInitialize();
	corePWMInitialize();
   	coreADConverterInitialize();
	coreNVMInitialize();
   	coreI2CInitialize();
#if defined (NSD_SUPPORT)&& (defined(MGS_ZST_Revision)||defined(IPECO_PSC_Revision)||defined(E_DEVICE_CAN))
   	coreCANInitialize();
#endif
#if defined(MGS_ZST_Revision)||defined(IPECO_PSC_Revision)
	coreRTCInitialize();
#endif
	}

