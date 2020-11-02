/*
 * MaintenanceInterface.cpp
 *
 * Created: 9/22/2020 10:14:58 AM
 * Author : GMateusDP
 */ 


#include "main.h"

uint8_t	StartLivePulse(void){
	//usb<<"**** Life Pulse Activated  ****"<<NEWLINE;
	//pwm_set_parameters(&PWM_0, 500, 1000);
	uint32_t p=pwm_enable(&LIVE_PULSE);
	return p;
}
bool	WriteUserParametersFlash(void){

	UserParameters	p=defaultParameters;
	flash.eraseFlash((uint32_t)&flashLayout->parameters,sizeof(UserParameters));
	
	flash.flushConfigurationData(&p);
	delay_ms(WRITE_DELAY);
	UserParameters	q;
	q=flash.readConfigurationData(q);
	if (p.flapperValveOffset==q.flapperValveOffset)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool	WriteUserParametersFram(void){

	UserParameters	p=defaultParameters;
	//flash.eraseFlash((uint32_t)&flashMap->parameters,sizeof(userParameters));
	
	fram.flushConfigurationData(&p);
	delay_ms(WRITE_DELAY);
	UserParameters	q;
	q=fram.readConfigurationData(q);
	if (p.flapperValveOffset==q.flapperValveOffset)
	{
		return true;
	}
	else
	{
		return false;
	}
}


int main(void)		
{
	atmel_start_init();
	usb.Init();
	StartLivePulse();
	delay_ms(500);
	flash.Init();
	WriteUserParametersFlash();
	Maintenance_Tool	toolApp;
	toolApp.Init();
	while (1)
	{
		if (toolApp.IsAppConnected())
		{
			if (toolApp.localBuffer[sizeof(HVACMessageHeader)]==(uint8_t)'f')
			{
				toolApp.NotifyConnectionAcknowledge();
				toolApp.ClearLocalBuffer();
				while (!usb.txReady);
				
			}
			else
			{
				toolApp.handleCommunication();
			}
			
			
			
		}
		else
		{
			toolApp.setConnected(false);
			delay_ms(100);
		}
		
	}

	
}

