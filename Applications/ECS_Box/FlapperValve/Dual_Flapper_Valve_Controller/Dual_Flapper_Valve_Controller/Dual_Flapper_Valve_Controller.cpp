/* 
* Dual_Flapper_Valve_Controller.cpp
*
* Created: 12/21/2020 12:38:37 PM
* Author: GMateusDP
*/


#include "Dual_Flapper_Valve_Controller.h"
#include "Event_Logger_Class.h"

static FlapperValveController staticFVController[FLAPPER_VALVE_QUANTITY];
static uint8_t local_valve_gpios_buffer[ FLAPPER_VALVE_EXPANDERS];

static void Valve_1_Has_Changed(void)
{
	flapper.valve1->ReadGPIOS(local_valve_gpios_buffer);
	logger.SaveEventGPIOValue("Valve 1 , gpios = ",local_valve_gpios_buffer,FLAPPER_VALVE_EXPANDERS);
}
static void Valve_2_Has_Changed(void)
{
	flapper.valve2->ReadGPIOS(local_valve_gpios_buffer);
	logger.SaveEventGPIOValue("Valve 2 , gpios = ",local_valve_gpios_buffer,FLAPPER_VALVE_EXPANDERS);
}

// default constructor
Dual_Flapper_Valve_Controller::Dual_Flapper_Valve_Controller()
{
} //Dual_Flapper_Valve_Controller

// default destructor
Dual_Flapper_Valve_Controller::~Dual_Flapper_Valve_Controller()
{
} //~Dual_Flapper_Valve_Controller

bool Dual_Flapper_Valve_Controller::Init(){
	for (uint8_t i = 0; i < FLAPPER_VALVE_QUANTITY; i++)
	{
		valve[i]=&staticFVController[i];
		valve[i]->InitController(i);
		valve[i]->SetPartner(valve[1-i]);
	
	}
	for (uint8_t i = 0; i < FLAPPER_VALVE_QUANTITY; i++)
	{
		
		valve[i]->SetPartner(valve[1-i]);
		
	}
	valve1=valve[0]; 
	valve2=valve[1]; 
	ext_irq_register(PIN_PA04, Valve_1_Has_Changed);
	ext_irq_register(PIN_PA05, Valve_2_Has_Changed);
	return (valve1->isOK)|(valve2->isOK);
}

bool Dual_Flapper_Valve_Controller::IsPartnerOK(uint8_t partner_id){
	boolResult=false;
	if (partner_id==0)
	{
		boolResult=	!valve2->dataStruct.controlOutputs.iAlcFvStandAloneOut;
	} 
	else
	{
		boolResult=	!valve1->dataStruct.controlOutputs.iAlcFvStandAloneOut;
	}
	return boolResult;
}

bool Dual_Flapper_Valve_Controller::IsStandAlone(void){
	is_stand_alone=valve1->IsStandAloneMode()|valve2->IsStandAloneMode();
	return is_stand_alone;
}

Dual_Flapper_Valve_Controller flapper;
