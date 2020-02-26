/*--------------------------------------------------------------------------

coreSPI.h

This file is part of e.Development

Interface
low level interface to the ATMega128 SPI controller

$Date: 2016-10-24 09:50:09 +0200 (Mo, 24 Okt 2016) $
$Revision: 984 $

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
	Constants for on-chip SPIS
*/
enum
	{
	kSPI0 = 0,	//!	SPI0
	kSPI1,
	kSPI2,
	kSPI3,
	kSPI4,
	kSPI5,
	kNumberOfSPIS
	};

int8_t coreSPIInitialize(void);
int8_t coreSPIConfigure(uint8_t spi, uint32_t baudrate, uint8_t bits);

void flashOpen(void);
void flashClose(void);

void flashGetID(uint8_t ID[2], uint8_t JEDEC[3]);
void flashGetUniqueID(uint8 id[16]);


void readFlash(void* dest, uint32_t src, uint32_t size);
void writeFlash(uint32_t dest, const void* src, uint32_t size);
void eraseFlash(uint32_t dest, uint32_t size);

#ifdef NO_IGNORE
/**
	Constants for on-chip SPI channels
*/
enum
	{
	kSPI0 = 0,	//!	SPI0
	kNumberOfSPI
	};

// SPI interface initializer
int8 coreSPIInitialize(void);

/** Open uart
*/
int8 coreSPIOpen(uint8 spi, uint32 baudrate);

/** Close uart
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
void coreSPITransferData(uint8 spi, void* dest, void* src, uint16 n);

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
#endif

#ifdef __cplusplus
	}
#endif

#endif

