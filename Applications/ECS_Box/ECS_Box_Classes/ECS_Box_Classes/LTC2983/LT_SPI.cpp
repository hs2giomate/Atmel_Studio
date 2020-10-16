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

static  SPI_Syn_Class spiStaticTemp;

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
	spiTemp->SetDescriptor(SPIA);
	spiTemp->init();
	txLiteBuffer=spiTemp->txBuffer;
	rxLiteBuffer=spiTemp->rxBuffer;
	spiTemp->Enable();
	ready=true;
}
void	LT_SPI::set_descriptor(spi_m_sync_descriptor * i2c_a){
	SPIA=i2c_a;
	ptrLTSPIClass=this;
}

// Reads and sends a byte
// Return 0 if successful, 1 if failed
void LT_SPI::spi_transfer_byte(uint32_t cs_pin, uint8_t tx, uint8_t *rx)
{
// 	gpio_set_pin_level(cs_pin,false)  ;              //! 1) Pull CS low
// 	
// 	spiTemp->Write(&tx,1);             //! 2) Read byte and send byte
// 	while(!spiTemp->xferDone);
// 	spiTemp->Read(rx,1);
// 	
// 	gpio_set_pin_level(cs_pin,true) ;             //! 3) Pull CS high
}

uint8_t LT_SPI::spi_transfer( uint8_t tx)
{
		
// 	spiTemp->Write(&tx,1);             //! 2) Read byte and send byte
// 	while(!spiTemp->xferDone);
// 	uint8_t rx;
// 	spiTemp->Read(&rx,1);
	return	tx;
}


// Reads and sends a word
// Return 0 if successful, 1 if failed


// Reads and sends a byte array
void LT_SPI::spi_transfer_block(uint32_t cs_pin, uint8_t *tx, uint8_t *rx, uint8_t length)
{
	uint8_t rOrw=*tx;
	Open(cs_pin);               //! 1) Pull CS low
	
	switch(rOrw){
		case WRITE_TO_RAM:
			spiTemp->Write(tx,length);				//! 2) Read byte and send byte
			//while(!spiTemp->xferDone);
			break;
		case READ_FROM_RAM:
			spiTemp->Write(tx,3); 
		//	while(!spiTemp->xferDone);
		//	gpio_set_pin_level(cs_pin,true) ;
			if (length>4)
			{
			//	delay_ms(1);
			//	gpio_set_pin_level(cs_pin,false);
				spiTemp->Read(rx,length-3);
					asm("nop");
			//	while (!spiTemp->xferDone);
				asm("nop");
// 				for (uint8_t i=0;i<length;i++)
// 				{
// 					//spiTemp.write(tx,1); tx++;
// 					//while(!spiTemp.xferDone);
// 					spiTemp->read(rx,1); 
// 					delay_us(1);
// 					while (!spiTemp->xferDone);
// 						rx++;	
// 				}
// 				rx-=length;
			}
			else
			{
				spiTemp->Read(rx,1);
				asm("nop");
			}
	
			break;
		default:
			spiTemp->Write(tx,length);             //! 2) Read byte and send byte
		//	while(!spiTemp->xferDone);
			spiTemp->Read(rx,length-3);
			break;
	}


	

	//while (!spiTemp->xferDone);
	Close(cs_pin) ;              //! 3) Pull CS high
	asm("nop");
	
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





 void LT_SPI::Open(uint32_t cs_pin){
	 __DMB();
	 __disable_irq();

	gpio_set_pin_level(cs_pin,false);
	 delay_us(10);
	spiTemp->Enable();
	 delay_us(10);
 }

 void LT_SPI::Close(uint32_t cs_pin){

	  delay_us(10);
	 spiTemp->Disable();
	delay_us(10);
	gpio_set_pin_level(cs_pin,true);
	__DMB();
	__enable_irq();
	 
 }