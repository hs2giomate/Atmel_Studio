/* 
* EvaporatorFanDataStruct_Class.h
*
* Created: 10/21/2020 10:57:20 AM
* Author: GMateusDP
*/


#ifndef __EVAPORATORFANDATASTRUCT_CLASS_H__
#define __EVAPORATORFANDATASTRUCT_CLASS_H__
#include "driver_init.h"
#define IC33_SLAVE_ADDRESS	0
#define NUMBER_FANS	2



struct __attribute__((__packed__)) IC33_Outputs
{
	bool	iAlcCdsFanEnable:1;
	bool	niAlcEvaFan2Enable:1;
	bool	niAlcEvaFan1Enable:1;
};
struct __attribute__((__packed__)) IC33_GPIO
{

	IC33_Outputs outputs;
	
};
struct __attribute__((__packed__)) IC36_Inputs
{
	bool	iAlcEvaFanPwmFault:1;
	bool	niAlcEvaFanEnableFault:1;
	bool	niAlcEvaFanExtFault:1;

};
struct __attribute__((__packed__))  IC36_GPIO
{
	IC36_Inputs	inputs[NUMBER_FANS];

};
struct __attribute__((__packed__)) EvaporatorGPIO
{
	IC33_GPIO	ic33gpio;
	IC36_GPIO	ic36gpio;
	uint8_t		pwmFan1;
	uint8_t		pwmFan2;	
	
	
};

class EvaporatorFanDataStruct_Class
{
//variables
public:
 IC36_GPIO	evaporatorFansStatus;
 
protected:
private:

//functions
public:
	EvaporatorFanDataStruct_Class();
	~EvaporatorFanDataStruct_Class();
protected:
private:
	EvaporatorFanDataStruct_Class( const EvaporatorFanDataStruct_Class &c );
	EvaporatorFanDataStruct_Class& operator=( const EvaporatorFanDataStruct_Class &c );

}; //EvaporatorFanDataStruct_Class

#endif //__EVAPORATORFANDATASTRUCT_CLASS_H__
