/* 
* TemperatureSensors_Class.cpp
*
* Created: 10/9/2020 12:06:36 PM
* Author: GMateusDP
*/


#include "TemperatureSensors_Class.h"
#include "Event_Logger_Class.h"



 TemperatureSensors_Class	*ptrTemperaturesClass;

static void Converter1Ready(void){
	temperatures.converterReady[0]=true;
	temperatures.SetModuleReady(0);
	temperatures.converterTimeout=false;
}
static void Converter2Ready(void){
	temperatures.converterReady[1]=true;
	temperatures.SetModuleReady(1);
	temperatures.converterTimeout=false;
}
static void Converter3Ready(void){
	temperatures.converterReady[2]=true;
	temperatures.SetModuleReady(2);
	temperatures.converterTimeout=false;
}
static void	ConversionTimeout(const struct timer_task *const timer_task){
	temperatures.converterTimeout=true;
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
	if (spiLite->ready)
	{
		logger.SaveEvent("SPI Temperatures Started");
	} 
	else
	{
		logger.SaveEvent("SPI Temperatures Failed");
	}
	ptrTemperaturesClass=this;
	ext_irq_register(PIN_PB08, Converter1Ready);
	ext_irq_register(PIN_PB09, Converter2Ready);
	ext_irq_register(PIN_PB01, Converter3Ready);
	InitModules();
	converterReady[0]=true;converterReady[1]=true;converterReady[3]=true;
	currentModule=0; currentChannelIndex=0;
	isOK=(module[0].isOK)&(module[1].isOK)&(module[2].isOK);
	return isOK;
}

bool TemperatureSensors_Class::InitModules(void){
	csPins[0]=CS_TEMP1;  csPins[1]=CS_TEMP2; csPins[2]=CS_TEMP3;
	for (uint8_t  ii = 0; ii < NUMBER_LTC2983_MODULES; ii++)
	{
		module[ii].Init(csPins[ii],spiLite);
		logger.SaveEventIndexResult("Module ",ii,(uint8_t)(!isOK));

	}
	return true;

}
void	TemperatureSensors_Class::StartOneConversion(void){
	converterTimeout=false;
	converterReady[currentModule]=false;
	temperatureTimer.Start_oneShot_task((FUNC_PTR)ConversionTimeout,TEMPERATURE_MEASSURE_TIMEOUT);
	module[currentModule].ConvertAsyncChannelIndex(currentChannelIndex);
	
}
void	TemperatureSensors_Class::StartOneConversion(uint8_t mod,uint8_t sen){
	converterTimeout=false;
	currentModule=mod; currentChannelIndex=sen;
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
void TemperatureSensors_Class::StartNextConversion(void){
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
	converterTimeout=false;
	converterReady[currentModule]=false;
	temperatureTimer.Start_oneShot_task((FUNC_PTR)ConversionTimeout,TEMPERATURE_MEASSURE_TIMEOUT);
	module[currentModule].ConvertAsyncChannelIndex(currentChannelIndex);
}
bool	TemperatureSensors_Class::IsConversionFinished(){
	return converterReady[0]&converterReady[1]&converterReady[2];
}
bool	TemperatureSensors_Class::IsOneConversionFinished(){
	return converterReady[0]|converterReady[1]|converterReady[2];
}
bool	TemperatureSensors_Class::SetModuleReady(uint8_t m){
	module[m].ready=true;
	return module[m].ready;
}

TemperatureSensors_Class temperatures(&SPI_TEMP);
