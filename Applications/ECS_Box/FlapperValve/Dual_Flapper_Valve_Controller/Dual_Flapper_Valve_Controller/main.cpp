/*
 * Dual_Flapper_Valve_Controller.cpp
 *
 * Created: 12/21/2020 11:26:55 AM
 * Author : GMateusDP
 */ 



#include "main.h"

int main(void)
{
	atmel_start_init();
	uint8_t position;

	usb.Init();
	delay_ms(100);

	memory.Init();
	memory.WriteDefaultParameters();
	fv.Init();

	//toolApp.Init();
	while (1)
	{
// 		for ( int i = 0; i < FLAPPER_VALVE_QUANTITY; i++)
// 		{
// 			
// 		}
		
		for (uint8_t i = 1; i < 8; i++)
		{
			fv.fv1->StartControlling(i*32);
			fv.fv2->StartControlling(i*32);
			while ((!fv.fv1->gotSetpoint)|(!fv.fv2->gotSetpoint))
			{
				fv.fv1->Control_NBC_StandAlone_Reset();
				fv.fv2->Control_NBC_StandAlone_Reset();
			}
		
			delay_ms(100);
			fv.fv1->StopControlling();
			fv.fv2->StopControlling();
		}
		delay_ms(1000);
   		for (uint8_t i = 1; i < 8; i++)
		{
			fv.fv1->StartControlling(255-i*32);
			fv.fv2->StartControlling(255-i*32);
			while ((!fv.fv1->gotSetpoint)|(!fv.fv2->gotSetpoint))
			{
				fv.fv1->Control_NBC_StandAlone_Reset();
				fv.fv2->Control_NBC_StandAlone_Reset();
			}
			delay_ms(100);
			fv.fv1->StopControlling();
			fv.fv2->StopControlling();
		
		}
	delay_ms(1000);
	


	

	}
}
