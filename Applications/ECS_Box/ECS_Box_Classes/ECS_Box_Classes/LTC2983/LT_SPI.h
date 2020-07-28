/* 
* LT_SPI.h
*
* Created: 6/9/2020 6:14:08 PM
* Author: GMateusDP
*/


#ifndef __LT_SPI_H__
#define __LT_SPI_H__
#include <stdint.h>
#include "driver_init.h"
#include "SPI_Asyn_Class.h"

class LT_SPI
{
//variables
public:
	static SPI_Asyn_Class	spiTemp;
	volatile	bool	ready;
protected:
private:
	spi_m_async_descriptor *SPIA;

//functions
public:
	LT_SPI();
	~LT_SPI();
	void init(void);
	void	set_descriptor(spi_m_async_descriptor *);
	uint8_t	transfer(uint8_t data);
	void spi_transfer_byte(uint8_t cs_pin,      //!< Chip select pin
	uint8_t tx,          //!< Byte to be transmitted
	uint8_t *rx          //!< Byte to be received
	);



	//! Reads and sends a byte array
	//! @return void
	void spi_transfer_block(uint8_t cs_pin,     //!< Chip select pin
	uint8_t *tx,        //!< Byte array to be transmitted
	uint8_t *rx,        //!< Byte array to be received
	uint8_t length      //!< Length of array
	);

	//! Connect SPI pins to QuikEval connector through the Linduino MUX. This will disconnect I2C.


	//! Configure the SPI port for 4Mhz SCK.
	//! This function or spi_enable() must be called
	//! before using the other SPI routines.
	

	//! Setup the processor for hardware SPI communication.
	//! Must be called before using the other SPI routines.
	//! Alternatively, call quikeval_SPI_connect(), which automatically
	//! calls this function.

	uint8_t spi_transfer( uint8_t tx);
	

	//! Write a data byte using the SPI hardware
	void spi_write(int8_t data  //!< Byte to be written to SPI port
	);

	//! Read and write a data byte using the SPI hardware
	//! @return the data byte read
	int8_t spi_read(int8_t data //!< The data byte to be written
	);
protected:
	
private:
	LT_SPI( const LT_SPI &c );
	LT_SPI& operator=( const LT_SPI &c );

}; //LT_SPI

#endif //__LT_SPI_H__
