#include "atmel_start.h"
#include "main.h"

/*============================ GLOBAL VARIABLES ==============================*/

/*! \brief A buffer to store ADC conversion result.
 *
 *  It is a double buffer. When one is filling conversion result, another one
 *  with full data is used to calculate FFT result.
 */
volatile uint16_t adc_result[2][64];

/*! \brief the ADC buffer number
 *
 *  It indicates which buffer is currently in use.
 */
volatile uint8_t buf_index = 0;

//! ADC complete flag. It is used to determine the sleep mode to be entered.
volatile bool adc_complete = false;

//! A flag indicates a buffer is full, and ready to perform data handling.
volatile uint8_t buf_full = 0;

int main(void)
{
	system_init();

	// Disable 2M RC oscillator after switching to 32M RC due to the atmel start driver does not do this.
	OSC.CTRL = OSC_RC32MEN_bm;

	// Disable EEPROM in NVM module for lower power.
	ccp_write_io((void *)&NVM.CTRLB, NVM_EPRM_bm);

	// Assign source address
	DMA.CH0.SRCADDR0 = (uint8_t)((uint16_t) & (ADCA.CH0.RES));
	DMA.CH0.SRCADDR1 = (uint8_t)((uint16_t) & (ADCA.CH0.RES) >> 8);
	DMA.CH0.SRCADDR2 = 0;

	// Assign destination address
	DMA.CH0.DESTADDR0 = (uint8_t)((uint16_t)adc_result[0]);
	DMA.CH0.DESTADDR1 = (uint8_t)((uint16_t)adc_result[0] >> 8);
	DMA.CH0.DESTADDR2 = 0;

	// Enable DMA Channel Transaction Complete Interrupt
	DMA.CH0.CTRLB = DMA_CH_TRNINTLVL_LO_gc;

	// Enable global interrupt
	sei();

	while (1) {
		if (adc_complete == true) {
			adc_complete = false;

			if (buf_full == 1) {
				buf_full = 0;
				// Toggle Idle LED
				PORTQ.OUTTGL = PIN3_bm;

				// Perform ADC conversion data handling
				// ...
			}
		}

		// Idle sleep
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_enable();
		sleep_cpu();
		sleep_disable();
	}
	return 0;
}

/*!
 *  This is the DMA transfer complete interrupt service routine
 */
ISR(DMA_CH0_vect)
{
	// Clear interrupt flag
	DMA.CH0.CTRLB |= DMA_CH_TRNIF_bm;

	// Switch to another buffer
	buf_index ^= 1;

	// Re-assign destination address
	DMA.CH0.DESTADDR0 = (uint8_t)((uint16_t)adc_result[buf_index]);
	DMA.CH0.DESTADDR1 = (uint8_t)((uint16_t)adc_result[buf_index] >> 8);
	DMA.CH0.DESTADDR2 = 0;
	// Reload DMA
	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;

	// Set flag to indicate it is time to perform the data handling.
	adc_complete = true;
	buf_full     = 1;
}
