/* 
* TemperatureSensors_Class.h
*
* Created: 10/9/2020 12:06:37 PM
* Author: GMateusDP
*/


#ifndef __TEMPERATURESENSORS_CLASS_H__
#define __TEMPERATURESENSORS_CLASS_H__
#include "LTC2983_Class.h"

#define NUMBER_LTC2983_MODULES 2



class TemperatureSensors_Class: private LTC2983_Class
{
//variables
public:
	volatile bool converterReady[NUMBER_LTC2983_MODULES];
	volatile bool isOK;
	 float	values[NUMBER_LTC2983_MODULES][NUMBER_TEMPERATURE_CHANNELS];
	 uint8_t faultData;
	 	float lastValue;
protected:
private:
	LTC2983_Class	module[NUMBER_LTC2983_MODULES];
	uint32_t		csPins[NUMBER_LTC2983_MODULES];
	spi_m_async_descriptor *SPIA;
	LT_SPI	spiLT;
	uint8_t i,j,k,currentModule,currentChannel;


//functions
public:
	TemperatureSensors_Class();
	~TemperatureSensors_Class();
	TemperatureSensors_Class(spi_m_async_descriptor *SPI_LTC);
	bool	Init(void);
	float GetConversionResult(void);
	void	StartOneConversion(void);
	bool	IsConversionFinished();
	bool SetModuleReady(uint8_t);
protected:
private:
	TemperatureSensors_Class( const TemperatureSensors_Class &c );
	TemperatureSensors_Class& operator=( const TemperatureSensors_Class &c );
	
	bool InitModules(void);

}; //TemperatureSensors_Class
extern TemperatureSensors_Class temperatures;
#endif //__TEMPERATURESENSORS_CLASS_H__
