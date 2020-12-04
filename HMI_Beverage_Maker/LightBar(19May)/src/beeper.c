/** \file		beeper.c
 *
 *  \brief		Buzzer drive
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
#include "beeper.h"


enum
{
	BeepPinMask	= PIN1_bm,	/**< Beeper connected on PF1 */
};

/** \brief Switch the beeper on.
 *
 *  \return  Nothing
 */
static void beepOn(void);

/** \brief Switch the beeper off.
 *
 *  \return  Nothing
 */
static void beepOff(void);



/** Initialise the beeper driver
	\return Nothing
*/   
void beepInit(void)
{
	/* BEEP */
	/* Buzzer drive, on PORTF */
	/* Driven via PWM, configure GPIO pin as output, ready for PWM signal */
	/*          Bit: 7 6 5 4  3 2 1 0 */
	/*    Direction: - - - -  - - 1 -  0=Input, 1=Output */
	/*Initial State: - - - -  - - 0 - */
	/*	              | | | |  | | | |  bit   */
	/*	              | | | |  | | | +-- b0 : */
	/*	              | | | |  | | +---- b1 : BEEP, output, OC0B */
	/*	              | | | |  | +------ b2 : */
	/*	              | | | |  +-------- b3 : */
	/*	              | | | +----------- b4 : */
	/*	              | | +------------- b5 : */
	/*	              | +--------------- b6 : */
	/*	              +----------------- b7 : */
	PORTF.OUTCLR = BeepPinMask;	/* Clear output drive to logic 0 */
	beepOff();

	/* configured to output, totem-pole */
	PORTF.PIN1CTRL = PORT_OPC_TOTEM_gc;
	
	/* Setup the timer for PWM, target buzzer frequency is 4kHz */
	TCF0.CTRLA = TC_CLKSEL_DIV2_gc;
	TCF0.CTRLB = TC0_CCBEN_bm | TC_WGMODE_SINGLESLOPE_gc;

	TCF0.PER = 1000;
	TCF0.CCB = 500;
	TCF0.CNT = 0;	
}

static void beepOn(void)
{
	PORTF.DIRSET = BeepPinMask;	/* Set pin as output */
}

static void beepOff(void)
{
	PORTF.DIRCLR = BeepPinMask;	/* Set pin as input */
}


/*** END of FILE ***/