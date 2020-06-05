/* 
* LTC2983_Class.cpp
*
* Created: 4/23/2020 9:06:36 AM
* Author: GMateusDP
*/


#include "LTC2983_Class.h"

// default constructor
LTC2983_Class::LTC2983_Class()
{
} //LTC2983_Class

// default destructor
LTC2983_Class::~LTC2983_Class()
{
} //~LTC2983_Class

SPI_Syn_Class	LTC2983_Class::spiLTC(&SPI_LTC2983);

void LTC2983_Class::setup(){
	spiLTC.init();
	spiLTC.enable();
	
}

