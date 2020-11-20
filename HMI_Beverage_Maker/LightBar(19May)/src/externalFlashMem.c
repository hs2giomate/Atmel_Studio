/** \file		externalFlashMem.c
 *
 *  \brief		perform write-read-verify test for HALT
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#include "bevMakerHmi.h"
#include "externalFlashMem.h"
#include "externalFlashMem_Private.h"
#include "spi.h"
#include "pscEvent.h"
#include "pscHALTCommand.h"

/*
	AT25SF161
	Max. CLock 104 MHz
	SPI modes 0 & 3
	msb first
	
*/


static const uint32_t gFlashStartAddr = 0;
bool gCheckForFlashEraseNextSector;
bool gReadVerifyPending;
uint32_t gFlashTestAddr;

/** \brief Check if the external flash memory IC is still busy, from a previous command.
 *
 *  \return  true if the external flash memory IC is busy
 */
static bool IsFlashBusy(void);

/** \brief Read multiple bytes from the external flash memory IC .
 *
 *  \return  Nothing
 */
static void FlashMultiRead(uint32_t dwAddr, uint8_t* pDest, uint16_t wCount);

/** \brief Erase a sector in the external flash memory IC .
 *
 *  \return  Nothing
 */
static void FlashSectorErase(uint32_t dwAddr);

/** \brief Write one or more bytes to the external flash memory IC .
 *
 *  \return  Nothing
 */
static void FlashPageWrite(uint32_t dwAddr, uint8_t* pcData, uint16_t wCount);

/** \brief Enable writing to the external flash memory IC .
 *
 *  \return  Nothing
 */
static void FlashSetWriteEnableLatch(void);

/** \brief Assert chip-select on the external flash memory IC, to allow access.
 *
 *  \return  Nothing
 */
static inline void CS_Flash_Enable(void)		{PORTC.OUTCLR = PIN4_bm;	/* assert CS pin (active low) */}

/** \brief De-assert access to the external flash memory IC, after access complete.
 *
 *  \return  Nothing
 */
static inline void CS_Flash_Disable(void)		{PORTC.OUTSET = PIN4_bm;	/* de-assert CS pin (active low) */}



void esfInit(void)
{
	/* Configure GPIO for external flash write-protect pin */
	PORTD.DIRSET = PIN4_bm;       /* /WP */
	PORTD.OUTSET = PIN4_bm;			/* de-assert /WP pin (active low) */

	/* request erase the first Flash sector before the test starts */
	gCheckForFlashEraseNextSector = true;
}


uint32_t glastSequenceCountWrittenToFlash;
uint32_t externalFlashMemSuccessCount;
uint32_t externalFlashMemFailedCount;

void esfTestRestart(void)
{
	externalFlashMemSuccessCount = 0;
	externalFlashMemFailedCount = 0;
	glastSequenceCountWrittenToFlash = 0;
}

enum PayloadSizeTestWriteToFlash_enum {PayloadSizeTestWriteToFlash	= 4,};

void esfHALTtestManage(void)
{
	uint32_t sequenceCount = pscHALT_GetSequenceCount();
	
	if(gReadVerifyPending)
	{
		if(IsFlashBusy())
		{}	/* Flash is busy */
		else
		{
			gReadVerifyPending = false;
			
			/* the write has completed */
			
			uint32_t readBackData;
			FlashMultiRead(gFlashTestAddr, (uint8_t*)&readBackData, PayloadSizeTestWriteToFlash);
			bool bOK = false;
			if(readBackData == glastSequenceCountWrittenToFlash)
			{	/* last write successfully verified */
				bOK = true;
				/* Another successful write-read-verify on the external SPI connected flash memory */
				externalFlashMemSuccessCount++;
			}
			else
			{	/* last write failed verification */
				/* Another failed write-read-verify on the external SPI connected flash memory */
				externalFlashMemFailedCount++;
			}
			pscOnExternalFlashResult(bOK, externalFlashMemSuccessCount, externalFlashMemFailedCount);
	
			/* write to a different flash location each time */
			gFlashTestAddr += PayloadSizeTestWriteToFlash;
			/* but do not go beyond the end */
			uint32_t testAddress = gFlashTestAddr + PayloadSizeTestWriteToFlash;
			if(testAddress > esfHighestValidAddress)
			{	/* target address beyond the end of the flash memory */
				gFlashTestAddr = gFlashStartAddr;
			}
			
			/* check if need to erase the next flash page in preparation for the next write test */
			gCheckForFlashEraseNextSector = true;
		}
	}
	else if(gCheckForFlashEraseNextSector)
	{
		if(IsSectorBoundary_SpiFlash(gFlashTestAddr))
		{	/* Each flash sector must be erased before it can be written, and needs time to complete */
			if(IsFlashBusy())
			{}	/* Flash is busy */
			else
			{
				gCheckForFlashEraseNextSector = false;
				FlashSectorErase(gFlashTestAddr);
			}
		}
		else
		{
			gCheckForFlashEraseNextSector = false;
		}
	}
	else if(glastSequenceCountWrittenToFlash < sequenceCount)
	{	/* The sequence count has changed since it was last test written to flash */
		/* perform another test write */ 
		if(IsFlashBusy())
		{}	/* Flash is busy */
		else
		{
			/* Record the values used in the most recent test write to flash */
			
			/* If the HMI is only a few counts behind the test box, have it try to catch up
				so the count of successful writes matches the sequence count and makes it easy to check the log.
				Don't allow this if the HMI is too far behind, which would suggest I2C errors */
			uint32_t diff = sequenceCount -glastSequenceCountWrittenToFlash;
			if(diff < 5)
			{
				glastSequenceCountWrittenToFlash++;
			}
			else
			{
				glastSequenceCountWrittenToFlash = sequenceCount;
			}
			
			FlashPageWrite(gFlashTestAddr, (uint8_t*)&glastSequenceCountWrittenToFlash, PayloadSizeTestWriteToFlash);
			
			gReadVerifyPending = true;
		}
	}
	else
	{}
}

