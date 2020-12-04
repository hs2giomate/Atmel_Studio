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

#include "bevMakerHmi.h"
#include "initialise.h"
#include "mainTimer.h"
#include "pscCommand.h"
#include "display.h"
#include "ledDriverIc.h"
#include "buttons.h"
#include "externalFlashMem.h"
#include "manualResetDetect.h"
#include "adc.h"
#include "rs485test.h"
#include "lowPower.h"
#include "proxSensor.h"
#include "quadDecoder.h"

/** Firmware main entry point   
	\return This function does not return, the firmware cannot have an exit point
*/   
int main(void)
{
	uint32_t oldSec = gdwSecondsAlive;
	bool stateChng = false;
	uint8_t indip=0;
	
	systemInitialisation();
	
	for(;;)	/* main / forever loop */
	{
		/* The watchdog timer must be reset every 1s or the watchdog will reset the micro-controller */
		/* The watchdog is setup and locked by the FUSES */
		__builtin_avr_wdr();	/* Watchdog Timer Reset */
		
		mainTimerService();		/* Update general timers / counters */

		manageManualResetDetect();
// 
// 		esfHALTtestManage();
// 
// 		pscCommandManager();	/* handle any message received from the PSC */

		ledManager();			/* update the LED driver ICs */
//		proxManager();
		
//		adcManager();
//		rs485testManager();
//		quadManager();		//ledSetIndicatorColour(cIndicator, cRed, cGreen, cBlue);// 		ledSetIndicatorColour(0, 10, 10, 10);
// 				ledSetIndicatorColour(1, 50, 50, 50);
// 						ledSetIndicatorColour(2, 120, 120, 120);
// 								ledSetIndicatorColour(3, 200, 200, 200);

/*	ledSetIndicatorIntensity(0, 2);	ledSetIndicatorIntensity(1, 80);
	ledSetIndicatorIntensity(2, 180);
	ledSetIndicatorIntensity(3, 255);
*/			

/*pscHALTprogressBarStep(20,10);*/

//////////////////////////////////////
//START-breathing effect GRADUAL
//////////////////////////////////
// static uint8_t RightSide = 10;
// 	if (gc100msTick != oldSec)
// 	{
// 		gc100msTick = oldSec;
// 		stateChng = !stateChng;
// 		if (stateChng)
// 		{
// 			if (RightSide < 20)
// 			{
// 				RightSide = RightSide + 2;
// 				for(uint8_t segmentIndex= 10; segmentIndex < RightSide; segmentIndex++)
// 				{
// 					ledSetProgressSegmentColour(segmentIndex, 1, 81, 81, 81);
// 				}			
// 			} 
// 			else
// 			{
// 				RightSide = 10;
// 			}
// 		}
// 	}

// static uint8_t RightSide = 10;
// static uint8_t per = 0;
// 	if (gdwSecondsAlive != oldSec)
// 	{
// 		gdwSecondsAlive = oldSec;
// 		stateChng = !stateChng;
// 		if (stateChng)
// 		{
// 			if (RightSide < 20)
// 			{
// 				RightSide = RightSide + 2;
// 				per = per + 2;
// 				for(uint8_t segmentIndex= 10; segmentIndex < RightSide; segmentIndex++)
// 				{
// 					float in= 24*per;
// 					ledSetProgressSegmentColour(segmentIndex, in, 81, 81, 81);
// 				}			
// 			} 
// 			else
// 			{
// 				RightSide = 10;
// 				per = 0;
// 			}
// 		}
// 	}

////////////////Start -with intensity control +1
// static uint8_t RightSide = 10;
// static uint8_t LeftSide = 10;
// static uint8_t per = 0;
// static bool bPattern = 1;
// static uint8_t state = 0;
// static uint8_t LEDinten[10];
// // 	if (gdwSecondsAlive != oldSec)
// // 	{
// // 		gdwSecondsAlive = oldSec;
// 	if (gc10msTick != oldSec)
// 	{
// 		gc10msTick = oldSec;
// 		stateChng = !stateChng;
// 		if (stateChng)
// 		{//RIGHT
// 			if (bPattern)
// 			{
// 				if (LEDinten[0] < 180)
// 				{
// 					LEDinten[0] = LEDinten[0] + 10;
// 				}
// 				else
// 				{
// 					LEDinten[0] = 180;
// 				}
// 				ledSetProgressSegmentColour(9, LEDinten[0], 81, 81, 81); //LED 9
// 				ledSetProgressSegmentColour(10, LEDinten[0], 81, 81, 81);//LED 10
// 				
// 				
// 				
// 				if (LEDinten[1] < 180)
// 				{
// 					LEDinten[1] = LEDinten[1] + 9;
// 				}
// 				else
// 				{
// 					LEDinten[1] = 180;
// 				}
// 				ledSetProgressSegmentColour(8, LEDinten[1], 81, 81, 81); //LED 8
// 				ledSetProgressSegmentColour(11, LEDinten[1], 81, 81, 81);//LED 11
// 				
// 				
// 				
// 				if (LEDinten[2] < 180)
// 				{
// 					LEDinten[2] = LEDinten[2] + 8;
// 				}
// 				else
// 				{
// 					LEDinten[2] = 180;
// 				}
// 				ledSetProgressSegmentColour(7, LEDinten[2], 81, 81, 81); //LED 7
// 				ledSetProgressSegmentColour(12, LEDinten[2], 81, 81, 81);//LED 12
// 				
// 				if (LEDinten[3] < 180)
// 				{
// 					LEDinten[3] = LEDinten[3] + 7;
// 				}
// 				else
// 				{
// 					LEDinten[3] = 180;
// 				}
// 				ledSetProgressSegmentColour(6, LEDinten[3], 81, 81, 81); //LED 6
// 				ledSetProgressSegmentColour(13, LEDinten[3], 81, 81, 81);//LED 13
// 				
// 				if (LEDinten[4] < 180)
// 				{
// 					LEDinten[4] = LEDinten[4] + 6;
// 				}
// 				else
// 				{
// 					LEDinten[4] = 180;
// 				}
// 				ledSetProgressSegmentColour(5, LEDinten[4], 81, 81, 81); //LED 5
// 				ledSetProgressSegmentColour(14, LEDinten[4], 81, 81, 81);//LED 14			
// 				
// 				if (LEDinten[5] < 180)
// 				{
// 					LEDinten[5] = LEDinten[5] + 5;
// 				}
// 				else
// 				{
// 					LEDinten[5] = 180;
// 				}
// 				ledSetProgressSegmentColour(4, LEDinten[5], 81, 81, 81); //LED 4
// 				ledSetProgressSegmentColour(15, LEDinten[5], 81, 81, 81);//LED 15		
// 				
// 				if (LEDinten[6] < 180)
// 				{
// 					LEDinten[6] = LEDinten[6] + 4;
// 				}
// 				else
// 				{
// 					LEDinten[6] = 180;
// 				}
// 				ledSetProgressSegmentColour(3, LEDinten[6], 81, 81, 81); //LED 3
// 				ledSetProgressSegmentColour(16, LEDinten[6], 81, 81, 81);//LED 16		
// 				
// 				if (LEDinten[7] < 180)
// 				{
// 					LEDinten[7] = LEDinten[7] + 3;
// 				}
// 				else
// 				{
// 					LEDinten[7] = 180;
// 				}
// 				ledSetProgressSegmentColour(2, LEDinten[7], 81, 81, 81); //LED 2
// 				ledSetProgressSegmentColour(17, LEDinten[7], 81, 81, 81);//LED 17		
// 				
// 				if (LEDinten[8] < 180)
// 				{
// 					LEDinten[8] = LEDinten[8] + 2;
// 				}
// 				else
// 				{
// 					LEDinten[8] = 180;
// 				}
// 				ledSetProgressSegmentColour(1, LEDinten[8], 81, 81, 81); //LED 1
// 				ledSetProgressSegmentColour(18, LEDinten[8], 81, 81, 81);//LED 18	
// 				
// 				if (LEDinten[9] < 180)
// 				{
// 					LEDinten[9] = LEDinten[9] + 1;
// 				}
// 				else
// 				{
// 					LEDinten[9] = 180;
// 					bPattern = !bPattern;
// 				}
// 				ledSetProgressSegmentColour(0, LEDinten[9], 81, 81, 81); //LED 0
// 				ledSetProgressSegmentColour(19, LEDinten[9], 81, 81, 81);//LED 19														
// 			} 
//  			else
//  			{
// 				if (LEDinten[0] > 10)
// 				{
// 					LEDinten[0] = LEDinten[0] - 1;
// 				}
// 				else
// 				{
// 					LEDinten[0] = 0;
// 										bPattern = !bPattern;
// 				}
// 				ledSetProgressSegmentColour(9, LEDinten[0], 81, 81, 81); //LED 9
// 				ledSetProgressSegmentColour(10, LEDinten[0], 81, 81, 81);//LED 10
// 				
// 				
// 				
// 				if (LEDinten[1] > 10)
// 				{
// 					LEDinten[1] = LEDinten[1] - 2;
// 				}
// 				else
// 				{
// 					LEDinten[1] = 0;
// 				}
// 				ledSetProgressSegmentColour(8, LEDinten[1], 81, 81, 81); //LED 8
// 				ledSetProgressSegmentColour(11, LEDinten[1], 81, 81, 81);//LED 11
// 				
// 				
// 				
// 				if (LEDinten[2] > 10)
// 				{
// 					LEDinten[2] = LEDinten[2] - 3;
// 				}
// 				else
// 				{
// 					LEDinten[2] = 0;
// 				}
// 				ledSetProgressSegmentColour(7, LEDinten[2], 81, 81, 81); //LED 7
// 				ledSetProgressSegmentColour(12, LEDinten[2], 81, 81, 81);//LED 12
// 				
// 				
// 				if (LEDinten[3] > 10)
// 				{
// 					LEDinten[3] = LEDinten[3] - 4;
// 				}
// 				else
// 				{
// 					LEDinten[3] = 0;
// 				}
// 				ledSetProgressSegmentColour(6, LEDinten[3], 81, 81, 81); //LED 6
// 				ledSetProgressSegmentColour(13, LEDinten[3], 81, 81, 81);//LED 13
// 
// 				
// 				if (LEDinten[4] > 10)
// 				{
// 					LEDinten[4] = LEDinten[4] - 5;
// 				}
// 				else
// 				{
// 					LEDinten[4] = 0;
// 				}
// 				ledSetProgressSegmentColour(5, LEDinten[4], 81, 81, 81); //LED 5
// 				ledSetProgressSegmentColour(14, LEDinten[4], 81, 81, 81);//LED 14			
// 				
// 				if (LEDinten[5] > 10)
// 				{
// 					LEDinten[5] = LEDinten[5] - 6;
// 				}
// 				else
// 				{
// 					LEDinten[5] = 0;
// 				}
// 				ledSetProgressSegmentColour(4, LEDinten[5], 81, 81, 81); //LED 4
// 				ledSetProgressSegmentColour(15, LEDinten[5], 81, 81, 81);//LED 15		
// 				
// 				if (LEDinten[6] > 10)
// 				{
// 					LEDinten[6] = LEDinten[6] - 7;
// 				}
// 				else
// 				{
// 					LEDinten[6] = 0;
// 				}
// 				ledSetProgressSegmentColour(3, LEDinten[6], 81, 81, 81); //LED 3
// 				ledSetProgressSegmentColour(16, LEDinten[6], 81, 81, 81);//LED 16		
// 				
// 				if (LEDinten[7] > 10)
// 				{
// 					LEDinten[7] = LEDinten[7] - 8;
// 				}
// 				else
// 				{
// 					LEDinten[7] = 0;
// 				}
// 				ledSetProgressSegmentColour(2, LEDinten[7], 81, 81, 81); //LED 2
// 				ledSetProgressSegmentColour(17, LEDinten[7], 81, 81, 81);//LED 17		
// 				
// 				if (LEDinten[8] >= 10)
// 				{
// 					LEDinten[8] = LEDinten[8] - 9;
// 				}
// 				else
// 				{
// 					LEDinten[8] = 0;
// 				}
// 				ledSetProgressSegmentColour(1, LEDinten[8], 81, 81, 81); //LED 1
// 				ledSetProgressSegmentColour(18, LEDinten[8], 81, 81, 81);//LED 18	
// 				
// 				if (LEDinten[9] > 10)
// 				{
// 					LEDinten[9] = LEDinten[9] - 10;
// 				}
// 				else
// 				{
// 					LEDinten[9] = 0;
// 
// 				}
// 				ledSetProgressSegmentColour(0, LEDinten[9], 81, 81, 81); //LED 0
// 				ledSetProgressSegmentColour(19, LEDinten[9], 81, 81, 81);//LED 19			
//  			}			
// 		}
// 	}
////////////////End -with intensity control

////////////////Start -with intensity control +1
static uint8_t RightSide = 10;
static uint8_t LeftSide = 10;
static uint8_t per = 0;
static bool bPattern = 1;
static uint8_t state = 0;
static float LEDinten[10];
// 	if (gdwSecondsAlive != oldSec)
// 	{
// 		gdwSecondsAlive = oldSec;
	if (gc10msTick != oldSec)
	{
		gc10msTick = oldSec;
		stateChng = !stateChng;
		if (stateChng)
		{//RIGHT
			if (bPattern)
			{
				if (LEDinten[0] < 100)
				{
					LEDinten[0] = LEDinten[0] + 5;
				}
				else
				{
					LEDinten[0] = 100;
				}
				ledSetProgressSegmentColour(9, LEDinten[0], 81, 81, 81); //LED 9
				ledSetProgressSegmentColour(10, LEDinten[0], 81, 81, 81);//LED 10
				
				
				
				if (LEDinten[1] < 100)
				{
					LEDinten[1] = LEDinten[1] + 4.5;
				}
				else
				{
					LEDinten[1] = 100;
				}
				ledSetProgressSegmentColour(8, LEDinten[1], 81, 81, 81); //LED 8
				ledSetProgressSegmentColour(11, LEDinten[1], 81, 81, 81);//LED 11
				
				
				
				if (LEDinten[2] < 100)
				{
					LEDinten[2] = LEDinten[2] + 4;
				}
				else
				{
					LEDinten[2] = 100;
				}
				ledSetProgressSegmentColour(7, LEDinten[2], 81, 81, 81); //LED 7
				ledSetProgressSegmentColour(12, LEDinten[2], 81, 81, 81);//LED 12
				
				if (LEDinten[3] < 100)
				{
					LEDinten[3] = LEDinten[3] + 3.5;
				}
				else
				{
					LEDinten[3] = 100;
				}
				ledSetProgressSegmentColour(6, LEDinten[3], 81, 81, 81); //LED 6
				ledSetProgressSegmentColour(13, LEDinten[3], 81, 81, 81);//LED 13
				
				if (LEDinten[4] < 100)
				{
					LEDinten[4] = LEDinten[4] + 3;
				}
				else
				{
					LEDinten[4] = 100;
				}
				ledSetProgressSegmentColour(5, LEDinten[4], 81, 81, 81); //LED 5
				ledSetProgressSegmentColour(14, LEDinten[4], 81, 81, 81);//LED 14			
				
				if (LEDinten[5] < 100)
				{
					LEDinten[5] = LEDinten[5] + 2.5;
				}
				else
				{
					LEDinten[5] = 100;
				}
				ledSetProgressSegmentColour(4, LEDinten[5], 81, 81, 81); //LED 4
				ledSetProgressSegmentColour(15, LEDinten[5], 81, 81, 81);//LED 15		
				
				if (LEDinten[6] < 100)
				{
					LEDinten[6] = LEDinten[6] + 2;
				}
				else
				{
					LEDinten[6] = 100;
				}
				ledSetProgressSegmentColour(3, LEDinten[6], 81, 81, 81); //LED 3
				ledSetProgressSegmentColour(16, LEDinten[6], 81, 81, 81);//LED 16		
				
				if (LEDinten[7] < 100)
				{
					LEDinten[7] = LEDinten[7] + 1.5;
				}
				else
				{
					LEDinten[7] = 100;
										bPattern = !bPattern;
				}
				ledSetProgressSegmentColour(2, LEDinten[7], 81, 81, 81); //LED 2
				ledSetProgressSegmentColour(17, LEDinten[7], 81, 81, 81);//LED 17		
				
				if (LEDinten[8] < 100)
				{
					LEDinten[8] = LEDinten[8] + 1;
				}
				else
				{
					LEDinten[8] = 100;
				}
				ledSetProgressSegmentColour(1, LEDinten[8], 81, 81, 81); //LED 1
				ledSetProgressSegmentColour(18, LEDinten[8], 81, 81, 81);//LED 18	
				
				if (LEDinten[9] < 100)
				{
					LEDinten[9] = LEDinten[9] + 0.5;
				}
				else
				{
					LEDinten[9] = 100;
/*					bPattern = !bPattern;*/
				}
				ledSetProgressSegmentColour(0, LEDinten[9], 81, 81, 81); //LED 0
				ledSetProgressSegmentColour(19, LEDinten[9], 81, 81, 81);//LED 19														
			} 
 			else
 			{
				if (LEDinten[0] > 10)
				{
					LEDinten[0] = LEDinten[0] - 1;
				}
				else
				{
					LEDinten[0] = 0;
/*										bPattern = !bPattern;*/
				}
				ledSetProgressSegmentColour(9, LEDinten[0], 81, 81, 81); //LED 9
				ledSetProgressSegmentColour(10, LEDinten[0], 81, 81, 81);//LED 10
				
				
				
				if (LEDinten[1] > 10)
				{
					LEDinten[1] = LEDinten[1] - 2;
				}
				else
				{
					LEDinten[1] = 0;
						bPattern = !bPattern;
				}
				ledSetProgressSegmentColour(8, LEDinten[1], 81, 81, 81); //LED 8
				ledSetProgressSegmentColour(11, LEDinten[1], 81, 81, 81);//LED 11
				
				
				
				if (LEDinten[2] > 10)
				{
					LEDinten[2] = LEDinten[2] - 3;
				}
				else
				{
					LEDinten[2] = 0;
				}
				ledSetProgressSegmentColour(7, LEDinten[2], 81, 81, 81); //LED 7
				ledSetProgressSegmentColour(12, LEDinten[2], 81, 81, 81);//LED 12
				
				
				if (LEDinten[3] > 10)
				{
					LEDinten[3] = LEDinten[3] - 4;
				}
				else
				{
					LEDinten[3] = 0;
				}
				ledSetProgressSegmentColour(6, LEDinten[3], 81, 81, 81); //LED 6
				ledSetProgressSegmentColour(13, LEDinten[3], 81, 81, 81);//LED 13

				
				if (LEDinten[4] > 10)
				{
					LEDinten[4] = LEDinten[4] - 5;
				}
				else
				{
					LEDinten[4] = 0;
				}
				ledSetProgressSegmentColour(5, LEDinten[4], 81, 81, 81); //LED 5
				ledSetProgressSegmentColour(14, LEDinten[4], 81, 81, 81);//LED 14			
				
				if (LEDinten[5] > 10)
				{
					LEDinten[5] = LEDinten[5] - 6;
				}
				else
				{
					LEDinten[5] = 0;
				}
				ledSetProgressSegmentColour(4, LEDinten[5], 81, 81, 81); //LED 4
				ledSetProgressSegmentColour(15, LEDinten[5], 81, 81, 81);//LED 15		
				
				if (LEDinten[6] > 10)
				{
					LEDinten[6] = LEDinten[6] - 7;
				}
				else
				{
					LEDinten[6] = 0;
				}
				ledSetProgressSegmentColour(3, LEDinten[6], 81, 81, 81); //LED 3
				ledSetProgressSegmentColour(16, LEDinten[6], 81, 81, 81);//LED 16		
				
				if (LEDinten[7] > 10)
				{
					LEDinten[7] = LEDinten[7] - 8;
				}
				else
				{
					LEDinten[7] = 0;
				}
				ledSetProgressSegmentColour(2, LEDinten[7], 81, 81, 81); //LED 2
				ledSetProgressSegmentColour(17, LEDinten[7], 81, 81, 81);//LED 17		
				
				if (LEDinten[8] >= 10)
				{
					LEDinten[8] = LEDinten[8] - 9;
				}
				else
				{
					LEDinten[8] = 0;
				}
				ledSetProgressSegmentColour(1, LEDinten[8], 81, 81, 81); //LED 1
				ledSetProgressSegmentColour(18, LEDinten[8], 81, 81, 81);//LED 18	
				
				if (LEDinten[9] > 10)
				{
					LEDinten[9] = LEDinten[9] - 10;
				}
				else
				{
					LEDinten[9] = 0;

				}
				ledSetProgressSegmentColour(0, LEDinten[9], 81, 81, 81); //LED 0
				ledSetProgressSegmentColour(19, LEDinten[9], 81, 81, 81);//LED 19			
 			}			
		}
	}
////////////////End -with intensity control
////////////Start -without intensity control +1
// static uint8_t RightSide = 10;
// static uint8_t LeftSide = 10;
// static uint8_t per = 0;
// static bool bPattern = 1;
// 	if (gdwSecondsAlive != oldSec)
// 	{
// 		gdwSecondsAlive = oldSec;
// 	if (gc100msTick != oldSec)
// 	{
// 		gc100msTick = oldSec;
// 		stateChng = !stateChng;
// 		if (stateChng)
// 		{//RIGHT
// 			if (bPattern)
// 			{
// 				if (RightSide < 20)
// 				{
// 					RightSide = RightSide + 1;
// 					per = per + 2;
// 					for(uint8_t segmentIndex= 10; segmentIndex < RightSide; segmentIndex++)
// 					{
// 						ledSetProgressSegmentColour(segmentIndex, 248, 81, 81, 81);
// 					}			
// 				} 
// 				else
// 				{
// 					//RightSide = 10;
// 					per = 0;
// 					bPattern = !bPattern; // Reverse the sequence
// 				}
// 				//LEFT
// 				if (LeftSide > 0)
// 				{
// 					LeftSide = LeftSide - 1;
// 					per = per + 2;
// 					for(uint8_t segmentIndex= 10; segmentIndex > LeftSide; segmentIndex--)
// 					{
// 						ledSetProgressSegmentColour(segmentIndex-1, 248, 81, 81, 81);
// 					}							
// 				} 
// 				else
// 				{
// 					//LeftSide = 10;
// 					per = 0;
// 				}
// 			} 
// 			else
// 			{
// 				if (RightSide > 10)
// 				{
// 					RightSide = RightSide - 1;
// 					per = per + 2;
// 					for(uint8_t segmentIndex= 20; segmentIndex > RightSide; segmentIndex--)
// 					{
// 						ledSetProgressSegmentColour(segmentIndex-1, 248, 0, 0, 0);
// 					}			
// 				} 
// 				else
// 				{
// 					RightSide = 10;
// 					LeftSide = 10;
// 					per = 0;
// 					bPattern = !bPattern; // Reverse the sequence
// 				}
// 				//LEFT
// 				if (LeftSide  < 10)
// 				{
// 					LeftSide = LeftSide +1;
// 					per = per + 2;
// 					for(uint8_t segmentIndex= 0; segmentIndex < LeftSide; segmentIndex++)
// 					{
// 						ledSetProgressSegmentColour(segmentIndex, 248, 0, 0, 0);
// 					}							
// 				} 
// 				else
// 				{
// 					LeftSide = 10;
// 					per = 0;
// 				}
// 			}			
// 		}
// 	}
////////////End -without intensity control

////////////////Start -without intensity control +2
// static uint8_t RightSide = 10;
// static uint8_t LeftSide = 10;
// static uint8_t per = 0;
// static bool bPattern = 1;
// // 	if (gdwSecondsAlive != oldSec)
// // 	{
// // 		gdwSecondsAlive = oldSec;
// if (gc100msTick != oldSec)
// {
// 	gc100msTick = oldSec;
// 	stateChng = !stateChng;
// 	if (stateChng)
// 	{//RIGHT
// 		if (bPattern)
// 		{
// 			if (RightSide < 20)
// 			{
// 				RightSide = RightSide + 2;
// 				per = per + 2;
// 				for(uint8_t segmentIndex= 10; segmentIndex < RightSide; segmentIndex++)
// 				{
// 					ledSetProgressSegmentColour(segmentIndex, 248, 81, 81, 81);
// 				}
// 			}
// 			else
// 			{
// 				//RightSide = 10;
// 				per = 0;
// 				bPattern = !bPattern; // Reverse the sequence
// 			}
// 			//LEFT
// 			if (LeftSide > 0)
// 			{
// 				LeftSide = LeftSide - 2;
// 				per = per + 2;
// 				for(uint8_t segmentIndex= 10; segmentIndex > LeftSide; segmentIndex--)
// 				{
// 					ledSetProgressSegmentColour(segmentIndex-1, 248, 81, 81, 81);
// 				}
// 			}
// 			else
// 			{
// 				//LeftSide = 10;
// 				per = 0;
// 			}
// 		}
// 		else
// 		{
// 			if (RightSide > 10)
// 			{
// 				RightSide = RightSide - 2;
// 				per = per + 2;
// 				for(uint8_t segmentIndex= 20; segmentIndex > RightSide; segmentIndex--)
// 				{
// 					ledSetProgressSegmentColour(segmentIndex-1, 248, 0, 0, 0);
// 				}
// 			}
// 			else
// 			{
// 				RightSide = 10;
// 				LeftSide = 10;
// 				per = 0;
// 				bPattern = !bPattern; // Reverse the sequence
// 			}
// 			//LEFT
// 			if (LeftSide  < 10)
// 			{
// 				LeftSide = LeftSide +2;
// 				per = per + 2;
// 				for(uint8_t segmentIndex= 0; segmentIndex < LeftSide; segmentIndex++)
// 				{
// 					ledSetProgressSegmentColour(segmentIndex, 248, 0, 0, 0);
// 				}
// 			}
// 			else
// 			{
// 				LeftSide = 10;
// 				per = 0;
// 			}
// 		}
// 	}
// }
////////////////End -without intensity control

//low to high
// sta
// for (uint8_t intn = 0;intn <248;)
// {
// 	__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 	mainTimerService();		/* Update general timers / counters */
// 	ledManager();			/* update the LED driver ICs */
// 	if (gc100msTick != oldSec)
// 	{
// 		intn=intn+24;
// 		gc100msTick = oldSec;
// 		stateChng = !stateChng;
// 		if (stateChng)
// 		{
// 			for(uint8_t segmentIndex= 11; segmentIndex < 20; segmentIndex=segmentIndex+2)
// 			{
// 				ledSetProgressSegmentColour(segmentIndex, intn, 81, 81, 81);
// 			}			
// 		}
// 	}
// }

//high to low
// for (uint8_t intn = 248;intn > 8;)
// {
// 	__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 	mainTimerService();		/* Update general timers / counters */
// 	ledManager();			/* update the LED driver ICs */
// 	if (gc10msTick != oldSec)
// 	{
// 		//stage--;
// 		intn=intn-3;
// 		gc10msTick = oldSec;
// 		stateChng = !stateChng;
// 		if (stateChng)
// 		{
// 			pscHALTprogressBarStep(20,intn);
// 		}
// 	}
// }
//////////////////////////////////////
//END-breathing effect GRADUAL
//////////////////////////////////

//////////////////////////////////////
//START-breathing effect ALL
//////////////////////////////////

// low to high			
// 		for (uint8_t intn = 8;intn <248;)
// 		{
// 			__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 			mainTimerService();		/* Update general timers / counters */
// 			ledManager();			/* update the LED driver ICs */
// 			if (gc10msTick != oldSec)
// 			{
// 				intn=intn+2;
// 				gc10msTick = oldSec;
// 				stateChng = !stateChng;
// 				if (stateChng)
// 				{
// 					pscHALTprogressBarStep(20,intn);
// 				}
// 			}
// 		}
// 	high to low	
// 		for (uint8_t intn = 248;intn > 8;)
// 		{
// 			__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 			mainTimerService();		/* Update general timers / counters */
// 			ledManager();			/* update the LED driver ICs */
// 			if (gc10msTick != oldSec)
// 			{
// 				//stage--;
// 				intn=intn-2;
// 				gc10msTick = oldSec;
// 				stateChng = !stateChng;
// 				if (stateChng)
// 				{
// 					pscHALTprogressBarStep(20,intn);
// 				}
// 			}
// 		}
//////////////////////////////////////
//END-breathing effect ALL
//////////////////////////////////

			
//////////////////////////////////////////
// All LED Blinking		
// 	gdwSecondsAlive = oldSec;
// 	for (uint8_t stage = 0;stage <6;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gdwSecondsAlive != oldSec)
// 		{
// 			stage++;
// 			gdwSecondsAlive = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				//pscHALTprogressBarStep(3);
// 				for (uint8_t step=0;step < 20; step++)
// 				{
// 					ledSetProgressSegmentColour(step, 255, 255, 255);
// 				}
// 			}
// 			else
// 			{
// 				//pscHALTprogressBarStep(5);
// 				for (uint8_t step=0;step < 20; step++)
// 				{
// 					ledSetProgressSegmentColour(step, 0, 0, 0);
// 				}
// 			}
// 		}
// 	}
// 
// 
// 

////////////////////////START
	////////white Jumping spot
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 
// 				for (uint8_t step=0; step < 20; step=step+2)
// 				{
// 					if (step == indip)
// 					{
// // 						ledSetProgressSegmentColour(step, 255, 255, 255);
// // 						ledSetProgressSegmentColour(step+1, 255, 255, 255);
// 						//ledSetProgressSegmentColour(step-1,Full, 0, 0, 0);
// 						ledSetProgressSegmentColour(step,Full, 255, 255, 255);
// 						ledSetProgressSegmentColour(step+1,Full, 255, 255, 255);						
// 					}
// 					else
// 					{
// // 						ledSetProgressSegmentColour(step, 0, 0, 0);
// // 						ledSetProgressSegmentColour(step+1, 0, 0, 0);
//   						ledSetProgressSegmentColour(step,Dim, 0, 0, 0);
//   						ledSetProgressSegmentColour(step+1,Dim, 0, 0, 0);					 
// 					}
// 						
// 				}
// 			}
// 			else
// 			{
// 				++indip		;
// 				if (indip >=20)
// 				{
// 					indip=0;
// 				}
// 			}
// 		}
// 	}
// 	//Rolling-reverse
// 	indip=20;
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 			__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				//pscHALTprogressBarStep(3);
// 				for (uint8_t step=20; step > 0; step=step-2)
// 				{
// 					if (step == indip)
// 					{
// // 						ledSetProgressSegmentColour(step-1, 255, 255, 255);
// // 						ledSetProgressSegmentColour(step-2, 255, 255, 255);
// 						ledSetProgressSegmentColour(step-1,Full, 255, 255, 255);
// 						ledSetProgressSegmentColour(step-2,Full, 255, 255, 255);
// 					}
// 					else
// 					{
// // 						ledSetProgressSegmentColour(step-1, 0, 0, 0);
// // 						ledSetProgressSegmentColour(step-2, 0, 0, 0);
// ledSetProgressSegmentColour(step-1,Dim, 0, 0, 0);
// ledSetProgressSegmentColour(step-2,Dim, 0, 0, 0);						
// 					}
// 						
// 				}
// 			}
// 			else
// 			{
// 				--indip		;
// 				if (indip < 0)
// 				{
// 					indip=20;
// 				}
// 			}
// 		}
// 	}
////////////////////////END
	////////White Rolling spot 

//start- white rolling spot
// static uint8_t step=0;
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				if (step == 0)
// 				{
// 
// 					ledSetProgressSegmentColour(step,Full, 255, 255, 255);
// 					ledSetProgressSegmentColour(step+1,Full, 255, 255, 255);
// 					step=step+1;
// 				}
// 				else
// 				{
// 					ledSetProgressSegmentColour(step-1,Full, 0, 0, 0);
// 					ledSetProgressSegmentColour(step,Full, 255, 255, 255);
// 					ledSetProgressSegmentColour(step+1,Full, 255, 255, 255);
// 					step=step+1;
// 				}		
// 			}
// 		}
// 	}
// 		//Reverse-rolling
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				if (step == 0)
// 				{
// 
// 					ledSetProgressSegmentColour(step,Full, 255, 255, 255);
// 					ledSetProgressSegmentColour(step+1,Full, 255, 255, 255);
// 				}
// 				else
// 				{
// 					ledSetProgressSegmentColour(step+1,Full, 0, 0, 0);
// 					ledSetProgressSegmentColour(step,Full, 255, 255, 255);
// 					ledSetProgressSegmentColour(step-1,Full, 255, 255, 255);
// 					step=step-1;
// 				}
// 			}
// 		}
// 	}
//End- white rolling spot

// static uint8_t step=0;
// for (step = 0; step < 20;step++)
// {
// 	ledSetProgressSegmentColour(step,Full ,80, 77, 34);
// }

//start- bright Blue rolling spot with other low intensity
// static uint8_t step=0;
// for (step = 0; step < 20;step++)
// {
// 	ledSetProgressSegmentColour(step-1,Dim, 0, 0, 255);
// }
// step = 0;
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				if (step == 0)
// 				{
// 
// 					ledSetProgressSegmentColour(step,Full, 0, 0, 255);
// 					ledSetProgressSegmentColour(step+1,Full, 0, 0, 255);
// 					step=step+1;
// 				}
// 				else
// 				{
// 					ledSetProgressSegmentColour(step-1,Dim, 0, 0, 255);
// 					ledSetProgressSegmentColour(step,Full, 0, 0, 255);
// 					ledSetProgressSegmentColour(step+1,Full, 0, 0, 255);
// 					step=step+1;
// 				}		
// 			}
// 		}
// 	}
// 		//Reverse-rolling
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				if (step == 0)
// 				{
// 
// 					ledSetProgressSegmentColour(step,Full, 0, 0, 255);
// 					ledSetProgressSegmentColour(step+1,Full, 0, 0, 255);
// 				}
// 				else
// 				{
// 					ledSetProgressSegmentColour(step+1,Dim, 0, 0, 255);
// 					ledSetProgressSegmentColour(step,Full, 0, 0, 255);
// 					ledSetProgressSegmentColour(step-1,Full, 0, 0, 255);
// 					step=step-1;
// 				}
// 			}
// 		}
// 	}
//end- bright Blue rolling spot with other low intensity

//start- bright Amber rolling spot with other low intensity 
// static uint8_t step=0;
// for (step = 0; step < 20;step++)
// {
// 	ledSetProgressSegmentColour(step,Dim, 80,77,34);
// }
// step = 0;
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				if (step == 0)
// 				{
// 
// 					ledSetProgressSegmentColour(step,Full,80,77,34);
// 					ledSetProgressSegmentColour(step+1,Full,80,77,34);
// 					step=step+1;
// 				}
// 				else
// 				{
// 					ledSetProgressSegmentColour(step-1,Dim, 80,77,34);
// 					ledSetProgressSegmentColour(step,Full, 80,77,34);
// 					ledSetProgressSegmentColour(step+1,Full, 80,77,34);
// 					step=step+1;
// 				}		
// 			}
// 		}
// 	}
// 		//Reverse-rolling
// 	for (uint8_t stage2 = 0;stage2 <40;)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc100msTick != oldSec)
// 		{
// 			stage2++;
// 			gc100msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				if (step == 0)
// 				{
// 
// 					ledSetProgressSegmentColour(step,Full,80,77,34);
// 					ledSetProgressSegmentColour(step+1,Full, 80,77,34);
// 				}
// 				else
// 				{
// 					ledSetProgressSegmentColour(step+1,Dim, 80,77,34);
// 					ledSetProgressSegmentColour(step,Full, 80,77,34);
// 					ledSetProgressSegmentColour(step-1,Full, 80,77,34);
// 					step=step-1;
// 				}
// 			}
// 		}
// 	}
//end- bright Amber rolling spot with other low intensity
	
#if 0
//////////////////////START
		////////Blue jumping spot with rest of LED as low intensity 
		for (uint8_t stage2 = 0;stage2 <40;)
		{
			__builtin_avr_wdr();	/* Watchdog Timer Reset */		
			mainTimerService();		/* Update general timers / counters */
			ledManager();			/* update the LED driver ICs */
			if (gc100msTick != oldSec)
			{
				stage2++;
				gc100msTick = oldSec;
				stateChng = !stateChng;
				if (stateChng)
				{
					//pscHALTprogressBarStep(3);
					for (uint8_t step=0; step < 20; step=step+2)
					{
						if (step == indip)
						{
							ledSetProgressSegmentColour(step, 255, 255, 255);
							ledSetProgressSegmentColour(step+1, 255, 255, 255);
							ledSetProgressSegmentColour(step,Full, 0, 0, 255);
							ledSetProgressSegmentColour(step+1,Full, 0, 0, 255);
						}
						else
						{
							ledSetProgressSegmentColour(step, 0, 0, 0);
							ledSetProgressSegmentColour(step+1, 0, 0, 0);
	
							ledSetProgressSegmentColour(step,Dim, 0, 0, 255);
							ledSetProgressSegmentColour(step+1,Dim, 0, 0, 255);
						}
							
					}
				}
				else
				{
					++indip		;
					if (indip >=20)
					{
						indip=0;
					}
				}
			}
		}
	
		//Rolling-reverse
		indip=20;
		for (uint8_t stage2 = 0;stage2 <40;)
		{
				__builtin_avr_wdr();	/* Watchdog Timer Reset */		
			mainTimerService();		/* Update general timers / counters */
			ledManager();			/* update the LED driver ICs */
			if (gc100msTick != oldSec)
			{
				stage2++;
				gc100msTick = oldSec;
				stateChng = !stateChng;
				if (stateChng)
				{
					//pscHALTprogressBarStep(3);
					for (uint8_t step=20; step > 0; step=step-2)
					{
						if (step == indip)
						{
							ledSetProgressSegmentColour(step-1, 255, 255, 255);
							ledSetProgressSegmentColour(step-2, 255, 255, 255);
							ledSetProgressSegmentColour(step-1,Full, 0, 0, 255);
							ledSetProgressSegmentColour(step-2,Full, 0, 0, 255);
						}
						else
						{
							ledSetProgressSegmentColour(step-1, 0, 0, 0);
							ledSetProgressSegmentColour(step-2, 0, 0, 0);
	ledSetProgressSegmentColour(step-1,Dim, 0, 0, 255);
	ledSetProgressSegmentColour(step-2,Dim, 0, 0, 255);						
						}
							
					}
				}
				else
				{
					--indip		;
					if (indip < 0)
					{
						indip=20;
					}
				}
			}
		}
	//////////////////////END
		////////Blue jumping spot with rest of LED as low intensity
		#endif
	
	
// 				gc250msTick = oldSec;
// //for(uint8_t cycle = 0;cycle <15;cycle++)		
// {		
// 	for (uint8_t stage = 20;stage <80;stage=stage+5)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc250msTick != oldSec)
// 		{
// 			stage++;
// 			gc250msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				for (uint8_t step=0;step < 20; step++)
// 				{
// 					ledSetProgressSegmentColour(step, stage, stage, stage);
// 				}
// 			}
// 		}
// 	}
// }

//color
// 				gc250msTick = oldSec;
// for(uint8_t cycle = 0;cycle <15;cycle++)		
// {		
// 	for (uint8_t stage = 0;stage <255;stage=stage+10)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc250msTick != oldSec)
// 		{
// 			stage++;
// 			gc250msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				for (uint8_t step=0;step < 20; step++)
// 				{
// 					ledSetProgressSegmentColour(step, stage, 0, 0);
// 				}
// 			}
// 		}
// 	}
// }
// 				gc250msTick = oldSec;
// for(uint8_t cycle = 0;cycle <15;cycle++)		
// {		
// 	for (uint8_t stage = 0;stage <255;stage=stage+10)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc250msTick != oldSec)
// 		{
// 			stage++;
// 			gc250msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				for (uint8_t step=0;step < 20; step++)
// 				{
// 					ledSetProgressSegmentColour(step, 0,stage, 0);
// 				}
// 			}
// 		}
// 	}
// }
// 
// 				gc250msTick = oldSec;
// for(uint8_t cycle = 0;cycle <15;cycle++)		
// {		
// 	for (uint8_t stage = 0;stage <255;stage=stage+50)
// 	{
// 		__builtin_avr_wdr();	/* Watchdog Timer Reset */		
// 		mainTimerService();		/* Update general timers / counters */
// 		ledManager();			/* update the LED driver ICs */
// 		if (gc250msTick != oldSec)
// 		{
// 			stage++;
// 			gc250msTick = oldSec;
// 			stateChng = !stateChng;
// 			if (stateChng)
// 			{
// 				for (uint8_t step=0;step < 20; step++)
// 				{
// 					ledSetProgressSegmentColour(step, 0,0, stage);
// 				}
// 			}
// 		}
// 	}
// }
	}
}


/*** END of FILE ***/
 