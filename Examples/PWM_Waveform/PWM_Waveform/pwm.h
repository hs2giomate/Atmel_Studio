/**
 * \file
 *
 * \brief PWM Generation
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

#ifndef PWM_H_
#define PWM_H_

#include "tcc_lite.h"

/**
 * \brief Enable TCC.
 *
 * \param[in] hw The pointer to hardware instance.
 *
 */
void tcc_enable(void *const hw);

/**
 * \brief Disable TCC.
 *
 * \param[in] hw The pointer to hardware instance.
 *
 */
void tcc_disable(void *const hw);

/**
 * \brief Enable Circular buffer operation on Compare channel.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_enable_circular_buffer_compare(void *const hw, uint8_t channel_index);

/**
 * \brief Enable Circular buffer for Period register .
 *
 * \param[in] hw The pointer to hardware instance.
 *
 */
void tcc_enable_circular_buffer_period(void *const hw);

/**
 * \brief Enable WO[x] polarity.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_enable_channel_polarity(void *const hw, uint8_t channel_index);

/**
 * \brief Enable WO[x] and WO[N/2 + x] Swap operation.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_enable_channel_swap(void *const hw, uint8_t channel_index);

/**
 * \brief Enable Dead Time on compare channel.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_enable_channel_dead_time(void *const hw, uint8_t channel_index);

/**
 * \brief Enable Channel Inversion.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_enable_channel_inversion(void *const hw, uint8_t channel_index);

/**
 * \brief Enable Interrupt.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] tcc_interrupt TCC Interrupt type value
 * \param[in] cb Callback function for TCC Interrupt type
 *
 */
void tcc_enable_interrupt(void *const hw, uint32_t tcc_interrupt, void *const cb);

/**
 * \brief Disable WO[x] polarity.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_disable_channel_polarity(void *const hw, uint8_t channel_index);

/**
 * \brief Disable Circular bufferoperation on Compare channel.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_disable_circular_buffer_compare(void *const hw, uint8_t channel_index);

/**
 * \brief Disable Circular buffer for Period register .
 *
 * \param[in] hw The pointer to hardware instance.
 *
 */
void tcc_disable_circular_buffer_period(void *const hw);

/**
 * \brief Disable WO[x] and WO[N/2 + x] Swap operation.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_disable_channel_swap(void *const hw, uint8_t channel_index);

/**
 * \brief Disable Dead Time on compare channel.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_disable_channel_dead_time(void *const hw, uint8_t channel_index);

/**
 * \brief Disable Channel Inversion.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_disable_channel_inversion(void *const hw, uint8_t channel_index);

/**
 * \brief Disable Interrupt.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] tcc_interrupt TCC Interrupt type value
 *
 */
void tcc_disable_interrupt(void *const hw, uint32_t tcc_interrupt);

/**
 * \brief Set channel compare register value.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] value compare channel register value
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_set_compare_value(void *const hw, uint32_t value, uint8_t channel_index);

/**
 * \brief Set channel compare buffer register value.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] value compare channel buffer register value
 * \param[in] channel_index Compare channel number
 *
 */
void tcc_set_compare_buffer_value(void *const hw, uint32_t value, uint8_t channel_index);

/**
 * \brief Set TCC period value.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] value tcc period register value
 *
 */
void tcc_set_period_value(void *const hw, uint32_t value);

/**
 * \brief Set TCC period buffer value.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] value tcc period buffer register value
 *
 */
void tcc_set_period_buffer_value(void *const hw, uint32_t value);

/**
 * \brief Set PWM output waveform low and high side dead time in number of clocks cycles.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] DTH_value PWM waveform high side dead time in number of clocks cycles.
 * \param[in] DTL_value PWM waveform low side dead time in number of clocks cycles.
 *
 */
void tcc_set_low_high_dead_time(void *const hw, uint8_t DTH_value, uint8_t DTL_value);

/**
 * \brief Set PWM output waveform dead time settings.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] channel tcc compare channel mask values.
 * \param[in] OTMX tcc Output Matrix configuration.
 * \param[in] DTH_value PWM waveform high side dead time in number of clocks cycles.
 * \param[in] DTL_value PWM waveform low side dead time in number of clocks cycles.
 *
 */
void tcc_set_dead_time(void *const hw, uint8_t channels, uint8_t OTMX, uint8_t DTH_value, uint8_t DTL_value);

/**
 * \brief Set PWM Waveform Matrix.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] OTMX tcc Output Matrix configuration.
 *
 */
void tcc_set_otmx_mode(void *const hw, uint8_t OTMX);

/**
 * \brief Set PWM output waveform pattern generation register.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] pattern_channels compare channel number for pattern.
 * \param[in] pattern waveform pattern on pattern_channels.
 *
 */
void tcc_set_pattern(void *const hw, uint8_t pattern_channels, uint8_t pattern);

/**
 * \brief Set PWM output waveform pattern generation buffer register.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] pattern_channels compare channel number for pattern.
 * \param[in] pattern waveform pattern on pattern_channels.
 *
 */
void tcc_set_pattern_buffer(void *const hw, uint8_t pattern_channels, uint8_t pattern);

/**
 * \brief Set PWM output waveform ramp operation.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] ramp_value waveform ramp operation.
 *
 */
void tcc_set_ramp_operation(void *const hw, uint8_t ramp_value);

/**
 * \brief Set PWM output waveform generation Operation.
 *
 * \param[in] hw The pointer to hardware instance.
 * \param[in] wavegen waveform generation Operation.
 *
 */
void tcc_set_wavegen_mode(void *const hw, uint8_t wavegen);

#endif /* PWM_H_ */
