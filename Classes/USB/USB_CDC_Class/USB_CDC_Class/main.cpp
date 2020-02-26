/*
 * USB_CDC_Class.cpp
 *
 * Created: 2/24/2020 1:15:45 PM
 * Author : GMateusDP
 */ 
#include <atmel_start.h>
#include "usb_start.h"
#include "USB_CDC_Class.h"

static uint8_t p[]="Usb port Test\n\r";

int main(void)
{
	atmel_start_init();
	USB_CDC_Class	usb0;
	usb0.init();
	uint8_t *ptr=p;
    /* Replace with your application code */
    while (1) 
    {
		while(!usb0.isUSBEnabled());
		usb0.write(ptr,sizeof(p));
		delay_ms(500);
    }
}
