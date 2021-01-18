/*
 * Scavenger_Fan_P0.cpp
 *
 * Created: 1/18/2021 11:23:57 AM
 * Author : GMateusDP
 */ 


#include "main.h"


Scavenge_Fan sca;

int main(void)
{
	atmel_start_init();
	sca.Init();
    /* Replace with your application code */
    while (1) 
    {
		sca.SetEnable(true);
		delay_ms(1000);
		sca.SetEnable(false);
		delay_ms(500);
    }
}
