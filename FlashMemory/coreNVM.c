/*--------------------------------------------------------------------------

coreNVM.c

This file is part of e.Development

Implementation
low level interface to NVM

$Date: 2016-09-28 14:13:45 +0200 (Mi, 28 Sep 2016) $
$Revision: 967 $

Copyright (c) 2006,2007,2009,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#include "coreSystemTimer.h"
#include "coreSPI.h"
#include "coreNVM.h"
#include "corePorts.h"
#ifdef NWM_SUPPORT
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

#if defined(MGS_ZST_Revision)
static const int32	nvmCapacities[] = {0x800L, 0x8000L, 0x800L};
static const uint8	nvmSPIs[] = {0, kSPI0, kSPI0};
static const uint32	nvmClocks[] = {0, 5000000UL, 5000000UL};
static uint8	spi;

static const controllerPin	nvmPins[] =
	{
		{0, 0},
		{kPortB, kPortPin30},	//	EEPROM
		{kPortB, kPortPin31}	//	FRAM
	};
#elif defined(ICB_Revision)
static const int32	nvmCapacities[] = {0x800L, 0x800L};
static const uint8	nvmSPIs[] = {0, kSPI0};
static const uint32	nvmClocks[] = {0, 5000000UL};
static uint8	spi;

static const controllerPin	nvmPins[] =
	{
		{0, 0},
		{kPort0, kPortPin16}	//	FRAM
	};
#endif
static int32	coreNVMCopyFromRAM(int32 adr, void* p, int32 n);
static int32	coreNVMCopyToRAM(int32 adr, const void* p, int32 n);

static int32	coreNVMWriteData(int32 adr, const void* p, int32 n);
static int32	coreNVMReadData(int32 adr, void* p, int32 n);
static int8		coreNVMWaitUntilWritten(void);
static int32	coreNVMWriteOneBlock(int32, const void*, int32 n);
	
static void		coreNVMSendInstruction(byte);

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
			nRead = coreNVMCopyFromRAM(adr, p, n);
			break;

		case kNVM1:
		//case kNVM2:
			spi = nvmSPIs[nvm];
			
			coreSPIOpen(spi, nvmClocks[nvm]);
			coreSPISetChipSelectPin(spi, nvmPins[nvm]);
			nRead = coreNVMReadData(adr, p, n);
			coreSPIClose(spi);
			break;

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
			nWritten = coreNVMCopyToRAM(adr, p, n);
			break;

		case kNVM1:
		//case kNVM2:
			spi = nvmSPIs[nvm];
			
			coreSPIOpen(spi, nvmClocks[nvm]);
			coreSPISetChipSelectPin(spi, nvmPins[nvm]);
			nWritten = coreNVMWriteData(adr, p, n);
			coreSPIClose(spi);
			break;

		default:
			break;
		}
	
	return nWritten;
	}

/*
	Copy from RTC RAM to buffer.
	Only double word access (32 bit)!
*/
int32 coreNVMCopyFromRAM(int32 adr, void* p, int32 n)
	{
	uint32*	source = (uint32*)0xE0084000;
	uint8*	destination = (uint8*)p;
	//	uint32*	destination32 = (uint32*)p;
	uint32	value;
	int32	i;

    source += (adr >> 2);

	if (adr & 0x0003)
		{
		value = *source++;
		
		switch (4 - (adr & 0x0003))
			{
			case 3:
				*destination++ = (uint8)((value >> 16) & 0xFF);
			case 2:
				*destination++ = (uint8)((value >> 8) & 0xFF);
			case 1:
				*destination++ = (uint8)(value & 0xFF);
				break;
			}
		
		//	destination32 = (uint32*)destination;
		}

	for (i = 0; i<(n >> 2); i++)
		{
		//	*destination32++ = *source++;
		value = *source++;
		*destination++ = (uint8)((value >> 24) & 0xFF);
		*destination++ = (uint8)((value >> 16) & 0xFF);
		*destination++ = (uint8)((value >> 8) & 0xFF);
		*destination++ = (uint8)(value & 0xFF);
		}


	if ((n + adr) & 0x0003)
		{
		value = *source++;
		
		switch (4 - ((n + adr) & 0x0003))
			{
			case 3:
				*destination++ = (uint8)((value >> 24) & 0xFF);
			case 2:
				*destination++ = (uint8)((value >> 16) & 0xFF);
			case 1:
				*destination++ = (uint8)((value >> 8) & 0xFF);
				break;
			}
		}

	return n;
	}

