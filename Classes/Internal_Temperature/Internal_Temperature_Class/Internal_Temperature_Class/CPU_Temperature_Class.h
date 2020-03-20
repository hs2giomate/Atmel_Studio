/* 
* CPU_Temperature_Class.h
*
* Created: 3/16/2020 9:25:57 AM
* Author: GMateusDP
*/


#ifndef __CPU_TEMPERATURE_CLASS_H__
#define __CPU_TEMPERATURE_CLASS_H__
#include "driver_init.h"
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

#ifndef CONF_ADC_CHANNEL
#define CONF_ADC_CHANNEL 0
#endif


class CPU_Temperature_Class
{
//variables
public:
	int16_t temp_cal_tl, temp_cal_th;
	int16_t temp_cal_vpl, temp_cal_vph, temp_cal_vcl, temp_cal_vch;
	int16_t tp_result, tc_result;
	int32_t result;
protected:
private:
	//CPU_Temperature_Class *pClass;

//functions
public:
	CPU_Temperature_Class();
	~CPU_Temperature_Class();
	void Init(void);
	void ADC_Init(void);
	int32_t  GetTemperatureCPU(void);
protected:
private:
	CPU_Temperature_Class( const CPU_Temperature_Class &c );
	CPU_Temperature_Class& operator=( const CPU_Temperature_Class &c );

}; //CPU_Temperature_Class

#endif //__CPU_TEMPERATURE_CLASS_H__
