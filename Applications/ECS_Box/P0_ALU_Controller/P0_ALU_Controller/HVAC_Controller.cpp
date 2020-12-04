/* 
* HVAC_Controller.cpp
*
* Created: 12/2/2020 10:53:31 AM
* Author: GMateusDP
*/


#include "HVAC_Controller.h"
#include "SingleHeater_Class.h"
#include "EvaporatorAndCondesatorFans_Class.h"
#include "FlapperValveController.h"
#include "States_Class.h"
#include "Timer_Class.h"

static bool staticTemperingTimeout=true;
static void  HeaterTemperingTimeout(const struct timer_task *const timer_task){
	hvac.temperingTimeout=true;
	staticTemperingTimeout=true;
}

// default constructor
HVAC_Controller::HVAC_Controller()
{
	temperingTimeout=true;
} //HVAC_Controller

// default destructor
HVAC_Controller::~HVAC_Controller()
{
} //~HVAC_Controller

uint8_t HVAC_Controller::SetHeaterDriver(uint8_t heatPower){
	if (staticTemperingTimeout)
	{
			if (AdjustFanSpeed(heatPower))
			{
				timeoutValueTempering=automaticHeatingMode?10000:5000;
				staticTemperingTimeout=false;
				hvacTimer.Start_oneShot_task(FUNC_PTR(HeaterTemperingTimeout),timeoutValueTempering);
				for (uint8_t i = 0; i < 4; i++)
				{
					if (i<heatPower)
					{
						heater.EnableIndex(i);
					}
					else
					{
						heater.DisableIndex(i);
					}
					
				}
			}
			else
			{
			}
	} 
	else
	{
		temperingTimeout=staticTemperingTimeout;
	}

	return heatPower;
}

bool	HVAC_Controller::AdjustFanSpeed(uint8_t heatPower){
	if (heatPower>0)
	{
		//	fans.evaporator[0]->SetPWM(heatersPower*64-1);
		fans.condesator->SetPWM(heatPower*64-1);
	}
	else
	{
		//fans.evaporator[0]->SetPWM(MINIMUN_FLOW_AIR);
		fans.condesator->SetPWM(MINIMUN_FLOW_AIR);
	}
	//fans.evaporator[0]->ReadStatus();
	fans.condesator->ReadStatus();
	//return fans.evaporator[0]->evaporatorFansStatus.inputs->iAlcEvaFanPwmFault;
	return fans.condesator->condesatorStatus.niAlcCdsFanExtFault;
}

uint8_t	HVAC_Controller::OperateFlapperValve(FlapperValveMode fvm){
	
	if (lastFlapperValveMode!=fvm)
	{
		lastFlapperValveMode=flapperValveMode;
		flapperValveMode=fvm;
		if (fvc.IsNBCMode())
		{
			if (flapperValveMode==NBC_MODE)
			{
			} 
			else
			{
				fvc.SetRemoteNBCMode(false);
			}
		} 
		else
		{
			switch (flapperValveMode)
			{
				case FRESHAIR_MODE:
				fvc.StartControlling(0);
				
				break;
				case INTERM_MODE:
				fvc.StartControlling(FLAPPER_VALVE_MINIMUM_AIR/2);
				break;
				case RECYCLE_MODE:
				fvc.StartControlling(FLAPPER_VALVE_MINIMUM_AIR);
				break;
				case NBC_MODE:
				fvc.SetRemoteNBCMode(true);
				break;
				default:
				fvc.StartControlling(FLAPPER_VALVE_MINIMUM_AIR);
				break;
			}
		}
		
		
	}else{
		flapperValveAngle=fvc.GetCurrentPosition();
	}
	hvac.hvacState->arincStatus.statusFV=flapperValveMode;
	return flapperValveAngle;
}