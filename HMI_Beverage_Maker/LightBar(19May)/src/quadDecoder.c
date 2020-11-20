/** \file		quadDecoder.c
 *
 *  \brief		Quadrature Decoder test
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
#include "quadDecoder.h"
#include "display.h"
#include "mainTimer.h"

#define MAXCOUNT 12 // number of steps per encoder revolution

volatile uint8_t PB0;
volatile uint8_t PB1;
volatile uint8_t PBDatOld;
volatile uint8_t StepCount;
volatile uint8_t QuadCount;
uint8_t OldQuad;
volatile uint8_t QDir;

uint8_t QDat[] = {0x06,0x0c,0x03,0x09};


/** \brief Update the quadrature decoder movement after another sample .
 *
 *  \return  Nothing
 */
static void quadUpdate(void);


/** Initialise the quadrature decoder
	\return Nothing
*/
void quadInit(void)
{
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */
	PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc;	/* configured to pull-up on input */
}

bool gQuadTestEnabled;

void quadTestEnable(bool bEnable)
{
	if((gQuadTestEnabled == false) && (bEnable == true))
	{	/* starting the test */
		
		dspOn();
	}
	dspErase();
	
	gQuadTestEnabled = bEnable;
}


uint8_t gcmsTick_QuadDecoder;
uint8_t gQuadCount_Old = 255;

void quadManager(void)
{
	if(gQuadTestEnabled)
	{
		/* Sample the encoder approx every ms */
		if(gcmsTick_QuadDecoder == gdw_msTimeCount)
		{}
		else
		{
			gcmsTick_QuadDecoder = gdw_msTimeCount;
			quadUpdate();

			uint8_t xPos;
			if(gQuadCount_Old != QuadCount)
			{
				if(gQuadCount_Old == 255)
				{}
				else
				{
					xPos = gQuadCount_Old << 3;
					dspShowVerticalLine(xPos, 0);	/* Erase the line at the old position */
				}
				xPos = QuadCount << 3;
				dspShowVerticalLine(xPos, 1);	/* draw a line on the OLED corresponding to the quadrature encoder position */
				gQuadCount_Old = QuadCount;
			}
		}
	}
}


void quadUpdate(void)
{
	uint8_t PBDat;
	
	PB0 = PB0 << 1;    //this is part of the filter
	PB1 = PB1 << 1;   //it makes room for the bit we are about to read
	PB0 = (PORTE.IN & PIN5_bm) ?PB0 | 1:PB0;//B input - if the pin of PortB.0 is high then OR the variable with 1
	PB1 = (PORTE.IN & PIN6_bm) ?PB1 | 1:PB1;//A input -  if the pin of PortB.1 is high then OR the variable with 1	
	
	//AND our input variables with 0x07 (three 1's) and see if we get a value of 7 (all 1's) or 0 (all 0's)
	if(((PB0 & 0x07) == 0x07 || (PB0 & 0x07) == 0 ) || ((PB1 & 0x07) == 0x07 || (PB1 & 0x07) == 0 ))
	{
		PBDat = (PB0 & 1) | (PB1 & 2);
		if((QDat[PBDatOld] & 0x03) == PBDat)
		{
			PBDatOld = PBDat;
			if(StepCount == 0)
			{
				StepCount = 4;
				QDir = 0;
				if(QuadCount > 0)
				{
					QuadCount--;
				}
			}
			StepCount--;
		}
		else if(((QDat[PBDatOld] >> 2) & 0x03) == PBDat)
		{
			PBDatOld = PBDat;
			StepCount++;
			if(StepCount > 3)
			{
				StepCount = 0;
				QDir = 1;
				if(QuadCount < MAXCOUNT)
				{
					QuadCount++;
				}
			}
		}
	}
}

/*** END of FILE ***/
