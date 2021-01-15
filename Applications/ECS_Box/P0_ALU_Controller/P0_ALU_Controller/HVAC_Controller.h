/* 
* HVAC_Controller.h
*
* Created: 12/2/2020 10:53:32 AM
* Author: GMateusDP
*/


#ifndef __HVAC_CONTROLLER_H__
#define __HVAC_CONTROLLER_H__

#include "AUTO_MODE_Controller.h"
#include "VENT_Mode.h"
#include "FlapperValveController.h"
#include "Cooling_Controller.h"

class HVAC_Controller: public AUTO_MODE_Controller, public  VENT_Mode, public Cooling_Controller
{
//variables
public:
	
protected:
private:
	FlapperValveMode flapperValveMode, lastFlapperValveMode;
	uint8_t flapperValveAngle;
	uint32_t timeoutValueTempering;
//functions
public:
	HVAC_Controller();
	~HVAC_Controller();
		 uint8_t SetHeaterDriver(uint8_t);
		 bool	AdjustFanSpeed(uint8_t);
		 uint8_t	OperateFlapperValve(FlapperValveMode fvm);
		 
protected:
private:
	HVAC_Controller( const HVAC_Controller &c );
	HVAC_Controller& operator=( const HVAC_Controller &c );
	void	SetFlapperValveDiscretePosition(FlapperValveMode fvm);

}; //HVAC_Controller

#endif //__HVAC_CONTROLLER_H__
