/** \file		main.c
 *
 *  \brief		main loop
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include "i2cbusses.h"
#include "../version.h"

#include "main.h"
#include "bevMakerTest.h"
#include "mainTimer.h"
#include "led.h"
#include "keyscan.h"
#include "hmiManualReset.h"

const uint8_t gcI2cAddress_BevMakerHmi = 0x42;

/* global flag used to exit from the main loop */
bool gbRun = true;

bool gbFatalError = false;



/** \brief main loop
 *
 *  \return  Nothing
 */
int main(void)
{
	int i2cFileHandle = -1;
	int i2cbus = 1;
	char filename[20];
	bool bOK = true;


	mainTimerInit();
	printfDateAndTime();
	printf("\n");
	printf("\nUsage: ");
	printf("\nPress <Esc> to finish the test");
	printf("\nPress <space bar> to pause/resume the test\n");

	ledInit();
	hmrInit();

	i2cFileHandle = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
	if(i2cFileHandle >= 0)
	{	/* Successfully opened I2C */

		/* Set HMI I2C slave address */
		if(ioctl(i2cFileHandle, I2C_SLAVE, gcI2cAddress_BevMakerHmi) < 0) 
		{
			bOK = false;
			close(i2cFileHandle);
			fprintf(stderr, "Error: Could not set I2C slave address to 0x%02x: %s\n", (unsigned)gcI2cAddress_BevMakerHmi, strerror(errno));
			OnFatalError();
		}
	}
	else
	{
		bOK = false;
		fprintf(stderr, "\nFailed to open I2C bus");
		OnFatalError();
	}
	
	/* Setup stdin to allow detection of keystrokes for pausing the process */
	changemode(1);

	if(bOK)
	{
		/* Run the HALT */
		runHalt(i2cFileHandle);
	}
	
	if(i2cFileHandle >= 0)
	{
		close(i2cFileHandle);
	}

	
	if(gbFatalError)
	{
		/* The red LED will be on until the program ends */
		fprintf(stderr, "\nFatal Error. Press Esc key to end\n");
		for(gbRun = true; gbRun; )
		{	/* Wait for Escape key to be pressed */
			if(kbhit())
			{
				int ch = getchar();
				if(ch == gcEscapeKey)
				{	/* Escape key pressed */
					/* Exit the program */
					gbRun = false;
				}
				else
				{}	/* No other key presses acknowledged */
			}
		}
	}

	changemode(0);
	
	ledAllOff();
	
	printf("\nHALT stopped\n");
	exit(0);
}

bool IsRun(void)
{
	return gbRun;
}

void setRun(bool bRun)
{
	gbRun = bRun;
}


void OnFatalError(void)
{
	ledAllOff();
	gbRun = false;
	ledRedOn();
	
	/* Set the fatal error flag last, because the red LED is always
		on once the flag is set */
	gbFatalError = true;
}

bool isFatalError(void)
{
	return gbFatalError;
}


/**** END OF FILE ****/