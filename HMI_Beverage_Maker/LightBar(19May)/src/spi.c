/** \file		spi.c
 *
 *  \brief		SPI driver for testing the external FLASH memory
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
#include "spi.h"

/* Global variables */
uint8_t gTemp;

/* static function prototypes */

void spiInit(void)
{
	/* Configure GPIO for SPI pins */
	PORTC.DIRSET = PIN4_bm | PIN5_bm | PIN7_bm;          /* MOSI, SCK, SS outputs; MISO input */
	PORTC.OUTSET = PIN4_bm;       /* de-assert CS pin (active low) */
	
	/* Configure SPI, Master, Mode 0, 512 kHz clock */
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc | SPI_PRESCALER_DIV16_gc;
	
	/* Flush master receive buffer */
	while(SPIC.STATUS & 0x80)
	{
		gTemp = SPIC.DATA;
	}
}

uint8_t SpiTransfer(uint8_t data)
{
	SPIC.DATA = data;      /* perform SPI transfer with slave */
	while((SPIC.STATUS & 0x80) == 0)
	{} /* wait for transfer to complete */
		
	uint8_t rxData = SPIC.DATA;          /* byte received from slave */

	return rxData;	
}




/*** END of FILE ***/