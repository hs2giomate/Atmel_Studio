/** \file		initialise.c
 *
 *  \brief		Initialise everything
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
#include "initialise.h"
#include "pscI2c.h"
#include "hmiI2c.h"
#include "adc.h"
#include "proxSensor.h"
#include "display.h"
#include "ledDriverIc.h"
#include "buttons.h"
#include "beeper.h"
#include "lowPower.h"
#include "clock.h"
#include "mainTimer.h"
#include "spi.h"
#include "externalFlashMem.h"
#include "manualResetDetect.h"
#include "reset.h"
#include "pscEvent.h"
#include "quadDecoder.h"


/** \brief Initialise the micro-controller clock system.
 *
 *  \return  Nothing
 */
static void cpuClockInit(void);

/** \brief Initialise the micro-controller watchdog.
 *
 *  \return  Nothing
 */
static void watchdogInit(void);

/** \brief Create a delay after reset, to allow power to settle.
 *
 *  \return  Nothing
 */
static void delayAfterPowerOnReset(void);

/** \brief wait for external devices to be ready after reset. The LED driver ICs require 2 ms after power up to fully wake up.
 *
 *  \return  Nothing
 */
static void delayAfterExternalHardwareReset(void);

/** Initialise everything
	\return Nothing
*/
void systemInitialisation(void)
{
	/* After Power-on-Reset, the XMEGA configures itself to use the internal 2MHz internal oscillator */
	/* The watchdog is enabled, locked and the interval set using the programmed FUSES */

	dspResetHold();	/* Assert the display reset signal early and hold */
	ledResetHold();	/* Assert the LED ICs reset signal early and hold */
	watchdogInit();	/* Watchdog active as soon as possible */
	
	/* delay to give the power time to settle */
	delayAfterPowerOnReset();	
	
	cpuClockInit();	/* Configure the clock source and frequency to be used */
	rstInit();
	dspResetHold();	/* Assert the display reset signal and hold */
	ledResetHold();	/* Assert the LED ICs reset signal early and hold */
	pscI2cInit();
	hmiI2cInit();
	adcInit();	/* forSupplyVoltageMeasurement, AmbientLightLevelMeasurement */
	proximitySensorInit();	/* Configure Prox. Sensor */
	btnInitButtons();
	manualResetDetectInit();
	beepInit();
	lowPowerInit(); /* Standby / low power (off / normal running mode) */
	mainTimerInit();	/* Used for general timing */
	dspInit();
	ledDriverInit();
	spiInit();
	esfInit();
	quadInit();


	
	/* wait for external devices to be ready after reset */
	/* The LED driver ICs require 2 ms after power up to fully wake up */
	delayAfterExternalHardwareReset();

	uint8_t lastReasonForReset = rstGetLastReasonForReset();
	pscSetLastReasonForReset(lastReasonForReset);
	
	
	/* Enable low interrupt level */
	PMIC.CTRL |= PMIC_LOLVLEN_bm;

	/* Enable global interrupts */
	EnableInterrupts();
}

/** Initialise the CPU clock to 8 MHz.
	The internal oscillator is used and at a low frequency to keep EMI to a minimum.
	Nothing on the HMI requires the accuracy of a crystal. There are no UARTs used
	(NOTE the RS485 and bluetooth units use UART comms, but these are not fitted to production PCBA)
	The external xtal is not fitted to production PCBA.
	\return Nothing
*/   
static void cpuClockInit(void)
{
	/* After Power-on-Reset, the XMEGA configures itself to use the internal 2MHz internal oscillator */

	/*  Configure PLL with the 2 MHz RC oscillator as source and
	 *  multiply by 4 to yield 8 MHz PLL clock. Wait
	 *  for it to be stable and set prescaler C to divide by two
	 *  CPU clock = 8 MHz
	 */
	CLKSYS_PLL_Config(OSC_PLLSRC_RC2M_gc, 4);	/* multiply by 4 */
	CLKSYS_Enable(OSC_PLLEN_bm);	/* Enable the clock source */
	CLKSYS_Prescalers_Config(CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc);	/* Set the prescalers to 1. No division */
	
	/* Wait for clock source to be stable */
	while(CLKSYS_IsReady(OSC_PLLRDY_bm) == 0)
	{}
		
	/* Select the clock source */
	CLKSYS_Main_ClockSource_Select(CLK_SCLKSEL_PLL_gc);
}

/** Initialise the Watchdog
	Nothing to do in firmware, The watchdog is enabled, locked and the interval set using the programmed FUSES
	\return Nothing
*/   
static void watchdogInit(void)
{
	/* The watchdog is enabled, locked and the interval set using the programmed FUSES */
}

/** Create a delay after reset
	\return Nothing
*/   
static void delayAfterPowerOnReset(void)
{
	/* Set up Timer/Counter 0 .
		CPU clock is 2 MHz
		prescale / divide by 2 to yield 1 MHz, or 1us timer clock
		Count full 16-bit range yields 65 ms
	 */
	TCD0.PER = 0;
	TCD0.CTRLA = ( TCD0.CTRLA & ~TC0_CLKSEL_gm ) | TC_CLKSEL_DIV2_gc;
	
	/* Wait until the interval has ellapsed */
	while((TCD0.INTFLAGS & TC0_OVFIF_bm) == 0)
	{}

	/* Disable the timer */
	TCD0.CTRLA = ( TCD0.CTRLA & ~TC0_CLKSEL_gm );
	
	/* Clear the overflow flag */
	TCD0.INTFLAGS = TC0_OVFIF_bm;
}


static void delayAfterExternalHardwareReset(void)
{
	/* Set up Timer/Counter 0 .
		CPU clock is 8 MHz
		Count full 16-bit range yields 8 ms
	 */
	TCD0.PER = 0;
	TCD0.CTRLA = ( TCD0.CTRLA & ~TC0_CLKSEL_gm ) | TC_CLKSEL_DIV1_gc;
	
	/* Wait until the interval has ellapsed */
	while((TCD0.INTFLAGS & TC0_OVFIF_bm) == 0)
	{}

	/* Disable the timer */
	TCD0.CTRLA = ( TCD0.CTRLA & ~TC0_CLKSEL_gm );
	
	/* Clear the overflow flag */
	TCD0.INTFLAGS = TC0_OVFIF_bm;
}

/*** END of FILE ***/