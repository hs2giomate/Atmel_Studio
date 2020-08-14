/*
 * CortexModultTest.cpp
 *
 * Created: 7/22/2020 12:27:59 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "main.h"


int main(void)
{
		atmel_start_init();
		app.Init();
    /* Replace with your application code */
    while (1) 
    {
	flash.eraseFlash(0x100,TEST_FLASH_SIZE*ISSI_BUFFER_SIZE*1);
	while (app.FastMemoryCheck());
		
		app.Run();
    }
}
