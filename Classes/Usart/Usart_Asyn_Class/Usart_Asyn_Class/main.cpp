/*
 * Usart_Asyn_Class.cpp
 *
 * Created: 2/12/2020 10:16:34 AM
 * Author : GMateusDP
 */ 


//#include "sam.h"
#include <atmel_start.h>
#include "Usart_Asyn_Class.h"

static uint8_t example_USART_0[] = "The parameters are:   \r\n";

int main(void)
{
	atmel_start_init();
	
	Usart_Asyn_Class	usart0(0);
	usart0.init();

    /* Replace with your application code */
    while (1) 
    {
		usart0.enable();
		usart0.write(example_USART_0,sizeof(example_USART_0));
		delay_ms(500);
		usart0.disable();
    }
}
