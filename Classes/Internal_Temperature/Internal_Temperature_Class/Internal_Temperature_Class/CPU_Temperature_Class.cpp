/* 
* CPU_Temperature_Class.cpp
*
* Created: 3/16/2020 9:25:57 AM
* Author: GMateusDP
*/


#include "CPU_Temperature_Class.h"


// default constructor
CPU_Temperature_Class::CPU_Temperature_Class()
{
} //CPU_Temperature_Class

// default destructor
CPU_Temperature_Class::~CPU_Temperature_Class()
{
} //~CPU_Temperature_Class
void CPU_Temperature_Class::Init(void){
	//pClass=this;
	ADC_Init();
}


void CPU_Temperature_Class::ADC_Init(void){
	
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

int32_t CPU_Temperature_Class::GetTemperatureCPU(void){
	uint8_t buffer[2];
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
	return result;
}
	