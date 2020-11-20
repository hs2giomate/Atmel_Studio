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


#ifdef LED_DRIVER_PCA9955B

LedDriver_t gLedIndicators = 	/* Led Driver IC for the Indicators */
{
	ledIndicatorsSlaveAddress_DriverIc,	/* LED driver slave address */
	ledControl_DriverIc,	/* control */

	{	/* LED Driver IC register default settings */
	0x00,	/* REG_MODE1       */
	0x10,	/* REG_MODE2, Clear Error       */
	ledDriverOutState,	/* REG_LEDOUT0     */
	ledDriverOutState,	/* REG_LEDOUT1     */
	ledDriverOutState,	/* REG_LEDOUT2     */
	ledDriverOutState,	/* REG_LEDOUT3     */
	ledDriverGroupPwmInit,	/* REG_GRPPWM      */
	0x00,	/* REG_GRPFREQ     */
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
//	ledDriverIrefInit,	/* REG_IREF0       */
	ledDriverIref3mA,	/* REG_IREF0       */
//	ledDriverIrefInit,	/* REG_IREF1       */
	ledDriverIref3mA,	/* REG_IREF1       */
//	ledDriverIrefInit,	/* REG_IREF2       */
	ledDriverIref3mA,	/* REG_IREF2       */

// 	ledDriverIrefInit,	/* REG_IREF3       */
// 	ledDriverIrefInit,	/* REG_IREF4       */
// 	ledDriverIrefInit,	/* REG_IREF5       */
	ledDriverIref3mA,	/* REG_IREF3       */
	ledDriverIref3mA,	/* REG_IREF4       */
	ledDriverIref3mA,	/* REG_IREF5       */
	
// 	ledDriverIrefInit,	/* REG_IREF6       */
// 	ledDriverIrefInit,	/* REG_IREF7       */
// 	ledDriverIrefInit,	/* REG_IREF8       */
	ledDriverIref3mA,	/* REG_IREF6       */
	ledDriverIref3mA,	/* REG_IREF7       */
	ledDriverIref3mA,	/* REG_IREF8       */
	
// 	ledDriverIrefInit,	/* REG_IREF9       */
// 	ledDriverIrefInit,	/* REG_IREF10      */
// 	ledDriverIrefInit,	/* REG_IREF11      */
		ledDriverIref3mA,	/* REG_IREF9       */
	ledDriverIref3mA,	/* REG_IREF10      */
	ledDriverIref3mA,	/* REG_IREF11      */
	
	ledDriverIrefInit,	/* REG_IREF12      */
	ledDriverIrefInit,	/* REG_IREF13      */
	ledDriverIrefInit,	/* REG_IREF14      */
	ledDriverIrefOff,	/* REG_IREF15      */
	0x00,	/* REG_RAMP_RATE_GRP0 */
	0x00,	/* REG_STEP_TIME_GRP0 */
	0x00,	/* REG_HOLD_CNTL_GRP0 */
	0x00,	/* REG_IREF_GRP0 */
	0x00,	/* REG_RAMP_RATE_GRP1 */
	0x00,	/* REG_STEP_TIME_GRP1 */
	0x00,	/* REG_HOLD_CNTL_GRP1 */
	0x00,	/* REG_IREF_GRP1 */
	0x00,	/* REG_RAMP_RATE_GRP2 */
	0x00,	/* REG_STEP_TIME_GRP2 */
	0x00,	/* REG_HOLD_CNTL_GRP2 */
	0x00,	/* REG_IREF_GRP2 */
	0x00,	/* REG_RAMP_RATE_GRP3 */
	0x00,	/* REG_STEP_TIME_GRP3 */
	0x00,	/* REG_HOLD_CNTL_GRP3 */
	0x00,	/* REG_IREF_GRP3 */
	0x00,	/* REG_GRAD_MODE_SEL0 */
	0x00,	/* REG_GRAD_MODE_SEL1 */
	0x00,	/* REG_GRAD_GRP_SEL0 */
	0x00,	/* REG_GRAD_GRP_SEL1 */
	0x00,	/* REG_GRAD_GRP_SEL2 */
	0x00,	/* REG_GRAD_GRP_SEL3 */
	0x00,	/* REG_GRAD_CNTL */
	0x0F,	/* REG_OFFSET */
	}
};

