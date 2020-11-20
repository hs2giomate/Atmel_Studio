/** \file		ledDriverIc.c
 *
 *  \brief		Initialisation data for the button and indicator backlights
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
#include "ledDriverIc_Private_tlc59116.h"

#ifdef LED_DRIVER_TLC59116

LedDriver_t gLedIndicators = 	/* Led Driver IC for the Indicators */
{
	ledIndicatorsSlaveAddress_DriverIc,	/* LED driver slave address */
	ledControl_DriverIc,	/* control */

	{	/* LED Driver IC register default settings */
		0x00,	/* REG_MODE1       */ 	/* Oscillator enable, device does not respond to other I2C addresses */
		0x00,	/* REG_MODE2      */ 	/* Group control = dimming */
		ledDriverPwmInit,	/* REG_PWM0        */
		ledDriverPwmInit,	/* REG_PWM1        */
		ledDriverPwmInit,	/* REG_PWM2        */
		ledDriverPwmInit,	/* REG_PWM3        */
		ledDriverPwmInit,	/* REG_PWM4        */
		ledDriverPwmInit,	/* REG_PWM5        */
		ledDriverPwmInit,	/* REG_PWM6        */
		ledDriverPwmInit,	/* REG_PWM7        */
		ledDriverPwmInit,	/* REG_PWM8        */
		ledDriverPwmInit,	/* REG_PWM9        */
		ledDriverPwmInit,	/* REG_PWM10       */
		ledDriverPwmInit,	/* REG_PWM11       */
		ledDriverPwmInit,	/* REG_PWM12       */
		ledDriverPwmInit,	/* REG_PWM13       */
		ledDriverPwmInit,	/* REG_PWM14       */
		ledDriverPwmOff,	/* REG_PWM15       */
		ledDriverGroupPwmInit,	/* REG_GRPPWM      */
		0x00,	/* REG_GRPFREQ     */
		ledDriverOutState,	/* REG_LEDOUT0     */
		ledDriverOutState,	/* REG_LEDOUT1     */
		ledDriverOutState,	/* REG_LEDOUT2     */
		ledDriverOutState,	/* REG_LEDOUT3     */
	},

};

LedDriver_t gLedButtonsLeftAndCtr = 	/* Led Driver IC for the Left and Centre buttons */
{
	ledButtonsLeftAndCtrSlaveAddress_DriverIc,	/* LED driver slave address */
	ledControl_DriverIc,	/* control */

	{	/* LED Driver IC register default settings */
		0x00,	/* REG_MODE1       */ 	/* Oscillator enable, device does not respond to other I2C addresses */
		0x00,	/* REG_MODE2      */ 	/* Group control = dimming */
		ledDriverPwmInit,	/* REG_PWM0        */
		ledDriverPwmInit,	/* REG_PWM1        */
		ledDriverPwmInit,	/* REG_PWM2        */
		ledDriverPwmInit,	/* REG_PWM3        */
		ledDriverPwmInit,	/* REG_PWM4        */
		ledDriverPwmInit,	/* REG_PWM5        */
		ledDriverPwmInit,	/* REG_PWM6        */
		ledDriverPwmInit,	/* REG_PWM7        */
		ledDriverPwmInit,	/* REG_PWM8        */
		ledDriverPwmOff,	/* REG_PWM9        */
		ledDriverPwmOff,	/* REG_PWM10       */
		ledDriverPwmOff,	/* REG_PWM11       */
		ledDriverPwmOff,	/* REG_PWM12       */
		ledDriverPwmOff,	/* REG_PWM13       */
		ledDriverPwmOff,	/* REG_PWM14       */
		ledDriverPwmOff,	/* REG_PWM15       */
		ledDriverGroupPwmInit,	/* REG_GRPPWM      */
		0x00,	/* REG_GRPFREQ     */
		ledDriverOutState,	/* REG_LEDOUT0     */
		ledDriverOutState,	/* REG_LEDOUT1     */
		ledDriverOutState,	/* REG_LEDOUT2     */
		ledDriverOutState,	/* REG_LEDOUT3     */
	},

};

LedDriver_t gLedButtonsCtrAndRight = 	/* Led Driver IC for the Centre and Right buttons */
{
	ledButtonsCtrAndRightSlaveAddress_DriverIc,	/* LED driver slave address */
	ledControl_DriverIc,	/* control */

	{	/* LED Driver IC register default settings */
		0x00,	/* REG_MODE1       */ 	/* Oscillator enable, device does not respond to other I2C addresses */
		0x00,	/* REG_MODE2      */ 	/* Group control = dimming */
		ledDriverPwmInit,	/* REG_PWM0        */
		ledDriverPwmInit,	/* REG_PWM1        */
		ledDriverPwmInit,	/* REG_PWM2        */
		ledDriverPwmInit,	/* REG_PWM3        */
		ledDriverPwmInit,	/* REG_PWM4        */
		ledDriverPwmInit,	/* REG_PWM5        */
		ledDriverPwmInit,	/* REG_PWM6        */
		ledDriverPwmInit,	/* REG_PWM7        */
		ledDriverPwmInit,	/* REG_PWM8        */
		ledDriverPwmOff,	/* REG_PWM9        */
		ledDriverPwmOff,	/* REG_PWM10       */
		ledDriverPwmOff,	/* REG_PWM11       */
		ledDriverPwmOff,	/* REG_PWM12       */
		ledDriverPwmOff,	/* REG_PWM13       */
		ledDriverPwmOff,	/* REG_PWM14       */
		ledDriverPwmInit,	/* REG_PWM15       */
		ledDriverGroupPwmInit,	/* REG_GRPPWM      */
		0x00,	/* REG_GRPFREQ     */
		ledDriverOutState,	/* REG_LEDOUT0     */
		ledDriverOutState,	/* REG_LEDOUT1     */
		ledDriverOutState,	/* REG_LEDOUT2     */
		ledDriverOutState,	/* REG_LEDOUT3     */
	},

};

