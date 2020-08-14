/*
 * CANOpenNMTMaster.cpp
 *
 * Created: 8/12/2020 9:53:04 AM
 * Author : GMateusDP
 */ 



#include "atmel_start.h"
#define NMT_MASTER_NODE	1
#include "CANOpen_Class.h"

 

int main(void)
{
	atmel_start_init();
	CANOpen_Class cano(&CAN_CCU);
	
	cano.Init(NMT_MASTER_NODE);

	/* Replace with your application code */
	while (1)
	{

		cano.Process();
		delay_ms(1);
// 		cano.Periodic_Process();
//  		delay_ms(1);
	}
}