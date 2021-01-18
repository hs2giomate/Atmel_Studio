/*
 * HeaterP0Board.cpp
 *
 * Created: 22.10.2020 14:24:41
 * Author : technik
 */ 


#include "main.h"

//static Maintenance_Tool	toolApp;
//static SingleTaskMessage singleTask;
#define DELAY_HEATER_COMMUNICATION 100

static void FirmwareAlive(const struct timer_task *const timer_task)
{
		//gpio_toggle_pin_level(iAlcFwIsAlive);
		gpio_set_pin_level(iAlcFwIsAlive,true);
}

int main(void)
{
	atmel_start_init();
	uint8_t status,enableMask,lastEnableMask;
	bool powerOn;

	usb.Init();
	delay_ms(100);
// 	usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
// 	usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	uint32_t p=pwm_enable(&LIVE_PULSE);
	hvacTimer.Start_periodic_task(FUNC_PTR(FirmwareAlive),250);
	gpio_set_pin_level(LED0,true);
	while (!heater.Init())
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
			for (uint8_t i = 0; i < 4; i++)
			{
			
					delay_ms(1);
                                                                  					usb<<"Enabling Heater: "<<i<<" .\t";
					heater.EnableIndex(i);
					delay_ms(DELAY_HEATER_COMMUNICATION);
					status= heater.ReadStatus();
					usb<<"Heater "<<i<< " Status :"<<heater.heaterGPIO.inputs.niAlcHeaterRelayFault[i]<<NEWLINE;
					delay_ms(DELAY_HEATER_COMMUNICATION);
					heater.DisableIndex(i);


			}
	/*	}*/
		
		

	}
}
