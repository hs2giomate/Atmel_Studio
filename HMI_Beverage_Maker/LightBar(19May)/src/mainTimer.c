/** \file		mainTimer.c
 *
 *  \brief		Support time intervals as required by the rest of the code
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#include "bevMakerHmi.h"
#include "mainTimer.h"
#include "mainTimer_Private.h"
#include "pscEvent.h"

static const uint8_t gcQuarterSecond_PartCount = 25;
static const uint8_t gcTenthSecond_PartCount = 10;

volatile uint8_t gc10msCount;
volatile uint8_t gc100msCount;
volatile uint8_t gc1sCount;
volatile uint8_t gcSubSecondCnt;
volatile uint32_t gdw_LocalmsTimeCount;

uint8_t gcSecondTimer;
uint8_t gcTwoSecondTimer;
uint8_t gcMinuteTimer;
uint8_t gcLastTimeHere_100ms, gcLastTimeHere_250ms;




/** Initialise the main timer which is used for general timing.
	\return Nothing
*/   
void mainTimerInit(void)
{
	/* Set up Timer/Counter 0 .
		Assume CPU clock is 8 MHz
		Target is 1000 interrupts per second (every 1 ms)
		8 MHz / 256 = 31250 Hz
	to work from CPUCLK/64, with period 10000 and
	 */
	TCC0.PER = 800;
	TCC0.CTRLA = ( TCC0.CTRLA & ~TC0_CLKSEL_gm ) | TC_CLKSEL_DIV1_gc;
	
	/* Enable overflow interrupt */
	TCC0.INTCTRLA = ( TCC0.INTCTRLA & ~TC0_OVFINTLVL_gm ) | TC_OVFINTLVL_LO_gc;

}

/** Interrupt handler for the main timer which is used for general timing
	\return Nothing
*/   
ISR(TCC0_OVF_vect)
{	/* Interrupt every 1ms */

	/* Increment a count of ms that can be used elsewhere */
	gdw_LocalmsTimeCount++;	/* Wraps every ~50 days */

	if(++gc10msCount >= 10)
	{	/* Another 10ms */
		gc10msCount = 0;

		/* Counter wraps every 2.56s */
		/* This is READ-ONLY from the main loop */
		gc10msTick++;	/* This is a simple counter to allow the main loop to use small time increments */

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


/** Manage general timing events.
	This is called once from the main loop and manages slower timing that can be handled outside of the interrupt.
	\return Nothing
*/   
void mainTimerService(void)
{
	bool bAnother_100ms;
	bool bAnother_250ms;
	uint32_t dwTemp;

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
	/* read repeatedly until 2 reads the same, to ensure int does not change it in middle of it being copied here */
	dwTemp = 0;
	do
	{
		gdw_msTimeCount = dwTemp;			/* Copy temp to destination */
		dwTemp = gdw_LocalmsTimeCount;	/* Update temp from the src */
	} while(gdw_msTimeCount != dwTemp);	/* check the src has not changed since setting the destination */


	if(bAnother_100ms)
	{	/* Another 100 ms */
	}


	if(bAnother_250ms)
	{	/* Another quarter second */
		if(++gcSecondTimer >= 4)
		{	/* Another second */
			gcSecondTimer = 0;

			/* Track how many seconds the controller has been up for. 32-bit. Can track 136 years */
			gdwSecondsAlive++;
			
			pscSetSecondsAlive(gdwSecondsAlive);


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








/**** END OF FILE ****/
