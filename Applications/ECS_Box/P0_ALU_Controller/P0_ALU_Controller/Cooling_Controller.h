/* 
* Cooling_Controller.h
*
* Created: 12/8/2020 11:37:36 AM
* Author: GMateusDP
*/


#ifndef __COOLING_CONTROLLER_H__
#define __COOLING_CONTROLLER_H__
#include "compiler.h"
#define  PROPORTIONAL_CONTROL_VALUE	64

class Cooling_Controller
{
//variables
public:
protected:
private:
	float	currentTemperature,errorTemperature,tolerance,floatPWM;
	uint8_t pwmCondesator;

//functions
public:
	Cooling_Controller();
	~Cooling_Controller();
	uint8_t Control_Cooler(float erroTemp);
protected:
private:
	Cooling_Controller( const Cooling_Controller &c );
	Cooling_Controller& operator=( const Cooling_Controller &c );
	uint8_t CalculatePWMCondesator(void);

}; //Cooling_Controller

#endif //__COOLING_CONTROLLER_H__
