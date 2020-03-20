/**
 * \file
 *
 * \brief ADC TSENS Example
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries.
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
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
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

#include "atmel_start.h"
//#include "tsens_example_config.h"
#ifndef CONF_ADC_CHANNEL
#define CONF_ADC_CHANNEL 0
#endif

#define NVM_TEMP_CAL_TLI_POS 0
#define NVM_TEMP_CAL_TLI_SIZE 8
#define NVM_TEMP_CAL_TLD_POS 8
#define NVM_TEMP_CAL_TLD_SIZE 4
#define NVM_TEMP_CAL_THI_POS 12
#define NVM_TEMP_CAL_THI_SIZE 8
#define NVM_TEMP_CAL_THD_POS 20
#define NVM_TEMP_CAL_THD_SIZE 4
#define NVM_TEMP_CAL_VPL_POS 40
#define NVM_TEMP_CAL_VPL_SIZE 12
#define NVM_TEMP_CAL_VPH_POS 52
#define NVM_TEMP_CAL_VPH_SIZE 12
#define NVM_TEMP_CAL_VCL_POS 64
#define NVM_TEMP_CAL_VCL_SIZE 12
#define NVM_TEMP_CAL_VCH_POS 76
#define NVM_TEMP_CAL_VCH_SIZE 12

int16_t temp_cal_tl, temp_cal_th;
int16_t temp_cal_vpl, temp_cal_vph, temp_cal_vcl, temp_cal_vch;
int16_t tp_result, tc_result;
int32_t result;

/**
 * Initlize ADC TSENS conversion
 */
void ADC_temperature_init(void)
{
	int8_t temp_cal_tli, temp_cal_thi, temp_cal_tld, temp_cal_thd;

	temp_cal_vpl = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_VPL_POS / 32)) >> (NVM_TEMP_CAL_VPL_POS % 32))
	               & ((1 << NVM_TEMP_CAL_VPL_SIZE) - 1);
	temp_cal_vph = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_VPH_POS / 32)) >> (NVM_TEMP_CAL_VPH_POS % 32))
	               & ((1 << NVM_TEMP_CAL_VPH_SIZE) - 1);
	temp_cal_vcl = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_VCL_POS / 32)) >> (NVM_TEMP_CAL_VCL_POS % 32))
	               & ((1 << NVM_TEMP_CAL_VCL_SIZE) - 1);
	temp_cal_vch = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_VCH_POS / 32)) >> (NVM_TEMP_CAL_VCH_POS % 32))
	               & ((1 << NVM_TEMP_CAL_VCH_SIZE) - 1);

	temp_cal_tli = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_TLI_POS / 32)) >> (NVM_TEMP_CAL_TLI_POS % 32))
	               & ((1 << NVM_TEMP_CAL_TLI_SIZE) - 1);
	temp_cal_tld = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_TLD_POS / 32)) >> (NVM_TEMP_CAL_TLD_POS % 32))
	               & ((1 << NVM_TEMP_CAL_TLD_SIZE) - 1);

	temp_cal_tl = ((uint16_t)temp_cal_tli) << 4 | ((uint16_t)temp_cal_tld);

	temp_cal_thi = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_THI_POS / 32)) >> (NVM_TEMP_CAL_THI_POS % 32))
	               & ((1 << NVM_TEMP_CAL_THI_SIZE) - 1);
	temp_cal_thd = (*((uint32_t *)(NVMCTRL_TEMP_LOG) + (NVM_TEMP_CAL_THD_POS / 32)) >> (NVM_TEMP_CAL_THD_POS % 32))
	               & ((1 << NVM_TEMP_CAL_THD_SIZE) - 1);

	temp_cal_th = ((uint16_t)temp_cal_thi) << 4 | ((uint16_t)temp_cal_thd);

	hri_supc_set_VREF_ONDEMAND_bit(SUPC);
	hri_supc_set_VREF_TSEN_bit(SUPC);
	hri_supc_clear_VREF_VREFOE_bit(SUPC);
	adc_sync_enable_channel(&ADC_0, CONF_ADC_CHANNEL);
}

int main(void)
{
	uint8_t buffer[2];
	atmel_start_init();
	printf("-- ADC Temperature Sensor Example --\r\n");
	ADC_temperature_init();

	while (1) {
		adc_sync_set_inputs(&ADC_0, ADC_INPUTCTRL_MUXPOS_CTAT_Val, ADC_INPUTCTRL_MUXNEG_GND_Val, CONF_ADC_CHANNEL);
		adc_sync_read_channel(&ADC_0, CONF_ADC_CHANNEL, buffer, 2);
		tc_result = ((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);

		adc_sync_set_inputs(&ADC_0, ADC_INPUTCTRL_MUXPOS_PTAT_Val, ADC_INPUTCTRL_MUXNEG_GND_Val, CONF_ADC_CHANNEL);
		adc_sync_read_channel(&ADC_0, CONF_ADC_CHANNEL, buffer, 2);
		tp_result = ((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);

		result = (int64_t)(temp_cal_tl * temp_cal_vph * tc_result - (int64_t)temp_cal_vpl * temp_cal_th * tc_result
		                   - (int64_t)temp_cal_tl * temp_cal_vch * tp_result
		                   + (int64_t)temp_cal_th * temp_cal_vcl * tp_result);
		result /= ((int32_t)temp_cal_vcl * tp_result - (int32_t)temp_cal_vch * tp_result
		           - (int32_t)temp_cal_vpl * tc_result + (int32_t)temp_cal_vph * tc_result);
		result >>= 4;

		printf("Temperature is: %d\r\n", result);
		delay_ms(1000);
	}
}
