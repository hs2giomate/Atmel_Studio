/*
 * USBSerialTest.cpp
 *
 * Created: 8/31/2020 4:12:24 PM
 * Author : GMateusDP
 */ 

#include "main.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	usb.Init();
	delay_ms(100);
	StartLivePulse();
	
	//DateTimeTest();
	//EEPROM_Test();
	//IO_ExpanderTest();
	/* Replace with your application code */
	while (1) {
		usb_echo_test();
		
	}
}

uint8_t	StartLivePulse(void){
	//usb<<"**** Life Pulse Activated****"<<NEWLINE;
	//pwm_set_parameters(&PWM_0, 500, 1000);
	uint32_t p=pwm_enable(&LIVE_PULSE);
	return p;
}

void usb_echo_test(void){
	char c;
	if (usb.available())
	{
		int inByte=usb.read();
		if (inByte>33)
		{
//			usb.writeData2(&inByte,1);
			delay_ms(100);
			c=char(inByte);
			usb.print(c);
// 			usb.println((float)inByte/23);
// 			usb<<"it does work"<<NEWLINE;
		}
	}
}
