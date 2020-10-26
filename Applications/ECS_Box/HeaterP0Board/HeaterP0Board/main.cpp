/*
 * HeaterP0Board.cpp
 *
 * Created: 22.10.2020 14:24:41
 * Author : technik
 */ 


#include "main.h"
static SingleHeater_Class heater;
#define DELAY_HEATER_COMMUNICATION 100

static void FirmwareAlive(const struct timer_task *const timer_task)
{
		//gpio_toggle_pin_level(iAlcFwIsAlive);
		gpio_set_pin_level(iAlcFwIsAlive,true);
}

int main(void)
{
	atmel_start_init();
	uint8_t status;

	usb.Init();
	delay_ms(100);
	usb<<NEWLINE<<NEWLINE<<"*** StartUp Algorithmen Test ***"<<NEWLINE;
	usb<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<NEWLINE<<NEWLINE;
	uint32_t p=pwm_enable(&LIVE_PULSE);
	hvacTimer.Start_periodic_task(FUNC_PTR(FirmwareAlive),250);
	heater.Init();
	while (1)
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			delay_ms(DELAY_HEATER_COMMUNICATION);
			usb<<"Enabling Heater: "<<i<<" .\t";
			heater.Enable(i);
			delay_ms(DELAY_HEATER_COMMUNICATION);
			status= heater.ReadStatus();
			usb<<"Heater "<<i<< " Status :"<<heater.heaterGPIO.inputs.niAlcHeaterRelayFault[i]<<NEWLINE;
			delay_ms(DELAY_HEATER_COMMUNICATION);
			heater.Disable(i);
		}
		

	}
}
