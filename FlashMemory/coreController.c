/*--------------------------------------------------------------------------

coreController.c

This file is part of e.Development

Implementation
low level interface to the SAMD21

$Author: steffen $
$Date: 2017-11-27 11:39:44 +0100 (Mo, 27 Nov 2017) $
$Revision: 1618 $
$HeadURL: https://svn.s2embedded.at/customers/hs2-engineering/trunk/HiLo/Firmware/Libraries/Cortex/e.Core/coreController.c $

--------------------------------------------------------------------------*/
#include "sam.h"
#include "coreController.h"


//	Frequency of the board main oscillator
#define VARIANT_MAINOSC		(32768ul)
//	Master clock frequency
#define VARIANT_MCK			(48000000ul)

// Constants for Clock multiplexers
#define GENERIC_CLOCK_MULTIPLEXER_DFLL48M (0u)


void coreControllerSetupClocks(void)
	{
#if (F_CPU == 48000000UL)
	//	Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet
 	 NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;
#endif

	//	Turn on the digital interface clock
	PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

	//	----------------------------------------------------------------------------------------------
	//	1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)
	//	----------------------------------------------------------------------------------------------
	SYSCTRL->XOSC32K.reg =
							SYSCTRL_XOSC32K_STARTUP(0x6u)
							| SYSCTRL_XOSC32K_RUNSTDBY
							| SYSCTRL_XOSC32K_XTALEN
							| SYSCTRL_XOSC32K_EN32K;
	SYSCTRL->XOSC32K.bit.ENABLE = 1;

	//	Wait for oscillator stabilization
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY) == 0)
		;

	//	Software reset the module to ensure it is re-initialized correctly
	//	Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
	//	CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete, as described in chapter 13.8.1
 	GCLK->CTRL.reg = GCLK_CTRL_SWRST;

	//	Wait for reset to complete
	while ((GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY))
		;

#if (F_CPU == 48000000UL)
	//	----------------------------------------------------------------------------------------------
	//	Put XOSC32K as source of Generic Clock Generator 1
	//	----------------------------------------------------------------------------------------------
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_XOSC32K); // Generic Clock Generator 1

	//	Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;

	//	Write Generic Clock Generator 1 configuration
	GCLK->GENCTRL.reg =
							// Generic Clock Generator 1
							GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_XOSC32K)
							// Selected source is External 32KHz Oscillator
                      		| GCLK_GENCTRL_SRC_XOSC32K
							// Output clock to a pin for tests
							//| GCLK_GENCTRL_OE
                      		| GCLK_GENCTRL_GENEN
							;

	//	Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;

	//	----------------------------------------------------------------------------------------------
	//	Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
	//	----------------------------------------------------------------------------------------------
	GCLK->CLKCTRL.reg =
							// Generic Clock Multiplexer 0
							GCLK_CLKCTRL_ID(GENERIC_CLOCK_MULTIPLEXER_DFLL48M)
							// Generic Clock Generator 1 is source
                      		| GCLK_CLKCTRL_GEN_GCLK1
                      		| GCLK_CLKCTRL_CLKEN
							;

	//	Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;

	//	----------------------------------------------------------------------------------------------
	//	Enable DFLL48M clock
	//	----------------------------------------------------------------------------------------------

	//	DFLL Configuration in Closed Loop mode, cf product datasheet chapter 15.6.7.1 - Closed-Loop Operation
	//	Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905
	SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;

	//	Wait for synchronization
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0)
		;

	SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP(31) | // Coarse step is 31, half of the max value
                         SYSCTRL_DFLLMUL_FSTEP(511) | // Fine step is 511, half of the max value
                         SYSCTRL_DFLLMUL_MUL((VARIANT_MCK/VARIANT_MAINOSC)); // External 32KHz is the reference

	//	Wait for synchronization
  	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0)
		;

	//	Write full configuration to DFLL control register
	SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | // Enable the closed loop mode
                           SYSCTRL_DFLLCTRL_WAITLOCK |
                           SYSCTRL_DFLLCTRL_QLDIS; // Disable Quick lock

	//	Wait for synchronization
  	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0)
		;

	//	Enable the DFLL
	SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE;

	//	Wait for locks flags
	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 ||
          (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0)
		{
		}

	//	Wait for synchronization
  	while ((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0)
		;

	//	----------------------------------------------------------------------------------------------
	//	Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
	//	----------------------------------------------------------------------------------------------
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_MAIN); // Generic Clock Generator 0

	//	Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;

	//	Write Generic Clock Generator 0 configuration
	GCLK->GENCTRL.reg =
						GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_MAIN) | // Generic Clock Generator 0
                      	GCLK_GENCTRL_SRC_DFLL48M | // Selected source is DFLL 48MHz
//                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
                      	GCLK_GENCTRL_IDC | // Set 50/50 duty cycle
                      	GCLK_GENCTRL_GENEN;

	//	Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;
#endif

#if (F_CPU != 1000000UL)
	//	----------------------------------------------------------------------------------------------
	//	Modify PRESCaler value of OSC8M to have 8MHz
	//	----------------------------------------------------------------------------------------------
	SYSCTRL->OSC8M.bit.PRESC = SYSCTRL_OSC8M_PRESC_0_Val;
	SYSCTRL->OSC8M.bit.ONDEMAND = 0;
	SYSCTRL->OSC8M.bit.RUNSTDBY = 1;
