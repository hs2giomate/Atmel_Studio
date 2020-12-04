/** \file		ledDriverIc_Private_tlc59116.h
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


#ifdef LED_DRIVER_TLC59116

#ifndef LEDDRIVERIC_PRIVATE_H
#define LEDDRIVERIC_PRIVATE_H


enum ledIcCount_enum
{
	ledIcCount = 7,
	progressBarIcCount = 4
};	/* Number of LED driver ICs */
enum OledRegCount_enum {gcLedDriverRegisterCount = 24};	/* Number of LED IC registers to be accessed */

enum ledDrvOutState_enum { ledDriverOutState = 0xFF };	/* PWM and GRPPWM enabled */

/*	CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	The average LED current (ie static current plus PWM) must not exceed 3 mA,
		otherwise at higher temperatures the LED will be destroyed.
		
	The Rext value allows a maximum current of 28 mA. This is way too high, 
		it is not the intention to run the LEDs at this current.
		
	The IREF register must be set to the correct value to limit the static LED current 
		to the correct value of 5.1 mA
		
	The global PWM must then be set to limit the average current at 3 mA

	CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
enum ledDrvIref_enum {
/*	ledDriverIrefMax = 0xFF,*/		/* 28 mA CAUTION ! The Iref resistor value fitted to the prototypes defines the max current at 28 mA */
	ledDriverIrefInit = 0x47,		/* 5.1mA . Do NOT use any other setting, or risk destroying the LEDs */
	};

enum ledDrvGroupPwm_enum {	ledDriverGroupPwmInit = 150,};	/* Target is a max. average current of about 3 mA, to keep the power dissipation within acceptable limits at higher temp */
	
enum ledChannelCountPerBacklight_enum { ledChannelCountPerBacklight = 3 };	/* Number of channels used per back-light, R,G,B = 3*/
enum ProgressBarSegmentCountPerDriver_enum { ProgressBarSegmentCountPerDriver = 5 };	/* Number of Progress Bar Segments per driver IC */
enum IndicatorCount_enum {IndicatorCount = 5 };	/* Number of HMI Indicator sites */

enum ledDrvPwm_enum {
	ledDriverPwmOff = 0,
	ledDriverPwmInit = 0,
	};

/*
	Slave addresses of LED driver ICs.
	Note these are the raw 7-bit slave address, NOT <<1
*/
enum ledDrvSlaveAddr_enum {
	ledIndicatorsSlaveAddress_DriverIc = 0x62,
	ledButtonsLeftAndCtrSlaveAddress_DriverIc = 0x60,
	ledButtonsCtrAndRightSlaveAddress_DriverIc = 0x61,
	ledProgress0SlaveAddress_DriverIc = 0x64,
	ledProgress1SlaveAddress_DriverIc = 0x65,
	ledProgress2SlaveAddress_DriverIc = 0x66,
	ledProgress3SlaveAddress_DriverIc = 0x67,
};

/* Index used to track which Driver IC to communicate with next */
enum ledIndexDriverIc_enum {
	ledIndex_DriverIc_Progress0 = 0,
	ledIndex_DriverIc_Progress1 = 1,
	ledIndex_DriverIc_Progress2 = 2,
	ledIndex_DriverIc_Progress3 = 3,
	ledIndex_DriverIc_Indicators = 4,
	ledIndex_DriverIc_ButtonsLeftAndCtr = 5,
	ledIndex_DriverIc_ButtonsCtrAndRight = 6
};


enum ledControl_DriverIc_enum {
	ledControl_DriverIc = 0x80,
};

/* Index used to identify one of the buttons */
enum ledButton_enum {
	ledButton_Left = 0,
	ledButton_Centre = 1,
	ledButton_Right = 2
};

enum {
	ReleaseReset_ledControllerState,
	WriteIref_ledControllerState,
	ReadIref_ledControllerState,
	VerifyIref_ledControllerState,
	WriteRegSet_ledControllerState,
};


typedef struct LedDriver
{
	uint8_t cSlaveAddress;		/* 7-bit address. Needs shifting to the left 1 bit before Tx */
	uint8_t cControl;				/* Control */
	uint8_t cRegister[gcLedDriverRegisterCount];	/* One for one copy of the IC register set */
	uint8_t valueReadBackFromIrefRegister;
} LedDriver_t;

    /* register addresses*/
enum Register
{
	REG_MODE1       = 0x00,
	REG_MODE2       = 0x01,

	REG_PWM0        = 0x02,
	REG_PWM1        = 0x03,
	REG_PWM2        = 0x04,
	REG_PWM3        = 0x05,
	REG_PWM4        = 0x06,
	REG_PWM5        = 0x07,
	REG_PWM6        = 0x08,
	REG_PWM7        = 0x09,
	REG_PWM8        = 0x0A,
	REG_PWM9        = 0x0B,
	REG_PWM10       = 0x0C,
	REG_PWM11       = 0x0D,
	REG_PWM12       = 0x0E,
	REG_PWM13       = 0x0F,
	REG_PWM14       = 0x10,
	REG_PWM15       = 0x11,

	REG_GRPPWM      = 0x12,
	REG_GRPFREQ     = 0x13,

	REG_LEDOUT0     = 0x14,
	REG_LEDOUT1     = 0x15,
	REG_LEDOUT2     = 0x16,
	REG_LEDOUT3     = 0x17,

	REG_SUBADR1 = 0x18, /* read/write I2C-bus subaddress 1 */
	REG_SUBADR2 = 0x19, /* read/write I2C-bus subaddress 2 */
	REG_SUBADR3 = 0x1A, /* read/write I2C-bus subaddress 3 */
	REG_ALLCALLADR = 0x1B, /* read/write All Call I2C-bus address */
	/* The register auto-inc mechanism wraps back to ZERO here */
		/* and does not include the IREF register */

	REG_IREF0       = 0x1C,	/* This register MUST be written to separately */

	REG_EFLAG1 = 0x1D, /* read only output error flag 1 */
	REG_EFLAG2 = 0x1E, /* read only output error flag 2 */
};	/* Led Driver IC register addresses */


void ledSetIndicatorColour(uint8_t cIndicator, uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetButtonColour(uint8_t cButton, uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetLeftButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetCentreButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetRightButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetIndicatorOff(uint8_t cIndicator);
void ledSetButtonOff(uint8_t cButton);



#endif /* LEDDRIVERIC_PRIVATE_H */

#endif /* LED_DRIVER_TLC59116 */
/*** END of FILE ***/