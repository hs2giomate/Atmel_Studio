/* 
* PinCodding_SAMD21Xplained.cpp
*
* Created: 7/27/2020 10:11:36 AM
* Author: GMateusDP
*/


#include "PinCodding_SAMD21Xplained.h"

uint8_t		ext2Inputs[]={
	GPIO(0,10),
	GPIO(0,20),
	GPIO(1,12),
	GPIO(1,14),
	GPIO(0,8),
	GPIO(1,11),
	GPIO(0,17),
	GPIO(0,16),
};

uint8_t		ext2Outputs[]={
	GPIO(0,11),
	GPIO(0,21),
	GPIO(1,13),
	GPIO(1,15),
	GPIO(0,9),
	GPIO(1,10),
	GPIO(0,18),
	GPIO(0,19),
};


// default constructor
PinCodding_SAMD21Xplained::PinCodding_SAMD21Xplained()
{
} //PinCodding_SAMD21Xplained
// default constructor
PinCodding_SAMD21Xplained::PinCodding_SAMD21Xplained(Connector& cn)
{
	connector=cn;
} //PinCodding_SAMD21Xplained

// default destructor
PinCodding_SAMD21Xplained::~PinCodding_SAMD21Xplained()
{
} //~PinCodding_SAMD21Xplained

bool	PinCodding_SAMD21Xplained::Init(void){
		for (i = 0; i < 8; i++)
		{
			connector.pinsInput[i]=ext2Inputs[i];
			gpio_set_pin_direction(connector.pinsInput[i],GPIO_DIRECTION_IN);
			gpio_set_pin_pull_mode(connector.pinsInput[i],GPIO_PULL_UP);
			gpio_set_pin_function(connector.pinsInput[i],GPIO_PIN_FUNCTION_OFF);
			connector.pinsOutput[i]=ext2Outputs[i];
			gpio_set_pin_direction(connector.pinsOutput[i],GPIO_DIRECTION_OUT);
			gpio_set_pin_function(connector.pinsOutput[i],GPIO_PIN_FUNCTION_OFF);
			gpio_set_pin_level(connector.pinsOutput[i],false);
			
		}
		return	i>=8;
}


PinCodding_SAMD21Xplained	EXT2;