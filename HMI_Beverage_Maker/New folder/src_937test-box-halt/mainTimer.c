
/** \file		mainTimer.c
 *
 *  \brief		main timer which is used for general timing
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
#include <time.h>
#include <stdio.h>

#include "mainTimer.h"
#include "mainTimer_Private.h"

const uint8_t gcQuarterSecond_PartCount = 25;
const uint8_t gcTenthSecond_PartCount = 10;

uint8_t gc10msCount;
uint8_t gc100msCount;
uint8_t gc1sCount;
uint8_t gcSubSecondCnt;
uint32_t gdw_LocalmsTimeCount;

uint8_t gcSecondTimer;
uint8_t gcTwoSecondTimer;
uint8_t gcMinuteTimer;
uint8_t gcLastTimeHere_100ms, gcLastTimeHere_250ms;
uint64_t gTickLast;

static uint64_t getTickCount_ms(void);
static void EveryMilliSecond(void);



/** Initialise the main timer which is used for general timing.
	\return Nothing
*/   
void mainTimerInit(void)
{
	printf("\nHALT started at: ");
	printfDateAndTime();
	printf("\n");
}

void GetDateAndTime(char* pBuf, uint8_t cMaxLength)
{
	time_t current_time = time(NULL);
	struct tm *tm = localtime(&current_time);
	strftime(pBuf, cMaxLength, "%c", tm);
}

/* print the current date and time to stdout */
void printfDateAndTime(void)
{
	char cBuf[100]; 
	GetDateAndTime(cBuf, sizeof(cBuf));
	printf("%s", cBuf);
}



/** Interrupt handler for the main timer which is used for general timing
	\return Nothing
*/   
static void EveryMilliSecond(void)
{
	uint64_t tickCurrent;
	
	tickCurrent = getTickCount_ms();
	if(gTickLast != tickCurrent)
	{	/* Another milli-second */
		gTickLast = tickCurrent;

		/* Increment a count of ms that can be used elsewhere */
		gdw_LocalmsTimeCount++;	/* Wraps every ~50 days */

		if(++gc10msCount >= 10)
		{	/* Another 10ms */
			gc10msCount = 0;

			/* Counter wraps every 2.56s */
			/* This is READ-ONLY from the main loop */
			gc10msTick++;	/*  simple counter to allow the main loop to use small time increments */

			if(++gc100msCount >= gcTenthSecond_PartCount)
			{	/* Another 100ms. Counter wraps every 25.6s */
				gc100msCount = 0;
				gc100msTick++;	/* This is a simple counter to allow the main loop to use small time increments
										This is READ-ONLY from the main loop */


				if(++gc1sCount >= 10)
				{	/* Another 1000ms = 1s */
					gc1sCount = 0;
					gc1sTick++;	/* This is a simple counter to allow the main loop to use small time increments
										This is READ-ONLY from the main loop */
				}
			}

			if(++gcSubSecondCnt >= gcQuarterSecond_PartCount)
			{	/* Another quarter second */
				gcSubSecondCnt = 0;
				gc250msTick++;	/* This is a simple counter to allow the main loop to use small time increments
										This is READ-ONLY from the main loop */
			}

		}
	}
}


/** Manage general timing events.
	This is called once from the main loop and manages slower timing that can be handled outside of the interrupt.
	\return Nothing
*/   
void mainTimerService(void)
{
	bool bAnother_100ms;
	bool bAnother_250ms;

	EveryMilliSecond();
	
	bAnother_100ms = false;
	bAnother_250ms = false;

	/* some rather weird looking code in here is most likely due to trying to avoid clashes with the interrupt 
		eg avoiding writing to variables that the interrupt also writes to */
	if(gcLastTimeHere_100ms != gc100msTick)
	{	/* At least 100ms since last check */
		gcLastTimeHere_100ms++;	/* Catch up */
		bAnother_100ms = true;
	}

	if(gcLastTimeHere_250ms != gc250msTick)
	{	/* At least 250ms since last check */
		gcLastTimeHere_250ms++;	/* Catch up */
		bAnother_250ms = true;
	}

	/* gdw_msTimeCount Wraps every ~50 days */
	gdw_msTimeCount = gdw_LocalmsTimeCount;	/* Update temp from the src */


	if(bAnother_100ms)
	{	/* Another 100 ms */
		gdw_100msTimeCount++;	/* wraps every 4971 days, so not an issue for HALT */
	}


	if(bAnother_250ms)
	{	/* Another quarter second */
		if(++gcSecondTimer >= 4)
		{	/* Another second */
			gcSecondTimer = 0;

			/* Track how many seconds the controller has been up for. 32-bit. Can track 136 years */
			gdwSecondsAlive++;

			gcTwoSecondTimer++;
			if(gcTwoSecondTimer >= 2)
			{
				/* Another two-seconds */
				gcTwoSecondTimer = 0;
			}

			if(++gcMinuteTimer >= 60)
			{
				/* Another minute */
				gcMinuteTimer = 0;
			}

		}	/* Another second */
	}
}


static uint64_t getTickCount_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)(ts.tv_nsec / 1000000) + ((uint64_t)ts.tv_sec * 1000ull);
}







/**** END OF FILE ****/