LedDriver_t gLedButtonsLeftAndCtr = 	/* Led Driver IC for the Left and Centre buttons */
{
	ledButtonsLeftAndCtrSlaveAddress_DriverIc,	/* LED driver slave address */
	ledControl_DriverIc,	/* control */

	{	/* LED Driver IC register default settings */
		0x00,	/* REG_MODE1       */
		0x10,	/* REG_MODE2, Clear Error       */
		ledDriverOutState,	/* REG_LEDOUT0     */
		ledDriverOutState,	/* REG_LEDOUT1     */
		ledDriverOutState,	/* REG_LEDOUT2     */
		ledDriverOutState,	/* REG_LEDOUT3     */
		ledDriverGroupPwmInit,	/* REG_GRPPWM      */
		0x00,	/* REG_GRPFREQ     */
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
		ledDriverIrefInit,	/* REG_IREF0       */
		ledDriverIrefInit,	/* REG_IREF1       */
		ledDriverIrefInit,	/* REG_IREF2       */
		ledDriverIrefInit,	/* REG_IREF3       */
		ledDriverIrefInit,	/* REG_IREF4       */
		ledDriverIrefInit,	/* REG_IREF5       */
		ledDriverIrefInit,	/* REG_IREF6       */
		ledDriverIrefInit,	/* REG_IREF7       */
		ledDriverIrefInit,	/* REG_IREF8       */
		ledDriverIrefOff,	/* REG_IREF9       */
		ledDriverIrefOff,	/* REG_IREF10      */
		ledDriverIrefOff,	/* REG_IREF11      */
		ledDriverIrefOff,	/* REG_IREF12      */
		ledDriverIrefOff,	/* REG_IREF13      */
		ledDriverIrefOff,	/* REG_IREF14      */
		ledDriverIrefOff,	/* REG_IREF15      */
		0x00,	/* REG_RAMP_RATE_GRP0 */
		0x00,	/* REG_STEP_TIME_GRP0 */
		0x00,	/* REG_HOLD_CNTL_GRP0 */
		0x00,	/* REG_IREF_GRP0 */
		0x00,	/* REG_RAMP_RATE_GRP1 */
		0x00,	/* REG_STEP_TIME_GRP1 */
		0x00,	/* REG_HOLD_CNTL_GRP1 */
		0x00,	/* REG_IREF_GRP1 */
		0x00,	/* REG_RAMP_RATE_GRP2 */
		0x00,	/* REG_STEP_TIME_GRP2 */
		0x00,	/* REG_HOLD_CNTL_GRP2 */
		0x00,	/* REG_IREF_GRP2 */
		0x00,	/* REG_RAMP_RATE_GRP3 */
		0x00,	/* REG_STEP_TIME_GRP3 */
		0x00,	/* REG_HOLD_CNTL_GRP3 */
		0x00,	/* REG_IREF_GRP3 */
		0x00,	/* REG_GRAD_MODE_SEL0 */
		0x00,	/* REG_GRAD_MODE_SEL1 */
		0x00,	/* REG_GRAD_GRP_SEL0 */
		0x00,	/* REG_GRAD_GRP_SEL1 */
		0x00,	/* REG_GRAD_GRP_SEL2 */
		0x00,	/* REG_GRAD_GRP_SEL3 */
		0x00,	/* REG_GRAD_CNTL */
		0x0F,	/* REG_OFFSET */
	}
};

