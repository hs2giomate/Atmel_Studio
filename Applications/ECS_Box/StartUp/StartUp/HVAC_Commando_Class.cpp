/* 
* HVAC_Commando_Class.cpp
*
* Created: 10/20/2020 12:00:05 PM
* Author: GMateusDP
*/


#include "HVAC_Commando_Class.h"
#include "ARINC_Interface.h"

// default constructor
HVAC_Commando_Class::HVAC_Commando_Class()
{
} //HVAC_Commando_Class

// default destructor
HVAC_Commando_Class::~HVAC_Commando_Class()
{
} //~HVAC_Commando_Class

bool HVAC_Commando_Class::IsCommando270Changed(void){
	
	if (lastCommando==UpdateCommando())
	{
		return false;
	} 
	else
	{
		return true;
	
	}
	return true;
}

uint32_t HVAC_Commando_Class::UpdateCommando(void){
	lastCommando=actualCommando;
	uint32_t value=arinc.ReadBufferLabel((uint32_t)270);
	commando270.AC_CMD=(bool)((value>>12)&0x01);
	commando270.statusArinc=(StatusHVACARINC)((value>>10)&0x03);
	actualCommando=value;
	return actualCommando;
}
uint32_t HVAC_Commando_Class::GetCommando270(void){
	return actualCommando;
}
uint8_t HVAC_Commando_Class::UpadateFlapperValveSetpoint(void){
	lastflapperValveSetpoint=flapperValveSetpoint;
	uint32_t value=arinc.ReadBufferLabel((uint32_t)275);
	uint8_t position=(uint8_t)((value>>10)&0xff);
	flapperValveSetpoint=position;
	return flapperValveSetpoint;
}
uint8_t HVAC_Commando_Class::GetFlapperValveSetpoint(void){
	return flapperValveSetpoint;
}

bool HVAC_Commando_Class::IsNewFlapperValveSetpoint(void){
		if (lastflapperValveSetpoint==UpadateFlapperValveSetpoint())
		{
			return false;
		}
		else
		{
			return true;
			
		}
		return true;
}