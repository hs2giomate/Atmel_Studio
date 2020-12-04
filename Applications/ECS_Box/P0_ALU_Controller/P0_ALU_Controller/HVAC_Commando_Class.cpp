/* 
* HVAC_Commando_Class.cpp
*
* Created: 10/20/2020 12:00:05 PM
* Author: GMateusDP
*/


#include "HVAC_Commando_Class.h"
#include "ARINC_Handler_Class.h"
#include "EventHandler_Class.h"
#include "States_Class.h"

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
	uint32_t value=arinc.ar->ReadBufferLabel((uint32_t)270);
	commando270.AC_CMD=(bool)((value>>12)&0x01);
	commando270.statusArinc=(StatusHVACARINC)((value>>10)&0x03);
	commando270.statusFV=(FlapperValveMode)((value>>15)&0x03);
	actualCommando=value;
	return actualCommando;
}
uint32_t HVAC_Commando_Class::GetCommando270(void){
	return actualCommando;
}
float HVAC_Commando_Class::UpadateTemperatureSetpoint(void){
	lastTemperatureSetpoint=temperatureSetpoint;
	uint32_t value=arinc.ar->ReadBufferLabel((uint32_t)204);
	uint16_t temp16=(uint16_t)((value>>12)&0xffff);
	temperatureSetpoint=temp16;
	return temperatureSetpoint;
}
uint8_t HVAC_Commando_Class::UpadateFlapperValveSetpoint(void){
	lastflapperValveSetpoint=flapperValveSetpoint;
	uint32_t value=arinc.ar->ReadBufferLabel((uint32_t)375);
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

void	HVAC_Commando_Class::HandleCommands(void){
	if (IsCommando270Changed())
	{
		// 		if (commando270.AC_CMD)
		// 		{
		//
		// 			prepareStateChangeEvent(kHVACStatePrepareStandbyON);
		// 			listener.SendEventSelf(kInterfaceEventClass,k_AC_CMD_ENABLE_Event);
		// 		}
		// 		else
		// 		{
		// 			prepareStateChangeEvent(kHVACStatePrepareStandbyOFF);
		// 			listener.SendEventSelf(kInterfaceEventClass,k_AC_CMD_DISABLE_Event);
		// 		}
		hvac.OperateFlapperValve(commando270.statusFV);
		switch(commando270.statusArinc)
		{
			case COMMAND_MODE_AUTO:
			//	hvac.prepareStateChangeEvent(kHVACStateStandbyAUTO);
				listener.SendEventSelf(kInterfaceEventClass,k_AC_AUTO_Event);
			break;
			case COMMAND_MODE_VENT:
				//hvac.prepareStateChangeEvent(kHVACStateStandbyVENT);
				listener.SendEventSelf(kInterfaceEventClass,k_AC_VENT_Event);
			break;
			case COMMAND_MODE_OFF:
			//	hvac.prepareStateChangeEvent(kHVACStateStandbyOFF);
				listener.SendEventSelf(kInterfaceEventClass,k_AC_OFF_Event);
				break;
			default:
				//hvac.prepareStateChangeEvent(kHVACStatePrepareStandbyOFF);
			//	listener.SendEventSelf(kInterfaceEventClass,k_AC_CMD_DISABLE_Event);
			
			break;
			
		}
	//	arinc.ar->SaveStatus(actualCommando);
		
		

	}
}