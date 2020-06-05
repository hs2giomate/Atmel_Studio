/*--------------------------------------------------------------------------

coreSPI.h

This file is part of e.Development

Interface
low level interface to the ATMega128 SPI controller

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2006,2007,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/

#ifndef CORE_SPI_H
#define CORE_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"
#include "corePorts.h"

/**
	Constants for on-chip SPI channels
*/
enum
	{
	kSPI0 = 0,	//!	SPI0
	kNumberOfSPI
	};

enum
	{
	kSPIClock4 = 0,		//!	f_clkio/4
	kSPIClock16 = 1,	//!	f_clkio/16
	kSPIClock64 = 2,	//!	f_clkio/64
	kSPIClock128 = 3,	//!	f_clkio/128
	kSPIClock2 = 4,		//!	f_clkio/2
	kSPIClock8 = 5,		//!	f_clkio/8
	kSPIClock32 = 6,	//!	f_clkio/32
	};

// SPI interface initializer
int8 coreSPIInitialize(void);

/** Open SPI
*/
int8 coreSPIOpen(uint8 spi, uint8 clock);

/** Close SPI
*/
int8 coreSPIClose(uint8 spi);

/*
	Set /CS pin for subsequent SPI operations
*/
void coreSPISetChipSelectPin(uint8 spi, controllerPin cs);
/*
	Set /CS low
*/
void coreSPISelect(uint8 spi);
/*
	Set /CS High
*/
void coreSPIRelease(uint8 spi);
/**
	Write <n> Byte to SPI
*/
void coreSPIWriteData(uint8 spi, const void* p, uint16 n);
/**
	Read <n> Byte from SPI
*/
void coreSPIReadData(uint8 spi, void* p, uint16 n);

/**
	Read/Write <n> Byte from SPI
*/
void coreSPITransferData(uint8 spi, void* dest, const void* src, uint16 n);

/**
	Write one Byte to SPI
*/
void coreSPIWriteByte(uint8 spi, uint8 data);

/**
	Read one Byte from SPI
*/
uint8 coreSPIReadByte(uint8 spi);
/**
	Read/Write one Byte from SPI
*/
uint8 coreSPITransferByte(uint8 spi, uint8 data);

#ifdef __cplusplus
	}
#endif

#endif

