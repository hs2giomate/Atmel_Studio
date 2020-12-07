/*
 * ARINC_Interface.cpp
 *
 * Created: 11/30/2020 9:27:29 AM
 * Author : GMateusDP
 */ 

#include "atmel_start.h"
#include "atmel_start_pins.h"

#include "Timer_Class.h"

#include "ARINC_Handler_Class.h"

static bool arincTXTimeUP=false;

static void ARINCTimeUp(const struct timer_task *const timer_task){
	arincTXTimeUP=true;
}


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	/*bool gotNewTemperature=false;*/
	atmel_start_init();
	//StartLivePulse();
	delay_ms(200);
	
	arinc.ar->Init();
	arincTimer.Start_oneShot_task(FUNC_PTR(ARINCTimeUp),500);
	/* Replace with your application code */
	while (1) {
		if (arincTXTimeUP)
		{
			arinc.PollData();
			arincTXTimeUP=false;
			arincTimer.Start_oneShot_task(FUNC_PTR(ARINCTimeUp),500);
			
		} 
		else
		{
			if (arinc.R1GotMessage())
			{
				arinc.HandleCommunication();
				
			}
			else
			{
// 				if ((fvc.IsInvalidPosition())&(!fvc.flapperValveIsMoving))
// 				{
// 					if (errorCounter<10)
// 					{
// 						errorCounter++;
// 						fvc.Control_NBC_StandAlone_Reset();
// 					}
// 					else
// 					{
// 						fvc.InitController();
// 						errorCounter=0;
// 					}
// 					
// 				}
// 				else
// 				{
// 					fvc.Control_NBC_StandAlone_Reset();
// 			// 				if (temperatures.IsConversionFinished())
// 				{
// 					
// 					temperatures.GetConversionResult();
// 					if (temperatures.faultData==VALID_TEMPERATURE)
// 					{
// 						lastTemperature=currentTemperature;
// 						currentTemperature=temperatures.lastValue;
// 						if (lastTemperature!=currentTemperature)
// 						{
// 							
// 							gotNewTemperature=true;
// 							
// 						}
// 						
// 					}
// 					temperatures.StartOneConversion();
// 					asm("nop");
// 					
// 				}
			}
		}
		
		
		
		
	}
}
