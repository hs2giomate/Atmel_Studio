/* 
* Cooling_Controller.cpp
*
* Created: 12/8/2020 11:37:35 AM
* Author: GMateusDP
*/


#include "Cooling_Controller.h"
#include "EvaporatorAndCondesatorFans_Class.h"
#include "SingleHeater_Class.h"
#include "utils.h"
#include "math.h"

// default constructor
Cooling_Controller::Cooling_Controller()
{
	tolerance=1;
} //Cooling_Controller

// default destructor
Cooling_Controller::~Cooling_Controller()
{
} //~Cooling_Controller

uint8_t Cooling_Controller::Control_Cooler(float errorTemp){
	errorTemperature=errorTemp;
	if (errorTemperature<0)
	{
		if (fans.condesator->IsEnabled())
		{
			if (heater.GetHeaterPowerLevel()>0)
			{
				heater.DisableAll();
				fans.evaporator[1]->SetPWM(EVAPORATOR_MINIMUN_FLOW_AIR);
				fans.evaporator[0]->SetPWM(EVAPORATOR_MINIMUN_FLOW_AIR);

			}
			else
			{
				
				if (abs(errorTemperature)>1)
				{
					fans.evaporator[1]->SetPWM(EVAPORATOR_MINIMUN_FLOW_AIR);
					fans.evaporator[0]->SetPWM(EVAPORATOR_MINIMUN_FLOW_AIR);
					fans.condesator->SetPWM(CalculatePWMCondesator());
					}else{
					fans.condesator->SetEnable(false);
				}
				
			}
		}
		else
		{
			fans.condesator->SetEnable(true);
		}
	} 
	else
	{
		fans.condesator->SetPWM(CONDESATOR_MINIMUN_FLOW_AIR);
	}
	
	return pwmCondesator;

}

uint8_t Cooling_Controller::CalculatePWMCondesator(void){
	if (errorTemperature<0)
	{
		floatPWM=(-1*errorTemperature*PROPORTIONAL_CONTROL_VALUE)+CONDESATOR_MINIMUN_FLOW_AIR;
		pwmCondesator=(uint8_t)(min(floatPWM,0xff));
	} 
	else
	{
		pwmCondesator=CONDESATOR_MINIMUN_FLOW_AIR;
	}

	
	return pwmCondesator;
}
