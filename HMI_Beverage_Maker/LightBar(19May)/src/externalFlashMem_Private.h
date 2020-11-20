/** \file		externalFlashMem_Private.h
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


#ifndef EXTERNALFLASHMEM_PRIVATE_H
#define EXTERNALFLASHMEM_PRIVATE_H

/*
FLASH
	AT25SF161			- FLASH - 16Mbit - 2MByte

	Page size : 256 byte
	Sector size : 4KB  (Minimum erase size)

	SPI Flash size 64Mbit = 8MB =		0x800000
				MAX Address					0x7FFFFF
				
	Flash size 16Mbit = 2MB =			0x200000
				MAX Address					0x1FFFFF
*/

#define SPI_FLASH_PAGE_SIZE							(uint16_t)0x100		/* 256 == 0x100 */
#define SPI_FLASH_ADDR_WITHIN_PAGE_MASK			(uint8_t)0xFF
#define SPI_FLASH_ADDR_BY_PAGE_MASK					(uint32_t)0xFFFFFF00

#define SPI_FLASH_SECTOR_SIZE							(uint16_t)0x1000		/* 0x1000 == 4096 == 4K */
#define SPI_FLASH_ADDR_WITHIN_SECTOR_MASK			(uint16_t)0x0FFF
#define SPI_FLASH_ADDR_BY_SECTOR_MASK				(uint32_t)0xFFFFF000

#define GetSectorStart_SpiFlash(dwAddr)				((dwAddr) & SPI_FLASH_ADDR_BY_SECTOR_MASK)
#define IsSectorBoundary_SpiFlash(dwAddr)			(((dwAddr) & SPI_FLASH_ADDR_WITHIN_SECTOR_MASK) == 0)

#define esfHighestValidAddress		0x0FFFFF






#endif /* EXTERNALFLASHMEM_PRIVATE_H */

/*** END of FILE ***/