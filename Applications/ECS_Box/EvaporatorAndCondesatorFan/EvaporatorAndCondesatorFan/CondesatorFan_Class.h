/* 
* CondesatorFan_Class.h
*
* Created: 10/21/2020 11:49:21 AM
* Author: GMateusDP
*/


#ifndef __CONDESATORFAN_CLASS_H__
#define __CONDESATORFAN_CLASS_H__
#include "driver_init.h"
#include "I2C_Sync_Class.h"
#include "MCP23008_Class.h"
#define		EVAPORATOR_AND_CONDESATOR_EXPANDERS	7
struct IC33_Inputs
{
	bool	niAlcCdsFanPwmFault:1;
	bool	niAlcCdsFanEnableFault:1;
	bool	niAlcCdsFanExtFault:1;
};

class CondesatorFan_Class
{
//variables
public:
		volatile bool condesatorchanged;
		bool enabled,isOK;
		IC33_Inputs		condesatorStatus;
protected:
private:
	uint8_t	pwmValue;
	I2C_Sync_Class*	i2c;
	MCP23008_Class* expanders[EVAPORATOR_AND_CONDESATOR_EXPANDERS];
	MCP23008_Class* currentExpander;

//functions
public:
	CondesatorFan_Class();
	~CondesatorFan_Class();
	bool InitCondesator(I2C_Sync_Class*	i2cClass,MCP23008_Class** expans);
	uint8_t	ReadStatus(void);
	uint8_t	Enable(void);
	uint8_t	Disable(void);
	uint8_t	SetPWM(uint8_t pwm);
protected:
private:
	CondesatorFan_Class( const CondesatorFan_Class &c );
	CondesatorFan_Class& operator=( const CondesatorFan_Class &c );
	uint8_t InitExpanderArray(MCP23008_Class** expanArray);
	bool SelfTest(void);
}; //CondesatorFan_Class

#endif //__CONDESATORFAN_CLASS_H__
