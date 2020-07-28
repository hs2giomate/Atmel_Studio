/*
 * LedLightingVerification.cpp
 *
 * Created: 7/15/2020 1:03:12 PM
 * Author : GMateusDP
 */ 

#include <atmel_start.h>
#include "main.h"

int main(void)
{
	atmel_start_init();
	lighter.Init();
    /* Replace with your application code */
    while (1) 
    {
		lighter.Run();
    }
}
