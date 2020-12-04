/** \file		ledDriverIc.c
 *
 *  \brief		LED driver for the button and indicator backlights
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
#include "ledDriverIc.h"

#include "hmiI2c.h"
#include "mainTimer.h"

#ifdef LED_DRIVER_PCA9955B


static uint8_t gcLedIcIndex;
static uint8_t gc10msTick_Local;

/** Assert the OLED display reset signal and hold until the display initialisation
	\return Nothing
*/   
void ledResetHold(void)
{
	/* LED-RST */
	/* LED driver ICs, Reset, on PORTF */
	/*          Bit:  7 6 5 4  3 2 1 0 */
	/*    Direction:  - - - -  - - - 1  0=Input, 1=Output */
	/*Initial State:  - - - -  - - - 0 */
	/*	               | | | |  | | | |  bit   */
	/*	               | | | |  | | | +-- b0 : LED-RST, output, Logic 0 = Reset. Initialise in reset */
	/*	               | | | |  | | +---- b1 : */
	/*	               | | | |  | +------ b2 : */
	/*	               | | | |  +-------- b3 : */
	/*	               | | | +----------- b4 : */
	/*	               | | +------------- b5 : */
	/*	               | +--------------- b6 : */
	/*	               +----------------- b7 : */
	PORTF.OUTSET = 0x01;	/* Set output drive to logic 1 */
	PORTF.DIRSET = 0x01;	/* Set outputs */

	/* configured to output, open-drain. There is an external pullup on the PCBA */
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
}



/** Initialise the LED driver
	\return Nothing
*/   
void ledDriverInit(void)
{
	/* LED-RST */
	/* LED driver ICs, Reset, on PORTF */
	/*          Bit:  7 6 5 4  3 2 1 0 */
	/*    Direction:  - - - -  - - - 1  0=Input, 1=Output */
	/*Initial State:  - - - -  - - - 1 */
	/*	               | | | |  | | | |  bit   */
	/*	               | | | |  | | | +-- b0 : LED-RST, output, Logic 1 = Release Reset */
	/*	               | | | |  | | +---- b1 : */
	/*	               | | | |  | +------ b2 : */
	/*	               | | | |  +-------- b3 : */
	/*	               | | | +----------- b4 : */
	/*	               | | +------------- b5 : */
	/*	               | +--------------- b6 : */
	/*	               +----------------- b7 : */
	PORTF.OUTCLR = 0x01;	/* Clear output drive to logic 0 */
}

/** Set the backlight colour of all 5 indicators.
  The colour is set as an RGB model, with each component having a range from 0 to 255
  \param cRed   red component
  \param cGreen green component
  \param cBlue blue component
	\return Nothing
*/   
void ledSetAllIndicatorColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	for(uint8_t cIndicator = 0; cIndicator < IndicatorCount; cIndicator++)
	{
		ledSetIndicatorColour(cIndicator, cRed, cGreen, cBlue);
	}
}

/** Set the backlight colour of one of the 5 indicators.
  Examples of the indicators on the BevMaker HMI are low-water, no-network.
  The colour is set as an RGB model, with each component having a range from 0 to 255
  \param cIndicator    the indicator. Range is 0 to 4. 0 targets the left-most indicator. 4 targets the right-most indicator
  \param cRed   red component
  \param cGreen green component
  \param cBlue blue component
	\return Nothing
*/   
void ledSetIndicatorColour(uint8_t cIndicator, uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	uint8_t cRegisterIndex;
	LedDriver_t* pLedDriver;

	pLedDriver = &gLedIndicators;

	/* Move to the register that sets the constant current of the red LED in the set for the target backlight */
	cRegisterIndex = REG_IREF0;
	//cRegisterIndex = REG_PWM0;
	cRegisterIndex += (cIndicator *ledChannelCountPerBacklight);	/* move to the RGB set for the target indicator */
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
	if(cRed  > ledDriverIref3mA)
	{
		cRed = ledDriverIref3mA;
	}
	if(cGreen  > ledDriverIref3mA)
	{
		cGreen = ledDriverIref3mA;
	}
	if(cBlue  > ledDriverIref3mA)
	{
		cBlue = ledDriverIref3mA;
	}

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
	
		cRegisterIndex = REG_PWM0;
		cRegisterIndex += (cIndicator *ledChannelCountPerBacklight);
		pLedDriver->cRegister[cRegisterIndex] = 255;
		cRegisterIndex++;	/* Advance to next register */
		pLedDriver->cRegister[cRegisterIndex] = 255;
		cRegisterIndex++;	/* Advance to next register */
		pLedDriver->cRegister[cRegisterIndex] = 255;
		cRegisterIndex++;
}

