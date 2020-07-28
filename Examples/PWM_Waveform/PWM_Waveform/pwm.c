/**
 * \file
 *
 * \brief TCC Application APIs
 *
 * Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "pwm.h"

/**
 * \brief TCC interrupt callbacks
 */
struct _tcc_callback {
	void (*pwm_mc0_cb)(void *const hw);
} tcc_cb;

void tcc_enable(void *const hw)
{
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_ENABLE);
	hri_tcc_set_CTRLA_ENABLE_bit(hw);
}

void tcc_disable(void *const hw)
{
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_ENABLE);
	hri_tcc_clear_CTRLA_ENABLE_bit(hw);
}

void tcc_enable_circular_buffer_compare(void *const hw, uint8_t channel_index)
{
	hri_tcc_set_WAVE_reg(hw, TCC_WAVE_CICCEN(1 << channel_index));
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_disable_circular_buffer_compare(void *const hw, uint8_t channel_index)
{
	hri_tcc_clear_WAVE_reg(hw, TCC_WAVE_CICCEN(1 << channel_index));
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_enable_circular_buffer_period(void *const hw)
{
	hri_tcc_set_WAVE_reg(hw, TCC_WAVE_CIPEREN);
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_disable_circular_buffer_period(void *const hw)
{
	hri_tcc_clear_WAVE_reg(hw, TCC_WAVE_CIPEREN);
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_enable_channel_polarity(void *const hw, uint8_t channel_index)
{
	hri_tcc_set_WAVE_reg(hw, TCC_WAVE_POL(1 << channel_index));
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_disable_channel_polarity(void *const hw, uint8_t channel_index)
{
	hri_tcc_clear_WAVE_reg(hw, TCC_WAVE_POL(1 << channel_index));
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_enable_channel_swap(void *const hw, uint8_t channel_index)
{
	hri_tcc_set_WAVE_reg(hw, TCC_WAVE_SWAP(1 << channel_index));
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_disable_channel_swap(void *const hw, uint8_t channel_index)
{
	hri_tcc_clear_WAVE_reg(hw, TCC_WAVE_SWAP(1 << channel_index));
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_set_ramp_operation(void *const hw, uint8_t ramp_value)
{
	hri_tcc_write_WAVE_RAMP_bf(hw, ramp_value);
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_set_wavegen_mode(void *const hw, uint8_t wavegen)
{
	hri_tcc_write_WAVE_WAVEGEN_bf(hw, wavegen);
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_WAVE);
}

void tcc_set_compare_value(void *const hw, uint32_t value, uint8_t channel_index)
{
	hri_tcc_write_CC_reg(hw, channel_index, value);
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_CC(1 << channel_index));
}

void tcc_set_compare_buffer_value(void *const hw, uint32_t value, uint8_t channel_index)
{
	hri_tcc_write_CCBUF_reg(hw, channel_index, value);
}

void tcc_set_period_value(void *const hw, uint32_t value)
{
	hri_tcc_write_PER_reg(hw, value);
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_PER);
}

void tcc_set_period_buffer_value(void *const hw, uint32_t value)
{
	hri_tcc_write_PERBUF_reg(hw, value);
}

void tcc_set_low_high_dead_time(void *const hw, uint8_t DTH_value, uint8_t DTL_value)
{
	hri_tcc_set_WEXCTRL_reg(hw, TCC_WEXCTRL_DTHS(DTH_value) | TCC_WEXCTRL_DTLS(DTL_value));
}

void tcc_enable_channel_dead_time(void *const hw, uint8_t channel_index)
{
	hri_tcc_set_WEXCTRL_reg(hw, TCC_WEXCTRL_DTIEN(1 << channel_index));
}

void tcc_disable_channel_dead_time(void *const hw, uint8_t channel_index)
{
	hri_tcc_clear_WEXCTRL_reg(hw, TCC_WEXCTRL_DTIEN(1 << channel_index));
}

void tcc_set_dead_time(void *const hw, uint8_t channels, uint8_t OTMX, uint8_t DTH_value, uint8_t DTL_value)
{
	hri_tcc_set_WEXCTRL_reg(hw,
	                        TCC_WEXCTRL_OTMX(OTMX) | TCC_WEXCTRL_DTIEN(channels) | TCC_WEXCTRL_DTHS(DTH_value)
	                            | TCC_WEXCTRL_DTLS(DTL_value));
}

void tcc_set_otmx_mode(void *const hw, uint8_t OTMX)
{
	hri_tcc_set_WEXCTRL_reg(hw, TCC_WEXCTRL_OTMX(OTMX));
}

void tcc_enable_channel_inversion(void *const hw, uint8_t channel_index)
{
	hri_tcc_set_DRVCTRL_reg(hw, TCC_DRVCTRL_INVEN(1 << channel_index));
}

void tcc_disable_channel_inversion(void *const hw, uint8_t channel_index)
{
	hri_tcc_clear_DRVCTRL_reg(hw, TCC_DRVCTRL_INVEN(1 << channel_index));
}

void tcc_set_pattern(void *const hw, uint8_t pattern_channels, uint8_t pattern)
{
	hri_tcc_write_PATT_reg(hw, TCC_PATT_PGE(pattern_channels) | TCC_PATT_PGV(pattern));
	hri_tcc_wait_for_sync(hw, TCC_SYNCBUSY_PATT);
}

void tcc_set_pattern_buffer(void *const hw, uint8_t pattern_channels, uint8_t pattern)
{
	hri_tcc_write_PATTBUF_reg(hw, TCC_PATTBUF_PGEB(pattern_channels) | TCC_PATTBUF_PGVB(pattern));
	hri_tcc_wait_for_sync(hw, 1 << 16);
}

void tcc_enable_interrupt(void *const hw, uint32_t tcc_interrupt, void *const cb)
{
	hri_tcc_set_INTEN_reg(hw, tcc_interrupt);
	switch (tcc_interrupt) {
	case TCC_INTENSET_MC0:
		tcc_cb.pwm_mc0_cb = cb;
		break;
	/* add more switch cases for other TCC interrupts and callbacks for each interrupt in tcc_cd structure */
	default:
		break;
	}
}

void tcc_disable_interrupt(void *const hw, uint32_t tcc_interrupt)
{
	hri_tcc_clear_INTEN_reg(hw, tcc_interrupt);
	switch (tcc_interrupt) {
	case TCC_INTENSET_MC0:
		tcc_cb.pwm_mc0_cb = NULL;
		break;
	/* add more switch cases for other TCC interrupts and callbacks for each interrupt in tcc_cd structure */
	default:
		break;
	}
}

/**
 * \brief TCC0 Match 0 interrupt handler
 * Refer Datasheet and DFP to see other TCC0 handlers
 */
void TCC0_1_Handler(void)
{
	hri_tcc_clear_INTFLAG_reg(TCC0, TCC_INTFLAG_MC0);
	tcc_cb.pwm_mc0_cb(TCC0);
}
