/*--------------------------------------------------------------------------

coreSPI.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the ATmega 128 SPI controller

$Author: steffen $
$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/coreSPI.c $

--------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "coreController.h"
#include "coreSPI.h"

#define CORE_CONFIGURE_SPI0

typedef struct
	{
	uint16			status;
	uint16			spiTransmitCounter;
	uint16			spiTransferRemaining;
	const byte*		spiTransferData;
	byte*			spiReceiveData;
	controllerPin	pin;
	} spiData;

static volatile spiData	spis[kNumberOfSPI];

int8 coreSPIInitializeChannel(uint8 spi);

// SPI interrupt service handler
#ifdef CORE_CONFIGURE_SPI0
SIGNAL(SIG_SPI)
	{
	corePortsClearPortPin(kPortG, kPortPin0);
	spis[kSPI0].spiTransmitCounter++;
	if (spis[kSPI0].spiReceiveData)
		{
		*spis[kSPI0].spiReceiveData++ = SPDR;
		if (--spis[kSPI0].spiTransferRemaining)
			{
			if (spis[kSPI0].spiTransferData)
				SPDR = *spis[kSPI0].spiTransferData++;
			else
				SPDR = 0x00;
			}
		}
	else if (spis[kSPI0].spiTransferRemaining > 0)
		{
		if (--spis[kSPI0].spiTransferRemaining)
			SPDR = *spis[kSPI0].spiTransferData++;
		}
	corePortsSetPortPin(kPortG, kPortPin0);
	}
#endif

/** Initialize SPI subsystem
*/
int8 coreSPIInitialize(void)
	{
#ifdef CORE_CONFIGURE_SPI0
	//	Setup SPI I/O pins:
	//	SCK: Port B, pin 7 as output and activate pull up
	//PORTB |= (1 << PORTB1);
	//	SCK: Port B, pin 1 as output
	DDRB |= (1 << PORTB1);
	//	MOSI: Port B, pin 2 as output
	DDRB |= (1 << PORTB2);
	coreSPIInitializeChannel(kSPI0);
#endif	
	return 0xFF;
	}

int8 coreSPIInitializeChannel(uint8 spi)
	{
	int8	result = 0;
	
	if (spi < kNumberOfSPI)
		{
		spis[spi].spiTransferRemaining = 0;
		spis[spi].spiTransferData = NULL;
		spis[spi].pin.port = kPortA;
		spis[spi].pin.pin = kPortPin0;
		}
	else
		result = -1;
	
	return result;
	}

int8 coreSPIOpen(uint8 spi, uint8 clock)
	{
	int8	result = 0;

	coreSPIInitializeChannel(spi);
	
	switch (spi)
		{
	#ifdef CORE_CONFIGURE_SPI0
		case kSPI0:
			// setup SPI interface:
			// clock = f/4
			// select clock phase positive-going in middle of data
			// Data order MSB first
			// master mode
			// enable SPI
			// enable SPI interrupt
			if (SPSR & (1 << SPIF))
				SPDR = 0;
			
			if (clock & 0x04)
				SPSR |= (1 << SPI2X);	// Double speed, Master onlys

			SPCR = (1 << MSTR)|(1 << SPE)|(1 << SPIE)|(clock & 0x03);
 			break;
	#endif
		
		default:
			result = -1;
			break;
		}
	return result;
	}

int8 coreSPIClose(uint8 spi)
	{
	int8	result = 0;

	switch (spi)
		{
	#ifdef CORE_CONFIGURE_SPI0
		case kSPI0:
			SPCR = 0;	//&= ~_BV(SPIE);
			break;
	#endif
		default:
			result = -1;
			break;
		}
	
	return result;		
	}

/*
	Set /CS pin for subsequent SPI operations
*/
void coreSPISetChipSelectPin(uint8 spi, controllerPin cs)
	{
	if (spi < kNumberOfSPI)
		{
		spis[spi].pin = cs;
		}
	}

/*
	Set /CS low
*/
void coreSPISelect(uint8 spi)
	{
	if (spi < kNumberOfSPI)
		{
		corePortsClearPortPin(spis[spi].pin.port, spis[spi].pin.pin);
		}
	}

/*
	Set /CS High
*/
void coreSPIRelease(uint8 spi)
	{
	if (spi < kNumberOfSPI)
		{
		corePortsSetPortPin(spis[spi].pin.port, spis[spi].pin.pin);
		}
	}

/*
void coreSPISetBitrate(uint8 spr)
{
	outb(SPCR, (inb(SPCR) & ((1<<SPR0)|(1<<SPR1))) | (spr&((1<<SPR0)|(1<<SPR1)))));
}
*/

void coreSPIWriteData(uint8 spi, const void* p, uint16 n)
	{
	if (spi < kNumberOfSPI)
		{
		byte	data;
	
		corePortsClearPortPin(kPortG, kPortPin1);
		corePortsClearPortPin(kPortG, kPortPin2);
		while (spis[spi].spiTransferRemaining)
			coreControllerIdle();
		corePortsSetPortPin(kPortG, kPortPin1);
		spis[spi].spiTransferData = (const byte*)p;
		spis[spi].spiReceiveData = NULL;
		data = *spis[spi].spiTransferData++;
		spis[spi].spiTransferRemaining = n;
		spis[spi].spiTransmitCounter = 0;
		SPDR = data;

		while (spis[spi].spiTransferRemaining)
			coreControllerIdle();
		corePortsSetPortPin(kPortG, kPortPin2);
		}
	}
/**
	Read <n> Byte from SPI
*/
void coreSPIReadData(uint8 spi, void* p, uint16 n)
	{
	if (spi < kNumberOfSPI)
		{
		while (spis[spi].spiTransferRemaining)
			coreControllerIdle();
		spis[spi].spiTransferData = NULL;
		spis[spi].spiReceiveData = (byte*)p;
		spis[spi].spiTransferRemaining = n;
		spis[spi].spiTransmitCounter = 0;
		SPDR = 0x00;

		while (spis[spi].spiTransferRemaining)
			coreControllerIdle();
		}
	}

/**
	Read/Write <n> Byte from SPI
*/
void coreSPITransferData(uint8 spi, void* dest, const void* src, uint16 n)
	{
	if (spi < kNumberOfSPI)
		{
		byte	data;
	
		while (spis[spi].spiTransferRemaining)
			coreControllerIdle();

		spis[spi].spiTransferData = (byte*)src;
		spis[spi].spiReceiveData = (byte*)dest;
		data = *spis[spi].spiTransferData++;
		spis[spi].spiTransferRemaining = n;
		spis[spi].spiTransmitCounter = 0;
		SPDR = data;

		while (spis[spi].spiTransferRemaining)
			coreControllerIdle();
		}
	}

void coreSPIWriteByte(uint8 spi, uint8 data)
	{
	uint8	result[1];
	
	result[0] = data;
	coreSPIWriteData(spi, result, 1);
	}

uint8 coreSPIReadByte(uint8 spi)
	{
	uint8	result[1];
	
	coreSPIReadData(spi, result, 1);
	return result[0];
	}

uint8 coreSPITransferByte(uint8 spi, uint8 data)
	{
	uint8	result[2];
	
	result[1] = data;
	coreSPITransferData(spi, &result[0], &result[1], 1);
	return result[0];
	}

