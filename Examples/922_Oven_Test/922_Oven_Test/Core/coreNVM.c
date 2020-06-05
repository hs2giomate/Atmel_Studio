/*--------------------------------------------------------------------------

coreNVM.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to NVM

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

--------------------------------------------------------------------------*/
#include <avr/io.h>
#include "coreController.h"
#include "coreNVM.h"
#ifdef IPECO_PSC_Revision
#include "coreSPI.h"
#include "coreRTC.h"
#endif

/*
	M95320 Properties	
*/
#define EEPROM_PAGESIZE	0x000000020L	//	Pagesize/Byte	
#define EEPROM_PAGEMASK	0x0000FFE0L 	//	Maske, um auf Pagesize auszurichten	
/*
	M95320 Instructions	
*/
#define	WREN  	0x06	//	Write enable	
#define	WRDI  	0x04	//	Write disable
#define	RDSR  	0x05	//	Read Status Register
#define	WRSR  	0x01	//	Write Status Register
#define	READ  	0x03	//	Read from Memory Array
#define	WRITE 	0x02	//	Write to Memory Array

/*
	M95320 Status Bits	
*/
#define	SRWD 	0x80	//	Status Register Write Protect	
#define	BP1  	0x08	//	Block Protect Bits	
#define	BP0  	0x04	//	Block Protect Bits	
#define	WEL		0x02	//	Write Enable Latch Bit	
#define	WIP  	0x01	//	Write in Progress Bit	

#if defined(IPECO_PSC_Revision)
#ifdef RTC_RAM_SUPPORT
static const int32	nvmCapacities[] = {0x1000L, 0x8000L, 0x800L, 0x8};
static const uint8	nvmSPIs[] = {0, kSPI0, kSPI0, 0};
static const uint8	nvmClocks[] = {0, kSPIClock4, kSPIClock2, 0};

static const controllerPin	nvmPins[] =
	{
		{0, 0},
		{kPortB, kPortPin5},
		{kPortC, kPortPin0},
		{0, 0}
	};
#else
static const int32	nvmCapacities[] = {0x1000L, 0x8000L, 0x800L};
static const uint8	nvmSPIs[] = {0, kSPI0, kSPI0};
static const uint8	nvmClocks[] = {0, kSPIClock4, kSPIClock2};

static const controllerPin	nvmPins[] =
	{
		{0, 0},
		{kPortB, kPortPin5},
		{kPortC, kPortPin0},
	};
#endif
#else
static const int32	nvmCapacities[] = {0x1000L};
#endif

static int16	coreNVMWriteDataToInternalEEPROM(uint16 adr, const void* p, int16 n);
static int16	coreNVMReadDataFromInternalEEPROM(uint16 adr, void* p, int16 n);

#ifdef IPECO_PSC_Revision
#ifdef RTC_RAM_SUPPORT
static int32	coreNVMWriteDataI2C(int32 adr, const void* p, int32 n);
static int32	coreNVMReadDataI2C(int32 adr, void* p, int32 n);
#endif
static int32	coreNVMWriteDataSPI(int32 adr, const void* p, int32 n);
static int32	coreNVMReadDataSPI(int32 adr, void* p, int32 n);
static int8		coreNVMWaitUntilWrittenSPI(void);
static int32	coreNVMWriteOneBlockSPI(int32, const void*, int32 n);
	
static void		coreNVMSendInstructionSPI(byte);

static uint8	spi;
#endif

int8 coreNVMInitialize(void)
	{
	return 0;
	}

/** Get NVM capacity
	\param nvm NVM selector
	\return NVM capacity in byte
*/
int32 coreNVMGetNVMCapacity(uint8 nvm)
	{
	int32	result = -1;

	if (nvm < kNumberOfNVMS)
		{
		result = nvmCapacities[nvm];
		}
	
	return result;
	}

