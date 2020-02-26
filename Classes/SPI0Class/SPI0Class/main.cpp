/*
 * SPI0Class.cpp
 *
 * Created: 1/30/2020 8:59:30 AM
 * Author : GMateusDP
 */ 


#include "sam.h"
#include <atmel_start.h>
#include "SPIClass.h"
int main(void)
{
	atmel_start_init();
	SPIClass	spi0_interface(0);
	spi0_interface.init();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}
