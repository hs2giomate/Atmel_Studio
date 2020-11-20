/** \file		pscCommand.c
 *
 *  \brief		process command data from the HALT test box
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#include "pscHALTCommand.h"


/* Global variables */
uint32_t gSequenceCount;	/* The test box increments this before each command is sent */

/* static function prototypes */

/** \brief Process a command received from the test-box.
 *
 *  \return  Nothing
 */


uint32_t pscHALT_GetSequenceCount(void)
{
	return gSequenceCount;
}

void pscHALTProcessDataFromPsc(void)
{
	PscHALTCommand_t	HaltCmd;

	/* Copy the PSC command to a local buffer */
	pscI2cReadCommandData((uint8_t*)&HaltCmd, sizeof(PscHALTCommand_t));

	/* process it */
	pscHALTProcessCmd(&HaltCmd);
}

static void pscHALTProcessCmd(PscHALTCommand_t* pCmd)
{
	uint8_t cCommand = pCmd->cCommand;

	gSequenceCount = pCmd->sequenceCount;	/* The test box increments this before each command is sent */
	if(gSequenceCount == 1)
	{	/* Start of Test */
		/* Reset the payload returned to the test box */
		esfTestRestart();
	}

	pscSetSequenceCount(gSequenceCount);	/* return the sequence count in the payload read by the test box */

	if(cCommand == HaltCmd)
	{
		pscHALTprogressBarStep(pCmd->progressBarStep,255);
		pscHALTdisplay(pCmd->displayOn, pCmd->displayStep);
		pscHALTIndicatorsColourIndex(pCmd->indicatorsColourIndex);
		pscHALTButtonsColourIndex(pCmd->buttonsColourIndex);
	}
	else if(cCommand == Rs485loopBackTestCmd)
	{	/* Perform RS485 LoopBack Test */
		/* The following connections must be made at the HMI connector P301 for this test :
			pin 1 to pin 3 (TXP, RXP)
			pin 2 to pin 4 (TXN, RXN)
		*/
		rs485testEnable(true);
	}
	else if(cCommand == Rs485DisableCmd)
	{	/* Stop RS485 LoopBack Test */
		rs485testEnable(false);
	}
	else if(cCommand == LowPowerTestCmd)
	{	/* enable the low-power Test */
		lowPowerTestEnable(true);
	}
	else if(cCommand == LowPowerDisableCmd)
	{	/* disable the low-power Test */
		lowPowerTestEnable(false);
	}
	else if(cCommand == ProxTestCmd)
	{	/* enable the low-power Test */
		proxTestEnable(true);
	}
	else if(cCommand == ProxDisableCmd)
	{	/* disable the low-power Test */
		proxTestEnable(false);
	}
	else if(cCommand == QuadTestCmd)
	{	/* enable the low-power Test */
		quadTestEnable(true);
	}
	else if(cCommand == QuadDisableCmd)
	{	/* disable the low-power Test */
		quadTestEnable(false);
	}
	else if(cCommand == AmbientLightTestCmd)
	{	/* enable the ambient light sensor Test */
		adcAmbientLightTestEnable(true);
	}
	else if(cCommand == AmbientLightDisableCmd)
	{	/* disable the ambient light sensor Test */
		adcAmbientLightTestEnable(false);
	}
	else
	{}	/* Unrecognised command */
}

/*
	0 = All off
	1 - 20 = number of segments illuminated white

*/
void pscHALTprogressBarStep(uint8_t step, uint8_t cInt)
{
	uint8_t segmentIndex;

	/* Switch on all segments up to the step indicated */
	for(segmentIndex = 0; segmentIndex < step; segmentIndex++)
	{
		uint8_t cRed = 255;
		uint8_t cGreen = 255;
		uint8_t cBlue = 255;
		//ledSetProgressSegmentColour(segmentIndex, cRed, cGreen, cBlue);
		ledSetProgressSegmentColour(segmentIndex, cInt, cRed, cGreen, cBlue);
	}

	/* Switch off all segments following the step indicated */
	for(; segmentIndex < ProgressBarSegmentCount; segmentIndex++)
	{
		uint8_t cRed = 0;
		uint8_t cGreen = 0;
		uint8_t cBlue = 0;
		//ledSetProgressSegmentColour(segmentIndex, cRed, cGreen, cBlue);
		ledSetProgressSegmentColour(segmentIndex, cInt, cRed, cGreen, cBlue);
	}
}

static void pscHALTdisplay(uint8_t onOff, uint8_t step)
{
	if(onOff == 0)
	{	/* switch off display */
		dspOff();
	}
	else
	{
		dspOn();

		if(step == displayRightIsOn_HaltStep)
		{
			dspInverseDisable();
		}
		else if(step == displayLeftIsOn_HaltStep)
		{
			dspInverseEnable();
		}
		else
		{}	/* Unrecognised */
	}
}

static void pscHALTIndicatorsColourIndex(uint8_t index)
{
	uint8_t cRed = 0;
	uint8_t cGreen = 0;
	uint8_t cBlue = 0;

	if(index == Red_BacklightColourIndex)
	{
		cRed = 255;
	}
	else if(index == Green_BacklightColourIndex)
	{
		cGreen = 255;
	}
	else if(index == Blue_BacklightColourIndex)
	{
		cBlue = 255;
	}
	else if(index == White_BacklightColourIndex)
	{
		cRed = 255;
		cGreen = 255;
		cBlue = 255;
	}
	else
	{}	/* Unrecognised */

	ledSetAllIndicatorColour(cRed, cGreen, cBlue);
}


static void pscHALTButtonsColourIndex(uint8_t index)
{
	uint8_t cRed = 0;
	uint8_t cGreen = 0;
	uint8_t cBlue = 0;

	if(index == Red_BacklightColourIndex)
	{
		cRed = 255;
	}
	else if(index == Green_BacklightColourIndex)
	{
		cGreen = 255;
	}
	else if(index == Blue_BacklightColourIndex)
	{
		cBlue = 255;
	}
	else if(index == White_BacklightColourIndex)
	{
		cRed = 255;
		cGreen = 255;
		cBlue = 255;
	}
	else
	{}	/* Unrecognised */

	ledSetAllButtonColour(cRed, cGreen, cBlue);
}

/*** END of FILE ***/
