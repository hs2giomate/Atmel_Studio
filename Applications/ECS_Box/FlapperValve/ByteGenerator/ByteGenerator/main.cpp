/*
 * ByteGenerator.cpp
 *
 * Created: 7/23/2020 5:45:42 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "CDC_Class.h"
#include "GPIOConnector_Class.h"
static	GPIOConnector_Class	gpio;

int main(void)
{
	char cmd;
	uint8_t	byte;
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
    /* Replace with your application code */
	serial.Init();
	gpio.Init();
	//	serial.write(test,sizeof(test));
	delay_ms(200);
	serial.println("*** USB Interface Test ***");
    while (1) 
    {
		serial<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<" ***"<<NEWLINE;
		byte=gpio.ReadByteOnEXT2();
		serial<<"Input Value on EXT2: ";
		serial.println(byte,HEX);
			
		byte=gpio.RequestByte();
		if (byte>0)
		{
			gpio.PutByteOnEXT2(byte);
		} 
		else
		{
			gpio.ClearByteOnEXT2();
		}
		delay_ms(500);
	
    }
}
