/*--------------------------------------------------------------------------

coreSPI.c

This file is part of e.Development

Implementation
low level interface to the ATmega 128 SPI controller

$Author: steffen $
$Date: 2016-10-24 09:50:09 +0200 (Mo, 24 Okt 2016) $
$Revision: 984 $
$HeadURL: https://svn.s2embedded.at/customers/hs2-engineering/trunk/HiLo/Firmware/Libraries/Cortex/e.Core/coreSPI.c $

Copyright (c) 2006,2007,2008 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#include "coreSPI.h"
#include "sam.h"
#include "corePorts.h"

#define SERCOM_SPI_CLOCK	8000000UL

#ifndef SERCOM_SPI_CLOCK
#define SERCOM_SPI_CLOCK	F_CPU
#endif


int8_t coreSPIInitialize(void);
int8_t coreSPIConfigure(uint8_t spi, uint32_t baudrate, uint8_t bits);
int8_t coreSPIOpen(uint8_t spi);
int8_t coreSPIClose(uint8_t spi);
int16_t coreSPIRead(uint8_t spi, void* p, int16_t n);
int16_t coreSPIWriteAsync(uint8_t spi, const void* p, int16_t n);
int16_t coreSPIWrite(uint8_t spi, const void* p, int16_t n);
int16_t coreSPITransferAsync(uint8_t spi, void* dest, const void* src, int16_t n);
int16_t coreSPITransferData(uint8_t spi, void* dest, const void* src, int16_t n);

#define kFlashRead			0x03
#define kFlashHighSpeedRead	0x0B
#define kFlash4KSectorErase	0x20
#define kFlash32KBlockErase	0x52
#define kFlash64KBlockErase	0xD8
#define kFlashChipErase		0x60
#define	kFlashByteProgram	0x02
#define	kFlashWordProgram	0xAD
#define kFlashReadSR		0x05
#define kFlashEnableWriteSR	0x50
#define kFlashWriteSR		0x01
#define kFlashEnableWrite	0x06
#define kFlashDisableWrite	0x04
#define kFlashReadID		0x90
#define kFlashReadJEDECID	0x9F
#define kFlashEnableSOBusy	0x70
#define kFlashDisableSOBusy	0x80

//8.28 READ UNIQUE ID NUMBER (RDUID, 4Bh)
#define kFlashReadUniqueID	0x4B

void CE_Low_Flash(void)
	{
	PORT->Group[0].OUTCLR.reg = (1UL << 27);
	}

void CE_High_Flash(void)
	{
	PORT->Group[0].OUTSET.reg = (1UL << 27);
	}

void CE_Low_FRAM(void)
	{
	PORT->Group[0].OUTCLR.reg = (1UL << 28);
	}

void CE_High_FRAM(void)
	{
	PORT->Group[0].OUTSET.reg = (1UL << 28);
	}

void Send_Byte(uint8_t byte)
	{
	coreSPIWrite(kSPI0, &byte, 1);
	}

uint8_t Get_Byte(void)
	{
	uint8_t byte;

	coreSPIRead(kSPI0, &byte, 1);
	return byte;
	}

void WREN(void)
	{
	CE_Low_Flash();			// enable device
	Send_Byte(0x06);	// send WREN command
	CE_High_Flash();			// disable device
	}
	void WREN_FRAM(void)
	{
		CE_Low_FRAM();			// enable device
		Send_Byte(0x06);	// send WREN command
		CE_High_FRAM();			// disable device
	}

void WRDI(void)
	{
	CE_Low_Flash();			// enable device
	Send_Byte(0x04);	// send WRDI command
	CE_High_Flash();			// disable device
	}
void WRDI_FRAM(void)
{
	CE_Low_FRAM();			// enable device
	Send_Byte(0x04);	// send WRDI command
	CE_High_FRAM();			// disable device
}

uint8_t ReadStatusRegister(void)
	{
	uint8_t byte = 0;
	CE_Low_Flash();			// enable device
	Send_Byte(0x05);	// send RDSR command
	byte = Get_Byte();		/* receive byte */
	CE_High_Flash();			// disable device
	return byte;
	}

/************************************************************************/
/* PROCEDURE: Chip_Erase						*/
/*									*/
/* This procedure erases the entire Chip.				*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/************************************************************************/
void Chip_Erase(void)
	{						
	CE_Low_Flash();				// enable device
	Send_Byte(0x60);		// send Chip Erase command (60h or C7h) */
	CE_High_Flash();				// disable device
	}

/************************************************************************/
/* PROCEDURE: Sector_Erase						*/
/*									*/
/* This procedure Sector Erases the Chip.				*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/************************************************************************/
void Sector_Erase(uint32_t Dst)
	{
	CE_Low_Flash();				// enable device
	Send_Byte(0x20);		// send Sector Erase command
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); // send 3 address bytes
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High_Flash();				// disable device
	}

