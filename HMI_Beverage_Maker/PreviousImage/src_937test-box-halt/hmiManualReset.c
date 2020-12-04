
/** \file		led.c
 *
 *  \brief		monitor the HMI Manual Reset signal 
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#include <stdint.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <string.h>
#include <stdio.h>

#include "bevMakerTest.h"
#include "hmiManualReset.h"
#include "led.h"
#include "mainTimer.h"

uint8_t gHmiManualResetState;
uint32_t gMarkManualResetActive_ms;

const	int gpioManualReset = 18;	/* RPi 2, GPIO header pin 12 */

void hmrInit(void)
{
	/* Setup the pins */
	pinMode(gpioManualReset, INPUT);
	
	pullUpDnControl(gpioManualReset, PUD_OFF);
}

void hmrReadHmiManualResetSignal(void)
{
	uint8_t hmiManualResetState = (uint8_t)digitalRead(gpioManualReset);

	if(gHmiManualResetState != hmiManualResetState)
	{	/* state changed */
		gHmiManualResetState = hmiManualResetState;
		
		if(hmiManualResetState)
		{	/* active */
			gMarkManualResetActive_ms = gdw_msTimeCount;

#ifdef MONITOR_SOME_ACTIVITY
			printf("\rPSC reset signal active...");
#endif /* MONITOR_SOME_ACTIVITY */
		}
		else
		{	/* inactive */
#ifdef MONITOR_SOME_ACTIVITY
			uint32_t diff = gdw_msTimeCount -gMarkManualResetActive_ms;
			printf("\rPSC reset signal active for: %lu ms", (unsigned long)diff);
#endif /* MONITOR_SOME_ACTIVITY */
		}
	}
	
	if(hmiManualResetState)
	{
		ledAmberOn();
	}
	else
	{
		ledAmberOff();
	}
}

/**** END OF FILE ****/