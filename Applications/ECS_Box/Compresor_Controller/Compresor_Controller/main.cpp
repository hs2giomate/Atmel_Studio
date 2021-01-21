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
	ccu.Set_CAN_Enable(ccu.IsEnabledPointer());
	ccu.SetEnable(false);
	bool  timeout;
	/* Replace with your application code */
	while (1)
	{
// 		if (ccu.IsSlaveOpeational())
// 		{
// 			ccu.Periodic_Task();
// 		} 
// 		else
// 		{
// 			ccu.Periodic_Task();
// 			
// 				
// 		}
		ccu.SetEnable(true);
		
		for (int dir = 1; dir > -2; dir=dir-2)
		{
			
			for (uint16_t i = (1-dir)*0x7fff; i < (dir+1)*0x7fff; i=i+dir*1)
			//for (uint16_t i = 0; i <0xffff ; i++)
			{
				ccu.Set_Motor_Speed(i);
				timeout=*ccu.syncCANOpenTimeout;
				while (!timeout)
				{
					gpio_set_pin_level(LED0,false);
					timeout=*ccu.syncCANOpenTimeout;
				}
				ccu.Periodic_Task();
				gpio_set_pin_level(LED0,true);
			}
				
				
		}
		ccu.SetEnable(true);
		delay_ms(1000);
		
	
// 
// 		cano.Process();
// 		delay_ms(1);
		// 		cano.Periodic_Process();
		//  		delay_ms(1);
	}
}