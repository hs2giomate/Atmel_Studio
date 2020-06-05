/*
 * I2C_Class.cpp
 *
 * Created: 4/23/2020 11:26:44 AM
 * Author : GMateusDP
 */ 


#include "atmel_start.h"
#include "I2C_Class.h"

static I2C_Class	eeprom(&I2C_EEPROM);



int main(void)
{
	atmel_start_init();
	eeprom.init(); 
	char demo_name[]="DC2209"; 

    /* Replace with your application code */
    while (1) 
    {
		
    }
}