int32 coreNVMRead(uint8 nvm, int32 adr, void* p, int32 n)
	{
	int32	nRead = -1;

	switch (nvm)
		{
		case kNVM0:
			nRead = coreNVMReadDataFromInternalEEPROM(adr, p, n);
			break;

	#if defined(IPECO_PSC_Revision)
		case kNVM1:
		case kNVM2:
			spi = nvmSPIs[nvm];
			
			coreSPIOpen(spi, nvmClocks[nvm]);
			coreSPISetChipSelectPin(spi, nvmPins[nvm]);
			nRead = coreNVMReadDataSPI(adr, p, n);
			coreSPIClose(spi);
			break;
	#ifdef RTC_RAM_SUPPORT
		case kNVM3:
			nRead = coreNVMReadDataI2C(adr, p, n);
			break;
	#endif
	#endif

		default:
			break;
		}
	
	return nRead;
	}

int32 coreNVMWrite(uint8 nvm, int32 adr, const void* p, int32 n)
	{
	int32	nWritten = -1;

	switch (nvm)
		{
		case kNVM0:
			coreNVMWriteDataToInternalEEPROM(adr, p, n);
			break;

	#if defined(IPECO_PSC_Revision)
		case kNVM1:
		case kNVM2:
			spi = nvmSPIs[nvm];
			
			coreSPIOpen(spi, nvmClocks[nvm]);
			coreSPISetChipSelectPin(spi, nvmPins[nvm]);
			nWritten = coreNVMWriteDataSPI(adr, p, n);
			coreSPIClose(spi);
			break;
	#ifdef RTC_RAM_SUPPORT
		case kNVM3:
			nWritten = coreNVMWriteDataI2C(adr, p, n);
			break;
	#endif
	#endif

		default:
			break;
		}
	
	return nWritten;
	}

/* Schreibe <n> Byte an EEPROM-Adresse <adr>
*/
int16 coreNVMWriteDataToInternalEEPROM(uint16 adr, const void* p, int16 n)
	{
	int16	i;
	byte*	sourcePtr = (byte*)p;
	
	for (i=0; (i<n) && (adr<0x1000); i++)
		{
		coreControllerServiceWatchdog();
		/* Wait for completion of previous write */
		while(EECR & (1<<EEWE))
			;
		/* Set up address register */
		EEAR = adr;
		/*
		EEARL = (adr & 0x00FF);
		EEARH = (adr >> 8);
		*/
		adr++;
		/* Set data to data register */
		EEDR = *sourcePtr++;
		coreControllerEnterCritical();
		/* Write logical one to EEMWE */
		EECR |= (1<<EEMWE);
		/* Start eeprom write by setting EEWE */
		EECR |= (1<<EEWE);
		coreControllerLeaveCritical();
		}

	return i;
	}

/* Lese <n> Byte von EEPROM-Adresse <adr>
*/
int16 coreNVMReadDataFromInternalEEPROM(uint16 adr, void* p, int16 n)
	{
	int16	i;
	byte*	destPtr = (byte*)p;
	
	for (i=0; (i<n) && (adr<0x1000); i++)
		{
		coreControllerServiceWatchdog();
		/* Wait for completion of previous write */
		while(EECR & (1<<EEWE))
			;
		/* Set up address register */
		EEAR = adr;
		/*
		EEARL = (adr & 0x00FF);
		EEARH = (adr >> 8);
		*/
		adr++;
		/* Start eeprom read by writing EERE */
		EECR |= (1<<EERE);
		/* Return data from data register */
		*destPtr++ = EEDR;
		}

	return i;
	}

#ifdef IPECO_PSC_Revision
#ifdef RTC_RAM_SUPPORT
int32 coreNVMWriteDataI2C(int32 adr, const void* p, int32 n)
	{
	int32		nWritten = 0;
	
	nWritten = coreRTCWriteRAM(adr, p, n);
	return nWritten;	
	}

int32 coreNVMReadDataI2C(int32 adr, void* p, int32 n)
	{
	int32		nRead = 0;
	
	nRead = coreRTCReadRAM(adr, p, n);

	return nRead;	
	}
