/** \file		ledDriverIc_Private_pca9955.h
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




#ifndef LEDDRIVERIC_PRIVATE_H
#define LEDDRIVERIC_PRIVATE_H
#ifdef __cplusplus
extern "C"{
	#endif

enum ledIcCount_enum
{
	ledIcCount = 7,
	progressBarIcCount = 4
};	/* Number of LED driver ICs */
enum {gcLedDriverRegisterCount = 0x40};	/* Number of LED IC registers to be accessed */

/*
	Rext = 6k2
	Minimum LED output current = 36.3 uA (when IREF = 1)
	LED current = 3 mA when Iref = 81
	See PCA9955B datasheet for details
*/
//enum ledDrvOutState_enum { ledDriverOutState = 0x55 };	/* PWM and GRPPWM disabled */
enum ledDrvOutState_enum { ledDriverOutState = 0xFF };	/* PWM and GRPPWM enabled */

enum ledDrvIref_enum {
	ledDriverIrefOff = 0,
	ledDriverIref3mA = 81,			/* Max. current 3mA. The Iref resistor value fitted to the prototypes defines the max current at 9.4 mA */
	ledDriverIrefInit = 0,
	};	/* Constant current set at 3 mA */
	
enum ledChannelCountPerBacklight_enum { ledChannelCountPerBacklight = 3 };	/* Number of channels used per back-light, R,G,B = 3*/
enum ProgressBarSegmentCountPerDriver_enum { ProgressBarSegmentCountPerDriver = 5 };	/* Number of Progress Bar Segments per driver IC */
enum IndicatorCount_enum {IndicatorCount = 5 };	/* Number of Indicators */

enum ledDrvGroupPwm_enum {	ledDriverGroupPwmInit = 255,};

enum ledDrvPwm_enum {
	ledDriverPwmOff = 0,
	ledDriverPwmMax = 255,
	ledDriverPwmInit = 0,
	};

/*
	Slave addresses of LED driver ICs.
	Note these are the raw 7-bit slave address, NOT <<1
*/
enum ledDrvSlaveAddr_enum {
	ledIndicatorsSlaveAddress_DriverIc = 0x15,
	ledButtonsLeftAndCtrSlaveAddress_DriverIc = 0x01,
	ledButtonsCtrAndRightSlaveAddress_DriverIc = 0x05,
	ledProgress0SlaveAddress_DriverIc = 0x65,
	ledProgress1SlaveAddress_DriverIc = 0x69,
	ledProgress2SlaveAddress_DriverIc = 0x79,
	ledProgress3SlaveAddress_DriverIc = 0x7D,
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


typedef struct LedDriver
{
	uint8_t cSlaveAddress;		/* 7-bit address. Needs shifting to the left 1 bit before Tx */
	uint8_t cControl;				/* Control */
	uint8_t cRegister[gcLedDriverRegisterCount];	/* One for one copy of the PCA9955B register set from 0 to 0x43 */
} LedDriver_t;

    /* register addresses */
    enum Register {
	REG_MODE1       = 0x00,
	REG_MODE2       = 0x01,
	REG_LEDOUT0     = 0x02,
	REG_LEDOUT1     = 0x03,
	REG_LEDOUT2     = 0x04,
	REG_LEDOUT3     = 0x05,
	REG_GRPPWM      = 0x06,
	REG_GRPFREQ     = 0x07,

	REG_PWM0        = 0x08,
	REG_PWM1        = 0x09,
	REG_PWM2        = 0x0A,
	REG_PWM3        = 0x0B,
	REG_PWM4        = 0x0C,
	REG_PWM5        = 0x0D,
	REG_PWM6        = 0x0E,
	REG_PWM7        = 0x0F,
	REG_PWM8        = 0x10,
	REG_PWM9        = 0x11,
	REG_PWM10       = 0x12,
	REG_PWM11       = 0x13,
	REG_PWM12       = 0x14,
	REG_PWM13       = 0x15,
	REG_PWM14       = 0x16,
	REG_PWM15       = 0x17,

	REG_IREF0       = 0x18,
	REG_IREF1       = 0x19,
	REG_IREF2       = 0x1A,
	REG_IREF3       = 0x1B,
	REG_IREF4       = 0x1C,
	REG_IREF5       = 0x1D,
	REG_IREF6       = 0x1E,
	REG_IREF7       = 0x1F,
	REG_IREF8       = 0x20,
	REG_IREF9       = 0x21,
	REG_IREF10      = 0x22,
	REG_IREF11      = 0x23,
	REG_IREF12      = 0x24,
	REG_IREF13      = 0x25,
	REG_IREF14      = 0x26,
	REG_IREF15      = 0x27,