/*
	Copy from buffer to RTC RAM.
	Only double word access (32 bit)!
*/
int32 coreNVMCopyToRAM(int32 adr, const void* p, int32 n)
	{
	const uint8*	source = (const uint8*)p;
	//	const uint32*	source32 = (const uint32*)p;
	uint32*	destination = (uint32*)0xE0084000;
	uint32	value;
	int32	i;

    destination += (adr >> 2);

	if (adr & 0x0003)
		{
		value = *destination;
		
		switch (4 - (adr & 0x0003))
			{
			case 3:
				value &= 0xFF00FFFF;
				value |= (uint32)*source++ << 16;
			case 2:
				value &= 0xFFFF00FF;
				value |= (uint32)*source++ << 8;
			case 1:
				value &= 0xFFFFFF00;
				value |= (uint32)*source++;
				break;
			}
			
		*destination++ = value;
		//	source32 = (const uint32*)source;
		}

	for (i = 0; i<(n >> 2); i++)
		{
		value = 0;
		value |= (uint32)*source++ << 24;
		value |= (uint32)*source++ << 16;
		value |= (uint32)*source++ << 8;
		value |= (uint32)*source++;
			
		*destination++ = value;
		//	*destination++ = *source32++;
		}

	if ((n + adr) & 0x0003)
		{
		value = *destination;
		
		switch (4 - ((n + adr) & 0x0003))
			{
			case 3:
				value &= 0x00FFFFFF;
				value |= (uint32)*source++ << 24;
			case 2:
				value &= 0x00FF00FF;
				value |= (uint32)*source++ << 16;
			case 1:
				value &= 0x00FFFF00;
				value |= (uint32)*source++ << 8;
				break;
			}
			
		*destination++ = value;
		}

	return n;
	}

/**
	Schreibe <n> Byte an EEPROM-Adresse <adr>
	<adr> muss dabei nicht Page-aligned sein
*/
int32 coreNVMWriteData(int32 adr, const void* p, int32 n)
	{
	int32		nRead = 0;
	int32		result = 0;
    const byte*	sourcePtr = (byte*)p;

	//	controller.asc0 << "coreNVMWriteData\r";
	result = coreNVMWaitUntilWritten();
		
	if (!result)
		{
		while ((result >= 0) && (n > 0))
			{
			coreNVMSendInstruction(WREN);

			result = coreNVMWriteOneBlock((int32)adr, sourcePtr, n);
			
			if (result > 0)
				{
				adr += result;
				sourcePtr += result;
				n -= result;
				nRead += result;
				}
			coreNVMSendInstruction(WRDI);
			}
		}
	
	return nRead;	
	}

int32 coreNVMReadData(int32 adr, void* p, int32 n)
	{
	int32	result = 0;
	byte	hi, lo;
    byte*	targetPtr = (byte*)p;


	hi = (byte)((adr >> 8) & 0x00FF);
	lo = (byte)(adr & 0x00FF);

	result = coreNVMWaitUntilWritten();
		
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


int8 coreNVMWaitUntilWritten(void)
	{
	int32	retry_counter = 0;
	byte	status;
	int32	error = 0;

	// controller.asc0 << "coreNVMWaitUntilWritten\r";
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

int32 coreNVMWriteOneBlock(int32 adr, const void* p, int32 n)
	{
	int32		result;
	int32		toWrite = EEPROM_PAGESIZE;
	byte		hi, lo;
    const byte*	sourcePtr = (byte*)p;
	
	// controller.asc0 << "coreNVMWriteOneBlock\r";
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
	
	result = coreNVMWaitUntilWritten();		//	Warten, bis Daten geschrieben sind

	if (!result)
		result = toWrite;

	return result;	
	}

void coreNVMSendInstruction(byte cmd)
	{
	uint8	i;
	
	// controller.asc0 << "coreNVMSendInstruction\r";
	coreSPISelect(spi);
    coreSPIWriteByte(spi, cmd);
	coreSPIRelease(spi);
	
	for (i=0; i<10; i++)
		;
	}
 #endif
 
