
/** \file		led.c
 *
 *  \brief		Switch the LED indicators on / off
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

#include "led.h"
#include "main.h"


/* Define the GPIO pins used */
const	int gpioLedRed = 21;		/* RPi 2, GPIO pin 40 */
const	int gpioLedAmber = 20;	/* RPi 2, GPIO pin 38 */
const	int gpioLedGreen = 16;	/* RPi 2, GPIO pin 36 */

bool gbGreenLedStatus;
bool gbAmberLedStatus;
bool gbRedLedStatus;

void ledInit(void)
{
	/* Initialize wiringPi and allow the use of BCM pin numbering */
	wiringPiSetupGpio();
	/* Setup the pins */
	pinMode(gpioLedRed, OUTPUT);
	pinMode(gpioLedAmber, OUTPUT);
	pinMode(gpioLedGreen, OUTPUT);

	ledAllOff();
}

void ledGreenToggle(void)
{
	gbGreenLedStatus = !gbGreenLedStatus;
	digitalWrite(gpioLedGreen, gbGreenLedStatus ? HIGH : LOW);
}
void ledAmberToggle(void)
{
	gbAmberLedStatus = !gbAmberLedStatus;
	digitalWrite(gpioLedAmber, gbAmberLedStatus ? HIGH : LOW);
}
void ledRedToggle(void)
{
	gbRedLedStatus = !gbRedLedStatus;
	digitalWrite(gpioLedRed, gbRedLedStatus ? HIGH : LOW);
}

void ledAllOff(void)
{
	digitalWrite(gpioLedRed, LOW);
	digitalWrite(gpioLedAmber, LOW);
	digitalWrite(gpioLedGreen, LOW);
}

void ledGreenOn(void)
{
	gbGreenLedStatus = 1;
	digitalWrite(gpioLedGreen, gbGreenLedStatus ? HIGH : LOW);
}
void ledGreenOff(void)
{
	gbGreenLedStatus = 0;
	digitalWrite(gpioLedGreen, gbGreenLedStatus ? HIGH : LOW);
}

void ledAmberOn(void)
{
	gbAmberLedStatus = 1;
	digitalWrite(gpioLedAmber, gbAmberLedStatus ? HIGH : LOW);
}
void ledAmberOff(void)
{
	gbAmberLedStatus = 0;
	digitalWrite(gpioLedAmber, gbAmberLedStatus ? HIGH : LOW);
}

void ledRedOn(void)
{
	gbRedLedStatus = 1;
	digitalWrite(gpioLedRed, gbRedLedStatus ? HIGH : LOW);
}
void ledRedOff(void)
{
	if(isFatalError())
	{}	/* red LED permanently on once a fatal error is flagged */
	else
	{
		gbRedLedStatus = 0;
		digitalWrite(gpioLedRed, gbRedLedStatus ? HIGH : LOW);
	}
}


/**** END OF FILE ****/