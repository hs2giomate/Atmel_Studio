/*
 * MaintenanceInterface.cpp
 *
 * Created: 9/22/2020 10:14:58 AM
 * Author : GMateusDP
 */ 


#include "main.h"
#define DELAY_ERROR	50
//static FlapperValveController	fvc;
static float currentTemperature,lastTemperature;
bool localtimeout=false;
uint32_t	errorCounter=0;
static bool is_firmware_OK=false;
static bool is_running=false;
static uint32_t	counter_running=0;
static uint32_t last_count=0;
static Maintenance_Tool	userApp;

static void FirmwareIsAlive(const struct timer_task *const timer_task)
{
	gpio_set_pin_level(iAlcFwIsAlive,true);
	is_firmware_OK=true;
	if (counter_running==last_count)
	{
		is_running=false;
	} 
	else
	{
		is_running=true;
		last_count=counter_running;
		
	}
	gpio_toggle_pin_level(LED0);
	localtimeout=true;
}

uint8_t	StartLivePulse(void){
	//usb<<"**** Life Pulse Activated  ****"<<NEWLINE;
	//pwm_set_parameters(&PWM_0, 500, 1000);
	uint32_t p=pwm_enable(&LIVE_PULSE);
	return p;
}






int main(void)		
{
	bool gotNewTemperature=false;
	atmel_start_init();
	usb.Init();
	//StartLivePulse();
	delay_ms(200);
	memory.Init();
	memory.WriteDefaultParameters();
	bool isOK=temperatures.Init();
	if (isOK)
	{
		temperatures.StartOneConversion();
	}
	gpio_set_pin_level(LED0,true);
	while (!flapper.Init())
	{
		delay_ms(DELAY_ERROR);
		gpio_toggle_pin_level(LED0);
	}
	while(!fans.Init()){
		delay_ms(DELAY_ERROR);
		gpio_toggle_pin_level(LED0);
		
	}
	fans.condesator->SetEnable(false);
	fans.evaporator[0]->SetEnable(false);
	fans.evaporator[1]->SetEnable(false);
// 	
	while(!heaters.Init()){
		delay_ms(DELAY_ERROR);
		gpio_toggle_pin_level(LED0);
	}
// 	heater.DisableIndex(0);
// 	heater.DisableIndex(1);
// 	heater.DisableIndex(2);
// 	heater.DisableIndex(3);

	hvacTimer.Start_periodic_task(FUNC_PTR(FirmwareIsAlive),1000);
	uint8_t localSetpoint=210;
	uint8_t localPosition;
 	//flapper.valve1->StartControlling(220);
 	//flapper.valve2->StartControlling(220);
	
	userApp.Init();

	
	




	while (1)
	{
		
		if (userApp.IsAppConnected())
		{
			userApp.handleCommunication();			
			
		}
		else
		{
			//userApp.setConnected(false);
			//delay_ms(100);
		}
	//	delay_ms(10);
		
	//	while ((!flapper.valve1->gotSetpoint)|((!flapper.valve2->gotSetpoint)&false))
	//	{
			flapper.valve1->Control_NBC_StandAlone_Reset();
			flapper.valve2->Control_NBC_StandAlone_Reset();
	//	}
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
		counter_running++;
			//delay_ms(10);
		
// 		fvc1.fv->ClearMoveFault(true);
//		fvc1.fv->SetDirection(true);
// 		fvc1.fv->SetEnable(true);
// 		for (uint8_t j = 1; j <0xff; j++)
// 		{
// 			
// 			fvc1.fv->WriteSetpoint(j);
// 			delay_ms(10);
// 			fvc1.fv->ReadStatusInputs();
// 			delay_ms(10);
// 			position=fvc1.fv->ReadActualPosition();
// 			delay_ms(10);
// 			
// 		}
// 		fvc1.fv->SetEnable(false);
// 		
// 		delay_ms(500);
// 		fvc1.fv->SetDirection(false);
// 		fvc1.fv->SetEnable(true);
// 		for (uint8_t j = 1; j <0xff; j++)
// 		{
// 
// 			fvc1.fv->WriteSetpoint(j);
// 			delay_ms(10);
// 			position=fvc1.fv->ReadActualPosition();
// 			delay_ms(10);
// 			fvc1.fv->ReadStatusInputs();
// 			delay_ms(10);
// 		}
// 		fvc1.fv->SetEnable(false);
// 		delay_ms(500	
//		}
	}

	
}

