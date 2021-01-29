/*
 * TemperatureSensorsSampling.cpp
 *
 * Created: 11/3/2020 10:36:03 AM
 * Author : GMateusDP
 */ 


#include "main.h"
//static Maintenance_Tool	toolApp;

static float currentTemperature,lastTemperature;
uint32_t error_counter=0;
uint8_t sensor_counter=0;

int main(void)
{
	bool gotNewTemperature=false;
	atmel_start_init();
	usb.Init();
	bool isOK=temperatures.Init();
	if (isOK)
	{
		temperatures.StartOneConversion();
	}
	while(1){
		if (temperatures.IsConversionFinished())
		{
			error_counter=0;
			temperatures.GetConversionResult();
			if (temperatures.faultData==VALID_TEMPERATURE)
			{
				lastTemperature=currentTemperature;
				currentTemperature=temperatures.lastValue;
				if (lastTemperature!=currentTemperature)
				{
					
					gotNewTemperature=true;
					
				}
				
			}
			
			sensor_counter++;
			temperatures.StartOneConversion();
			asm("nop");
			
		}
		else
		{
			if (temperatures.converterTimeout)
			{
				sensor_counter++;
				temperatures.StartNextConversion();
				error_counter=0;
			} 
			else
			{
				error_counter++;
				if (error_counter%0xffff==0)
				{
					if (error_counter>0xffffff)
					{
										
						temperatures.Init();
						error_counter=0;
						
					}
					else
					{
						//temperatures.StartNextConversion();
					}
					
				}
				else
				{
				}
			}
			
			
		}
// 		if (gotNewTemperature)
// 		{
// 			if (toolApp.IsAppConnected())
// 			{
// 				if (toolApp.handleCommunication())
// 				{
// 
// 					
// 
// 				}
// 				else
// 				{
// 					
// 				}
// 
// 				
// 			}
// 			gotNewTemperature=false;
// 			
// 		}
		
		
	}
}
