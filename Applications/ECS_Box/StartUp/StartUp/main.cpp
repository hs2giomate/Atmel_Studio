/*
 * StartUp.cpp
 *
 * Created: 6/19/2020 3:07:23 PM
 * Author : GMateusDP
 */ 


#include "main.h"


void	QSPIDriverTest(void){
	bool is_corrupted = false;
	uint8_t tx_buffer[QSPI_BUFFER_SIZE] ;
	uint8_t rx_buffer[QSPI_BUFFER_SIZE] ;
	qspiFlash.Init();

	usb.print("QSPI Program Started\n\r");
	/* Initialize Tx buffer */
	for (int i = 0; i <QSPI_BUFFER_SIZE ; i++) {
		tx_buffer[i] = (uint8_t)i;
	}
	
	//qspiFlash.Erase();
	/* Erase flash memory */
	uint32_t	flashAddres=0;
	while (1) {
		delay_us(100);
		/* Write data to flash memory */
		if (ERR_NONE ==qspiFlash.WriteAddress((uint8_t *)tx_buffer,flashAddres,QSPI_BUFFER_SIZE)) {
			//usb.print("Flash write successful \n\r");
		}
		delay_us(100);
		/* Read data from flash memory */
		if (ERR_NONE == qspiFlash.ReadAddress((uint8_t *)rx_buffer,flashAddres,QSPI_BUFFER_SIZE)) {
			//while(!memory.xferDone);
			//usb.print("Flash read successful\n\r");
		}
		delay_us(100);
		is_corrupted = false;
		for (int i = 0; i < QSPI_BUFFER_SIZE; i++) {
			if (tx_buffer[i] != rx_buffer[i]) {
				is_corrupted = true;
				usb.print("Flash data verification failed.\n\r");
				usb.print(flashAddres,HEX);
				usb<<" bit :"<<i<<NEWLINE;
				i=QSPI_BUFFER_SIZE;
				break;
			}
			
		}

		if (!is_corrupted) {
			usb.print("Write - Read is successful in QSPI Flash memory ");
			usb.println(flashAddres,HEX);
			for (int i = 0; i <QSPI_BUFFER_SIZE ; i++) {
				rx_buffer[i] = (uint8_t)(QSPI_BUFFER_SIZE-i);
			}
		}
		flashAddres=(QSPI_BUFFER_SIZE+flashAddres)%N25Q_FLASH_SIZE;
		
	}
}

int main(void)
{
	atmel_start_init();
	
	usb.Init();
	delay_ms(100);
	usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	//QSPIDriverTest();
	memory.Init();
	alu.Init();
	alu.RunController();
	usb<<"*** ALU initiated ***"<<NEWLINE;
	uint8_t	line= alu.GetSelectedAMMC();
	usb<<"Active line: "<<line<<NEWLINE;
	if (!pBIT)
	{
		hvac.Stop(0);
	} 
	else
	{
		hvac.Start(0);
	}
    /* Replace with your application code */
    while (1) 
    {
		delay_ms(1000);
		usb<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
    }
}
