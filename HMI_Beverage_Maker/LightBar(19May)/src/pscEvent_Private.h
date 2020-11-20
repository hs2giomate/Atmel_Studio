/** \file		pscEvent_Private.h
 *
 *  \brief		Provide a collection point for HMI data to be passed to the HALT test box
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 



/* types */

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


/* function prototypes */

/*** END of FILE ***/
