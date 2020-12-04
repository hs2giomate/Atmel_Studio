/* 
* VENT_MODE_Controller.h
*
* Created: 12/2/2020 10:54:32 AM
* Author: GMateusDP
*/


#ifndef __VENT_MODE_CONTROLLER_H__
#define __VENT_MODE_CONTROLLER_H__
#include "compiler.h"

#define		FLAPPER_VALVE_STEP    8


class AUTO_MODE_Controller
{
//variables
public:
	bool automaticHeatingMode;
protected:
private:
	float cabinCurrentTemperature,setpointTemperature,errorTemperature, lastCabinTemperature;
	uint8_t	heatersPower,evaFanSpeed,currentAngle, lastPower;
	uint8_t	tolerance;
//functions
public:
	AUTO_MODE_Controller();
	~AUTO_MODE_Controller();
	void ControlTemperatureSetpoint(uint8_t opt=0);
	virtual uint8_t SetHeaterDriver(uint8_t)=0;
	virtual bool	AdjustFanSpeed(uint8_t heatPower)=0;
protected:
private:
	AUTO_MODE_Controller( const AUTO_MODE_Controller &c );
	AUTO_MODE_Controller& operator=( const AUTO_MODE_Controller &c );
	
	uint8_t StepFlapperValve(int dir);

}; //VENT_MODE_Controller

#endif //__VENT_MODE_CONTROLLER_H__
