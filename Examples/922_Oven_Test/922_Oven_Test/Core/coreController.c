/*--------------------------------------------------------------------------

coreController.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the ATmega128/AT90CAN128

$Author: steffen $
$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/coreController.c $

--------------------------------------------------------------------------*/
#include "coreController.h"
#include "corePorts.h"

void (*app_start)(void) = 0x0000;
void (*bootloader_start)(void) = (void (*)())0xF000;

volatile uint8	SREGSave;
static volatile bool	watchdogTimerState;

volatile uint16	externalInterruptCounter[4];
static externalInterruptHandler	interruptHandler[4];

SIGNAL(SIG_INTERRUPT4)
	{
	externalInterruptCounter[0]++;
	if (interruptHandler[0])
		interruptHandler[0](4);
	}

SIGNAL(SIG_INTERRUPT5)
	{
	externalInterruptCounter[1]++;
	if (interruptHandler[1])
		interruptHandler[1](5);
	}

SIGNAL(SIG_INTERRUPT6)
	{
	externalInterruptCounter[2]++;
	if (interruptHandler[2])
		interruptHandler[2](6);
	}

SIGNAL(SIG_INTERRUPT7)
	{
	externalInterruptCounter[3]++;
	if (interruptHandler[3])
		interruptHandler[3](7);
	}

void coreControllerInitialize(void)
	{
	uint8	i;
	
	cli();
    watchdogTimerState = false;
	// Sleepmode setzen, wichtig für coreControllerIdle() !!!!!
	set_sleep_mode(SLEEP_MODE_IDLE);
	
	for (i=0; i<4; i++)
		{
		externalInterruptCounter[i] = 0;
		interruptHandler[i] = NULL;
		}
	}

void coreControllerRelease(void)
	{
	coreControllerDisableInterrupts();
	coreControllerDisableWatchdog();
	}

void coreControllerReset(void)
	{
	coreControllerEnterCritical();
	WDTCR |= (1<<WDCE) | (1<<WDE);
	WDTCR = (1<<WDE);
	coreControllerLeaveCritical();
	
	while (true)
		;
	}

/**
Enable global interrupts
*/
void coreControllerEnableInterrupts(void)
	{
	sei();
	}

/**
Disable global interrupts
*/
void coreControllerDisableInterrupts(void)
	{
	cli();
	}

/**
Enable external interrupt on pin <pin>
*/
void coreControllerEnableExternalInterrupt(uint8 pin, externalInterruptHandler handler)
	{
	EIMSK &= ~(1 << pin);
	if (pin < 4)	// Asynchronous External Interrupt
		{
		EICRA &= ~(0x03 << (2 * pin));
		EICRA |=  (0x03 << (2 * pin));
		}
	else 			//	Synchronous External Interrupt
		{
		EICRB &= ~(0x03 << (2 * (pin - 4)));
		EICRB |=  (0x03 << (2 * (pin - 4)));
		
		interruptHandler[pin - 4] = handler;
		}
	EIMSK |= (1 << pin);
	}
/**
Disable external interrupt on pin <pin>
*/
void coreControllerDisableExternalInterrupt(uint8 pin)
	{
	EIMSK &= ~(1 << pin);
	if (pin < 4)	// Asynchronous External Interrupt
		{
		EICRA &= ~(0x03 << (2 * pin));
		}
	else 			//	Synchronous External Interrupt
		{
		EICRB &= ~(0x03 << (2 * (pin - 4)));
		interruptHandler[pin - 4] = NULL;
		}
	}

/** Enable the watchdog timer.
*/
void coreControllerEnableWatchdog(void)
	{
	if (!watchdogTimerState)
		{
		coreControllerEnterCritical();
		WDTCR |= (1<<WDCE) | (1<<WDE);
		WDTCR = (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
		coreControllerLeaveCritical();
   		coreControllerServiceWatchdog();
		
		watchdogTimerState = true;
   		}
	}


/** Disable the watchdog timer.
*/
void coreControllerDisableWatchdog(void)
	{
	if (watchdogTimerState)
		{
		coreControllerEnterCritical();
		// See AT90CAN manual page 59 for details
		WDTCR |= (1<<WDCE) | (1<<WDE);
		// Turn off WDT
		WDTCR = 0;
		coreControllerLeaveCritical();
		
		watchdogTimerState = false;
		}
	}

bool coreControllerGetWatchdogState(void)
	{
	return watchdogTimerState;
	}

void coreControllerSetWatchdogState(bool state)
	{
	if (state)
		coreControllerEnableWatchdog();
	else
		coreControllerDisableWatchdog();	
	}

void coreControllerEnterBootloader(void)
	{
	coreControllerDisableWatchdog();
	cli();
	corePortsInitialize();
	bootloader_start();
	}

