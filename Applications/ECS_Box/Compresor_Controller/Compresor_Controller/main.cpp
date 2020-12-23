/*
 * Compresor_Controller.cpp
 *
 * Created: 12/16/2020 10:35:04 AM
 * Author : GMateusDP
 */ 


#include "atmel_start.h"
#include "Compresor_Controller.h"



int main(void)
{
	atmel_start_init();
	
	ccu.Init();
	uint8_t i = 0;
	/* Replace with your application code */
	while (1)
	{
		if (ccu.IsSlaveOpeational())
		{
			ccu.Periodic_Task();
		} 
		else
		{
			
			
				
		}
		ccu.expander->WriteGPIORegister(i);
		delay_ms(10);
		
		i++;
// 
// 		cano.Process();
// 		delay_ms(1);
		// 		cano.Periodic_Process();
		//  		delay_ms(1);
	}
}