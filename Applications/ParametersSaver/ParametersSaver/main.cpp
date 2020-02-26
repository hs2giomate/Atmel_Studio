/*
 * ParametersSaver.cpp
 *
 * Created: 2/4/2020 10:34:01 AM
 * Author : GMateusDP
 */ 


//#include "sam.h"
#include <atmel_start.h>
#include "FRAM_Memory_Class.h"
#include "Usart_Asyn_Class.h"

static uint8_t		banner[]="The Parameters are: \r\n";

int main(void)
{
		atmel_start_init();
		
		userParameters	oldParameters, storedParameters, newParameters;
		FRAM_Memory_Class	cypress;
		Usart_Asyn_Class	usart0(0);
		
				
		
	
		oldParameters	 =	defaultParameters;
		uint8_t		dataToSend[sizeof(oldParameters)];

		cypress.init(&oldParameters);
		cypress.flushParameters(&oldParameters);
		usart0.init();
    /* Replace with your application code */
    while (1) 
    {
		cypress.readParameters(&storedParameters);
		delay_ms(500);
		gpio_set_pin_level(Read_OK,false);
		gpio_set_pin_level(Read_Fail,false);
		gpio_set_pin_level(Write_Fail,false);
		gpio_set_pin_level(Write_OK,false);
		memcpy(&dataToSend,&storedParameters,sizeof(storedParameters));
		usart0.enable();
		usart0.write(banner,sizeof(banner));
		usart0.write(dataToSend,sizeof(storedParameters));
		delay_ms(500);
		usart0.disable();
		cypress.flushParameters(&oldParameters);
		
    }
}
