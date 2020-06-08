/*
 * ECS_Box_Classes.cpp
 *
 * Created: 6/5/2020 3:51:23 PM
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
				usb<<"it does work";
			}
		}
		delay_ms(100);
	}
}