LedDriver_t gLedButtonsCtrAndRight = 	/* Led Driver IC for the Centre and Right buttons */
{
	ledButtonsCtrAndRightSlaveAddress_DriverIc,	/* LED driver slave address */
	ledControl_DriverIc,	/* control */

	{	/* LED Driver IC register default settings */
		0x00,	/* REG_MODE1       */
		0x10,	/* REG_MODE2, Clear Error       */
		ledDriverOutState,	/* REG_LEDOUT0     */
		ledDriverOutState,	/* REG_LEDOUT1     */
		ledDriverOutState,	/* REG_LEDOUT2     */
		ledDriverOutState,	/* REG_LEDOUT3     */
		ledDriverGroupPwmInit,	/* REG_GRPPWM      */
		0x00,	/* REG_GRPFREQ     */
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
		ledDriverIrefInit,	/* REG_IREF0       */
		ledDriverIrefInit,	/* REG_IREF1       */
		ledDriverIrefInit,	/* REG_IREF2       */
		ledDriverIrefInit,	/* REG_IREF3       */
		ledDriverIrefInit,	/* REG_IREF4       */
		ledDriverIrefInit,	/* REG_IREF5       */
		ledDriverIrefInit,	/* REG_IREF6       */
		ledDriverIrefInit,	/* REG_IREF7       */
		ledDriverIrefInit,	/* REG_IREF8       */
		ledDriverIrefOff,	/* REG_IREF9       */
		ledDriverIrefOff,	/* REG_IREF10      */
		ledDriverIrefOff,	/* REG_IREF11      */
		ledDriverIrefOff,	/* REG_IREF12      */
		ledDriverIrefOff,	/* REG_IREF13      */
		ledDriverIrefOff,	/* REG_IREF14      */
		ledDriverIrefInit,	/* REG_IREF15      */
		0x00,	/* REG_RAMP_RATE_GRP0 */
		0x00,	/* REG_STEP_TIME_GRP0 */
		0x00,	/* REG_HOLD_CNTL_GRP0 */
		0x00,	/* REG_IREF_GRP0 */
		0x00,	/* REG_RAMP_RATE_GRP1 */
		0x00,	/* REG_STEP_TIME_GRP1 */
		0x00,	/* REG_HOLD_CNTL_GRP1 */
		0x00,	/* REG_IREF_GRP1 */
		0x00,	/* REG_RAMP_RATE_GRP2 */
		0x00,	/* REG_STEP_TIME_GRP2 */
		0x00,	/* REG_HOLD_CNTL_GRP2 */
		0x00,	/* REG_IREF_GRP2 */
		0x00,	/* REG_RAMP_RATE_GRP3 */
		0x00,	/* REG_STEP_TIME_GRP3 */
		0x00,	/* REG_HOLD_CNTL_GRP3 */
		0x00,	/* REG_IREF_GRP3 */
		0x00,	/* REG_GRAD_MODE_SEL0 */
		0x00,	/* REG_GRAD_MODE_SEL1 */
		0x00,	/* REG_GRAD_GRP_SEL0 */
		0x00,	/* REG_GRAD_GRP_SEL1 */
		0x00,	/* REG_GRAD_GRP_SEL2 */
		0x00,	/* REG_GRAD_GRP_SEL3 */
		0x00,	/* REG_GRAD_CNTL */
		0x0F,	/* REG_OFFSET */
	}
};