/************************************************************************/
/* PROCEDURE: Block_Erase_32K						*/
/*									*/
/* This procedure Block Erases 32 KByte of the Chip.			*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/************************************************************************/
void Block_Erase_32K(uint32_t Dst)
	{
	CE_Low_Flash();				// enable device
	Send_Byte(0x52);		// send 32 KByte Block Erase command
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); // send 3 address bytes
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High_Flash();				// disable device
	}

/************************************************************************/
/* PROCEDURE: Block_Erase_64K						*/
/*									*/
/* This procedure Block Erases 64 KByte of the Chip.			*/
/*									*/
/* Input:								*/
/*		Dst:		Destination Address 000000H - 1FFFFFH	*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/************************************************************************/
void Block_Erase_64K(uint32_t Dst)
	{
	CE_Low_Flash();				// enable device
	Send_Byte(0xD8);		// send 64KByte Block Erase command
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); // send 3 address bytes
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	CE_High_Flash();				// disable device
	}

/************************************************************************/
/* PROCEDURE: WaitBusy							*/
/*									*/
/* This procedure waits until device is no longer busy (can be used by	*/
/* Byte-Program, Sector-Erase, Block-Erase, Chip-Erase).		*/
/*									*/
/* Input:								*/
/*		None							*/
/*									*/
/* Returns:								*/
/*		Nothing							*/
/************************************************************************/
void WaitBusy(void)
	{
	uint8_t	status = ReadStatusRegister();
	
	while ((status & 0x03) == 0x03)	// waste time until not busy
		status = ReadStatusRegister();
	}

void AAIProgrammingA(uint32_t Dst, uint8_t byte1, uint8_t byte2)
	{
	CE_Low_Flash();				// enable device
	Send_Byte(0xAD);		// send AAI command
	Send_Byte(((Dst & 0xFFFFFF) >> 16)); // send 3 address bytes
	Send_Byte(((Dst & 0xFFFF) >> 8));
	Send_Byte(Dst & 0xFF);
	Send_Byte(byte1);		// send 1st byte to be programmed	
	Send_Byte(byte2);		// send 2nd byte to be programmed
	CE_High_Flash();				// disable device
	}

void AAIProgrammingB(uint8_t byte1, uint8_t byte2)
	{
	CE_Low_Flash();				// enable device
	Send_Byte(0xAD);		// send AAI command
	Send_Byte(byte1);		// send 1st byte to be programmed
	Send_Byte(byte2);		// send 2nd byte to be programmed
	CE_High_Flash();				// disable device
	}

void WaitBusyAAI(void)
	{
	uint8_t	status = ReadStatusRegister();
	
	while ((status & 0x43) == 0x43)	// waste time until not busy
		status = ReadStatusRegister();
	}

void flashOpen(void)
	{
	coreSPIOpen(kSPI0);
	}

void flashClose(void)
	{
	coreSPIClose(kSPI0);
	}

void flashGetID(uint8_t ID[2], uint8_t JEDEC[3])
	{
	CE_Low_Flash();
	Send_Byte(kFlashReadID);
	Send_Byte(0);
	Send_Byte(0);
	Send_Byte(0);
	ID[0] = Get_Byte();
	ID[1] = Get_Byte();
	coreSPIRead(kSPI0, ID, 2);
	CE_High_Flash();
	CE_Low_Flash();
	Send_Byte(kFlashReadJEDECID);
	JEDEC[0] = Get_Byte();
	JEDEC[1] = Get_Byte();
	JEDEC[2] = Get_Byte();
	CE_High_Flash();
	}

void flashGetUniqueID(uint8_t id[16])
	{
	uint8	i;
		
	CE_Low_Flash();
	Send_Byte(kFlashReadUniqueID);
	Send_Byte(0);
	Send_Byte(0);
	Send_Byte(0);
	Send_Byte(0);
	
	for (i=0; i<16; i++)
		id[i] = Get_Byte();
	CE_High_Flash();
	}

void readFlash(void* dest, uint32_t src, uint32_t size)
	{
	CE_Low_Flash();
	Send_Byte(kFlashRead);
	//	send 3 address bytes
	Send_Byte(((src & 0xFFFFFF) >> 16));
	Send_Byte(((src & 0xFFFF) >> 8));
	Send_Byte(src & 0xFF);
	coreSPIRead(kSPI0, dest, size);
	CE_High_Flash();
	}
	void readFRAM(void* dest, uint32_t src, uint32_t size)
	{
		CE_Low_FRAM();
		Send_Byte(kFlashRead);
		//	send 3 address bytes
		Send_Byte(((src & 0xFFFFFF) >> 16));
		Send_Byte(((src & 0xFFFF) >> 8));
		Send_Byte(src & 0xFF);
		coreSPIRead(kSPI0, dest, size);
		CE_High_FRAM();
	}

