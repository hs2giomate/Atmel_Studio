/* 
* N25Q256_Class.cpp
*
* Created: 6/26/2020 9:33:37 AM
* Author: GMateusDP
*/


#include "N25Q256_Class.h"
#include "stdlib.h"

	

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

bool N25Q256_Class::Init(void){
	xferDone=false;
	spi_nor_flash_init();
	return SelfTest();
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
uint32_t	N25Q256_Class::Erase(uint32_t add){
	uint32_t e=(uint32_t)(SPI_NOR_FLASH->interface->erase(SPI_NOR_FLASH, add, QSPI_ERBLK));
	return	e;
}


uint32_t	N25Q256_Class::WriteAddress(uint8_t *p,uint32_t addr, uint32_t size){
	//uint32_t e=(uint32_t)(SPI_NOR_FLASH->interface->erase(SPI_NOR_FLASH, addr, size));
	uint32_t w=(uint32_t)(SPI_NOR_FLASH->interface->write(SPI_NOR_FLASH, p,addr, size));
	return	w;
}
uint32_t	N25Q256_Class::ReadAddress(uint8_t *p,uint32_t addr, uint32_t size){
	xferDone=false;
	uint32_t r=(uint32_t)(SPI_NOR_FLASH->interface->read(SPI_NOR_FLASH,p, addr, size));
	return	r;
}

uint8_t	N25Q256_Class::GetStatus(void){
		cmd.inst_frame.bits.inst_en  = 1;
		cmd.inst_frame.bits.data_en  = 1;
		cmd.inst_frame.bits.tfr_type = QSPI_READ_ACCESS;
		cmd.instruction              = N25Q_READ_STATUS_REGISTER;
		cmd.buf_len                  = 1;
		cmd.rx_buf                   = &status;
		
		qspi_sync_serial_run_command(ptrQSPIDescriptor, &cmd);
		return status;
	}
uint32_t	N25Q256_Class::GetMemoryCapacity(){
	return	memoryCapacity;
}
bool	N25Q256_Class::IsReady(){
	return	((GetStatus())&(0x01))==0;
}

bool	N25Q256_Class::WaitOnBusy(void){
		do{
			status=GetStatus();
		}while(status & (1 << 0));
		return	(status&(0x01))==0;
}
bool	N25Q256_Class::SelfTest(void){
	currentAddress=2*QSPI_ERBLK;
// 	for (uint32_t i = 0; i <QSPI_ERBLK ; i++) {
// 		tx_buffer[i] = (uint8_t)rand();
// 		rx_buffer[i] = (uint8_t)(QSPI_BUFFER_SIZE-i);
// 	}	uint8_t	testRXBuffer[QSPI_ERBLK];
	uint8_t	testTXBuffer[QSPI_ERBLK];
	ReadAddress(testTXBuffer,currentAddress,QSPI_ERBLK);
	WaitOnBusy();
	Erase(currentAddress);
	WaitOnBusy();
	WriteAddress(testTXBuffer,currentAddress,QSPI_ERBLK);
	WaitOnBusy();
	ReadAddress(testRXBuffer,currentAddress,QSPI_ERBLK);
	WaitOnBusy();
	isOK	 = true;
	for (int i = 0; i < QSPI_ERBLK; i++) {
		if (testRXBuffer[i] != testTXBuffer[i]) {
			isOK = false;
			//usb.print("Flash data verification failed.\n\r");
		//	usb<<"bit :"<<i<<NEWLINE;
			break;
		}
					
	}
	return	isOK;
}
N25Q256_Class	qspiFlash(&QSPI_N25Q256);