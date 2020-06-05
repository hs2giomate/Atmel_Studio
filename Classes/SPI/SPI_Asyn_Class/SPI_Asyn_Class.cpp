/* 
* SPI_Asyn_Class.cpp
*
* Created: 2/5/2020 12:17:57 PM
* Author: GMateusDP
*/


#include "SPI_Asyn_Class.h"

 SPI_Asyn_Class* ptrSPIClass;
static void complete_cb_SPI(const struct spi_m_async_descriptor *const io_descr)
{
	//SPI_Asyn_Class spiHandler;
	//ptrSPIClass=pSPIClass;
	//ptrSPIClass->ready=true;
	//asm("nop");
	ptrSPIClass->setSPIReady();
	//asm("nop");
}
// default constructor
SPI_Asyn_Class::SPI_Asyn_Class(spi_m_async_descriptor *spi)
{
	portNumber=0;
	SPI0=spi;
	ptrSPIClass=this;
} //SPI_Asyn_Class
SPI_Asyn_Class::SPI_Asyn_Class()
{
	portNumber=0;
	SPI0=SPI_0;
	ptrSPIClass=this;
} //SPI_Asyn_Class

// default destructor
SPI_Asyn_Class::~SPI_Asyn_Class()
{
} //~SPI_Asyn_Class

SPI_Asyn_Class::SPI_Asyn_Class(uint8_t p){
	portNumber=p;
	SPI0=SPI_0;
	ptrSPIClass=this;
}

void SPI_Asyn_Class::set_descriptor(spi_m_async_descriptor *spi){
	ptrSPIClass=this;
	SPI0=spi;
}

void SPI_Asyn_Class::init(){
	SPI_0_CLOCK_init();
	spi_m_async_init(&SPI0, SERCOM0);
	SPI_0_PORT_init();
	pSPIClass=this;
	ptrSPIClass=pSPIClass;
	xferDone=true;
}

int32_t SPI_Asyn_Class::enable(){
	spi_m_async_get_io_descriptor(&SPI0, &spio);
	spi_m_async_register_callback(&SPI0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)(complete_cb_SPI));
	spi_m_async_enable(&SPI0);
	xferDone=true;

	return 0;
}

int32_t  SPI_Asyn_Class::write(const uint8_t * p, int32_t n){
	int32_t w;
		while(!xferDone){
			//ready=isSPIReady();
		}
	xferDone=false;
	callbackCounter=0;
	w= io_write(spio,p,(uint16_t)n);

	 return w;
	
}
int32_t  SPI_Asyn_Class::read(uint8_t * p, int32_t n){
	int32_t r;
		while(!xferDone){
			//ready=isSPIReady();
		}
	rxReady=false;
	callbackCounter=0;
	r=io_read(spio,p,(uint16_t)n);
	return  r;
}
void SPI_Asyn_Class::disable(void){
	spi_m_async_disable(&SPI0);
	
}
void  SPI_Asyn_Class::setSPIReady(void)
{
	xferDone=true;
	callbackCounter++;
}

/*
void SPI_Asyn_Class::classInterruptHandler(void) {
	SPICallbackHandler(setSPIReady());
}
*/
bool SPI_Asyn_Class::isSPIReady(void){
	//spi_m_async_register_callback(&SPI0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)(&SPI_Asyn_Class::complete_cb_SPI_0));
	if (xferDone==true)
	{
		
		return true;
	} 
	else
	{
		return false;
	}
	return xferDone;
	
}

bool	SPI_Asyn_Class::isRXDone(){
	spi_m_async_get_status(&SPI0,&stat);
	if (stat.flags==SPI_M_ASYNC_STATUS_RX_DONE)
	{
		return true;
	} 
	else
	{
		return false;
	}
}
bool	SPI_Asyn_Class::isTXDone(){
	spi_m_async_get_status(&SPI0,&stat);
	if (stat.flags==SPI_M_ASYNC_STATUS_TX_DONE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool	SPI_Asyn_Class::isBusy(){
	spi_m_async_get_status(&SPI0,&stat);
	if (stat.flags==SPI_M_ASYNC_STATUS_BUSY)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//SPI_Asyn_Class::ptrSPIClass->SPICallbackHandler();