void writeFlash(uint32_t dest, const void* src, uint32_t size)
	{
	const uint8*	srcPtr = (const uint8*)src;
		
	while (size > 0)
		{
		uint16	i;
		
		WREN();
		CE_Low_Flash();
		Send_Byte(0x02);
		Send_Byte(((dest & 0xFFFFFF) >> 16));
		Send_Byte(((dest & 0xFFFF) >> 8));
		Send_Byte(dest & 0xFF);
	
		for (i=0; (i < 256) && (i<size); i++)
			Send_Byte(*srcPtr++);
		CE_High_Flash();
		WRDI();
		size -= i;
		dest += i;
		}
		
/*
	uint32_t	i;
	uint8_t*	srcPtr = (uint8_t*)src;
	uint8_t		loByte, hiByte;
	uint8_t		status;

	status = ReadStatusRegister();
	if (status & 0x1C)
		{
		CE_Low_Flash();
		Send_Byte(kFlashEnableWriteSR);
		CE_High_Flash();
		CE_Low_Flash();
		Send_Byte(kFlashWriteSR);
		Send_Byte(0);
		CE_High_Flash();
		}
	
	hiByte = *srcPtr++;
	loByte = *srcPtr++;
	WREN();
	AAIProgrammingA(dest, hiByte, loByte);
	WaitBusyAAI();

	for (i=1; i<size/2; i++)
		{
		hiByte = *srcPtr++;
		loByte = *srcPtr++;
		AAIProgrammingB(hiByte, loByte);
		WaitBusyAAI();
		}

	WRDI();
*/
	}
	void writeFRAM(uint32_t dest, const void* src, uint32_t size)
	{
	const uint8*	srcPtr = (const uint8*)src;
		
	while (size > 0)
		{
		uint16	i;
		
		WREN();
		CE_Low_FRAM();
		Send_Byte(0x02);
		Send_Byte(((dest & 0xFFFFFF) >> 16));
		Send_Byte(((dest & 0xFFFF) >> 8));
		Send_Byte(dest & 0xFF);
	
		for (i=0; (i < 256) && (i<size); i++)
			Send_Byte(*srcPtr++);
		CE_High_FRAM();
		WRDI();
		size -= i;
		dest += i;
		}
		
/*
	uint32_t	i;
	uint8_t*	srcPtr = (uint8_t*)src;
	uint8_t		loByte, hiByte;
	uint8_t		status;

	status = ReadStatusRegister();
	if (status & 0x1C)
		{
		CE_Low_Flash();
		Send_Byte(kFlashEnableWriteSR);
		CE_High_Flash();
		CE_Low_Flash();
		Send_Byte(kFlashWriteSR);
		Send_Byte(0);
		CE_High_Flash();
		}
	
	hiByte = *srcPtr++;
	loByte = *srcPtr++;
	WREN();
	AAIProgrammingA(dest, hiByte, loByte);
	WaitBusyAAI();

	for (i=1; i<size/2; i++)
		{
		hiByte = *srcPtr++;
		loByte = *srcPtr++;
		AAIProgrammingB(hiByte, loByte);
		WaitBusyAAI();
		}

	WRDI();
*/
	}

void eraseFlash(uint32_t dest, uint32_t size)
	{
	uint32_t	eraseRemaining = size;
	uint32_t	eraseAddress = dest;

	while (eraseRemaining > 0)
		{
		WREN();
		if (eraseRemaining <= 4 * 1024)
			{
			Sector_Erase(eraseAddress);
			eraseRemaining = 0;
			}
		else
			{
			Sector_Erase(eraseAddress);
			eraseAddress += (4 * 1024);
			eraseRemaining -= (4 * 1024);
			}
/*
		else if (eraseRemaining >= 64 * 1024)
			{
			Block_Erase_64K(eraseAddress);
			eraseAddress += (64 * 1024);
			eraseRemaining -= (64 * 1024);
			}
		else if (eraseRemaining >= 32 * 1024)
			{
			Block_Erase_32K(eraseAddress);
			eraseAddress += (32 * 1024);
			eraseRemaining -= (32 * 1024);
			}
*/
		WaitBusy();
		WRDI();
		}
	}

#define CORE_CONFIGURE_SERCOM0
//#define CORE_CONFIGURE_SERCOM1

typedef struct
{
	volatile int16_t	spiDataReceivePointer;
	int16_t				spiReceiveBufferSize;
	uint8_t*			spiReceiveBufferPointer;

	volatile int16_t	spiDataRemaining;
	const uint8_t*	spiTransmitBuffer;
	
	Sercom*			SERCOM;
	int32_t			pads;
	IRQn_Type		irq;
	uint8_t			saved_INTFLAG;
	bool			spiTransmissionPending;
	//uint32_t	powerConfiguration;
} spiData;

