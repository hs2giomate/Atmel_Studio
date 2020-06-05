/*
 * ARINCTransceiver.cpp
 *
 * Created: 2/24/2020 12:33:37 PM
 * Author : GMateusDP
 */ 
#include <atmel_start.h>
//#include "USB_CDC_Class.h"
#include "ARINC_Interface.h"
static uint8_t p[]="   Usb Interface Test\n\r";

int main(void)
{		
		atmel_start_init();
	//	USB_CDC_Class	usb1;
		//usb1.init();
		ARINC_Interface		ECSBox;
		ECSBox.Init();
		
		//delay_ms(3000);
		//ECSBox.SayHello();
		delay_ms(300);
		//ECSBox.TransmitReceiveWithLabels_Mode(SELFTEST_ON);
		
    /* Replace with your application code */
    while (1) 
    {
		//usb1.write(&p[0],sizeof(p));
		ECSBox.SayHello();
		delay_ms(300);
		ECSBox.CustomMessage(SELFTEST_OFF);
			
    }
}

