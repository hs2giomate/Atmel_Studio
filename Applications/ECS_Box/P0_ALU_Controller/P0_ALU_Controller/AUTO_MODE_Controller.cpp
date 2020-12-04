/* 
* VENT_MODE_Controller.cpp
*
* Created: 12/2/2020 10:54:31 AM
* Author: GMateusDP
*/


#include "AUTO_MODE_Controller.h"
#include "TemperatureSensors_Class.h"
#include "SingleHeater_Class.h"
#include "FlapperValveController.h"
#include "States_Class.h"

// default constructor
AUTO_MODE_Controller::AUTO_MODE_Controller()
{
	heatersPower=0;
	evaFanSpeed=0;
	
} //VENT_MODE_Controller

// default destructor
AUTO_MODE_Controller::~AUTO_MODE_Controller()
{
} //~VENT_MODE_Controller


void AUTO_MODE_Controller::ControlTemperatureSetpoint(uint8_t opt){
	if (hvac.temperingTimeout)
	{
		cabinCurrentTemperature=temperatures.values[0][3];
		if (abs(lastCabinTemperature-cabinCurrentTemperature)>1/2)
		{
			lastCabinTemperature=cabinCurrentTemperature;
			if (cabinCurrentTemperature>0)
			{
				
				setpointTemperature=hvac.UpadateTemperatureSetpoint();
				errorTemperature=setpointTemperature-cabinCurrentTemperature;
				tolerance=errorTemperature>0?1:2;
				if (abs(errorTemperature)>tolerance)
				{
					
					heatersPower=heater.GetHeaterPowerLevel();
					if (errorTemperature>0)
					{
						if (heatersPower>3)
						{
							heatersPower=4;
							if (opt==1)
							{
								StepFlapperValve(1);
							}
							
						}
						else
						{
							
							heatersPower++;
							automaticHeatingMode=true;
						
							
						}
						
					}
					else
					{
						if (heatersPower<1)
						{
							heatersPower=0;
							if (opt==1)
							{
								StepFlapperValve(-1);
							}
							
						}
						else
						{
							heatersPower--;
							automaticHeatingMode=false;
							
						}
						
					}
					if (lastPower!=heatersPower)
					{
						lastPower=heatersPower;
						SetHeaterDriver(heatersPower);
					}
					
					
				}
				else
				{
				}
			}

		}
	}
	
	
	
	
	
}



uint8_t AUTO_MODE_Controller::StepFlapperValve(int dir){
	currentAngle=fvc.GetCurrentPosition();
	if (dir>0)
	{
		if (currentAngle<(0xff-FLAPPER_VALVE_STEP))
		{
			fvc.StartControlling(currentAngle+FLAPPER_VALVE_STEP);
		} 
		else
		{
			
		}
		
	} 
	else
	{
		if (currentAngle<(FLAPPER_VALVE_STEP+1))
		{
			
		}
		else
		{
			fvc.StartControlling(currentAngle-FLAPPER_VALVE_STEP);
		}
	}
	
}