enum spi_pad_settings {
	SPI_MISO_PAD0_MOSI_PAD2_SCK_PAD3 = SERCOM_SPI_CTRLA_DIPO(0x00)|SERCOM_SPI_CTRLA_DOPO(0x01),
	SPI_MISO_PAD0_MOSI_PAD3_SCK_PAD1 = SERCOM_SPI_CTRLA_DIPO(0x00)|SERCOM_SPI_CTRLA_DOPO(0x02),

	SPI_MISO_PAD1_MOSI_PAD2_SCK_PAD3 = SERCOM_SPI_CTRLA_DIPO(0x01)|SERCOM_SPI_CTRLA_DOPO(0x01),
	SPI_MISO_PAD1_MOSI_PAD0_SCK_PAD3 = SERCOM_SPI_CTRLA_DIPO(0x01)|SERCOM_SPI_CTRLA_DOPO(0x03),

	SPI_MISO_PAD2_MOSI_PAD0_SCK_PAD1 = SERCOM_SPI_CTRLA_DIPO(0x02)|SERCOM_SPI_CTRLA_DOPO(0x00),
	SPI_MISO_PAD2_MOSI_PAD3_SCK_PAD1 = SERCOM_SPI_CTRLA_DIPO(0x02)|SERCOM_SPI_CTRLA_DOPO(0x02),
	SPI_MISO_PAD2_MOSI_PAD0_SCK_PAD3 = SERCOM_SPI_CTRLA_DIPO(0x02)|SERCOM_SPI_CTRLA_DOPO(0x03),

	SPI_MISO_PAD3_MOSI_PAD0_SCK_PAD1 = SERCOM_SPI_CTRLA_DIPO(0x03)|SERCOM_SPI_CTRLA_DOPO(0x00),
};

static volatile spiData	spis[] =
{
	#ifdef CORE_CONFIGURE_SERCOM0
	{0, 0, NULL, 0, NULL, SERCOM0, SPI_MISO_PAD1_MOSI_PAD2_SCK_PAD3, SERCOM0_IRQn},
	#else
	{0, 0, NULL, 0, NULL, SERCOM0},
	#endif
	

	#ifdef CORE_CONFIGURE_SERCOM1
	{0, 0, NULL, 0, NULL, SERCOM1, SERCOM_SPI_CTRLA_DIPO(0x01)|SERCOM_SPI_CTRLA_DOPO(0x01), SERCOM1_IRQn},
	#else
	{0, 0, NULL, 0, NULL, SERCOM1},
	#endif
	#ifdef CORE_CONFIGURE_SERCOM2
	{0, 0, NULL, 0, NULL, SERCOM2, UART_RX_PAD3_TX_PAD2, SERCOM2_IRQn},
	#else
	{0, 0, NULL, 0, NULL, SERCOM2},
	#endif
	#ifdef CORE_CONFIGURE_SERCOM3
	{0, 0, NULL, 0, NULL, SERCOM3, UART_RX_PAD3_TX_PAD2, SERCOM3_IRQn},
	#else
	{0, 0, NULL, 0, NULL, SERCOM3},
	#endif
	#ifdef CORE_CONFIGURE_SERCOM4
	{0, 0, NULL, 0, NULL, SERCOM4, UART_RX_PAD3_TX_PAD2, SERCOM4_IRQn},
	#else
	{0, 0, NULL, 0, NULL, SERCOM4},
	#endif
	#ifdef CORE_CONFIGURE_SERCOM5
	{0, 0, NULL, 0, NULL, SERCOM5, UART_RX_PAD2_TX_PAD0, SERCOM5_IRQn},
	#else
	{0, 0, NULL, 0, NULL, SERCOM5},
	#endif
};

static void SERCOM_Handler(volatile spiData* spi);

static void SERCOM_Handler(volatile spiData* spi)
	{
	//	Read all flags
	spi->saved_INTFLAG = spi->SERCOM->SPI.INTFLAG.reg;

	if (spi->SERCOM->SPI.INTFLAG.bit.RXC)
		{
		uint16_t	ch;
		
		ch = spi->SERCOM->SPI.DATA.reg;
		if (spi->spiReceiveBufferPointer)
			{
			//	Receive Data Available
			if (spi->spiDataReceivePointer < spi->spiReceiveBufferSize)
				spi->spiReceiveBufferPointer[spi->spiDataReceivePointer++] = ch;
			}
		spi->spiTransmissionPending = (spi->spiDataRemaining > 0);
		}
	
	if (spi->SERCOM->SPI.INTFLAG.bit.TXC)
		{
		if (spi->spiDataRemaining > 0)
			{
			if (spi->spiTransmitBuffer)
				spi->SERCOM->SPI.DATA.reg = *spi->spiTransmitBuffer++;
			else
				spi->SERCOM->SPI.DATA.reg = 0x1FF;
			spi->spiDataRemaining--;
			}
		else
			spi->spiTransmissionPending = false;
		}
	
	//	Clear all flags
	spi->SERCOM->SPI.INTFLAG.reg = spi->saved_INTFLAG;
	}

