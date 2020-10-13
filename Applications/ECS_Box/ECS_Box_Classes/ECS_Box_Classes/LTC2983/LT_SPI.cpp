/* 
* LT_SPI.cpp
*
* Created: 6/9/2020 6:14:07 PM
* Author: GMateusDP
*/


#include "LT_SPI.h"
#include "LTC2983_configuration_constants.h"
#include "string.h"

LT_SPI	*ptrLTSPIClass;

static  SPI_Asyn_Class spiStaticTemp;


// default constructor
LT_SPI::LT_SPI()
{
	ptrLTSPIClass=this;
	spiTemp=&spiStaticTemp;
} //LT_SPI

// default destructor
LT_SPI::~LT_SPI()
{
} //~LT_SPI

void LT_SPI::init(){
	spiTemp->set_descriptor(SPIA);
	spiTemp->init();
	spiTemp->enable();
	ready=true;
}
void	LT_SPI::set_descriptor(spi_m_async_descriptor * i2c_a){
	SPIA=i2c_a;
	ptrLTSPIClass=this;
}

// Reads and sends a byte
// Return 0 if successful, 1 if failed
void LT_SPI::spi_transfer_byte(uint32_t cs_pin, uint8_t tx, uint8_t *rx)
{
	gpio_set_pin_level(cs_pin,false)  ;              //! 1) Pull CS low
	
	spiTemp->write(&tx,1);             //! 2) Read byte and send byte
	while(!spiTemp->xferDone);
	spiTemp->read(rx,1);
	
	gpio_set_pin_level(cs_pin,true) ;             //! 3) Pull CS high
}

uint8_t LT_SPI::spi_transfer( uint8_t tx)
{
		
	spiTemp->write(&tx,1);             //! 2) Read byte and send byte
	while(!spiTemp->xferDone);
	uint8_t rx;
	spiTemp->read(&rx,1);
	return	rx;
}


// Reads and sends a word
// Return 0 if successful, 1 if failed


// Reads and sends a byte array
void LT_SPI::spi_transfer_block(uint32_t cs_pin, uint8_t *tx, uint8_t *rx, uint8_t length)
{
	uint8_t rOrw=*tx;
	gpio_set_pin_level(cs_pin,false)  ;               //! 1) Pull CS low
	
	switch(rOrw){
		case WRITE_TO_RAM:
			spiTemp->write(tx,length);				//! 2) Read byte and send byte
			while(!spiTemp->xferDone);
			break;
		case READ_FROM_RAM:
			spiTemp->write(tx,3); 
			//while(!spiTemp.xferDone);
			if (length>4)
			{
				spiTemp->read(rx,length); 
// 				for (uint8_t i=0;i<length;i++)
// 				{
// 					//spiTemp.write(tx,1); tx++;
// 					//while(!spiTemp.xferDone);
// 					spiTemp->read(rx,1); 
// 						rx++;	
// 				}
// 			
// 				rx-=length;
			}
			else
			{
				spiTemp->read(rx,1);
				asm("nop");
			}
	
			break;
		default:
			spiTemp->write(tx,length);             //! 2) Read byte and send byte
			while(!spiTemp->xferDone);
			spiTemp->read(rx,length-3);
			break;
	}


	


	gpio_set_pin_level(cs_pin,true)   ;              //! 3) Pull CS high
}

// Connect SPI pins to QuikEval connector through the Linduino MUX. This will disconnect I2C.


// Configure the SPI port for 4MHz SCK.
// This function or spi_enable() must be called
// before using the other SPI routines.


// Setup the processor for hardware SPI communication.
// Must be called before using the other SPI routines.
// Alternatively, call quikeval_SPI_connect(), which automatically
// calls this function.


// Disable the SPI hardware port


// Write a data byte using the SPI hardware
void LT_SPI::spi_write(int8_t  data)  // Byte to be written to SPI port
{
	uint8_t tx=(uint8_t)data;
	spiTemp->write(&tx,1); 

}

// Read and write a data byte using the SPI hardware
// Returns the data byte read
int8_t LT_SPI::spi_read(int8_t  data) //!The data byte to be written
{
	uint8_t rx;
	spiTemp->read(&rx,1);
	return	(int8_t)rx;

}

