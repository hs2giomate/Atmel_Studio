/*
 * SingleFlapperValveP0.cpp
 *
 * Created: 11/6/2020 3:54:32 PM
 * Author : GMateusDP
 */ 

#include "main.h"
#define		DELAY_TIME_REQUEST 100
bool dir=false;
uint32_t counter=0;
//static Maintenance_Tool	toolApp;
//static EvaporatorAndCondesatorFans_Class fans;
//static SingleFlapperValve_Class fv(0);


static void FirmwareAlive(const struct timer_task *const timer_task)
{
	gpio_set_pin_level(iAlcFwIsAlive,true);
}


int main(void)
{
	atmel_start_init();
	uint8_t position;

	usb.Init();
	delay_ms(100);
	//	usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	//	usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	pwm_enable(&LIVE_PULSE);
	delay_ms(200);
	memory.Init();
	memory.WriteDefaultParameters();
	fvc.InitController();

	hvacTimer.Start_periodic_task(FUNC_PTR(FirmwareAlive),250);
	//toolApp.Init();
	while (1)
	{
// 		for (uint8_t i = 0; i < 8; i++)
// 		{
// 					fvc.StartControlling(0x20 +i*30);
// 					while (!fvc.gotSetpoint)
// 					{
// 						fvc.Control_NBC_StandAlone_Reset();
// 					}
// 					
// 					delay_ms(100);
// 					fvc.StopControlling();
// 		}
// 			for (uint8_t i = 0; i < 8; i++)
// 			{
// 				fvc.StartControlling(230 -i*30);
// 				while (!fvc.gotSetpoint)
// 				{
// 				fvc.Control_NBC_StandAlone_Reset();
// 				}
// 				delay_ms(100);
// 				fvc.StopControlling();
// 			}
fvc.singlefv->SetDirection(dir);
fvc.singlefv->SetEnable(true);
while(counter<0x8fff){
	fvc.singlefv->ReadActualPosition();
	counter++;
}
counter=0;
fvc.singlefv->SetEnable(false);
while(counter<0x4ff){
	fvc.singlefv->ReadActualPosition();
	counter++;
}
counter=0;
dir=!dir;
		
// 		if (toolApp.IsAppConnected())
// 		{
// 			if (toolApp.handleCommunication())
// 			{
// 				// 				if (toolApp.singleTaskMessage.header.task==kHVACCommandSetHeaters)
// 				// 				{
// 				// 					lastEnableMask=enableMask;
// 				// 					enableMask=toolApp.singleTaskMessage.description;
// 				// 					if (enableMask!=lastEnableMask)
// 				// 					{
// 				// 						for (uint8_t i = 0; i < 4; i++)
// 				// 						{
// 				// 							powerOn=enableMask&(0x01<<i);
// 				// 							//	usb<<"Setting Heater: "<<i<<"to " <<powerOn<<" .\t";
// 				// 							heater.SetRelay(i,powerOn);
// 				// 							//delay_ms(DELAY_HEATER_COMMUNICATION/10);
// 				// 							// 					status= heater.ReadStatus();
// 				// 							// 					usb<<"Heater "<<i<< " Status :"<<heater.heaterGPIO.inputs.niAlcHeaterRelayFault[i]<<NEWLINE;
// 				// 							// 					delay_ms(DELAY_HEATER_COMMUNICATION/10);
// 				//
// 				//
// 				// 						}
// 				// 					}
// 				// 				}
// 				
// 
// 			}
// 			else
// 			{
// 				
// 			}
			// 			if (heater.statusChanged)
			// 			{
			//
			// 				singleTask.description=heater.ReadStatus();
			// 				memcpy(toolApp.localBuffer,(void*)&singleTask,sizeof(SingleTaskMessage));
			// 				usb.write(toolApp.localBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
			// 				heater.statusChanged=false;
			//
			// 			}
			
	//	}
// 		fv1.ClearMoveFault(true);
// 		fv1.SetDirection(true);
// 		fv1.SetEnable(true);
// 		for (uint8_t j = 1; j <0xff; j++)
// 		{
// 	
// 			fv1.WriteSetpoint(j);
// 			delay_ms(10);
// 			fv1.ReadStatusInputs();
// 			delay_ms(10);
// 			position=fv1.ReadActualPosition();
// 			delay_ms(10);
// 			
// 		}
// 		fv1.SetEnable(false);
// 	
// 		delay_ms(500);
// 			fv1.SetDirection(false);
// 		fv1.SetEnable(true);
// 		for (uint8_t j = 1; j <0xff; j++)
// 		{
// 
// 			fv1.WriteSetpoint(j);
// 			delay_ms(10);
// 			position=fv1.ReadActualPosition();
// 			delay_ms(10);
// 			fv1.ReadStatusInputs();
// 			delay_ms(10);
// 		}
// 		fv1.SetEnable(false);
// 		delay_ms(500);

		

	}
}
