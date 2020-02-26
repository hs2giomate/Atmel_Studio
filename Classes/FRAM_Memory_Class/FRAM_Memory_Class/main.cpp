/*
 * FRAM_Memory_Class.cpp
 *
 * Created: 2/14/2020 12:51:37 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "FRAM_Memory_Class.h"


int main(void)
{
	atmel_start_init();
	userParameters	parameters;
	FRAM_Memory_Class cypress;
	parameters	 =	defaultParameters;
		
	cypress.init(&parameters);
	cypress.readConfigurationData(&parameters);
    /* Replace with your application code */
    while (1) 
    {
				cypress.readConfigurationData(&parameters);
				delay_ms(1000);
				gpio_set_pin_level(Read_OK,false);
				gpio_set_pin_level(Read_Fail,false);
				gpio_set_pin_level(Write_Fail,false);
				gpio_set_pin_level(Write_OK,false);
				cypress.flushConfigurationData(&parameters);
    }
}
