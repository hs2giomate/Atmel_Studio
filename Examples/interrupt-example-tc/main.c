/*
 * Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries
 *
 * File:            main.c
 * Project:         interrupt-example-tc
 * Date:            July 16, 2018
 * Compiler:        arm-gcc v6.3.1
 * 
 * Basic Interrupt Code Example Project using a Timer Counter Module (TC3)
 *
 * Configure TC3 to trigger an interrupt every 100mS and toggle LED0.
 * 
 * Hardware Configuration: SAMD21 Xplained PRO (ATSAMD21J18A) 
 *
 */

/** INCLUDES ******************************************************************/

#include "sam.h"
#include "definitions.h"

/** MACROS ********************************************************************/

// All application macros are defined in "definitions.h"

/** VARIABLES *****************************************************************/


/** LOCAL PROTOTYPES **********************************************************/

void AppInit(void);				// Application Hardware/Software Initialization

/** main() ********************************************************************/

int main(void){	
    
	/* Initialize the SAM system - auto-generated code */
    SystemInit();
	
	/* Application hardware and software initialization */
	AppInit();

    /* Replace with your application code */
    while(1){
    }
}

/*******************************************************************************
 * Function:        void AppInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine takes care of all of the hardware/software
 *                  initialization that is required.
 *
 * Note:
 *
 ******************************************************************************/
void AppInit(void){
	
	/*	Clock Initialization (CPU, AHB, APBx)
		The System RC Oscillator (OSC8M) provides the source for the main clock
		at chip startup. It is set to 1 MHz. Clock Generator 0 is enabled. */
		
	// Add code here to change the system clock and enable clock generator(s)
	
	/* Disable all IRQs until all application initialization completed */
	__disable_irq();
	
	/* Digital I/O Initialization */	
	PORT->Group[LED0_PORT].DIRSET.reg = LED0_PIN ;								// Assign the LED0 pin as OUTPUT
	PORT->Group[LED0_PORT].OUTSET.reg = LED0_PIN ;								// Set the LED0 pin level, i.e. put to 3.3V -> this turns off the LED

	/* TC3 Initialization (Generate Compare Interrupts every 100mS) */
	
	// Configure asynchronous clock source
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TCC2_TC3_Val;							// select TC3 peripheral channel
	GCLK->CLKCTRL.reg |= GCLK_CLKCTRL_GEN_GCLK0;								// select source GCLK_GEN[0]
	GCLK->CLKCTRL.bit.CLKEN = 1;												// enable generic clock
	
	// Configure synchronous bus clock
	PM->APBCSEL.bit.APBCDIV = 0;												// no prescaler
	PM->APBCMASK.bit.TC3_ = 1;													// enable TC3 interface
	
	// Configure Count Mode (16-bit)
	TC3->COUNT16.CTRLA.bit.MODE = 0x0;
	
	// Configure Prescaler for divide by 2 (500kHz clock to COUNT)
	TC3->COUNT16.CTRLA.bit.PRESCALER = 0x1;
	
	// Configure TC3 Compare Mode for compare channel 0
	TC3->COUNT16.CTRLA.bit.WAVEGEN = 0x1;										// "Match Frequency" operation
	
	// Initialize compare value for 100mS @ 500kHz
	TC3->COUNT16.CC[0].reg = 50000;						
	
	// Enable TC3 compare mode interrupt generation
	TC3->COUNT16.INTENSET.bit.MC0 = 0x1;										// Enable match interrupts on compare channel 0 	
	
	// Enable TC3
	TC3->COUNT16.CTRLA.bit.ENABLE = 1;
	
	// Wait until TC3 is enabled
	while(TC3->COUNT16.STATUS.bit.SYNCBUSY == 1);
	
	/* Configure/Enable Interrupts */
	NVIC_SetPriority(TC3_IRQn, 3);												// Set the interrupt priority to lowest value
	NVIC_EnableIRQ(TC3_IRQn);													// Enable the interrupt
	
	/* Enable all IRQs */
	__enable_irq();
	
}

/*******************************************************************************
 * Function:        void TC3_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This interrupt handler is called on TC3 compare channel 0 event
 *
 * Note:
 *
 ******************************************************************************/
void TC3_Handler(void){
	
	// Toggle LED0
	PORT->Group[LED0_PORT].OUTTGL.reg = LED0_PIN;
	// Acknowledge the interrupt (clear MC0 interrupt flag to re-arm)
	TC3->COUNT16.INTFLAG.reg |= 0b00010000;
}