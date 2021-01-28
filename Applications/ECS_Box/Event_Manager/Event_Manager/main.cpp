/*
 * Event_Manager.cpp
 *
 * Created: 12/15/2020 8:47:15 AM
 * Author : GMateusDP
 */ 


#include "main.h"
char  stringChar[16];
uint8_t status;

int main(void)
{
	atmel_start_init();
	int counter=0;
	while(!heater.Init()){
		delay_ms(100);
		gpio_toggle_pin_level(LED0);
	}
	gpio_set_pin_level(LED0,true);
	
	usb.Init();
	delay_ms(100);
	
	logger.Init();

	//usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	//usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	if (memory.Init())
	{
		memory.WriteDefaultParameters();
			logger.SaveEvent("Info:Memory Started");
	}
	else
	{
	}
	delay_ms(1000);
	while(1){
		status=heater.ReadStatus();
		if (status&0x01>0)
		{
			logger.SaveEvent("Heater 1: is OK");
		} 
		else
		{
			logger.SaveEvent("H1OV1: fail");
		}
		
		delay_ms(100);
	}
}
