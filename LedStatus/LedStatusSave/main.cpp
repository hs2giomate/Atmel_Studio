/*
 * LedStatusSave.cpp
 *
 * Created: 1/27/2020 12:30:05 PM
 * Author : GMateusDP
 */ 


#include "sam.h"
//#include <atmel_start.h>
#include "flashCortexModule.h"

int main(void)
{
		//atmel_start_init();
		initDriverFlashing();
			configurationDataStruct	configuration;
			configuration= defaultConfiguration;
    /* Replace with your application code */
    while (1) 
    {
		readConfigurationData(&configuration);
			PORT->Group[1].OUTCLR.reg = (1UL << 9);
			PORT->Group[1].OUTCLR.reg = (1UL << 0);
			PORT->Group[0].OUTCLR.reg = (1UL << 4);
				configuration.portAStatus=PORT->Group[0].OUT.reg;
				configuration.portBStatus=PORT->Group[1].OUT.reg;
				coreSystemTimerWait(1000);
				flushConfigurationData(&configuration);
				/*
				if (gpio_get_pin_level(NMI))
				{
					
				}
				else
				{
					gpio_toggle_pin_level(redLed);
					while(!gpio_get_pin_level(NMI));
					delay_ms(200);
					
				}
				*/
    }
}
