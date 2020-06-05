/*
 * USB_Serial_Class.cpp
 *
 * Created: 6/4/2020 2:44:06 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "CDC_Class.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	CDC_Class	usb;
	usb.init();
	

	/* Replace with your application code */
	while (1) {
		if (usb.is_enabled())
		{
			int inByte=usb.read();
			if (inByte>33)
			{
				usb.println(inByte);
				usb.println((char)inByte);
				usb.println((float)(inByte/2));
				usb.println("it works");
			}
		}
		delay_ms(100);
	}
}
