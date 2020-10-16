/* 
* SPI_Asyn_Class.cpp
*
* Created: 2/5/2020 12:17:57 PM
* Author: GMateusDP
*/


#include "SPI_Asyn_Class.h"

static	uint8_t	txStaticBuffer[SPI_ASYN_BUFFER_SIZE];
static	uint8_t	rxStaticBuffer[SPI_ASYN_BUFFER_SIZE];
 SPI_Asyn_Class* ptrSPIAClass;
static void complete_cb_SPI(const struct spi_m_async_descriptor *const io_descr)
{

	ptrSPIAClass->xferDone=true;

}
static void error_cb_SPI(const struct spi_m_async_descriptor *const io_descr)
{

	ptrSPIAClass->xferDone=true;

}
// default constructor
SPI_Asyn_Class::SPI_Asyn_Class(spi_m_async_descriptor *spi)
{
	SPIA=spi;
	ptrSPIAClass=this;
} //SPI_Asyn_Class
SPI_Asyn_Class::SPI_Asyn_Class()
{
	ptrSPIAClass=this;
} //SPI_Asyn_Class

// default destructor
SPI_Asyn_Class::~SPI_Asyn_Class()
{
} //~SPI_Asyn_Class



void SPI_Asyn_Class::set_descriptor(spi_m_async_descriptor *spi){
	ptrSPIAClass=this;
	SPIA=spi;
}

void SPI_Asyn_Class::init(){
	txBuffer=txStaticBuffer;
	rxBuffer=txStaticBuffer;
	spi_m_async_get_io_descriptor(SPIA, &spio);
	spi_m_async_register_callback(SPIA, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)(complete_cb_SPI));
	spi_m_async_register_callback(SPIA, SPI_M_ASYNC_CB_ERROR, (FUNC_PTR)(error_cb_SPI));
	xferDone=true;
}

int32_t SPI_Asyn_Class::enable(){

	spi_m_async_enable(SPIA);
	xferDone=true;

	return 0;
}

int32_t  SPI_Asyn_Class::Write(const uint8_t * p, int32_t n){

	while(!xferDone);
	xferDone=false;
	int32_t w= io_write(spio,p,(uint16_t)n);
	 return w;
	
}
int32_t  SPI_Asyn_Class::Read(uint8_t * p, int32_t n){
	while(!xferDone);
	xferDone=false;
	int32_t r=io_read(spio,p,(uint16_t)n);
	asm("nop");
	//while((!xferDone));
	return  r;
}
void SPI_Asyn_Class::disable(void){
	spi_m_async_disable(SPIA);
	
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
	spi_m_async_get_status(SPIA,&stat);
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
	spi_m_async_get_status(SPIA,&stat);
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
	spi_m_async_get_status(SPIA,&stat);
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

