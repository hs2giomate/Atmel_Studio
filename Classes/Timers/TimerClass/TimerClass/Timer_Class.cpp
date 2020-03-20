/* 
* Timer_Class.cpp
*
* Created: 3/2/2020 11:38:07 AM
* Author: GMateusDP
*/


#include "Timer_Class.h"

// default constructor
Timer_Class::Timer_Class()
{
	timerRegister=TC7;
} //Timer_Class

// default destructor
Timer_Class::~Timer_Class()
{
	stopTimer();
} //~Timer_Class

Timer_Class::Timer_Class(Tc* value){
	timerRegister=value;

	
}
Timer_Class *TimerPtr;

void Timer_Class::setUpTimer(void){
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_TC6_TC7_Val) | GCLK_CLKCTRL_GEN(GENERIC_CLOCK_GENERATOR_OSC8M) | GCLK_CLKCTRL_CLKEN;							// select TC3 peripheral channel
	PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1;
	if(timerRegister== TC7){
		PM->APBCMASK.reg |= PM_APBCMASK_TC7;													// enable TC3 interface
		}else if( timerRegister== TC6){
		PM->APBCMASK.reg |= PM_APBCMASK_TC6;												// Configure synchronous bus clock
	}
	PORT->Group[1].DIRSET.reg = (1UL << 8);
	PORT->Group[1].DIRSET.reg = (1UL << 0);
	timerRegister->COUNT32.CTRLA.reg = TC_CTRLA_RESETVALUE ;
}

void Timer_Class::startTimer(time_t timeoutsec){
	cpu_irq_disable();
	uint32_t	timesec=timeoutsec*8;
	// Configure Count Mode (32-bit)
	timerRegister->COUNT32.CTRLA.reg =TC_CTRLA_MODE_COUNT32|TC_CTRLA_WAVEGEN_MFRQ|TC_CTRLA_PRESCALER_DIV1024;
	
	// Initialize compare value for 100mS @ 500kHz
	timerRegister->COUNT32.CC[0].reg = timesec;
	while(timerRegister->COUNT32.STATUS.bit.SYNCBUSY == 1);
	// Enable TC3 compare mode interrupt generation
	timerRegister->COUNT32.INTENSET.reg = TC_INTENSET_MC0;										// Enable match interrupts on compare channel 0
	
	// Enable TC3
	timerRegister->COUNT32.CTRLA.reg |= TC_CTRLA_ENABLE;
	
	// Wait until TC3 is enabled
	while(timerRegister->COUNT32.STATUS.bit.SYNCBUSY == 1);
	
	NVIC_EnableIRQ((timerRegister==TC6)?TC6_IRQn:TC7_IRQn);						// Enable the interrupt
	
	/* Enable all IRQs */
	cpu_irq_enable();
	enabled=1;
	(timerRegister==TC6)?PORT->Group[1].OUTSET.reg = (1UL << 0):PORT->Group[1].OUTSET.reg = (1UL << 8);
	
}

void Timer_Class::stopTimer( void){
	cpu_irq_disable();
	NVIC_DisableIRQ((timerRegister==TC6)?TC6_IRQn:TC7_IRQn);		// Disable the interrupt
	
	// Disable TC3
	timerRegister->COUNT32.CTRLA.bit.SWRST = 1 ;
	timerRegister->COUNT32.CTRLA.reg = TC_CTRLA_RESETVALUE;
	(timerRegister==TC6)?PORT->Group[1].OUTCLR.reg = (1UL << 0):PORT->Group[1].OUTCLR.reg = (1UL << 8);
	cpu_irq_enable();
	enabled=0;
	
	
}

bool Timer_Class::timerTimeout(void){
	bool overflow_tcx=(timerRegister->COUNT32.INTFLAG.bit.OVF);
	if (overflow_tcx){
		enabled=0;
		(timerRegister==TC6)?PORT->Group[1].OUTCLR.reg = (1UL << 0):PORT->Group[1].OUTCLR.reg = (1UL << 8);
	}
	return overflow_tcx;
	
}



void TC7_Handler(void){
	if (TC7->COUNT32.INTFLAG.bit.OVF){
		PORT->Group[1].OUTCLR.reg = (1UL << 8);
		TimerPtr->enabled=0;
		TimerPtr->stopTimer();
	}
	
	
}
void TC6_Handler(void){
	if (TC6->COUNT32.INTFLAG.bit.OVF){
		PORT->Group[1].OUTCLR.reg = (1UL << 0);
		Door.TTLTimer.enabled=0;
		Door.TTLTimer.stopTimer();
		#if BOOTABLE >1
		Door.BootTimer.enabled=0;
		Door.BootTimer.stopTimer();
		#endif
	}
	
	
}