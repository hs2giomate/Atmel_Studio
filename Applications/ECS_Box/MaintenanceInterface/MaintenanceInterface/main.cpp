/*
 * MaintenanceInterface.cpp
 *
 * Created: 9/22/2020 10:14:58 AM
 * Author : GMateusDP
 */ 


#include "main.h"
//static FlapperValveController	fvc;
static float currentTemperature,lastTemperature;
bool localtimeout=false;
uint32_t	errorCounter=0;
static void FirmwareIsAlive(const struct timer_task *const timer_task)
{
	gpio_set_pin_level(iAlcFwIsAlive,true);
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
	bool isOK=temperatures.Init();
	if (isOK)
	{
		temperatures.StartOneConversion();
	}
	gpio_set_pin_level(LED0,true);
	while (!fvc.InitController())
	{
		delay_ms(200);
		gpio_toggle_pin_level(LED0);
	}
	while(!fans.Init()){
		delay_ms(200);
		gpio_toggle_pin_level(LED0);
		
	}
	fans.condesator->SetEnable(false);
	fans.evaporator[0]->SetEnable(false);
	fans.evaporator[1]->SetEnable(false);
	
	while(!heater.Init()){
		delay_ms(200);
		gpio_toggle_pin_level(LED0);
	}
	heater.DisableIndex(0);
	heater.DisableIndex(1);
	heater.DisableIndex(2);
	heater.DisableIndex(3);

	//hvacTimer.Start_periodic_task(FUNC_PTR(FirmwareIsAlive),2500);
	uint8_t localSetpoint=210;
	uint8_t localPosition;

	
	toolApp.Init();
	memory.Init();
	memory.WriteDefaultParameters();

	fvc.UpdateFlapperValveData();
	fvc.fv->SetEnable(false);
	fvc.fv->SetInvalidPosition(false);
	fvc.fv->ClearMoveFault(false);
	delay_ms(100);
	fvc.fv->SetInvalidPosition(true);
	if (fvc.dataStruct.controlOutputs.niAlcFvMotorEnable)
	{
		fvc.fv->SetEnable(false);
	}
	
	fvc.fv->ClearMoveFault(true);
	fvc.fv->WriteSetpoint(localSetpoint);
	fvc.fv->SetDirection(false);
	if (fvc.fv->ReadActualPosition()>localSetpoint)
	{
		fvc.fv->SetDirection(true);
	} 
	else
	{
		fvc.fv->SetDirection(false);
	}
		hvacTimer.Start_oneShot_task(FUNC_PTR(FirmwareIsAlive),5000);
			fvc.fv->SetEnable(true);
	localPosition=fvc.fv->ReadActualPosition();
	while ((abs(localPosition-localSetpoint)>6)&(!localtimeout))
	{
		localPosition=fvc.fv->ReadActualPosition();
	}
	fvc.fv->SetEnable(false);

	while (1)
	{
		if (toolApp.IsAppConnected())
		{
			toolApp.handleCommunication();			
			
		}
		else
		{
			toolApp.setConnected(false);
			//delay_ms(100);
		}
		
		if ((fvc.IsInvalidPosition())&(!fvc.flapperValveIsMoving))
		{
			if (errorCounter<10)
			{
				errorCounter++;
				fvc.Control_NBC_StandAlone_Reset();
			} 
			else
			{
				fvc.InitController();
				errorCounter=0;
			}
			
		} 
		else
		{
			fvc.Control_NBC_StandAlone_Reset();
		}
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
		//delay_ms(10);
		
// 		fvc1.fv->ClearMoveFault(true);
// 		fvc1.fv->SetDirection(true);
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
	}

	
}

