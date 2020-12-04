/** \file		adc.c
 *
 *  \brief		read various ADC channels
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
#include "adc.h"
#include "adc_Private.h"
#include "mainTimer.h"
#include "pscEvent.h"
#include "display.h"

uint8_t gAdcDiscardSamples;
uint8_t gAdcChannel;


/** \brief Read the ADC calibration data that was stored at the factory.
 *
 *  \return  Nothing
 */
static uint8_t adcReadCalibrationByte(uint8_t index);

/** \brief Change to the next ADC channel in preparation for reading that channel.
 *
 *  \return  Nothing
 */
static void adcAdvanceToNextChannel(void);

/** \brief Setup the ADC for a conversion on the currently active channel.
 *
 *  \return  Nothing
 */
static void adcSetupForNextChannel(void);

/** \brief handle the result of an ADC conversion.
 *
 *  \return  Nothing
 */
static void adcOnResult(uint16_t adcResult);

/** \brief Handle the ambient light sensor test.
 *
 *  \return  Nothing
 */
static void adcTestAmbientLight(uint8_t adcResult);



/** Initialise the ADC
	\return Nothing
*/
void adcInit(void)
{

	/* VS-MON, on PORTB */
	/* Allows measurement of the 12V5 supply voltage using the ADC */

	/* LUX, on PORTB */
	/* Allows measurement of the ambient light level using the ADC */

	/*          Bit: 7 6 5 4  3 2 1 0 */
	/*    Direction: - - - -  - - 0 0  0=Input, 1=Output */
	/*Initial State: - - - -  - - - - */
	/*	              | | | |  | | | |  bit   */
	/*	              | | | |  | | | +-- b0 : VS-MON, input disabled */
	/*	              | | | |  | | +---- b1 : LUX, input disabled */
	/*	              | | | |  | +------ b2 : */
	/*	              | | | |  +-------- b3 : */
	/*	              | | | +----------- b4 : */
	/*	              | | +------------- b5 : */
	/*	              | +--------------- b6 : */
	/*	              +----------------- b7 : */
	PORTB.DIRCLR = 0x03;	/* Clear to input */



	/* Calibration values are stored at production time
	 Load stored bytes into the calibration registers
	 First NVM read is junk and must be thrown away
	 */
	ADCA.CALL = adcReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );
	ADCA.CALH = adcReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );
	ADCA.CALL = adcReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );
	ADCA.CALH = adcReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );

	adcSetupForNextChannel();

	ADCA.CH0.CTRL |= ADC_CH_START_bm;	/* Start a conversion */
}

static uint8_t adcReadCalibrationByte(uint8_t index)
{
	uint8_t result;
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	result = pgm_read_byte(index);

	NVM_CMD = NVM_CMD_NO_OPERATION_gc;

	return(result);
}

static void adcAdvanceToNextChannel(void)
{
	gAdcChannel++;	/* advance to next channel */
	if(gAdcChannel > gAdcHighestValidChannel)
	{
		gAdcChannel = 0;
	}

	adcSetupForNextChannel();
}