void ledSetIndicatorIntensity(uint8_t cIndicator, uint8_t cInt, uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	uint8_t cRegisterIndex;
	LedDriver_t* pLedDriver;

	pLedDriver = &gLedIndicators;

	/* Move to the register that sets the constant current of the red LED in the set for the target backlight */

	cRegisterIndex = REG_PWM0;
	cRegisterIndex += (cIndicator *ledChannelCountPerBacklight);	/* move to the RGB set for the target indicator */
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
// 	if(cInt  > 240)
// 	{
// 		cInt = 240;
// 	}

	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	
	
	//color
	cRegisterIndex = REG_IREF0;
	cRegisterIndex += (cIndicator *ledChannelCountPerBacklight);	/* move to the RGB set for the target indicator */
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
	if(cRed  > ledDriverIref3mA)
	{
		cRed = ledDriverIref3mA;
	}
	if(cGreen  > ledDriverIref3mA)
	{
		cGreen = ledDriverIref3mA;
	}
	if(cBlue  > ledDriverIref3mA)
	{
		cBlue = ledDriverIref3mA;
	}

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
}

void ledSetIndicatorOff(uint8_t cIndicator)
{
	ledSetIndicatorColour(cIndicator, 0,0,0);
}

/** Set the backlight colour of all the buttons.
  The colour is set as an RGB model, with each component having a range from 0 to 255
  \param cRed   red component
  \param cGreen green component
  \param cBlue blue component
	\return Nothing
*/   
void ledSetAllButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	ledSetButtonColour(ledButton_Left, cRed, cGreen, cBlue);
	ledSetButtonColour(ledButton_Centre, cRed, cGreen, cBlue);
	ledSetButtonColour(ledButton_Right, cRed, cGreen, cBlue);
}

/** Set the backlight colour of one of the buttons.
  The colour is set as an RGB model, with each component having a range from 0 to 255
  \param cButton    the button. Range is 0 to 3. 0 targets the left-most button. 2 targets the right-most button
  \param cRed   red component
  \param cGreen green component
  \param cBlue blue component
	\return Nothing
*/   
void ledSetButtonColour(uint8_t cButton, uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	if(cButton == ledButton_Left)
	{
		ledSetLeftButtonColour(cRed, cGreen, cBlue);
	}
	else if(cButton == ledButton_Centre)
	{
		ledSetCentreButtonColour(cRed, cGreen, cBlue);
	}
	else /*if(cButton == ledButton_Right) */
	{
		ledSetRightButtonColour(cRed, cGreen, cBlue);
	}
}

void ledSetButtonOff(uint8_t cButton)
{
	ledSetButtonColour(cButton, 0,0,0);
}

void ledSetLeftButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	uint8_t cRegisterIndex;
	LedDriver_t* pLedDriver;

	/* The back-lights for each button are arranged on two driver channels */

	pLedDriver = &gLedButtonsLeftAndCtr;

	/* Move to the register index to that sets the constant current of the red LED in the set for the target backlight */
	cRegisterIndex = REG_IREF0;
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
	if(cRed  > ledDriverIref3mA)
	{
		cRed = ledDriverIref3mA;
	}
	if(cGreen  > ledDriverIref3mA)
	{
		cGreen = ledDriverIref3mA;
	}
	if(cBlue  > ledDriverIref3mA)
	{
		cBlue = ledDriverIref3mA;
	}

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
	cRegisterIndex++;	/* Advance to next register */
	
	/* Both channels are set the same */
	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
	
	//cRegisterIndex = driverSegment *ledChannelCountPerBacklight;	/* move to the RGB set for the target  */
	cRegisterIndex = REG_PWM0;
	
	pLedDriver->cRegister[cRegisterIndex] = 255;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = 255;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = 255;
	cRegisterIndex++;
	pLedDriver->cRegister[cRegisterIndex] = 255;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = 255;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = 255;
	cRegisterIndex++;
}

void ledSetLeftButtonColourInt(uint8_t cInt,uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	uint8_t cRegisterIndex;
	LedDriver_t* pLedDriver;

	/* The back-lights for each button are arranged on two driver channels */

	pLedDriver = &gLedButtonsLeftAndCtr;

	/* Move to the register index to that sets the constant current of the red LED in the set for the target backlight */
	cRegisterIndex = REG_IREF0;
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
	if(cRed  > ledDriverIref3mA)
	{
		cRed = ledDriverIref3mA;
	}
	if(cGreen  > ledDriverIref3mA)
	{
		cGreen = ledDriverIref3mA;
	}
	if(cBlue  > ledDriverIref3mA)
	{
		cBlue = ledDriverIref3mA;
	}

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
	cRegisterIndex++;	/* Advance to next register */
	
	/* Both channels are set the same */
	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
	
	//cRegisterIndex = driverSegment *ledChannelCountPerBacklight;	/* move to the RGB set for the target  */
	cRegisterIndex = REG_PWM0;
	
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;
}

void ledSetCentreButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	uint8_t cRegisterIndex;
	LedDriver_t* pLedDriver;

	/* The back-lights for each button are arranged on two driver channels */

	pLedDriver = &gLedButtonsLeftAndCtr;

	/* Move to the register index to that sets the constant current of the red LED in the set for the target backlight */
	cRegisterIndex = REG_IREF0;
	cRegisterIndex += 6;	/* move to the RGB set for the target  */
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
	if(cRed  > ledDriverIref3mA)
	{
		cRed = ledDriverIref3mA;
	}
	if(cGreen  > ledDriverIref3mA)
	{
		cGreen = ledDriverIref3mA;
	}
	if(cBlue  > ledDriverIref3mA)
	{
		cBlue = ledDriverIref3mA;
	}

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;

	/* The 2nd channel for the centre button is connected on a different driver */	
	pLedDriver = &gLedButtonsCtrAndRight;

	/* Move to the register index to that sets the constant current of the red LED in the set for the target backlight */
	cRegisterIndex = REG_IREF0;

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
}

void ledSetRightButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	uint8_t cRegisterIndex;
	LedDriver_t* pLedDriver;

	/* The back-lights for each button are arranged on two driver channels */

	pLedDriver = &gLedButtonsCtrAndRight;

	/* Move to the register index to that sets the constant current of the red LED in the set for the target backlight */
	cRegisterIndex = REG_IREF0;
	cRegisterIndex += 3;	/* move to the RGB set for the target  */
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
	if(cRed  > ledDriverIref3mA)
	{
		cRed = ledDriverIref3mA;
	}
	if(cGreen  > ledDriverIref3mA)
	{
		cGreen = ledDriverIref3mA;
	}
	if(cBlue  > ledDriverIref3mA)
	{
		cBlue = ledDriverIref3mA;
	}

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
	cRegisterIndex++;	/* Advance to next register */
	
	/* Both channels are set the same */
	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
}

