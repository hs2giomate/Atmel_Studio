/** \file		spi.h
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





/** \brief Initialise the SPI peripheral.
 *
 *  \return  Nothing
 */
void spiInit(void);

/** \brief Perform a SPI transfer with slave
 *
 *  \return  Nothing
 */
uint8_t SpiTransfer(uint8_t data);







/*** END of FILE ***/