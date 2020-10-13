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

	UserParameters	p=defaultParameters;
	flash.eraseFlash((uint32_t)&flashLayout->parameters,sizeof(UserParameters));
	delay_ms(WRITE_DELAY);
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
		delay_ms(WRITE_DELAY);
		WriteUserParametersFlash();
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
bool	WriteUserParametersEEPROM(void){

	UserParameters	p=defaultParameters;
	//flash.eraseFlash((uint32_t)&flashMap->parameters,sizeof(userParameters));
	uint32_t add32= (uint32_t)&eepromLayout->parameters;
	uint8_t  add8=(uint8_t)(0xff&add32);
	eeprom.WriteAddress((uint8_t*)&p,add8,sizeof(UserParameters));
	delay_ms(WRITE_DELAY);
	UserParameters	q;
	eeprom.ReadAddress((uint8_t*)&q,add8,sizeof(UserParameters));
	if (p.flapperValveOffset==q.flapperValveOffset)
	{
		return true;
	}
	else
	{
		WriteUserParametersEEPROM();
	}
	return false;
}


int main(void)		
{
	atmel_start_init();
	usb.Init();
	StartLivePulse();
	delay_ms(500);
	eeprom.Init();
	WriteUserParametersEEPROM();
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

