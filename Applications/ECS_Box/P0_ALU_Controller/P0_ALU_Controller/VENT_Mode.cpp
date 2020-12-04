/* 
* VENT_Mode.cpp
*
* Created: 12/3/2020 5:04:52 PM
* Author: GMateusDP
*/


#include "VENT_Mode.h"
#include "TemperatureSensors_Class.h"
#include "fastmath.h"
#include "States_Class.h"

// default constructor
VENT_Mode::VENT_Mode()
{
	heatersPower=0;
	evaFanSpeed=0;
} //VENT_Mode

// default destructor
VENT_Mode::~VENT_Mode()
{
} //~VENT_Mode

void VENT_Mode::OperateTemperatureBySetpoint(uint8_t opt){
	cabinCurrentTemperature=temperatures.values[0][3];
	if (cabinCurrentTemperature>0)
	{
		currentSetpointTemperature=hvac.UpadateTemperatureSetpoint();
		if (currentSetpointTemperature>0)
		{
			if (abs(lastSetpoint-currentSetpointTemperature)>VENT_MODE_TEMPERATURE_STEP)
			{
				lastSetpoint=currentSetpointTemperature;
				
				
				
				errorTemperature=currentSetpointTemperature-cabinCurrentTemperature;
				if (abs(errorTemperature)>2*VENT_MODE_TEMPERATURE_STEP)
				{
					if (errorTemperature>0)
					{
						if (heatersPower>3)
						{
							heatersPower=4;
							if (opt==1)
							{
							//	StepFlapperValve(1);
							}
							
						}
						else
						{
							heatersPower++;
							
						}
						
					}
					else
					{
						if (heatersPower<1)
						{
							heatersPower=0;
							if (opt==1)
							{
							//	StepFlapperValve(-1);
							}
							
						}
						else
						{
							heatersPower--;
						}
						
					}
					SetHeaterDriver(heatersPower);
					
				}
				else
				{
				}
			}

		}
	
		
	}
	
	
	
}

