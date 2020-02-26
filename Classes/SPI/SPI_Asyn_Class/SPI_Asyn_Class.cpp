/* 
* SPI_Asyn_Class.cpp
*
* Created: 2/5/2020 12:17:57 PM
* Author: GMateusDP
*/


#include "SPI_Asyn_Class.h"
#include "driver_init.h"
 SPI_Asyn_Class* ptrSPIClass;
static void complete_cb_SPI_0(const struct spi_m_async_descriptor *const io_descr)
{
	//SPI_Asyn_Class spiHandler;
	//ptrSPIClass=pSPIClass;
	//ptrSPIClass->ready=true;
	asm("nop");
	ptrSPIClass->setSPIReady();
	asm("nop");
}
// default constructor
//SPI_Asyn_Class::ptrSPIClass->SPICallbackHandler();
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
/*
SPI_Asyn_Class::pSPIClass-{
		ptrSPIClass=pSPIClass;
}
*/
void SPI_Asyn_Class::init(){
	SPI_0_CLOCK_init();
	spi_m_async_init(&SPI0, SERCOM0);
	SPI_0_PORT_init();
	pSPIClass=this;
	ptrSPIClass=pSPIClass;
	ready=true;
	
}

int32_t SPI_Asyn_Class::enable(){
	spi_m_async_get_io_descriptor(&SPI0, &spio);
	spi_m_async_register_callback(&SPI0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)(complete_cb_SPI_0));
	spi_m_async_enable(&SPI0);
	ready=true;
	return 0;
}

int32_t  SPI_Asyn_Class::write(const uint8_t * p, int32_t n){
	int32_t w;
		while(!ready){
			//ready=isSPIReady();
		}
	ready=false;
	w= io_write(spio,p,(uint16_t)n);

	 return w;
	
}
int32_t  SPI_Asyn_Class::read(uint8_t * p, int32_t n){
	int32_t r;
		while(!ready){
			//ready=isSPIReady();
		}
	ready=false;
	r=io_read(spio,p,(uint16_t)n);

	return  r;
}
void SPI_Asyn_Class::disable(void){
	spi_m_async_disable(&SPI0);
	
}
void  SPI_Asyn_Class::setSPIReady(void)
{
	ready=true;
	ready=true;
}

/*
void SPI_Asyn_Class::classInterruptHandler(void) {
	SPICallbackHandler(setSPIReady());
}
*/
bool SPI_Asyn_Class::isSPIReady(void){
	//spi_m_async_register_callback(&SPI0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)(&SPI_Asyn_Class::complete_cb_SPI_0));
	if (ready==true)
	{
		return true;
	} 
	else
	{
		return false;
	}
	return ready;
	
}

//SPI_Asyn_Class::ptrSPIClass->SPICallbackHandler();

