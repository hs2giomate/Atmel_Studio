/** \file		ledDriverIc.h
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


#ifndef LEDDRIVERIC_H
#define LEDDRIVERIC_H

#include "ledDriverIc_Private_pca9955.h"	

#ifdef __cplusplus
extern "C"{
	#endif
	

	
extern LedDriver_t gLedIndicators; 	/* Led Driver IC for the Indicators */
extern LedDriver_t gLedButtonsLeftAndCtr; 	/* Led Driver IC for the Left and Centre buttons */
extern LedDriver_t gLedButtonsCtrAndRight; 	/* Led Driver IC for the Centre and Right buttons */
extern LedDriver_t gLedProgressBar[progressBarIcCount]; 	/* Led Driver ICs for Progress Bar */

static uint8_t gcLedIcIndex;
static uint8_t gc10msTick_Local;

/** \brief Translate the given index to the corresponding instance of LED data.
 *
 *  \return  Nothing
 */
	
	
	
	
	
enum ProgressBarSegments_enum
{
	ProgressBarSegmentCount = 20,
};

enum Intensity_enum
{
	Dim = 8,
	Full = 248,
};

/** \brief Initialise the LED driver module.
 *
 *  \return  Nothing
 */
void ledDriverInit(void);

/** \brief Assert the reset line on all the LED driver ICs.
 *
 *  \return  Nothing
 */
void ledResetHold(void);

/** \brief Set the backlight colour for all the Indicators.
 *
 *  \return  Nothing
 */
void ledSetAllIndicatorColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);

/** \brief Set the backlight colour for all the buttons.
 *
 *  \return  Nothing
 */
void ledSetAllButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);

/** \brief Set the backlight colour for the given segment of the progress bar.
 *
 *  \return  Nothing
 */
//void ledSetProgressSegmentColour(uint8_t cSegment, uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetProgressSegmentColour(uint8_t cSegment, uint8_t cInt, uint8_t cRed, uint8_t cGreen, uint8_t cBlue);

/** \brief Manage updates to the LED driver ICs.
 *
 *  \return  Nothing
 */
void ledManager(void);
static LedDriver_t* ledIndexToRegisterCache(uint8_t cLedIcIndex);

#ifdef __cplusplus
}
#endif

#endif /* LEDDRIVERIC_H */
/*** END of FILE ***/
