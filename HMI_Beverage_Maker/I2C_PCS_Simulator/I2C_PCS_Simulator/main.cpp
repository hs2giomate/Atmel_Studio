/*
 * I2C_PCS_Simulator.cpp
 *
 * Created: 11/5/2020 4:08:05 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "I2C_Sync_Class.h"

static I2C_Sync_Class	pcs(&I2C_0);
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	pcs.Init(0x42);
	/* Replace with your application code */
	while (1) {
		
		for (uint8_t i = 1; i < 10; i++)
		{
			pcs.Write(&i,1);
			asm("nop");
			delay_ms(10000);
			asm("nop");
		}
		
	}
}