LedDriver_t gLedProgressBar[progressBarIcCount] = 	/* Led Driver ICs for Progress Bar */
{
	{
		ledProgress0SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */
			0x10,	/* REG_MODE2, Clear Error       */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
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
			ledDriverIrefInit,	/* REG_IREF0       */
			ledDriverIrefInit,	/* REG_IREF1       */
			ledDriverIrefInit,	/* REG_IREF2       */
			ledDriverIrefInit,	/* REG_IREF3       */
			ledDriverIrefInit,	/* REG_IREF4       */
			ledDriverIrefInit,	/* REG_IREF5       */
			ledDriverIrefInit,	/* REG_IREF6       */
			ledDriverIrefInit,	/* REG_IREF7       */
			ledDriverIrefInit,	/* REG_IREF8       */
			ledDriverIrefInit,	/* REG_IREF9       */
			ledDriverIrefInit,	/* REG_IREF10      */
			ledDriverIrefInit,	/* REG_IREF11      */
			ledDriverIrefInit,	/* REG_IREF12      */
			ledDriverIrefInit,	/* REG_IREF13      */
			ledDriverIrefInit,	/* REG_IREF14      */
			ledDriverIrefOff,	/* REG_IREF15      */
			0x00,	/* REG_RAMP_RATE_GRP0 */
			0x00,	/* REG_STEP_TIME_GRP0 */
			0x00,	/* REG_HOLD_CNTL_GRP0 */
			0x00,	/* REG_IREF_GRP0 */
			0x00,	/* REG_RAMP_RATE_GRP1 */
			0x00,	/* REG_STEP_TIME_GRP1 */
			0x00,	/* REG_HOLD_CNTL_GRP1 */
			0x00,	/* REG_IREF_GRP1 */
			0x00,	/* REG_RAMP_RATE_GRP2 */
			0x00,	/* REG_STEP_TIME_GRP2 */
			0x00,	/* REG_HOLD_CNTL_GRP2 */
			0x00,	/* REG_IREF_GRP2 */
			0x00,	/* REG_RAMP_RATE_GRP3 */
			0x00,	/* REG_STEP_TIME_GRP3 */
			0x00,	/* REG_HOLD_CNTL_GRP3 */
			0x00,	/* REG_IREF_GRP3 */
			0x00,	/* REG_GRAD_MODE_SEL0 */
			0x00,	/* REG_GRAD_MODE_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL0 */
			0x00,	/* REG_GRAD_GRP_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL2 */
			0x00,	/* REG_GRAD_GRP_SEL3 */
			0x00,	/* REG_GRAD_CNTL */
			0x0F	/* REG_OFFSET */
		}
	},
	{
		ledProgress1SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */
			0x10,	/* REG_MODE2, Clear Error       */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
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
			ledDriverIrefInit,	/* REG_IREF0       */
			ledDriverIrefInit,	/* REG_IREF1       */
			ledDriverIrefInit,	/* REG_IREF2       */
			ledDriverIrefInit,	/* REG_IREF3       */
			ledDriverIrefInit,	/* REG_IREF4       */
			ledDriverIrefInit,	/* REG_IREF5       */
			ledDriverIrefInit,	/* REG_IREF6       */
			ledDriverIrefInit,	/* REG_IREF7       */
			ledDriverIrefInit,	/* REG_IREF8       */
			ledDriverIrefInit,	/* REG_IREF9       */
			ledDriverIrefInit,	/* REG_IREF10      */
			ledDriverIrefInit,	/* REG_IREF11      */
			ledDriverIrefInit,	/* REG_IREF12      */
			ledDriverIrefInit,	/* REG_IREF13      */
			ledDriverIrefInit,	/* REG_IREF14      */
			ledDriverIrefOff,	/* REG_IREF15      */
			0x00,	/* REG_RAMP_RATE_GRP0 */
			0x00,	/* REG_STEP_TIME_GRP0 */
			0x00,	/* REG_HOLD_CNTL_GRP0 */
			0x00,	/* REG_IREF_GRP0 */
			0x00,	/* REG_RAMP_RATE_GRP1 */
			0x00,	/* REG_STEP_TIME_GRP1 */
			0x00,	/* REG_HOLD_CNTL_GRP1 */
			0x00,	/* REG_IREF_GRP1 */
			0x00,	/* REG_RAMP_RATE_GRP2 */
			0x00,	/* REG_STEP_TIME_GRP2 */
			0x00,	/* REG_HOLD_CNTL_GRP2 */
			0x00,	/* REG_IREF_GRP2 */
			0x00,	/* REG_RAMP_RATE_GRP3 */
			0x00,	/* REG_STEP_TIME_GRP3 */
			0x00,	/* REG_HOLD_CNTL_GRP3 */
			0x00,	/* REG_IREF_GRP3 */
			0x00,	/* REG_GRAD_MODE_SEL0 */
			0x00,	/* REG_GRAD_MODE_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL0 */
			0x00,	/* REG_GRAD_GRP_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL2 */
			0x00,	/* REG_GRAD_GRP_SEL3 */
			0x00,	/* REG_GRAD_CNTL */
			0x0F	/* REG_OFFSET */
		}
	},
	{
		ledProgress2SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */
			0x10,	/* REG_MODE2, Clear Error       */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
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
			ledDriverIrefInit,	/* REG_IREF0       */
			ledDriverIrefInit,	/* REG_IREF1       */
			ledDriverIrefInit,	/* REG_IREF2       */
			ledDriverIrefInit,	/* REG_IREF3       */
			ledDriverIrefInit,	/* REG_IREF4       */
			ledDriverIrefInit,	/* REG_IREF5       */
			ledDriverIrefInit,	/* REG_IREF6       */
			ledDriverIrefInit,	/* REG_IREF7       */
			ledDriverIrefInit,	/* REG_IREF8       */
			ledDriverIrefInit,	/* REG_IREF9       */
			ledDriverIrefInit,	/* REG_IREF10      */
			ledDriverIrefInit,	/* REG_IREF11      */
			ledDriverIrefInit,	/* REG_IREF12      */
			ledDriverIrefInit,	/* REG_IREF13      */
			ledDriverIrefInit,	/* REG_IREF14      */
			ledDriverIrefOff,	/* REG_IREF15      */
			0x00,	/* REG_RAMP_RATE_GRP0 */
			0x00,	/* REG_STEP_TIME_GRP0 */
			0x00,	/* REG_HOLD_CNTL_GRP0 */
			0x00,	/* REG_IREF_GRP0 */
			0x00,	/* REG_RAMP_RATE_GRP1 */
			0x00,	/* REG_STEP_TIME_GRP1 */
			0x00,	/* REG_HOLD_CNTL_GRP1 */
			0x00,	/* REG_IREF_GRP1 */
			0x00,	/* REG_RAMP_RATE_GRP2 */
			0x00,	/* REG_STEP_TIME_GRP2 */
			0x00,	/* REG_HOLD_CNTL_GRP2 */
			0x00,	/* REG_IREF_GRP2 */
			0x00,	/* REG_RAMP_RATE_GRP3 */
			0x00,	/* REG_STEP_TIME_GRP3 */
			0x00,	/* REG_HOLD_CNTL_GRP3 */
			0x00,	/* REG_IREF_GRP3 */
			0x00,	/* REG_GRAD_MODE_SEL0 */
			0x00,	/* REG_GRAD_MODE_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL0 */
			0x00,	/* REG_GRAD_GRP_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL2 */
			0x00,	/* REG_GRAD_GRP_SEL3 */
			0x00,	/* REG_GRAD_CNTL */
			0x0F	/* REG_OFFSET */
		}
	},
	{
		ledProgress3SlaveAddress_DriverIc,	/* LED driver slave address */
		ledControl_DriverIc,	/* control */

		{	/* LED Driver IC register default settings */
			0x00,	/* REG_MODE1       */
			0x10,	/* REG_MODE2, Clear Error       */
			ledDriverOutState,	/* REG_LEDOUT0     */
			ledDriverOutState,	/* REG_LEDOUT1     */
			ledDriverOutState,	/* REG_LEDOUT2     */
			ledDriverOutState,	/* REG_LEDOUT3     */
			ledDriverGroupPwmInit,	/* REG_GRPPWM      */
			0x00,	/* REG_GRPFREQ     */
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
			ledDriverIrefInit,	/* REG_IREF0       */
			ledDriverIrefInit,	/* REG_IREF1       */
			ledDriverIrefInit,	/* REG_IREF2       */
			ledDriverIrefInit,	/* REG_IREF3       */
			ledDriverIrefInit,	/* REG_IREF4       */
			ledDriverIrefInit,	/* REG_IREF5       */
			ledDriverIrefInit,	/* REG_IREF6       */
			ledDriverIrefInit,	/* REG_IREF7       */
			ledDriverIrefInit,	/* REG_IREF8       */
			ledDriverIrefInit,	/* REG_IREF9       */
			ledDriverIrefInit,	/* REG_IREF10      */
			ledDriverIrefInit,	/* REG_IREF11      */
			ledDriverIrefInit,	/* REG_IREF12      */
			ledDriverIrefInit,	/* REG_IREF13      */
			ledDriverIrefInit,	/* REG_IREF14      */
			ledDriverIrefOff,	/* REG_IREF15      */
			0x00,	/* REG_RAMP_RATE_GRP0 */
			0x00,	/* REG_STEP_TIME_GRP0 */
			0x00,	/* REG_HOLD_CNTL_GRP0 */
			0x00,	/* REG_IREF_GRP0 */
			0x00,	/* REG_RAMP_RATE_GRP1 */
			0x00,	/* REG_STEP_TIME_GRP1 */
			0x00,	/* REG_HOLD_CNTL_GRP1 */
			0x00,	/* REG_IREF_GRP1 */
			0x00,	/* REG_RAMP_RATE_GRP2 */
			0x00,	/* REG_STEP_TIME_GRP2 */
			0x00,	/* REG_HOLD_CNTL_GRP2 */
			0x00,	/* REG_IREF_GRP2 */
			0x00,	/* REG_RAMP_RATE_GRP3 */
			0x00,	/* REG_STEP_TIME_GRP3 */
			0x00,	/* REG_HOLD_CNTL_GRP3 */
			0x00,	/* REG_IREF_GRP3 */
			0x00,	/* REG_GRAD_MODE_SEL0 */
			0x00,	/* REG_GRAD_MODE_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL0 */
			0x00,	/* REG_GRAD_GRP_SEL1 */
			0x00,	/* REG_GRAD_GRP_SEL2 */
			0x00,	/* REG_GRAD_GRP_SEL3 */
			0x00,	/* REG_GRAD_CNTL */
			0x0F	/* REG_OFFSET */
		}
	}
};

#endif /* LED_DRIVER_PCA9955B */

/*** END of FILE ***/