static void adcSetupForNextChannel(void)
{
	//////////////////////////////////////////////////////////////////////
	//     7       6       5       4       3       2       1       0
	// |   -   |   -   |   -   |   -   |       |     PRESCALER[2:0]    |
	//     0       0       0       0       0       0       0       0
	// The ADC runs off of the CPU_per clock, 8MHz
	// Set ADC clock to 63kHz:  CPU_per/128    =>    8MHz/128
	ADCA.PRESCALER = ADC_PRESCALER2_bm | ADC_PRESCALER0_bm;
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	//     7       6       5       4       3       2       1       0
	// |   -   |   -   |   -   |   -   |   -   |   -   |   -   | CH0IF |
	//     0       0       0       0       0       0       0       0
	// Ensure the ADC complete flag is cleared (by writing a '1' to it)
	ADCA.INTFLAGS = ADC_CH0IF_bm;
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	//     7       6       5       4       3       2        1         0
	// |   -   |      REFSEL[2:0]      |   -   |   -   | BANDGAP | TEMPREF |
	//     0       0       0       0       0       0        0         0
	// Set Vref to the internal 1V0 bandgap
	// Enable the bandgap
	// Enable the temperature sensor
	ADCA.REFCTRL = ADC_BANDGAP_bm | ADC_TEMPREF_bm;
	//////////////////////////////////////////////////////////////////////

	if(gAdcChannel == gAdcAvccChannel)
	{
		/////////////////////////////////////////////////////////////////////
		//     7        6       5        4        3        2       1       0
		// | START  |   -   |   -   |         GAIN[2:0]        | INPUTMODE[1:0] |
		//     0        0       0        0        0        0       0       0
		// Gain set to 1
		ADCA.CH0.CTRL = 0;	/* Internal signal */

		ADCA.CH0.MUXCTRL = ADC_CH_MUXINT1_bm;	/* 1/10 Scaled Vcc */
	}
	else if(gAdcChannel == gAdcTemperatureChannel)
	{
		/////////////////////////////////////////////////////////////////////
		//     7        6       5        4        3        2       1       0
		// | START  |   -   |   -   |         GAIN[2:0]        | INPUTMODE[1:0] |
		//     0        0       0        0        0        0       0       0
		// Gain set to 1
		ADCA.CH0.CTRL = 0;	/* Internal signal */

		ADCA.CH0.MUXCTRL = 0;	/* Temperature */
	}
	else if(gAdcChannel == gHmiSupplyVoltageChannel)
	{
		//////////////////////////////////////////////////////////////////////
		//     7        6       5        4        3        2       1       0
		// | START  |   -   |   -   |         GAIN[2:0]        | INPUTMODE[1:0] |
		//     0        0       0        0        0        0       0       0
		// Gain set to 1
		ADCA.CH0.CTRL = ADC_CH_INPUTMODE0_bm;

		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS3_bm;		/* ADC8, PB0 */
	}
	else if(gAdcChannel == gAmbientLightChannel)
	{
		//////////////////////////////////////////////////////////////////////
		//     7        6       5        4        3        2       1       0
		// | START  |   -   |   -   |         GAIN[2:0]        | INPUTMODE[1:0] |
		//     0        0       0        0        0        0       0       0
		// Gain set to 1
		ADCA.CH0.CTRL = ADC_CH_INPUTMODE0_bm;

		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS3_bm | ADC_CH_MUXPOS0_bm;		/* ADC9, PB1 */
	}
	else
	{}

	//////////////////////////////////////////////////////////////////////
	//ADCA.CTRLA
	//     7       6       5       4       3         2        1        0
	// |   -   |   -   |   -   |   -   |   -   | CH0START | FLUSH | ENABLE |
	//     0       0       0       0       0         0        0        0
	// Enable ADC
	ADCA.CTRLA = ADC_ENABLE_bm;
	//////////////////////////////////////////////////////////////////////

	gAdcDiscardSamples = 1;	/* Discard the first sample after switching channels */
}

static void adcOnResult(uint16_t adcResult)
{
	if(gAdcChannel == gAdcAvccChannel)
	{
		pscSetVccAdcResult(adcResult);
	}
	else if(gAdcChannel == gAdcTemperatureChannel)
	{
		pscSetTemperatureAdcResult(adcResult);
	}
	else if(gAdcChannel == gHmiSupplyVoltageChannel)
	{
		pscSetHmiSupplyVoltageAdcResult(adcResult);
	}
	else if(gAdcChannel == gAmbientLightChannel)
	{
		adcTestAmbientLight(adcResult);
	}
	else
	{}
}

void adcManager(void)
{
	if((ADCA.INTFLAGS & ADC_CH0IF_bm) != 0)
	{	/* conversion complete */
		ADCA.INTFLAGS = ADC_CH0IF_bm;	/* clear the interrupt flag */

		if(gAdcDiscardSamples > 0)
		{	/* Discard this sample */
			gAdcDiscardSamples--;
		}
		else
		{	/* use this sample */
			uint16_t adcResult = (uint8_t )ADCA.CH0.RES;	/* Read the adc result */
			adcOnResult(adcResult);

			adcAdvanceToNextChannel();		/* take sample on next channel */
		}

		ADCA.CH0.CTRL |= ADC_CH_START_bm;	/* Start a conversion */
	}
}

bool gAmbientLightTestEnabled;
uint8_t gc100msTick_AmbientLight;

static void adcTestAmbientLight(uint8_t adcResult)
{
	if(gAmbientLightTestEnabled)
	{
		/* idle interval between updates */
		if(gc100msTick_AmbientLight == gc100msTick)
		{}
		else
		{
			gc100msTick_AmbientLight = gc100msTick;
			static uint8_t gAmbientLightResult_Old;
			static bool resultOnDisplay = false;

			if(resultOnDisplay)
			{
				uint8_t xPos = gAmbientLightResult_Old >> 1;	/* scale the result to fit the width of the OLED */
				dspShowVerticalLine(xPos, 0);	/* Erase the line at the old position */
			}
			uint8_t xPos = adcResult >> 1;	/* scale the result to fit the width of the OLED */
			dspShowVerticalLine(xPos, 1);	/* draw a line on the OLED corresponding to the result */
			gAmbientLightResult_Old = adcResult;
			resultOnDisplay = true;
		}
	}
}

void adcAmbientLightTestEnable(bool bEnable)
{
	if((gAmbientLightTestEnabled == false) && (bEnable == true))
	{	/* starting the test */
		dspOn();
	}
	dspErase();
	
	gAmbientLightTestEnabled = bEnable;
}


/*** END of FILE ***/
