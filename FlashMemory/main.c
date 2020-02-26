/*
 * FlashMemory.c
 *
 * Created: 1/8/2020 3:45:27 PM
 * Author : GMateusDP
 */ 



#include "flashCortexModule.h"
//#include "coreSystemTimer.h"



int main(void)
{
	
	initDriverFlashing();
	configurationDataStruct	configuration;
	configuration= defaultConfiguration;

    /* Replace with your application code */
    while (1) 
    {
		readConfigurationData(&configuration);
		coreSystemTimerWait(1000);
		
		PORT->Group[1].OUTCLR.reg = (1UL << 9);
		PORT->Group[1].OUTCLR.reg = (1UL << 0);
		PORT->Group[0].OUTCLR.reg = (1UL << 4);
		configuration.portAStatus=PORT->Group[0].OUT.reg;
		configuration.portBStatus=PORT->Group[1].OUT.reg;
		flushConfigurationData(&configuration);
	
		
	}
    
}