	REG_RAMP_RATE_GRP0 = 0x28, /* read/write ramp enable and rate control for group 0 */
	REG_STEP_TIME_GRP0 = 0x29, /* read/write step time control for group 0 */
	REG_HOLD_CNTL_GRP0 = 0x2A, /* read/write hold ON/OFF time control for group 0 */
	REG_IREF_GRP0 = 0x2B, /* read/write output gain control for group 0 */

	REG_RAMP_RATE_GRP1 = 0x2C, /* read/write ramp enable and rate control for group 1 */
	REG_STEP_TIME_GRP1 = 0x2D, /* read/write step time control for group 1 */
	REG_HOLD_CNTL_GRP1 = 0x2E, /* read/write hold ON/OFF time control for group 1 */
	REG_IREF_GRP1 = 0x2F, /* read/write output gain control for group 1 */

	REG_RAMP_RATE_GRP2 = 0x30, /* read/write ramp enable and rate control for group 2 */
	REG_STEP_TIME_GRP2 = 0x31, /* read/write step time control for group 2 */
	REG_HOLD_CNTL_GRP2 = 0x32, /* read/write hold ON/OFF time control for group 2 */
	REG_IREF_GRP2 = 0x33, /* read/write output gain control for group 2 */

	REG_RAMP_RATE_GRP3 = 0x34, /* read/write ramp enable and rate control for group 3 */
	REG_STEP_TIME_GRP3 = 0x35, /* read/write step time control for group 3 */
	REG_HOLD_CNTL_GRP3 = 0x36, /* read/write hold ON/OFF time control for group 3 */
	REG_IREF_GRP3 = 0x37, /* read/write output gain control for group 3 */

	REG_GRAD_MODE_SEL0 = 0x38, /* read/write gradation mode select register for channel 7 to channel 0 */
	REG_GRAD_MODE_SEL1 = 0x39, /* read/write gradation mode select register for channel 15 to channel 8 */
	REG_GRAD_GRP_SEL0 = 0x3A, /* read/write gradation group select for channel 3 to channel 0 */
	REG_GRAD_GRP_SEL1 = 0x3B, /* read/write gradation group select for channel 7 to channel 4 */
	REG_GRAD_GRP_SEL2 = 0x3C, /* read/write gradation group select for channel 11 to channel 8 */
	REG_GRAD_GRP_SEL3 = 0x3D, /* read/write gradation group select for channel 15 to channel 12 */
	REG_GRAD_CNTL = 0x3E, /* read/write gradation control register for all four groups */

	REG_OFFSET = 0x3F, /* read/write Offset/delay on LEDn outputs */

	REG_SUBADR1 = 0x40, /* read/write I2C-bus subaddress 1 */
	REG_SUBADR2 = 0x41, /* read/write I2C-bus subaddress 2 */
	REG_SUBADR3 = 0x42, /* read/write I2C-bus subaddress 3 */
	REG_ALLCALLADR = 0x43, /* read/write All Call I2C-bus address */

	REG_PWMALL = 0x44, /* write only brightness control for all LEDn */
	REG_IREFALL = 0x45, /* write only output gain control for all registers IREF0 to IREF15 */
	REG_EFLAG0 = 0x46, /* read only output error flag 0 */
	REG_EFLAG1 = 0x47, /* read only output error flag 1 */
	REG_EFLAG2 = 0x48, /* read only output error flag 2 */
	REG_EFLAG3 = 0x49, /* read only output error flag 3 */
/* 4Ah to 7Fh - - - - - - - reserved read only not used */



    };


void ledSetIndicatorColour(uint8_t cIndicator, uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetIndicatorIntensity(uint8_t cIndicator, uint8_t cInt, uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetButtonColour(uint8_t cButton, uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetLeftButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetLeftButtonColourInt(uint8_t cInt,uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetCentreButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetRightButtonColour(uint8_t cRed, uint8_t cGreen, uint8_t cBlue);
void ledSetIndicatorOff(uint8_t cIndicator);
void ledSetButtonOff(uint8_t cButton);

#ifdef __cplusplus
}
#endif

#endif /* LEDDRIVERIC_PRIVATE_H */


/*** END of FILE ***/