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
#include "EvaporatorAndCondesatorFans_Class.h"

// default constructor
AUTO_MODE_Controller::AUTO_MODE_Controller()
{
	heatersRequestPower=0;
	evaFanSpeed=0;
	heatingMode=true;
	
} //VENT_MODE_Controller

// default destructor
AUTO_MODE_Controller::~AUTO_MODE_Controller()
{
} //~VENT_MODE_Controller


void AUTO_MODE_Controller::ControlTemperatureSetpoint(uint8_t opt){
// 	if (hvac.temperingTimeout)
// 	{
		cabinCurrentTemperature=temperatures.values[0][3];
		if (abs(lastCabinTemperature-cabinCurrentTemperature)>1/4)
		{
			lastCabinTemperature=cabinCurrentTemperature;
			if (cabinCurrentTemperature>0)
			{
				
				setpointTemperature=hvac.UpadateTemperatureSetpoint();
				errorTemperature=setpointTemperature-cabinCurrentTemperature;
				tolerance=errorTemperature>0?1:2;
				if (abs(errorTemperature)>tolerance)
				{
					
					heatersRequestPower=heater.GetHeaterPowerLevel();
					currentPower=heatersRequestPower;
					if (errorTemperature>0)
					{
						if (heatersRequestPower>3)
						{
							heatersRequestPower=4;
							if (opt==1)
							{
								StepFlapperValve(1);
							}
							
						}
						else
						{
							
							heatersRequestPower++;
							automaticHeatingMode=true;
							
						
							
						}
						
					}
					else
					{
						if (heatersRequestPower<1)
						{
							heatersRequestPower=0;
						
							if (opt==1)
							{
								StepFlapperValve(-1);
							}
							
						}
						else
						{
							heatersRequestPower--;
							automaticHeatingMode=false;
							
						}
						
					}
					if (automaticHeatingMode!=heatingMode)
					{
						changedMode=true;
					} 
					else
					{
						changedMode=false;
					}
					if (temperingTimeout|true)
					{
						if (heatersRequestPower>currentPower)
						{
							if (temperingTimeout)
							{
									if ((lastPower!=heatersRequestPower)|temperingTimeout)
									{
										lastPower=heatersRequestPower;
										lastControlTemperature=cabinCurrentTemperature;
										lastMode=heatingMode;
										heatingMode=true;
										SetHeaterDriver(heatersRequestPower);
									}
							} 
							else
							{
							}
// 							if (currentPower>0)
// 							{
// 								if ((cabinCurrentTemperature>lastControlTemperature+1/1)|changedMode|temperingTimeout|true)
// 								{
// 											if (lastPower!=heatersPower)
// 											{
// 												lastPower=heatersPower;
// 												lastControlTemperature=cabinCurrentTemperature;
// 													lastMode=heatingMode;
// 													heatingMode=true;
// 												SetHeaterDriver(heatersPower);
// 											}
// 								} 
// 								else
// 								{
// 						
// 								}
// 							} 
// 							else
// 							{
// 										if ((lastPower!=heatersPower)|temperingTimeout)
// 										{
// 											lastPower=heatersPower;
// 											lastControlTemperature=cabinCurrentTemperature;
// 													lastMode=heatingMode;
// 													heatingMode=true;
// 											SetHeaterDriver(heatersPower);
// 										}
// 							}
						} 
						else
						{
							if (heatersRequestPower<currentPower)
							{
								if (currentPower<4)
								{
									if (((lastControlTemperature-1/2)>cabinCurrentTemperature)|temperingTimeout)
									{
										if ((lastPower!=heatersRequestPower)|temperingTimeout)
										{
											lastPower=heatersRequestPower;
											lastControlTemperature=cabinCurrentTemperature;
													lastMode=heatingMode;
													heatingMode=false;
											SetHeaterDriver(heatersRequestPower);
										}
									}
									else
									{
									}
								}
								else
								{
									if (lastPower!=heatersRequestPower)
									{
										lastPower=heatersRequestPower;
										lastControlTemperature=cabinCurrentTemperature;
										lastMode=heatingMode;
										heatingMode=false;
										SetHeaterDriver(heatersRequestPower);
									}
								}
							}
							
						}
					
					} 
					else
					{
					}
					if (heatersRequestPower==0)
					{
						hvac.Control_Cooler(errorTemperature);
					}
					else
					{
						fans.condesator->SetPWM(CONDESATOR_MINIMUN_FLOW_AIR);
					}
			
				
					
					
				}
				else
				{
				}
			}

		/*}*/
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