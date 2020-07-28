/* 
* N25Q256_Class.cpp
*
* Created: 6/26/2020 9:33:37 AM
* Author: GMateusDP
*/


#include "N25Q256_Class.h"

	

N25Q256_Class	*ptrN25Q256Class;

n25q256a  N25Q256_Class::SPI_NOR_FLASH_descr;

void QSPI_N25Q256_exit_xip(void)
{
	gpio_set_pin_function(FLASH_IO0, 0);
	gpio_set_pin_function(FLASH_CS, 0);
	gpio_set_pin_function(FLASH_CLK, 0);

	gpio_set_pin_direction(FLASH_IO0, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(FLASH_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(FLASH_CLK, GPIO_DIRECTION_OUT);

	gpio_set_pin_level(FLASH_IO0, true);
	gpio_set_pin_level(FLASH_CS, false);
	gpio_set_pin_level(FLASH_CLK, false);

	delay_us(1);

	for (int i = 0; i < 7; i++) {
		gpio_set_pin_level(FLASH_CLK, true);
		delay_us(1);
		gpio_set_pin_level(FLASH_CLK, false);
		delay_us(1);
	}

	gpio_set_pin_level(FLASH_CS, true);
	delay_us(1);
	QSPI_N25Q256_PORT_init();
}

// default constructor
N25Q256_Class::N25Q256_Class()
{

} //N25Q256_Class
N25Q256_Class::N25Q256_Class(qspi_sync_descriptor *d)
{
	ptrQSPIDescriptor=d;

	memoryCapacity=N25Q_FLASH_SIZE-QSPI_BUFFER_SIZE;
	
	//tx_buffer = {0};
	//rx_buffer = {0};
} //N25Q256_Class

// default destructor
N25Q256_Class::~N25Q256_Class()
{
} //~N25Q256_Class

void N25Q256_Class::Init(void){
	xferDone=false;
	
	spi_nor_flash_init();
}

void N25Q256_Class::Init(qspi_sync_descriptor  *d){
	ptrQSPIDescriptor=d;
	Init();
}

void N25Q256_Class::spi_nor_flash_init(void)
{
	qspi_sync_enable(ptrQSPIDescriptor);
	SPI_NOR_FLASH = n25q256a_construct(
	&SPI_NOR_FLASH_descr.parent, ptrQSPIDescriptor, QSPI_N25Q256_exit_xip, CONF_SPI_NOR_FLASH_0_QUAD_MODE);
}
uint32_t	N25Q256_Class::Erase(void){
	uint32_t e=(uint32_t)(SPI_NOR_FLASH->interface->erase(SPI_NOR_FLASH, 0, QSPI_ERSIZE));
	return	e;
}

uint32_t	N25Q256_Class::Write(uint8_t *p, uint32_t size){
	uint32_t w=(uint32_t)(SPI_NOR_FLASH->interface->write(SPI_NOR_FLASH,p, 0, size));
	return	w;
}
uint32_t	N25Q256_Class::WriteAddress(uint8_t *p,uint32_t addr, uint32_t size){
	uint32_t w=(uint32_t)(SPI_NOR_FLASH->interface->write(SPI_NOR_FLASH, p,addr, size));
	return	w;
}
uint32_t	N25Q256_Class::ReadAddress(uint8_t *p,uint32_t addr, uint32_t size){
	xferDone=false;
	uint32_t r=(uint32_t)(SPI_NOR_FLASH->interface->read(SPI_NOR_FLASH,p, addr, size));
	return	r;
}

uint8_t	N25Q256_Class::GetReadStatus(void){
		n25q256a *n25q  = ( n25q256a *)SPI_NOR_FLASH;
		uint8_t          width = n25q->quad_mode ? QSPI_INST4_ADDR4_DATA4 : QSPI_INST1_ADDR1_DATA1;
		uint8_t vcfg = n25q256a_read_reg(SPI_NOR_FLASH, width, N25Q_READ_STATUS_REGISTER);
		return	vcfg;
	
	}
uint32_t	N25Q256_Class::GetMemoryCapacity(){
	return	memoryCapacity;
}
N25Q256_Class	flash(&QSPI_N25Q256);