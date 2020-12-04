/* 
* VENT_Mode.h
*
* Created: 12/3/2020 5:04:53 PM
* Author: GMateusDP
*/


#ifndef __VENT_MODE_H__
#define __VENT_MODE_H__
#define  VENT_MODE_TEMPERATURE_STEP 1
#include "compiler.h"

class VENT_Mode
{
//variables
public:
protected:
private:
	float cabinCurrentTemperature,currentSetpointTemperature,lastSetpoint,errorTemperature, lastCabinTemperature;
	uint8_t	heatersPower,evaFanSpeed,currentAngle;

//functions
public:
	VENT_Mode();
	~VENT_Mode();
	void OperateTemperatureBySetpoint(uint8_t opt);
	virtual		uint8_t SetHeaterDriver(uint8_t heatPower)=0;
	virtual	bool	AdjustFanSpeed(uint8_t heatPower)=0;
protected:
private:
	VENT_Mode( const VENT_Mode &c );
	VENT_Mode& operator=( const VENT_Mode &c );
	

}; //VENT_Mode

#endif //__VENT_MODE_H__
