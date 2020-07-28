/*
 * StartUp.cpp
 *
 * Created: 6/19/2020 3:07:23 PM
 * Author : GMateusDP
 */ 


#include "main.h"


int main(void)
{
	atmel_start_init();
	
	usb.init();
	delay_ms(100);
	usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	memory.Init();
	alu.Init();
	usb<<"*** ALU initiated ***"<<NEWLINE;
	uint8_t	line= alu.GetSelectedAMMC();
	usb<<"Active line: "<<line<<NEWLINE;
	if (!pBIT)
	{
		hvac.Stop();
	} 
	else
	{
		hvac.Start();
	}
    /* Replace with your application code */
    while (1) 
    {
		delay_ms(1000);
		usb<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
    }
}
