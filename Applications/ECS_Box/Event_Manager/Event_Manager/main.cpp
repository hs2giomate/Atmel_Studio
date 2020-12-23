/*
 * Event_Manager.cpp
 *
 * Created: 12/15/2020 8:47:15 AM
 * Author : GMateusDP
 */ 


#include "main.h"
char  stringChar[16];

int main(void)
{
	atmel_start_init();
	int counter=0;
	
	usb.Init();
	delay_ms(100);

	usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	if (memory.Init())
	{
		memory.WriteDefaultParameters();
			logger.SaveEvent((uint8_t*)"Info:Memory Started");
	}
	else
	{
	}
	delay_ms(1000);
	while(1){
		char	*str=&stringChar[0];
		itoa(counter,str,10);
		logger.SaveEvent((uint8_t*)str);
		delay_ms(1000);
	}
}