#endif
	//	----------------------------------------------------------------------------------------------
	//	Setup Generic Clock Generator 2 for WDT
	//	----------------------------------------------------------------------------------------------
	//	GCLK 2: 1024 Hz
	GCLK->GENDIV.reg =
					GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_OSCULP32K)
					| GCLK_GENDIV_DIV(0x04);	//	DIV32
	// 0x00000402;				//	DIV 32 on GCLK 2
	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
		;
	
	//	Write Generic Clock Generator 2 configuration
	GCLK->GENCTRL.reg =
						// Generic Clock Generator 2
						GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_OSCULP32K)
						//	Selected source is ULP 32k (already enabled at reset)
                      	| GCLK_GENCTRL_SRC_OSCULP32K
						//	Output clock to a pin for tests
						//	| GCLK_GENCTRL_OE
						| GCLK_GENCTRL_DIVSEL
                      	| GCLK_GENCTRL_GENEN
						;

	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
		;

	//	----------------------------------------------------------------------------------------------
	//	Put OSC8M as source for Generic Clock Generator 3
	//	----------------------------------------------------------------------------------------------
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_OSC8M); // Generic Clock Generator 3

	//	Write Generic Clock Generator 3 configuration
	GCLK->GENCTRL.reg =
						// Generic Clock Generator 3
						GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_OSC8M)
						//	Selected source is RC OSC 8MHz (already enabled at reset)
                      	| GCLK_GENCTRL_SRC_OSC8M
						//	Output clock to a pin for tests
						//	| GCLK_GENCTRL_OE
                      	| GCLK_GENCTRL_GENEN
						;

	//	Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;

	//	----------------------------------------------------------------------------------------------
	//	Put XOSC32K as source for Generic Clock Generator 4
	//	----------------------------------------------------------------------------------------------
 	//	GCLK 4: 1024 Hz for RTC and EIC
	GCLK->GENDIV.reg =
					GCLK_GENDIV_ID(GENERIC_CLOCK_GENERATOR_XOSC_RTC)
					| GCLK_GENDIV_DIV(0x04);	//	DIV32
	GCLK->GENCTRL.reg =
							// Generic Clock Generator 4
							GCLK_GENCTRL_ID(GENERIC_CLOCK_GENERATOR_XOSC_RTC)
							// Selected source is External 32KHz Oscillator
                      		| GCLK_GENCTRL_SRC_XOSC32K
							// Output clock to a pin for tests
							//	| GCLK_GENCTRL_OE
							| GCLK_GENCTRL_DIVSEL
                      		| GCLK_GENCTRL_GENEN
							;

	//	Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;

	//	----------------------------------------------------------------------------------------------
	//	Now that all system clocks are configured, we can set CPU and APBx BUS clocks.
	//	There values are normally the one present after Reset.
 	//	----------------------------------------------------------------------------------------------
	PM->CPUSEL.reg  = PM_CPUSEL_CPUDIV_DIV1;
	PM->APBASEL.reg = PM_APBASEL_APBADIV_DIV1_Val;
	PM->APBBSEL.reg = PM_APBBSEL_APBBDIV_DIV1_Val;
	PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1_Val;

	SystemCoreClock = VARIANT_MCK;

	//	Disable automatic NVM write operations
	NVMCTRL->CTRLB.bit.MANW = 1;
	}

void coreControllerInitialize(void)
	{
	// In systemSAMD21.c
	SystemInit();
	coreControllerSetupClocks();	
	}

void coreControllerReset(void)
	{
	NVIC_SystemReset();
	}

void coreControllerSleep(void)
	{
	SCB->SCR = SCB_SCR_SLEEPDEEP_Msk;
	PM->SLEEP.bit.IDLE = 0x02;
	__WFI();
	}

void coreControllerIdle(void)
	{
	SCB->SCR = SCB->SCR & ~SCB_SCR_SLEEPDEEP_Msk;
	PM->SLEEP.bit.IDLE = 0x02;
	__WFI();
	}

/** Enable Brown Out detection.
*/
void coreControllerEnableBOD(void)
	{
	SYSCTRL->BOD33.bit.ACTION = 0x01;
	SYSCTRL->BOD33.bit.RUNSTDBY = true;
	SYSCTRL->BOD33.bit.ENABLE = true;
	}

/** Disable Brown Out detection.
*/
void coreControllerDisableBOD(void)
	{
	SYSCTRL->BOD33.bit.ENABLE = false;
	SYSCTRL->BOD33.bit.RUNSTDBY = false;
	SYSCTRL->BOD33.bit.ACTION = 0x00;
	}

/** Enable the watchdog timer.
*/
void coreControllerEnableWatchdog(void)
	{
	WDT->CONFIG.bit.PER = 0x9;
	WDT->CTRL.bit.ENABLE = 1;
	while (WDT->STATUS.bit.SYNCBUSY)
		;
	}

/** Disable the watchdog timer.
*/
void coreControllerDisableWatchdog(void)
	{
	WDT->CTRL.bit.ENABLE = 0;
	while (WDT->STATUS.bit.SYNCBUSY)
		;
	}

void coreControllerServiceWatchdog(void)
	{
	WDT->CLEAR.bit.CLEAR = WDT_CLEAR_CLEAR_KEY;
	}

