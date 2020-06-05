/*--------------------------------------------------------------------------

eDevicePWM.c

This file is part of e.Development

Implementation
Application level PWM access

$Author: steffen $
$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/ApplicationLayer/PSC/eDevicePWM.cc $

Copyright (c) 2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#include "eDevicePWM.h"

eDevicePWM::eDevicePWM(void)
	{
	uint8	i;
	
	for (i=0; i<kPWMPinCount; i++)
		channels[i].pin = (pwmPin)i;
	}

