/*
 * Internal_Temperature_Class.cpp
 *
 * Created: 3/16/2020 9:24:27 AM
 * Author : GMateusDP
 */ 


#include "atmel_start.h"


#include "CPU_Temperature_Class.h"

int main(void)
{
		atmel_start_init();
		CPU_Temperature_Class tCPU;
		tCPU.Init();
		int32_t t;
    /* Replace with your application code */
    while (1) 
    {
		t=tCPU.GetTemperatureCPU();
		delay_ms(1000);
    }
}
