/*
 * Interface_SAMD21.cpp
 *
 * Created: 9/14/2020 4:17:08 PM
 * Author : GMateusDP
 */ 

#include "main.h"

uint8_t		localbuffer[64];
volatile	bool  appInterfaceTimeout;


static void InterfaceListenerTimeout(const struct timer_task *const timer_task){
	appInterfaceTimeout=true;
}

bool	IsMaintenanceAppConnected(void){
	bool	result(false);
	uint32_t	magic=0;
	uint32_t	xorResult;
	if (usb.rxReady)
	{
	//	usbTimer.Start_oneShot_task((FUNC_PTR)InterfaceListenerTimeout,MAINTENANCE_TOOL_TIMEOUT);
	//	appInterfaceTimeout=false;
//		usb.readDataAsyn(localbuffer,4);
		while ((!usb.rxReady)&(!appInterfaceTimeout))
		{
			;
		}
	//	usbTimer.Stop();
		if (usb.rxReady)
		{
			
			for (int i = 0; i < 4; i++)
			{
				magic=magic<<8;
				magic|= localbuffer[i];
			}
			xorResult=magic^MAINTENANCE_TOOL_KEYWORD;
			if (xorResult==0)
			{
				result=true;
			}
			else
			{
				//delay_ms(1000);
				result=false;
			}
		} 
		else
		{
			result=false;
		}
		
	} 
	else
	{
		
		result=false;
	}
	usb.readDataAsyn(localbuffer,4);
	return	result;
}

bool	WriteUserParametersFlash(void){

	userParameters	p=defaultParameters;
	flash.eraseFlash((uint32_t)&flashMap->parameters,sizeof(userParameters));
	
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
		return false;
	}
}


int main(void)
{
	atmel_start_init();
		usb.Init();
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
				usb.writeData(localbuffer,4);
				toolApp.setConnected(false);
				delay_ms(100);
			}
			
		}

	
}
