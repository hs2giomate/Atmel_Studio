/*
 * BeverageMakerHMIStatus.cpp
 *
 * Created: 11/4/2020 3:51:55 PM
 * Author : GMateusDP
 */ 

#include "main.h"

static LedStateMachine hmi;

int main(void)
{
	hmi.Init();
    /* Replace with your application code */
    while (1) 
    {
		hmi.RunManual();
		//hmi.WhiteBarPulsing();
		//hmi.LedLeftButtonGreenPulsing();
		//hmi.Heating();
		//hmi.Filling();
		//hmi.Ready();
		//hmi.WaitNoWater();
	//	hmi.WaitOverTemperature();
	//hmi.FaultLowWater();
	//	hmi.FaultOvertemeperature();
		//hmi.FaultDrainage();
    }
}


