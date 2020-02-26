/*
 * SPIClass.cpp
 *
 * Created: 1/30/2020 9:06:04 AM
 *  Author: GMateusDP
 */ 
#include "SPIClass.h"
#include "driver_init.h"
#include "utils.h"

//spi_m_sync_descriptor SPIClass::SPI0;

SPIClass::SPIClass(uint8_t p){
	portNumber=p;
	SPI0=SPI_0;
}
SPIClass::SPIClass(void){
		portNumber=0;
		SPI0=SPI_0;
}
SPIClass::~SPIClass(void){
	spi_m_sync_deinit(&SPI0);
}
	
void SPIClass::init(){
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI0, SERCOM0);
	SPI_0_PORT_init();
}

int32_t SPIClass::enable(){
		spi_m_sync_get_io_descriptor(&SPI0, &spio);
		spi_m_sync_enable(&SPI0);
		return 0;
		}
	
int32_t  SPIClass::write(const uint8_t * p, int32_t n){
	return io_write(spio,p,(uint16_t)n);
}
int32_t  SPIClass::read(uint8_t * p, int32_t n){
	return io_read(spio,p,(uint16_t)n);
}
void SPIClass::disable(void){
	spi_m_sync_disable(&SPI0);
	
}