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
	delay_ms(100);
	cano.Init();
	delay_ms(100);
	//	cano.Process();
	/* Replace with your application code */
	while (1)
	{
		cano.Process(1);
		//delay_ms(10);
		
		cano.Periodic_Process(1000);
		//delay_ms(10);
		
		cano.Pass_Temperature_To_voltage();
	}
}