void ledSetProgressSegmentColour(uint8_t cSegment, uint8_t cInt, uint8_t cRed, uint8_t cGreen, uint8_t cBlue)
{
	uint8_t cRegisterIndex;
	LedDriver_t* pLedDriver;

	/* The progress bar back-lights are arranged on four driver ICs */
	uint8_t driverIndex = cSegment / ProgressBarSegmentCountPerDriver;

	pLedDriver = &gLedProgressBar[driverIndex];

	/* Move to the register index to that sets the constant current of the red LED in the set for the target backlight */
	uint8_t driverSegment = cSegment % ProgressBarSegmentCountPerDriver;	/* range is 0 to 4 */
	cRegisterIndex = driverSegment *ledChannelCountPerBacklight;	/* move to the RGB set for the target  */
	cRegisterIndex += REG_IREF0;
	
	/* Limit the current to 3mA. This is the max LED current at higher temperatures */
	if(cRed  > ledDriverIref3mA)
	{
		cRed = ledDriverIref3mA;
	}
	if(cGreen  > ledDriverIref3mA)
	{
		cGreen = ledDriverIref3mA;
	}
	if(cBlue  > ledDriverIref3mA)
	{
		cBlue = ledDriverIref3mA;
	}
	

	pLedDriver->cRegister[cRegisterIndex] = cRed;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] =cGreen;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cBlue;
	cRegisterIndex++;	/* Advance to next register */
	
// 	///////////////////////////
	/* Move to the register index to that sets the constant current of the red LED in the set for the target backlight */
	driverSegment = cSegment % ProgressBarSegmentCountPerDriver;	/* range is 0 to 4 */
	cRegisterIndex = driverSegment *ledChannelCountPerBacklight;	/* move to the RGB set for the target  */
	cRegisterIndex += REG_PWM0;
	
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */
	pLedDriver->cRegister[cRegisterIndex] = cInt;
	cRegisterIndex++;	/* Advance to next register */

}

static LedDriver_t* ledIndexToRegisterCache(uint8_t cLedIcIndex)
{
	LedDriver_t* pLedDriver;

	pLedDriver = &gLedIndicators;

	if(cLedIcIndex <= ledIndex_DriverIc_Progress3)
	{
		pLedDriver = &gLedProgressBar[cLedIcIndex]; 	/* Led Driver ICs for Progress Bar */
	}
	else if(cLedIcIndex == ledIndex_DriverIc_Indicators)
	{
		pLedDriver = &gLedIndicators;
	}
	else if(cLedIcIndex == ledIndex_DriverIc_ButtonsLeftAndCtr)
	{
		pLedDriver = &gLedButtonsLeftAndCtr; 	/* Led Driver IC for the Left and Centre buttons */
	}
	else if(cLedIcIndex == ledIndex_DriverIc_ButtonsCtrAndRight)
	{
		pLedDriver = &gLedButtonsCtrAndRight; 	/* Led Driver IC for the Centre and Right buttons */
	}
	else
	{}

	return pLedDriver;
}


/** Manage the LED driver IC settings.
	\return Nothing
*/   
void ledManager(void)
{
	LedDriver_t* pLedDriver;

	
	if(gc10msTick_Local == gc10msTick)
	{}	/* Leave some time between accesses on the I2C, to give the prox sensor access an opportunity */
	else if(hmiI2cReady())
	{	/* HMI I2C bus is free */

		/* get a pointer to the LED IC variable */
		pLedDriver = ledIndexToRegisterCache(gcLedIcIndex);
		/* send data-set to IC */
		hmiI2cWrite(pLedDriver->cSlaveAddress, &(pLedDriver->cControl), gcLedDriverRegisterCount +1);	/* include the control register */

		gcLedIcIndex++;	/* Move on to next driver next time */
		if(gcLedIcIndex > ledIcCount)
		{	/* Transferred data to all ICs */
			gcLedIcIndex = 0;	/* Start again */
		}
		
		gc10msTick_Local = gc10msTick;	/* mark the time of the last access to a LED IC */
	}
	else
	{	/* HMI I2C bus is busy */
		/* try again next time around the main loop */
	}
}

#endif /* LED_DRIVER_PCA9955B */

/*** END of FILE ***/

