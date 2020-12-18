/*
 * P0_ALU_Controller.cpp
 *
 * Created: 11/30/2020 2:13:34 PM
 * Author : GMateusDP
 */ 


#include "main.h"


int main(void)
{
    /* Replace with your application code */
	atmel_start_init();
	//  	uint32_t p=pwm_enable(&LIVE_PULSE);
	//  	while (1);

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
	
	
	
	alu.Init();
	alu.RunController();
	usb<<"*** ALU initiated ***"<<NEWLINE;
	uint8_t	line= alu.GetSelectedAMMC();
	usb<<"Active line: "<<line<<NEWLINE;
	if (!pBit)
	{
		hvac.Stop(0);
	}
	else
	{
		hvac.Start(0);
	}
	/* Replace with your application code */
	while (1)
	{
		delay_ms(1000);
		usb<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	}
}