#ifdef CORE_CONFIGURE_SERCOM0
void SERCOM0_Handler(void)
{
	SERCOM_Handler(&spis[0]);
}
#endif

#ifdef CORE_CONFIGURE_SERCOM1
void SERCOM1_Handler(void)
{
	SERCOM_Handler(&spis[1]);
}
#endif

#ifdef CORE_CONFIGURE_SERCOM2
void SERCOM2_Handler(void)
{
	SERCOM_Handler(&spis[2]);
}
#endif

#ifdef CORE_CONFIGURE_SERCOM3
void SERCOM3_Handler(void)
{
	SERCOM_Handler(&spis[3]);
}
#endif

#ifdef CORE_CONFIGURE_SERCOM4
void SERCOM4_Handler(void)
{
	SERCOM_Handler(&spis[4]);
}
#endif

#ifdef CORE_CONFIGURE_SERCOM5
void SERCOM5_Handler(void)
{
	SERCOM_Handler(&spis[5]);
}
#endif

/** Initialize async serial subsystem
*/
int8_t coreSPIInitialize(void)
	{
	int8_t	result = 0;
	uint8_t	i;

	for (i=0; i<kNumberOfSPIS; i++)
		{
		NVIC_DisableIRQ((IRQn_Type)spis[i].irq);
		NVIC_ClearPendingIRQ((IRQn_Type)spis[i].irq);
		}

#ifdef CORE_CONFIGURE_SERCOM0
	/* Enable & configure alternate function C */
	PM->APBCMASK.reg |= (1u << 2);
	GCLK->CLKCTRL.reg = 0x4313;	// SERCOMx SLOW 0x13
	GCLK->CLKCTRL.reg = 0x4314;	// SERCOM0 0x14
	PORT->Group[0].WRCONFIG.reg = 0x5201E000;	// SERCOM0 PA09, PA10 & PA11
	//	Setup PA09 MISO
	PORT->Group[0].DIRCLR.reg = (1 << 9);
	PORT->Group[0].WRCONFIG.reg = 0x40020200;	//	PA09 as input
	PORT->Group[0].WRCONFIG.reg = 0x52010200;	//	PA09 MUX C
	//	Setup PA10 MOSI
	PORT->Group[0].OUTCLR.reg = (1UL << 10);
	PORT->Group[0].DIRSET.reg = (1UL << 10);
	PORT->Group[0].WRCONFIG.reg = 0x52010400;	//	PA10 MUX C
	//	Setup PA11 SCK
	PORT->Group[0].OUTCLR.reg = (1UL << 11);
	PORT->Group[0].DIRSET.reg = (1UL << 11);
	PORT->Group[0].WRCONFIG.reg = 0x52010800;	//	PA11 MUX C
#endif
			
#ifdef CORE_CONFIGURE_SERCOM1
	/* Enable & configure alternate function C */
	PM->APBCMASK.reg |= (1u << 3);
	GCLK->CLKCTRL.reg = 0x4015;	// SERCOM1 0x15
	//	Setup PA17 MISO
	PORT->Group[0].DIRCLR.reg = (1 << 17);
	PORT->Group[0].WRCONFIG.reg = 0xC0020002;	//	PA17 as input
	PORT->Group[0].WRCONFIG.reg = 0xD2010002;	//	PA17 MUX C
	//	Setup PA18 MOSI
	PORT->Group[0].OUTCLR.reg = (1UL << 18);
	PORT->Group[0].DIRSET.reg = (1UL << 18);
	PORT->Group[0].WRCONFIG.reg = 0xD2010004;	//	PA18 MUX C
	//	Setup PA19 SCK
	PORT->Group[0].OUTCLR.reg = (1UL << 19);
	PORT->Group[0].DIRSET.reg = (1UL << 19);
	PORT->Group[0].WRCONFIG.reg = 0xD2010008;	//	PA19 MUX C
#endif
			
#ifdef CORE_CONFIGURE_SERCOM2
#endif
			
#ifdef CORE_CONFIGURE_SERCOM3
#endif
			
#ifdef CORE_CONFIGURE_SERCOM4
#endif
			
#ifdef CORE_CONFIGURE_SERCOM5
#endif
	
	return result;
	}