LedDriver_t gLedProgressBar[progressBarIcCount] = 	/* Led Driver ICs for Progress Bar */
{
	{
		ledProgress0SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control, auto-increment thru registers 0 - 27 (0x1B) */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */ 	/* Oscillator enable, device does not respond to other I2C addresses */
			0x00,	/* REG_MODE2      */ 	/* Group control = dimming */
			ledDriverPwmInit,	/* REG_PWM0        */
			ledDriverPwmInit,	/* REG_PWM1        */
			ledDriverPwmInit,	/* REG_PWM2        */
			ledDriverPwmInit,	/* REG_PWM3        */
			ledDriverPwmInit,	/* REG_PWM4        */
			ledDriverPwmInit,	/* REG_PWM5        */
			ledDriverPwmInit,	/* REG_PWM6        */
			ledDriverPwmInit,	/* REG_PWM7        */
			ledDriverPwmInit,	/* REG_PWM8        */
			ledDriverPwmInit,	/* REG_PWM9        */
			ledDriverPwmInit,	/* REG_PWM10       */
			ledDriverPwmInit,	/* REG_PWM11       */
			ledDriverPwmInit,	/* REG_PWM12       */
			ledDriverPwmInit,	/* REG_PWM13       */
			ledDriverPwmInit,	/* REG_PWM14       */
			ledDriverPwmOff,	/* REG_PWM15       */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
		},

	},
	{
		ledProgress1SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */ 	/* Oscillator enable, device does not respond to other I2C addresses */
			0x00,	/* REG_MODE2      */ 	/* Group control = dimming */
			ledDriverPwmInit,	/* REG_PWM0        */
			ledDriverPwmInit,	/* REG_PWM1        */
			ledDriverPwmInit,	/* REG_PWM2        */
			ledDriverPwmInit,	/* REG_PWM3        */
			ledDriverPwmInit,	/* REG_PWM4        */
			ledDriverPwmInit,	/* REG_PWM5        */
			ledDriverPwmInit,	/* REG_PWM6        */
			ledDriverPwmInit,	/* REG_PWM7        */
			ledDriverPwmInit,	/* REG_PWM8        */
			ledDriverPwmInit,	/* REG_PWM9        */
			ledDriverPwmInit,	/* REG_PWM10       */
			ledDriverPwmInit,	/* REG_PWM11       */
			ledDriverPwmInit,	/* REG_PWM12       */
			ledDriverPwmInit,	/* REG_PWM13       */
			ledDriverPwmInit,	/* REG_PWM14       */
			ledDriverPwmOff,	/* REG_PWM15       */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
		},

	},
	{
		ledProgress2SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */ 	/* Oscillator enable, device does not respond to other I2C addresses */
			0x00,	/* REG_MODE2      */ 	/* Group control = dimming */
			ledDriverPwmInit,	/* REG_PWM0        */
			ledDriverPwmInit,	/* REG_PWM1        */
			ledDriverPwmInit,	/* REG_PWM2        */
			ledDriverPwmInit,	/* REG_PWM3        */
			ledDriverPwmInit,	/* REG_PWM4        */
			ledDriverPwmInit,	/* REG_PWM5        */
			ledDriverPwmInit,	/* REG_PWM6        */
			ledDriverPwmInit,	/* REG_PWM7        */
			ledDriverPwmInit,	/* REG_PWM8        */
			ledDriverPwmInit,	/* REG_PWM9        */
			ledDriverPwmInit,	/* REG_PWM10       */
			ledDriverPwmInit,	/* REG_PWM11       */
			ledDriverPwmInit,	/* REG_PWM12       */
			ledDriverPwmInit,	/* REG_PWM13       */
			ledDriverPwmInit,	/* REG_PWM14       */
			ledDriverPwmOff,	/* REG_PWM15       */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
		},

	},
	{
		ledProgress3SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */ 	/* Oscillator enable, device does not respond to other I2C addresses */
			0x00,	/* REG_MODE2      */ 	/* Group control = dimming */
			ledDriverPwmInit,	/* REG_PWM0        */
			ledDriverPwmInit,	/* REG_PWM1        */
			ledDriverPwmInit,	/* REG_PWM2        */
			ledDriverPwmInit,	/* REG_PWM3        */
			ledDriverPwmInit,	/* REG_PWM4        */
			ledDriverPwmInit,	/* REG_PWM5        */
			ledDriverPwmInit,	/* REG_PWM6        */
			ledDriverPwmInit,	/* REG_PWM7        */
			ledDriverPwmInit,	/* REG_PWM8        */
			ledDriverPwmInit,	/* REG_PWM9        */
			ledDriverPwmInit,	/* REG_PWM10       */
			ledDriverPwmInit,	/* REG_PWM11       */
			ledDriverPwmInit,	/* REG_PWM12       */
			ledDriverPwmInit,	/* REG_PWM13       */
			ledDriverPwmInit,	/* REG_PWM14       */
			ledDriverPwmOff,	/* REG_PWM15       */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
		},

	}
};

#endif /* LED_DRIVER_TLC59116 */

/*** END of FILE ***/