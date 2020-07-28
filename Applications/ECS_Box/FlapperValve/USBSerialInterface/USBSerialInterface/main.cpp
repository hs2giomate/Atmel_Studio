/*
 * USBSerialInterface.cpp
 *
 * Created: 7/23/2020 6:03:23 PM
 * Author : GMateusDP
 */ 





#include <atmel_start.h>
#include "CDC_Class.h"
uint8_t	test[]="por que no funciona\n\r";
int main(void)
{
	char cmd;
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	//delay_ms(2000);
	serial.Init();
//	serial.write(test,sizeof(test));
	delay_ms(200);
	serial.println("*** USB Interface Test ***");
	/* Replace with your application code */
	while (1) {
		serial<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<" ***"<<NEWLINE;
		serial<<"Type  a Command!"<<NEWLINE;
		cmd=(char)serial.read();
		serial<<"Command: "<<cmd<<NEWLINE;
	}
}