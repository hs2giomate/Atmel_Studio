/*
 * EvaporatorAndCondesatorFan.cpp
 *
 * Created: 10/23/2020 1:41:53 PM
 * Author : GMateusDP
 */ 


#include "main.h"

#define		DELAY_TIME_REQUEST 100
static EvaporatorAndCondesatorFans_Class fans;

static void FirmwareAlive(const struct timer_task *const timer_task)
{
	gpio_toggle_pin_level(iAlcFwIsAlive);
}


int main(void)
{
	atmel_start_init();
	uint8_t status;

	usb.Init();
	delay_ms(100);
	usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	pwm_enable(&LIVE_PULSE);
	fans.Init();
	hvacTimer.Start_periodic_task(FUNC_PTR(FirmwareAlive),250);
	while (1)
	{
		for (uint8_t j = 0; j <0xff; j++)
		{
			for (uint8_t i = 0; i < 2; i++)
			{
				delay_ms(DELAY_TIME_REQUEST);
				usb<<"Enabling Evaporator Fan: "<<i<<" .\t";
				fans.evaporator[i]->Enable();
				delay_ms(DELAY_TIME_REQUEST);
				status= fans.evaporator[i]->ReadStatus();
				usb<<"Evaporator Fan "<<i<< " Status :"<<status<<" .\t";
				delay_ms(DELAY_TIME_REQUEST);
				status= fans.evaporator[i]->SetPWM(j);
				usb<<"Evaporator Fan "<<i<< " SetPoint :"<<status<<" .\t";
				delay_ms(DELAY_TIME_REQUEST);
				fans.evaporator[i]->Disable();
				usb<<" EvaporatorFan "<<i<< " Disabled"<<NEWLINE;
			
			}
			delay_ms(DELAY_TIME_REQUEST);
			usb<<NEWLINE<<"Enabling Condesator .\t";
			fans.condesator->Enable();
			delay_ms(DELAY_TIME_REQUEST);
			status= fans.condesator->ReadStatus();
			usb<<"Condesator  Status :"<<status<<" .\t";
			delay_ms(DELAY_TIME_REQUEST);
			status= fans.condesator->SetPWM(j);
			usb<<"Condesator SetPoint :"<<status<<" .\t";
			delay_ms(DELAY_TIME_REQUEST);
			fans.condesator->Disable();
			usb<<"Condesator Disabled"<<NEWLINE;
		}

		

	}
}
