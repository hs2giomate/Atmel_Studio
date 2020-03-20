/*
 * ARINCTransceiver.cpp
 *
 * Created: 2/24/2020 12:33:37 PM
 * Author : GMateusDP
 */ 
#include <atmel_start.h>

#include "ARINC_Interface.h"

int main(void)
{
		atmel_start_init();
		ARINC_Interface		ECSBox;
    /* Replace with your application code */
    while (1) 
    {
		ECSBox.SayHello();
			delay_ms(1000);
    }
}
