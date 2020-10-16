/* 
* TemperatureSensors_Class.cpp
*
* Created: 10/9/2020 12:06:36 PM
* Author: GMateusDP
*/


#include "TemperatureSensors_Class.h"
static TemperatureSensors_Class	*ptrTemperaturesClass;

static void Converter1Ready(void){
	ptrTemperaturesClass->converterReady[0]=true;
	ptrTemperaturesClass->SetModuleReady(0);
	ptrTemperaturesClass->converterTimeout=false;
}
static void Converter2Ready(void){
	ptrTemperaturesClass->converterReady[1]=true;
	ptrTemperaturesClass->SetModuleReady(1);
	ptrTemperaturesClass->converterTimeout=false;
}
static void	ConversionTimeout(const struct timer_task *const timer_task){
	ptrTemperaturesClass->converterTimeout=true;
	asm("nop");
}

static LT_SPI	ltSPI;
// default constructor
TemperatureSensors_Class::TemperatureSensors_Class()
{
} //TemperatureSensors_Class
TemperatureSensors_Class::TemperatureSensors_Class(spi_m_sync_descriptor *SPI_LTC)
{
		spiLite=&ltSPI;
		ptrTemperaturesClass=this;
		SPIA=SPI_LTC;
		
} //TemperatureSensors_Class

// default destructor
TemperatureSensors_Class::~TemperatureSensors_Class()
{
} //~TemperatureSensors_Class

bool	TemperatureSensors_Class::Init(void){
	spiLite->set_descriptor(SPIA);
	spiLite->init();
	ptrTemperaturesClass=this;
	ext_irq_register(PIN_PB08, Converter1Ready);
	ext_irq_register(PIN_PB09, Converter2Ready);
	InitModules();
	currentModule=0; currentChannelIndex=0;
	isOK=(module[0].isOK)&(module[1].isOK);
	return isOK;
}

bool TemperatureSensors_Class::InitModules(void){
	csPins[0]=CS_TEMP1;  csPins[1]=CS_TEMP2;
	for (uint8_t  ii = 0; ii < NUMBER_LTC2983_MODULES; ii++)
	{
		module[ii].Init(csPins[ii],spiLite);
	}

}
void	TemperatureSensors_Class::StartOneConversion(void){
	converterTimeout=false;
	converterReady[currentModule]=false;
	temperatureTimer.Start_oneShot_task((FUNC_PTR)ConversionTimeout,TEMPERATURE_MEASSURE_TIMEOUT);
	module[currentModule].ConvertAsyncChannelIndex(currentChannelIndex);
	
}
float TemperatureSensors_Class::GetConversionResult(void){
	temperatureTimer.Remove_task(FUNC_PTR(ConversionTimeout));
	lastValue=module[currentModule].GetSingleChannelResultIndex(currentChannelIndex);
	faultData=module[currentModule].fault_data;
		if (faultData==VALID_TEMPERATURE)
		{
			values[currentModule][currentChannelIndex]=lastValue;
		}
		else
		{
			lastValue=-999;
		}
	
	currentChannelIndex++;

	if (currentChannelIndex==NUMBER_TEMPERATURE_CHANNELS)
	{
		currentChannelIndex=0;
		currentModule++;
		if (currentModule==NUMBER_LTC2983_MODULES)
		{
			currentModule=0;
		}
	} 
	else
	{
	}
	return lastValue;
}
bool	TemperatureSensors_Class::IsConversionFinished(){
	return converterReady[0]&converterReady[1];
}
bool	TemperatureSensors_Class::SetModuleReady(uint8_t m){
	module[m].ready=true;
	return module[m].ready;
}

TemperatureSensors_Class temperatures(&SPI_TEMP);
