/*
 * Dual_Heater_P0.cpp
 *
 * Created: 1/12/2021 10:24:43 AM
 * Author : GMateusDP
 */ 


#include "main.h"
#define DELAY_HEATER_COMMUNICATION 100


int main(void)
{
	atmel_start_init();
	uint8_t status,enableMask,lastEnableMask;
	bool powerOn;

	usb.Init();
	delay_ms(100);

	gpio_set_pin_level(LED0,true);
	while (!heaters.Init())
	{
		delay_ms(200);
		gpio_toggle_pin_level(LED0);
		
	}
	gpio_set_pin_level(LED0,true);
	
	//	toolApp.Init();
	while (1)
	{
		// 		if (toolApp.IsAppConnected())
		// 		{
		// 			if (toolApp.handleCommunication())
		// 			{
		// 				if (toolApp.singleTaskMessage.header.task==kHVACCommandSetHeaters)
		// 				{
		// 					lastEnableMask=enableMask;
		// 					enableMask=toolApp.singleTaskMessage.description;
		// 					if (enableMask!=lastEnableMask)
		// 					{
		// 						for (uint8_t i = 0; i < 4; i++)
		// 						{
		// 							powerOn=enableMask&(0x01<<i);
		// 							//	usb<<"Setting Heater: "<<i<<"to " <<powerOn<<" .\t";
		// 							heater.SetRelay(i,powerOn);
		// 							//delay_ms(DELAY_HEATER_COMMUNICATION/10);
		// 							// 					status= heater.ReadStatus();
		// 							// 					usb<<"Heater "<<i<< " Status :"<<heater.heaterGPIO.inputs.niAlcHeaterRelayFault[i]<<NEWLINE;
		// 							// 					delay_ms(DELAY_HEATER_COMMUNICATION/10);
		//
		//
		// 						}
		// 					}
		// 				}
		//
		//
		// 			}
		// 			else
		// 			{
		//
		// 			}
		// 			if (heater.heaterStatusChanged)
		// 			{
		//
		// 				singleTask.description=heater.ReadStatus();
		// 				memcpy(toolApp.localBuffer,(void*)&singleTask,sizeof(SingleTaskMessage));
		// 				usb.write(toolApp.localBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
		// 				heater.heaterStatusChanged=false;
		//
		// 			}
		//
		// 		}
		// 		else
		// 		{
		enableMask=0xff;
		for (uint8_t j = 0; j <2 ; j++)
		{
			for (uint8_t i = 0; i <4 ; i++)
			{
				
				delay_ms(1);
				usb<<"Enabling Heater: "<<i<<" .\t";
				heaters.module[j]->EnableIndex(i);
				delay_ms(DELAY_HEATER_COMMUNICATION);
				status= heaters.module[j]->ReadStatus();
				usb<<"Heater "<<i<< " Status :"<<heaters.module[j]->heaterGPIO.inputs.niAlcHeaterRelayFault[i]<<NEWLINE;
				delay_ms(DELAY_HEATER_COMMUNICATION);
				heaters.module[j]->DisableIndex(i);


			}
		}
		
		/*	}*/
		
		

	}
}

