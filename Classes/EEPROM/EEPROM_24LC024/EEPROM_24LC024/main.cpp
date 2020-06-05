/*
 * EEPROM_24LC024.cpp
 *
 * Created: 4/29/2020 8:49:12 AM
 * Author : GMateusDP
 */ 

#include "atmel_start.h"
#include "EEPROM_24LC024_Class.h"


 


int main(void)
{
	
	atmel_start_init();
	EEPROM_24LC024_Class	eeprom(&I2C_EEPROM);
	eeprom.init();
	char demo_name[]="DC2209";
    /* Replace with your application code */
    while (1) 
    {
		eeprom.discover_demo(demo_name);
    }
}
