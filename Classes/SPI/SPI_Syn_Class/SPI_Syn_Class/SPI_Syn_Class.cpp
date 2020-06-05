/* 
* SPI_Syn_Class.cpp
*
* Created: 4/6/2020 10:22:27 AM
* Author: GMateusDP
*/


#include "SPI_Syn_Class.h"

SPI_Syn_Class	*ptrSPISync;

// default constructor
SPI_Syn_Class::SPI_Syn_Class()
{
		ptrSPISync=this;
//	SPI=SPI_1;
} //SPI_Syn_Class

SPI_Syn_Class::SPI_Syn_Class(spi_m_sync_descriptor * spiDesc)
{
	SPI=*spiDesc;
	ptrSPISync=this;
} //SPI_Syn_Class

// default destructor
SPI_Syn_Class::~SPI_Syn_Class()
{
} //~SPI_Syn_Class

void SPI_Syn_Class::init(){
	//SPI_1_CLOCK_init();
	spi_m_sync_init(&SPI, SPI.dev.prvt);
	//SPI_1_PORT_init();
	
	ready=true;
	
}

int32_t SPI_Syn_Class::enable(){
	spi_m_sync_get_io_descriptor(&SPI, &spio);
	spi_m_sync_enable(&SPI);
	ready=true;
	return 0;
}

int32_t  SPI_Syn_Class::write(const uint8_t * p, int32_t n){
	int32_t w;
	wFlag=true;
	w= io_write(spio,p,(uint16_t)n);
	wFlag=(bool)w;
	 return w;
	
}
int32_t  SPI_Syn_Class::read(uint8_t * p, int32_t n){
	int32_t r;
	rFlag=true;
	r=io_read(spio,p,(uint16_t)n);
	rFlag=(bool)r;
	return  r;
}
void SPI_Syn_Class::disable(void){
	spi_m_sync_disable(&SPI);
	
}


/*
void SPI_Asyn_Class::classInterruptHandler(void) {
	SPICallbackHandler(setSPIReady());
}
*/
bool SPI_Syn_Class::isSPIReady(void){
	ready=~(rFlag||wFlag);
	//spi_m_async_register_callback(&SPI0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)(&SPI_Asyn_Class::complete_cb_SPI_0));
	if (ready)
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

