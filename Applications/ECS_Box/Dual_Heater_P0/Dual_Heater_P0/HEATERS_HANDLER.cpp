/* 
* HEATERS_HANDLER.cpp
*
* Created: 1/12/2021 10:30:22 AM
* Author: GMateusDP
*/


#include "HEATERS_HANDLER.h"

static SingleHeater_Class	local_heaters[NUMBER_OF_HEATERS];

// default constructor
HEATERS_HANDLER::HEATERS_HANDLER()
{
} //HEATERS_HANDLER

// default destructor
HEATERS_HANDLER::~HEATERS_HANDLER()
{
} //~HEATERS_HANDLER

bool HEATERS_HANDLER::Init(void){

	for (uint8_t i = 0; i < NUMBER_OF_HEATERS; i++)
	{
		module[i]=&local_heaters[i];
		while (!module[i]->Init(i))
		{
			delay_ms(200);
			gpio_toggle_pin_level(LED0);
			
		}
	}

	
	isOK=(module[0]->isOK)&(module[1]->isOK);
	return	isOK;
	
}
HEATERS_HANDLER heaters;