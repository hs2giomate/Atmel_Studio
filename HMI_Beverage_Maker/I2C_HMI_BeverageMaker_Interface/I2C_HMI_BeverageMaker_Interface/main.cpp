/*
 * I2C_HMI_BeverageMaker_Interface.cpp
 *
 * Created: 11/5/2020 5:11:32 PM
 * Author : GMateusDP
 */ 

#include <avr/io.h>
#include "compiler.h"
#include "LedStateMachine.h"
#include "PSC_I2C_Interface.h"
static LedStateMachine hmi;
static	PSC_I2C_Interface psc;
volatile bool back=false;


int main(void)
{
	hmi.Init();
	psc.Init();
	uint8_t commando=0;
    /* Replace with your application code */
    while (1) 
    {
		if (psc.GotData())
		{
			commando=psc.ReadCommand();
		} 
		else
		{
			hmi.RunState(commando);
		}
		
    }
}