/** Configure spi
*/
int8_t coreSPIConfigure(uint8_t spi, uint32_t baudrate, uint8_t bits)
	{
	int8_t	result = 0;

	if (spi < kNumberOfSPIS)
		{
		/* Disable the SERCOM UART module */
		spis[spi].SERCOM->SPI.CTRLA.bit.ENABLE = 0;
		/* Wait for synchronization */
		while(spis[spi].SERCOM->SPI.SYNCBUSY.bit.ENABLE)
			;

		/* Perform a software reset */
		//spis[spi].SERCOM->SPI.CTRLA.bit.SWRST = 1;
		/* Wait for synchronization */
		while (spis[spi].SERCOM->SPI.SYNCBUSY.bit.SWRST || spis[spi].SERCOM->SPI.SYNCBUSY.bit.ENABLE)
			;
	
		/* Update the SPI pad settings, mode and data order settings */
		//spis[spi].SERCOM->SPI.CTRLA.reg = 0x11000C;	// spis[spi].pads | SERCOM_SPI_CTRLA_MODE(0x3);
		spis[spi].SERCOM->SPI.CTRLA.reg = spis[spi].pads | SERCOM_SPI_CTRLA_MODE(0x3);
		/* Enable transmit and receive and set data size to 8 bits */
		spis[spi].SERCOM->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN | SERCOM_SPI_CTRLB_CHSIZE(0);
		/* Wait for synchronization */
		while(spis[spi].SERCOM->SPI.SYNCBUSY.bit.CTRLB)
			;
		/* Load the baud value */
		spis[spi].SERCOM->SPI.BAUD.reg = SERCOM_SPI_CLOCK/(2 * baudrate) - 1;
		}
	else
		result = -1;

	return result;
	}

/** Open spi
*/
int8_t coreSPIOpen(uint8_t spi)
	{
	int8_t	result = 0;

	if (spi < kNumberOfSPIS)
		{
		/* Enable SERCOM UART */
		spis[spi].SERCOM->SPI.CTRLA.bit.ENABLE = 1;
		/* Wait for synchronization */
		while(spis[spi].SERCOM->SPI.SYNCBUSY.bit.ENABLE)
			;

		NVIC_ClearPendingIRQ((IRQn_Type)spis[spi].irq);
		NVIC_EnableIRQ((IRQn_Type)spis[spi].irq);

		spis[spi].SERCOM->SPI.INTENSET.bit.RXC = 1;
		spis[spi].SERCOM->SPI.INTENSET.bit.TXC = 1;
		}
	else
		result = -1;
		
	return result;
	}

/** Close spi
*/
int8_t coreSPIClose(uint8_t spi)
	{
	int8_t	result = 0;

	if (spi < kNumberOfSPIS)
		{
		spis[spi].SERCOM->SPI.INTENCLR.bit.RXC = 1;
		spis[spi].SERCOM->SPI.INTENCLR.bit.TXC = 1;

		NVIC_DisableIRQ((IRQn_Type)spis[spi].irq);
		NVIC_ClearPendingIRQ((IRQn_Type)spis[spi].irq);

		/* Disable the SERCOM UART module */
		spis[spi].SERCOM->SPI.CTRLA.bit.ENABLE = 0;
		/* Wait for synchronization */
		while(spis[spi].SERCOM->SPI.SYNCBUSY.bit.ENABLE)
			;
		}
	else
		result = -1;
	
	return result;
	}

void coreSPIReset(uint8_t spi)
	{
	}

int16_t coreSPIReadAsync(uint8_t spi, void* p, int16_t n)
	{
	if ((n> 0) && (spi < kNumberOfSPIS))
		{
		while (spis[spi].spiTransmissionPending)
			;//coreControllerIdle();

		spis[spi].spiDataRemaining = n - 1;
		spis[spi].spiTransmitBuffer = NULL;
		spis[spi].spiReceiveBufferPointer = (uint8_t*)p;
		spis[spi].spiReceiveBufferSize = n;
		spis[spi].spiDataReceivePointer = 0;
		spis[spi].spiTransmissionPending = true;
		spis[spi].SERCOM->SPI.DATA.reg = 0x1FF;
		}

	return n;
	}

int16_t coreSPIRead(uint8_t spi, void* p, int16_t n)
	{
	if (spi < kNumberOfSPIS)
		{
		coreSPIReadAsync(spi, p, n);

		while (spis[spi].spiDataRemaining)
			;//coreControllerIdle();

		while (spis[spi].spiTransmissionPending)
			;//coreControllerIdle();
		}
	
	return n;
	}

int16_t coreSPIWriteAsync(uint8_t spi, const void* p, int16_t n)
	{
	if ((n> 0) && (spi < kNumberOfSPIS))
		{
		while (spis[spi].spiTransmissionPending)
			;//coreControllerIdle();

		spis[spi].spiDataRemaining = (n - 1);
		spis[spi].spiTransmitBuffer = (const uint8_t*)p;
		spis[spi].spiTransmissionPending = true;
		spis[spi].spiReceiveBufferPointer = NULL;
		spis[spi].spiReceiveBufferSize = n;
		spis[spi].spiDataReceivePointer = 0;
		spis[spi].SERCOM->SPI.DATA.reg = *spis[spi].spiTransmitBuffer++;
		}

	return n;
	}