#endif
/**
	Schreibe <n> Byte an EEPROM-Adresse <adr>
	<adr> muss dabei nicht Page-aligned sein
*/
int32 coreNVMWriteDataSPI(int32 adr, const void* p, int32 n)
	{
	int32		nRead = 0;
	int32		result = 0;
    const byte*	sourcePtr = (byte*)p;

	//	controller.asc0 << "coreNVMWriteDataSPI\r";
	result = coreNVMWaitUntilWrittenSPI();
		
	if (!result)
		{
		while ((result >= 0) && (n > 0))
			{
			coreNVMSendInstructionSPI(WREN);

			result = coreNVMWriteOneBlockSPI((int32)adr, sourcePtr, n);
			
			if (result > 0)
				{
				adr += result;
				sourcePtr += result;
				n -= result;
				nRead += result;
				}
			coreNVMSendInstructionSPI(WRDI);
			}
		}
	
	return nRead;	
	}

int32 coreNVMReadDataSPI(int32 adr, void* p, int32 n)
	{
	int32	result = 0;
	byte	hi, lo;
    byte*	targetPtr = (byte*)p;


	hi = (byte)((adr >> 8) & 0x00FF);
	lo = (byte)(adr & 0x00FF);

	result = coreNVMWaitUntilWrittenSPI();
		
	if (!result)
		{
		coreSPISelect(spi);
		coreSPIWriteByte(spi, READ); 				//	Lesesequenz starten
		coreSPIWriteByte(spi, hi);				//	Adresse High-Byte
		coreSPIWriteByte(spi, lo);				//	Adresse Low-Byte
		coreSPIReadData(spi, targetPtr, n);		//	Daten übertragen
		coreSPIRelease(spi);
		result = n;
		}
	
	return result;
	}


int8 coreNVMWaitUntilWrittenSPI(void)
	{
	int32	retry_counter = 0;
	byte	status;
	int8	error = 0;

	// controller.asc0 << "coreNVMWaitUntilWrittenSPI\r";
	do
		{
		asm("nop");
		coreSPISelect(spi);
	    coreSPIWriteByte(spi, RDSR);
	    //	m95320_delay(5);
		status = coreSPITransferByte(spi, 0);
		coreSPIRelease(spi);

		if (retry_counter++ > 1024)
			error = -1;
		}
	while (!error && (status & WIP));

	return error;
	}

int32 coreNVMWriteOneBlockSPI(int32 adr, const void* p, int32 n)
	{
	int32		result;
	int32		toWrite = EEPROM_PAGESIZE;
	byte		hi, lo;
    const byte*	sourcePtr = (byte*)p;
	
	// controller.asc0 << "coreNVMWriteOneBlockSPI\r";
	hi = (byte)((adr >> 8) & 0x00FF);
	lo = (byte)(adr & 0x00FF);
	
	toWrite -= (adr & ~EEPROM_PAGEMASK); 	//	Wenn Adresse nicht auf page boundary,
												//	dann die ersten Byte überspringen
	if (toWrite > n)							//	Seite nicht bis Ende beschreiben?
		toWrite = n;							//	Nein.
	
	coreSPISelect(spi);

	coreSPIWriteByte(spi, WRITE);				//	Schreibsequenz starten:
	coreSPIWriteByte(spi, hi);				//	Adresse High-Byte
	coreSPIWriteByte(spi, lo);				//	Adresse Low-Byte

	coreSPIWriteData(spi, sourcePtr, toWrite);		//	Daten übertragen

	coreSPIRelease(spi);
	
	result = coreNVMWaitUntilWrittenSPI();		//	Warten, bis Daten geschrieben sind

	if (!result)
		result = toWrite;

	return result;	
	}

void coreNVMSendInstructionSPI(byte cmd)
	{
	uint8	i;
	
	// controller.asc0 << "coreNVMSendInstructionSPI\r";
	coreSPISelect(spi);
    coreSPIWriteByte(spi, cmd);
	coreSPIRelease(spi);
	
	for (i=0; i<10; i++)
		;
	}
#endif
