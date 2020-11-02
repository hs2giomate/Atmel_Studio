/* 
* EvaporatorFan_Class.h
*
* Created: 10/21/2020 10:56:30 AM
* Author: GMateusDP
*/


#ifndef __EVAPORATORFAN_CLASS_H__
#define __EVAPORATORFAN_CLASS_H__
#include "EvaporatorFanDataStruct_Class.h"
#include "I2C_Sync_Class.h"
#include "MCP23008_Class.h"
#define		EVAPORATOR_EXPANDERS	6

class EvaporatorFan_Class: public EvaporatorFanDataStruct_Class
{
//variables
public:
	volatile bool evaporatorchanged;
	bool enabled,isOK;
	

protected:
private:
	I2C_Sync_Class*	i2c;
	MCP23008_Class* expanders[EVAPORATOR_EXPANDERS];
	MCP23008_Class* currentExpander;
	uint8_t	fanNumber;
	uint8_t	pwmValue;

//functions
public:
	EvaporatorFan_Class();
	~EvaporatorFan_Class();
	bool InitEvaporator(uint8_t number, I2C_Sync_Class*	i2cClass,MCP23008_Class** expans);
	uint8_t	ReadStatus(void);
	uint8_t	Enable(void);
	uint8_t	Disable(void);
	uint8_t	SetPWM(uint8_t pwm);
	uint8_t	SetEnable(bool state);
	
protected:
private:
	EvaporatorFan_Class( const EvaporatorFan_Class &c );
	EvaporatorFan_Class& operator=( const EvaporatorFan_Class &c );
	uint8_t InitExpanderArray(MCP23008_Class** expanArray);
	bool SelfTest(void);

}; //EvaporatorFan_Class

#endif //__EVAPORATORFAN_CLASS_H__
