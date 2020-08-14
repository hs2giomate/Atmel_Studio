/*
 * CANOpenNMTSlave.cpp
 *
 * Created: 8/12/2020 12:57:31 PM
 * Author : GMateusDP
 */ 



#include "atmel_start.h"

#include "CANOpen_Class.h"

static CANOpen_Class cano(&CAN_CCU);

int main(void)
{
	atmel_start_init();
	delay_ms(1000);
	cano.Init();
	/* Replace with your application code */
	while (1)
	{
		cano.Process();
		delay_us(500);
		
		cano.Periodic_Process();
		delay_us(500);
	}
}