void FlashMultiRead(uint32_t dwAddr, uint8_t* pDest, uint16_t wCount)
{
	uint8_t c, cData;
	uint8_t* pAddr;

	pAddr = (uint8_t*)(&dwAddr);
	pAddr += 2;	/* Move to the MSB (24-bit) */

	CS_Flash_Enable();

	/* read op-code */
	SpiTransfer(0x03);

	/* address, MSB first */
	for(c = 0; c < 3; c++)
	{
		SpiTransfer(*pAddr);
		pAddr--;
	}

	/* Get Data */
	for(; wCount; wCount--)
	{
		/* send don't care bytes to get the response data */
		cData = SpiTransfer(0);
		*pDest = cData;
		pDest++;
	}

	CS_Flash_Disable();
}


/*
	Send:
	write enable op-code
*/
void FlashSetWriteEnableLatch(void)
{
	CS_Flash_Enable();

	/* Set Write enable latch */
	SpiTransfer(0x06);

	CS_Flash_Disable();
}

/*
	p68
	Page Program:
	write op-code
	24-bit address 
	Data (256-byte page)
*/
void FlashPageWrite(uint32_t dwAddr, uint8_t* pcData, uint16_t wCount)
{
	uint8_t c;
	uint8_t* pAddr;

	FlashSetWriteEnableLatch();

	pAddr = (uint8_t*)(&dwAddr);
	pAddr += 2;	/* Move to the MSB (24-bit) */

	/* 256-byte page */
	if(wCount > SPI_FLASH_PAGE_SIZE)
	{
		wCount = SPI_FLASH_PAGE_SIZE;
	}

	CS_Flash_Enable();

	/* write op-code, page program */
	SpiTransfer(0x02);

	/* address, MSB first */
	for(c = 0; c < 3; c++)
	{
		SpiTransfer(*pAddr);
		pAddr--;
	}

	/* Data */
	for(; wCount; wCount--)
	{
		SpiTransfer(*pcData);
		pcData++;
	}


	CS_Flash_Disable();
}

/*
	p68
	Sector Erase: (4KB, max erase time 450 ms )
	write op-code
	24-bit address 
	Data (256-byte page)
*/
void FlashSectorErase(uint32_t dwAddr_In)
{
	uint8_t c;
	uint8_t* pAddr;
	uint32_t dwAddr;

	dwAddr = dwAddr_In;

	FlashSetWriteEnableLatch();

	pAddr = (uint8_t*)(&dwAddr);
	pAddr += 2;	/* Move to the MSB (24-bit). The MSB of the 32-bit address is not relevant to this device */

	CS_Flash_Enable();

	/* write op-code, sector erase */
	SpiTransfer(0x20);

	/* address, MSB first */
	for(c = 0; c < 3; c++)	/* 3 bytes == 24-bits */
	{
		SpiTransfer(*pAddr);
		pAddr--;
	}

	CS_Flash_Disable();
}

/*
	check is BUSY bit is still set
	BUSY flag - SR1, bit0
*/
bool IsFlashBusy(void)
{
	uint8_t cData;
	bool bBusy;

	CS_Flash_Enable();

	/* read SR1 op-code */
	SpiTransfer(0x05);

	/* Get Data */
	/* send don't care bytes to get the response data */
	cData = SpiTransfer(0);

	CS_Flash_Disable();

	/* Recover the BUSY flag */
	bBusy = (cData & 0x01);

	return(bBusy);
}


/*** END of FILE ***/
