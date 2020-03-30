/*
 * TimerClass.cpp
 *
 * Created: 3/2/2020 11:36:46 AM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "Timer_Class.h"


int main(void)
{
    /* Initialize the SAM system */
    atmel_start_init();
	Timer_Class			timer0;
	timer0.Init();
	timer0.start(1);
    /* Replace with your application code */
    while (1) 
    {
		bool to=timer0.timeout;
		if (to)
		{
			timer0.stop();
			
			gpio_toggle_pin_level(LED0);
			timer0.start(10);
		}
		
    }
}