int16_t coreSPIWrite(uint8_t spi, const void* p, int16_t n)
	{
	if (spi < kNumberOfSPIS)
		{
		coreSPIWriteAsync(spi, p, n);

		while (spis[spi].spiDataRemaining)
			;//coreControllerIdle();

		while (spis[spi].spiTransmissionPending)
			;//coreControllerIdle();
		}

	return n;
	}

int16_t coreSPITransferAsync(uint8_t spi, void* dest, const void* src, int16_t n)
	{
	if ((n> 0) && (spi < kNumberOfSPIS))
		{
		while (spis[spi].spiTransmissionPending)
			;//coreControllerIdle();

		spis[spi].spiDataRemaining = (n - 1);
		spis[spi].spiTransmitBuffer = (const uint8_t*)src;
		spis[spi].spiReceiveBufferPointer = (uint8_t*)dest;
		spis[spi].spiReceiveBufferSize = n;
		spis[spi].spiDataReceivePointer = 0;
		spis[spi].spiTransmissionPending = true;
		spis[spi].SERCOM->SPI.DATA.reg = *spis[spi].spiTransmitBuffer++;
		}

	return n;
	}

int16_t coreSPITransferData(uint8_t spi, void* dest, const void* src, int16_t n)
	{
	if (spi < kNumberOfSPIS)
		{
		coreSPITransferAsync(spi, dest, src, n);

		while (spis[spi].spiDataRemaining)
			;//coreControllerIdle();

		while (spis[spi].spiTransmissionPending)
			;//coreControllerIdle();
		}

	return n;
	}





#ifdef NO_IGNORE

#ifdef SPI_SUPPORT

#include "lpc24xx.h"
#include "coreController.h"
#include "coreInterrupts.h"
#include "coreSPI.h"

#if MGS_ZST_Revision >= 0x0500
#define CORE_CONFIGURE_SPI0
#elif ICB_Revision >= 0x0100
#define CORE_CONFIGURE_SPI0
#endif

#define ABRT		1 << 3		/* SPI0 interrupt status */
#define MODF		1 << 4
#define ROVR		1 << 5
#define WCOL		1 << 6
#define SPIF		1 << 7

typedef struct
	{
	uint16			status;
	uint16			spiTransmitCounter;
	uint16			spiTransferRemaining;
	const byte*		spiTransferData;
	byte*			spiReceiveData;
	controllerPin	pin;
	} spiData;

volatile spiData	spis[kNumberOfSPI];

int8 coreSPIInitializeChannel(uint8 spi);

#ifdef CORE_CONFIGURE_SPI0
void SPI0Handler(void)
	{
	uint32	regValue;
	
	S0SPINT = 1;	//	clear interrupt flag
	
	//  IENABLE;	// handles nested interrupt

  	regValue = S0SPSR;

	if (regValue & SPIF)
		{
		spis[0].status = 1;
		spis[0].spiTransmitCounter++;
		}
	//	IDISABLE;

/*	if (spis[0].spiReceiveData)
		*spis[0].spiReceiveData++ = S0SPDR;
	if (--spis[0].spiTransferRemaining)
		{
		if (spis[0].spiTransferData)
			S0SPDR = *spis[0].spiTransferData++;
		else
			S0SPDR = 0x00;
		}
	else
		spis[0].spiTransferData = NULL;
*/
	}
#endif

/** Initialize SPI subsystem
*/
int8 coreSPIInitialize(void)
	{
	int8	result = 0;
	uint8	i;
	
	for (i = 0;	i<kNumberOfSPI; i++)
		{
		coreSPIInitializeChannel(i);
		}
	
#ifdef CORE_CONFIGURE_SPI0
	PINSEL0 |= 0xC0000000;		//	Enable SCL pin
	PINSEL1 |= 0x0000003C;		//	Enable MOSI/MISO pins
	PCLKSEL0 &= ~(1 << 17);		//	PCLK_SPI = F_CPU
	PCLKSEL0 |= (1 << 16);		//	PCLK_SPI = F_CPU
#endif
	return result;
	}

int8 coreSPIInitializeChannel(uint8 spi)
	{
	int8	result = 0;
	
	if (spi < kNumberOfSPI)
		{
		spis[spi].spiTransferRemaining = 0;
		spis[spi].spiTransferData = NULL;
		spis[spi].pin.port = kPort0;
		spis[spi].pin.pin = kPortPin0;
		}
	else
		result = -1;
	
	return result;
	}

