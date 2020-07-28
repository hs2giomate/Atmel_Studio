/**
 * \file
 *
 * \brief ADC related functionality implementation.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_adc_init ADC Init Driver
 * \ingroup doc_driver_adc
 *
 * \section doc_driver_adc_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#include <adc.h>

/**
 * \brief Initialize adc interface
 * \return Initialization status.
 */
int8_t ADC_0_init()
{

	// ADCA.CAL = 0x0; /* Calibration Value: 0x0 */

	ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc                  /* 1x gain */
	                | ADC_CH_INPUTMODE_SINGLEENDED_gc; /* Single-ended input, no gain */

	// ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc /* Input pin 0 */
	//		 | ADC_CH_MUXNEG_PIN0_gc; /* Input pin 0 (Input Mode = 2) */

	// ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc /* Interrupt on conversion complete */
	//		 | ADC_CH_INTLVL_OFF_gc; /* Interrupt disabled */

	// ADCA.CH0.SCAN = 0x0 << ADC_CH_SCANNUM_gp /* Number of Channels included in scan: 0x0 */;

	// ADCA.CH1.CTRL = ADC_CH_GAIN_1X_gc /* 1x gain */
	//		 | ADC_CH_INPUTMODE_INTERNAL_gc; /* Internal inputs, no gain */

	// ADCA.CH1.MUXCTRL = ADC_CH_MUXINT_TEMP_gc /* Temperature Reference */
	//		 | ADC_CH_MUXNEG_PIN0_gc; /* Input pin 0 (Input Mode = 2) */

	// ADCA.CH1.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc /* Interrupt on conversion complete */
	//		 | ADC_CH_INTLVL_OFF_gc; /* Interrupt disabled */

	// ADCA.CH2.CTRL = ADC_CH_GAIN_1X_gc /* 1x gain */
	//		 | ADC_CH_INPUTMODE_INTERNAL_gc; /* Internal inputs, no gain */

	// ADCA.CH2.MUXCTRL = ADC_CH_MUXINT_TEMP_gc /* Temperature Reference */
	//		 | ADC_CH_MUXNEG_PIN0_gc; /* Input pin 0 (Input Mode = 2) */

	// ADCA.CH2.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc /* Interrupt on conversion complete */
	//		 | ADC_CH_INTLVL_OFF_gc; /* Interrupt disabled */

	// ADCA.CH3.CTRL = ADC_CH_GAIN_1X_gc /* 1x gain */
	//		 | ADC_CH_INPUTMODE_INTERNAL_gc; /* Internal inputs, no gain */

	// ADCA.CH3.MUXCTRL = ADC_CH_MUXINT_TEMP_gc /* Temperature Reference */
	//		 | ADC_CH_MUXNEG_PIN0_gc; /* Input pin 0 (Input Mode = 2) */

	// ADCA.CH3.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc /* Interrupt on conversion complete */
	//		 | ADC_CH_INTLVL_OFF_gc; /* Interrupt disabled */

	// ADCA.CTRLB = 0 << ADC_IMPMODE_bp /* Gain Stage Impedance Mode: disabled */
	//		 | ADC_CURRLIMIT_NO_gc /* No limit */
	//		 | 0 << ADC_CONMODE_bp /* Conversion Mode: disabled */
	//		 | 0 << ADC_FREERUN_bp /* Free Running Mode Enable: disabled */
	//		 | ADC_RESOLUTION_12BIT_gc; /* 12-bit right-adjusted result */

	ADCA.EVCTRL = ADC_SWEEP_0_gc      /* ADC Channel 0 */
	              | ADC_EVSEL_7_gc    /* Event Channel 7 */
	              | ADC_EVACT_CH0_gc; /* First event triggers channel 0 */

	ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc   /* Internal VCC / 1.6 */
	               | 0 << ADC_BANDGAP_bp  /* Bandgap enable: disabled */
	               | 0 << ADC_TEMPREF_bp; /* Temperature Reference Enable: disabled */

	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc; /* Divide clock by 32 */

	// ADCA.CMP = 0x0; /* Compare Value: 0x0 */

	ADCA.CTRLA = 0 << ADC_FLUSH_bp    /* Flush Pipeline: disabled */
	             | 1 << ADC_ENABLE_bp /* Enable ADC: enabled */
	             | ADC_DMASEL_OFF_gc; /* Combined DMA request OFF */

	return 0;
}
