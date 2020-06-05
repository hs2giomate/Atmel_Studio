/*
 * AT24MAC_Class.cpp
 *
 * Created: 6/3/2020 10:54:08 AM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "AT24MAC_Class.h"
#include "USB_CDC_Class.h"
uint8_t p[]="---EEPROM connection error!!-----";
char	array[16];
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	AT24MAC_Class	mem(&I2C_0);
	mem.init();
	USB_CDC_Class	usb;
	usb.init();
	uint8_t addr=0x00;
	/* Replace with your application code */
	while (1) {
		if (mem.is_EEPROM_ready())
		{
			
			mem.read_byte(addr);
			char *c=utoa(addr,array,10);
			usb.xprint(c);
			usb.xprint("\r\n")
			mem.write_byte(addr,16-addr)
			addr++;
			if (addr<16){
				addr++;
			}
			else{
				addr=0;
			}
		} 
		else
		{
			if (usb.isUSBEnabled())
			{
				usb.xprint( "---I2C connection error!!-----\r\n");
			}
			
			
		}
	}
}
