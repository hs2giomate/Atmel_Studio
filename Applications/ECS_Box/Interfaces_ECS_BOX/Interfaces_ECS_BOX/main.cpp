/*
 * Interfaces_ECS_BOX.cpp
 *
 * Created: 9/23/2020 10:34:12 AM
 * Author : GMateusDP
 */ 


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

	userParameters	p=defaultParameters;
	flash.eraseFlash((uint32_t)&flashLayout->parameters,sizeof(userParameters));
	delay_ms(WRITE_DELAY);
	flash.flushConfigurationData(&p);
	delay_ms(WRITE_DELAY);
	userParameters	q;
	q=flash.readConfigurationData(q);
	if (p.flapperValveOffset==q.flapperValveOffset)
	{
		return true;
	}
	else
	{
		delay_ms(WRITE_DELAY);
		WriteUserParametersFlash();
		return false;
	}
}

bool	WriteUserParametersFram(void){

	userParameters	p=defaultParameters;
	//flash.eraseFlash((uint32_t)&flashMap->parameters,sizeof(userParameters));
	
	fram.flushConfigurationData(&p);
	delay_ms(WRITE_DELAY);
	userParameters	q;
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
			if (toolApp.localBuffer[sizeof(GAINMessageHeader)]==(uint8_t)'f')
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

