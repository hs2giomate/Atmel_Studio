/*
 * SavePersitenteFile.cpp
 *
 * Created: 1/24/2020 10:16:40 AM
 * Author : GMateusDP
 */ 


#include "sam.h"
#include "flashCortexModule.h"



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
		PORT->Group[1].OUTTGL.reg = (1UL << 8);
		PORT->Group[1].OUTCLR.reg = (1UL << 9);
		PORT->Group[1].OUTCLR.reg = (1UL << 0);
		PORT->Group[0].OUTCLR.reg = (1UL << 4);
		flushConfigurationData(&configuration);
    }
}
