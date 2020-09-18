/*
 * MemoriesTest.cpp
 *
 * Created: 7/20/2020 12:40:32 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "main.h"
#include "stdlib.h"


int main(void)
{
	atmel_start_init();
	/* Replace with your application code */
	while (1)
	{
		FlashMemoryTest();
	}
}

void	FlashMemoryTest(void){
	bool is_corrupted = false;
	uint8_t tx_buffer[ISSI_BUFFER_SIZE] ;
	uint8_t rx_buffer[ISSI_BUFFER_SIZE] ;
	
	flash.Init();

	//usb.print("QSPI Program Started\n\r");
	/* Initialize Tx buffer */
	for (int i = 0; i <ISSI_BUFFER_SIZE ; i++) {
		tx_buffer[i] = (uint8_t)i;
	}

	flash.eraseFlash(0,TEST_FLASH_SIZE);
	/* Erase flash memory */
	uint32_t	m=0;
	while (1) {
		//delay_us(100);
		/* Write data to flash memory */
		if (ERR_NONE ==flash.WriteAddress((uint8_t *)tx_buffer,m,ISSI_BUFFER_SIZE)) {
			
		}
		delay_ms(WRITE_DELAY);
		//flash.WaitBusy();
		/* Read data from flash memory */
		if (ERR_NONE == flash.ReadAddress((uint8_t *)rx_buffer,m,ISSI_BUFFER_SIZE)) {
			//while(!memory.xferDone);

		}
		//	delay_us(100);
		is_corrupted = false;
		gpio_set_pin_level(NMI,true);
		for (int i = 1; i < ISSI_BUFFER_SIZE; i++) {
			if (tx_buffer[i] != rx_buffer[i]) {
				is_corrupted = true;
				// 				usb.print("Flash data verification failed.\n\r");
				// 				usb<<"bit :"<<i<<NEWLINE;
				i=ISSI_BUFFER_SIZE;
				gpio_set_pin_level(NMI,false);
				break;
			}
			
		}

		delay_ms(WRITE_DELAY);
		m=(m+ISSI_BUFFER_SIZE)%TEST_FLASH_SIZE;
		for (int i = 0; i <ISSI_BUFFER_SIZE ; i++) {
			tx_buffer[i] = (uint8_t)rand();
		}
		
	}
}
void	FramMemoryTest(void){

	bool is_corrupted = false;
	uint8_t tx_buffer[FRAM_BUFFER_SIZE] ;
	uint8_t rx_buffer[FRAM_BUFFER_SIZE] ;
	
	fram.Init();

	//usb.print("QSPI Program Started\n\r");
	/* Initialize Tx buffer */
	for (int i = 0; i <FRAM_BUFFER_SIZE ; i++) {
		tx_buffer[i] = (uint8_t)i;
	}
	/* Erase flash memory */
	uint32_t	m=0;
	while (1) {
		//delay_us(100);
		/* Write data to flash memory */
		if (ERR_NONE ==flash.WriteAddress((uint8_t *)tx_buffer,m,FRAM_BUFFER_SIZE)) {
			
		}
		delay_ms(WRITE_DELAY);
		//flash.WaitBusy();
		/* Read data from flash memory */
		if (ERR_NONE == flash.ReadAddress((uint8_t *)rx_buffer,m,FRAM_BUFFER_SIZE)) {
			//while(!memory.xferDone);

		}
		//	delay_us(100);
		is_corrupted = false;
		gpio_set_pin_level(NMI,true);
		for (int i = 1; i < FRAM_BUFFER_SIZE; i++) {
			if (tx_buffer[i] != rx_buffer[i]) {
				is_corrupted = true;
				// 				usb.print("Flash data verification failed.\n\r");
				// 				usb<<"bit :"<<i<<NEWLINE;
				i=ISSI_BUFFER_SIZE;
				gpio_set_pin_level(NMI,false);
				break;
			}
			
		}

		delay_ms(WRITE_DELAY);
		m=(m+FRAM_BUFFER_SIZE)%FRAM_TEST_SIZE;
		for (int i = 0; i <FRAM_BUFFER_SIZE ; i++) {
			tx_buffer[i] = (uint8_t)rand();
		}
		
	}
}