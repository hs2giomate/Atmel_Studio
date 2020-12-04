/*
 * TemperatureSensorsSampling.cpp
 *
 * Created: 11/3/2020 10:36:03 AM
 * Author : GMateusDP
 */ 


#include "main.h"
//static Maintenance_Tool	toolApp;

static float currentTemperature,lastTemperature;

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
			temperatures.StartOneConversion();
			asm("nop");
			
		}
		else
		{
		}
		if (gotNewTemperature)
		{
			if (toolApp.IsAppConnected())
			{
				if (toolApp.handleCommunication())
				{

					

				}
				else
				{
					
				}

				
			}
			gotNewTemperature=false;
			
		}
		
		
	}
}