int8 coreSPIOpen(uint8 spi, uint32 baudrate)
	{
	int8	result = 0;
 	uint32	Fdiv;

	coreSPIInitializeChannel(spi);
	
	switch (spi)
		{
	#ifdef CORE_CONFIGURE_SPI0
		case kSPI0:
		#ifdef CPU_48MHZ
			Fdiv = 48000000UL/baudrate;
		#elif defined(CPU_72MHZ)
			Fdiv = 72000000UL/baudrate;
		#else
			Fdiv = CCLK/baudrate;
		#endif
			if (Fdiv < 8)
				Fdiv = 8;

	   		PCONP |= (1 << 8);			//	Power on SPI0
    		S0SPCR = (1 << 5);			//	Master
    		//S0SPCCR = 72;				//	SCLK == 1 MHz
    		S0SPCCR = Fdiv;	//18;				//	SCLK == 1 MHz
			/*
			if (coreInterruptsInstallHandler(kSPI0Interrupt, kHighestPriority, SPI0Handler))
				{
				S0SPCR |= (1 << 7);	//	Enable SPI interrupt
				}
			*/
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
	#ifdef CORE_CONFIGURE_UART0
		case kUART0:
			S0SPCR &= ~(1 << 7);	//	Disable SPI interrupt
    		PCONP &= ~(1 << 8);		//	Power off SPI0
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
/**
	Write <n> Byte to SPI
*/
void coreSPIWriteData(uint8 spi, const void* p, uint16 n)
	{
	if (spi < kNumberOfSPI)
		{
		uint16	i;
		byte	data;
		const byte*	pData = ((const byte*)p);
		
		for (i=0; i<n; i++)
			{
			S0SPDR = *pData++;
			while (!(S0SPSR & SPIF))
				;
			data = S0SPDR;
			}
/*	
		while(spis[spi].spiTransferRemaining)
			coreControllerIdle();

	    spis[spi].spiTransferData = (const byte*)p;
	    data = *spis[spi].spiTransferData++;
		spis[spi].spiTransferRemaining = n;

		S0SPDR = data;
		while(spis[spi].spiTransferRemaining)
			coreControllerIdle();
*/		
		}
	}
/**
	Read <n> Byte from SPI
*/
void coreSPIReadData(uint8 spi, void* p, uint16 n)
	{
	if (spi < kNumberOfSPI)
		{
		uint16	i;
		byte*	pData = ((byte*)p);
		
		for (i=0; i<n; i++)
			{
			S0SPDR = 0xFF;
			while (!(S0SPSR & SPIF))
				;
			*pData++ = S0SPDR;
			}
/*	
		while(spis[spi].spiTransferRemaining)
			coreControllerIdle();

		spis[spi].spiReceiveData = (byte*)p;
		spis[spi].spiTransferRemaining = n;

		S0SPDR = 0;
		while(spis[spi].spiTransferRemaining)
			coreControllerIdle();
*/
		}
	}

/**
	Read/Write <n> Byte from SPI
*/
void coreSPITransferData(uint8 spi, void* dest, void* src, uint16 n)
	{
	if (spi < kNumberOfSPI)
		{
		uint16	i;
		byte*	destData = ((byte*)dest);
		byte*	srcData = ((byte*)src);
		
		for (i=0; i<n; i++)
			{
			S0SPDR = *srcData++;
			while (!(S0SPSR & SPIF))
				;
			*destData++ = S0SPDR;
			}
/*	
		byte	data;
	
		while(spis[spi].spiTransferRemaining)
			coreControllerIdle();

		spis[spi].spiReceiveData = (byte*)dest;
	    spis[spi].spiTransferData = (const byte*)src;
		spis[spi].spiTransferRemaining = n;
	    data = *spis[spi].spiTransferData++;

		S0SPDR = data;
		while(spis[spi].spiTransferRemaining)
			coreControllerIdle();
*/
		}
	}

void coreSPIWriteByte(uint8 spi, uint8 data)
	{
	if (spi < kNumberOfSPI)
		{
		S0SPDR = data;
		while (!(S0SPSR & SPIF))
			;
		data = S0SPDR;
		}
/*	
	// send a byte over SPI and ignore reply
	while (spis[spi].spiTransferRemaining)
		coreControllerIdle();

	spis[spi].spiTransferRemaining = 0x01;

	S0SPDR = data;
*/
	}

uint8 coreSPIReadByte(uint8 spi)
	{
	uint8	result = 0;
	
	if (spi < kNumberOfSPI)
		{
		S0SPDR = 0xFF;
		while (!(S0SPSR & SPIF))
			;
		result = S0SPDR;
		}
	
	return result;
	}

uint8 coreSPITransferByte(uint8 spi, uint8 data)
	{
	uint8	result = 0;
	
	if (spi < kNumberOfSPI)
		{
		S0SPDR = data;
		while (!(S0SPSR & SPIF))
			;
		result = S0SPDR;
		}
	
	return result;
	}
#endif
#endif
