/* 
* HEATERS_HANDLER.cpp
*
* Created: 1/12/2021 10:30:22 AM
* Author: GMateusDP
*/


#include "HEATERS_HANDLER.h"
#include "Event_Logger_Class.h"

static SingleHeater_Class	local_heaters[NUMBER_OF_HEATERS];
static	MCP23008_Class local_heater_expanders[NUMBER_OF_HEATERS*SINGLE_HEATER_EXPANDERS];
static uint8_t local_gpios_buffer[NUMBER_OF_HEATERS];

static void Heater_1_Has_Changed(void)
{
	heaters.module[0]->ReadGPIOs(local_gpios_buffer);
	logger.SaveEventGPIOValue("Heater 1 , gpios =  ",local_gpios_buffer,SINGLE_HEATER_EXPANDERS);
}
static void Heater_2_Has_Changed(void)
{
	heaters.module[1]->ReadGPIOs(local_gpios_buffer);
	logger.SaveEventGPIOValue("Heater 2 , gpios = ",local_gpios_buffer,SINGLE_HEATER_EXPANDERS);
}

// default constructor
HEATERS_HANDLER::HEATERS_HANDLER()
{
} //HEATERS_HANDLER

// default destructor
HEATERS_HANDLER::~HEATERS_HANDLER()
{
} //~HEATERS_HANDLER

bool HEATERS_HANDLER::Init(void){

	//InitHeaterExpanders();

	heater_expanders=local_heater_expanders;
	for (uint8_t i = 0; i < NUMBER_OF_HEATERS; i++)
	{
		module[i]=&local_heaters[i];
		while (!module[i]->Init(i,heater_expanders))
		{
			delay_ms(200);
			gpio_toggle_pin_level(LED0);
			
		}
		
	}
	ext_irq_register(PIN_PA03, Heater_1_Has_Changed);
	ext_irq_register(PIN_PC10, Heater_2_Has_Changed);
	
	isOK=(module[0]->isOK)&(module[1]->isOK);
	return	isOK;
	
}

void HEATERS_HANDLER::InitHeaterExpanders(void){
	for (uint8_t i = 0; i < NUMBER_OF_HEATERS; i++)
	{
		for (uint8_t j = 0; j < SINGLE_HEATER_EXPANDERS; j++)
		{
			heater_expanders=&local_heater_expanders[2*i+j];
		}
	}
}

void	HEATERS_HANDLER::ReadAllGPIOs(uint8_t *buffer){
	for (uint8_t i = 0; i < 2; i++)
	{
	//	heater_data_array[i]=local_heater_expanders[i].ReadGPIORegister();
		module[i]->ReadGPIOs(&buffer[i*2]);
		//heater_data_array=buffer;
	}
	
}
HEATERS_HANDLER heaters;