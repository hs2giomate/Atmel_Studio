/* 
* Dual_Flapper_Valve_Controller.cpp
*
* Created: 12/21/2020 12:38:37 PM
* Author: GMateusDP
*/


#include "Dual_Flapper_Valve_Controller.h"

static FlapperValveController staticFVController[FLAPPER_VALVE_QUANTITY];

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
		fvx[i]=&staticFVController[i];
		fvx[i]->InitController(i);
		fvx[i]->SetPartner(fvx[1-i]);
	
	}
	fv1=fvx[0]; 
	fv2=fvx[1]; 
	return (fv1->isOK)&(fv2->isOK);
}

bool Dual_Flapper_Valve_Controller::IsPartnerOK(uint8_t partner_id){
	boolResult=false;
	if (partner_id==0)
	{
		boolResult=	!fv2->dataStruct.controlOutputs.iAlcFvStandAloneOut;
	} 
	else
	{
		boolResult=	!fv1->dataStruct.controlOutputs.iAlcFvStandAloneOut;
	}
	return boolResult;
}

Dual_Flapper_Valve_Controller fv;
