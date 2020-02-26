/*
 * UpdateParameters.cpp
 *
 * Created: 1/29/2020 3:27:41 PM
 * Author : GMateusDP
 */ 



#include <atmel_start.h>
#include "FlashMemoryClass.h"


int main(void)
{
	atmel_start_init();
	userParameters	parameters;
	parameters	 =	defaultParameters;
	FlashMemoryClass	issi(&parameters);
	issi.spi0.init();

    /* Replace with your application code */
    while (1) 
    {
		//gpio_set_pin_level(Read_OK,true);
		issi.readConfigurationData(&parameters);
		delay_ms(1000);
		gpio_set_pin_level(Read_OK,false);
		gpio_set_pin_level(Read_Fail,false);
		gpio_set_pin_level(Write_Fail,false);
		gpio_set_pin_level(Write_OK,false);
		issi.flushConfigurationData(&parameters);
		
    }
}
