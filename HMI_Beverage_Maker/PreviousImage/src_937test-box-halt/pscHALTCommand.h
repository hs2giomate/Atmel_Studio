
/** \file		pscHALTCommand.h
 *
 *  \brief		PSC Command Handler. Handles commands targeted at the HMI from the PSC
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 




/*
	Structure of HALT event data collected from the HMI by the test box
*/
typedef struct PscHALTEvent
{
	uint32_t sequenceCount;	/* Incremented by test box before each i2c write transfer to send a command to the HMI */
	uint32_t externalFlashMemSuccessCount;	/* Incremented by HMI */
	uint32_t externalFlashMemFailedCount;	/* Incremented by HMI */
	uint32_t secondsAlive;					/* tracks how many seconds have elapsed since last HMI reset */
	uint16_t temperatureAdcResult;		/* most recent adc result for temperature */
	uint16_t hmiSupplyVoltageAdcResult;	/* most recent adc result for HMI supply voltage (divided by 13) */
	uint16_t vccAdcResult;					/* most recent adc result for Vcc */
	uint8_t lastReasonForReset;				/* all the flags reported at the last reset of the micro */
	uint8_t leftButtonStatus : 1;		/* 0 if button not pressed, 1 if button pressed */
	uint8_t centreButtonStatus : 1;	/* 0 if button not pressed, 1 if button pressed */
	uint8_t rightButtonStatus : 1;	/* 0 if button not pressed, 1 if button pressed*/
} PscHALTEvent_t;

enum ProgressBarSegments_enum
{
	ProgressBarSegmentCount = 20,
};

/*
	Structure of HALT command data sent from test box
*/
typedef struct PscHALTCommand
{
	uint32_t sequenceCount;	/* The test box increments this before each command is sent */
	uint8_t command;	/* command from the test box */
	uint8_t progressBarStep;	/* test box commands the HALT step of the progress bar */
	uint8_t displayStep;	/* test box commands the HALT step of the display */
	uint8_t displayOn;	/* 0 = display OFF, 1 = display ON */ 
	uint8_t indicatorsColourIndex;	/* test box commands the HALT colour of the Indicators */
	uint8_t buttonsColourIndex;	/* test box commands the HALT colour of the buttons */
} PscHALTCommand_t;


enum PscCmd_enum
{
	void_Cmd	= 0,
	HaltCmd = 1,
};

enum HaltDisplayStep_enum
{
	displayRightIsOn_HaltStep = 0,
	displayLeftIsOn_HaltStep = 1,
};

enum HaltProgressBarStep_enum
{
	AllOff_ProgressBarStep = 0,
};

enum HaltBacklightsColourIndex_enum
{
	Off_BacklightColourIndex = 0,
	Red_BacklightColourIndex = 1,
	Green_BacklightColourIndex = 2,
	Blue_BacklightColourIndex = 3,
	White_BacklightColourIndex = 4,
};



/*** END of FILE ***/
