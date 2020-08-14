/*
 * CANOpenMaster.cpp
 *
 * Created: 8/13/2020 2:43:30 PM
 * Author : GMateusDP
 */ 



#include "atmel_start.h"

#include "CANOpen_Class.h"

static CANOpen_Class cano(&CAN_CCU);

int main(void)
{
	atmel_start_init();
	cano.Init(1);
	while(!cano.StartNMTService());
	/* Replace with your application code */
	while (1)
	{
		
		cano.Process();
		delay_us(500);
		
		cano.Periodic_Process();
		delay_us(500);
	}
}
