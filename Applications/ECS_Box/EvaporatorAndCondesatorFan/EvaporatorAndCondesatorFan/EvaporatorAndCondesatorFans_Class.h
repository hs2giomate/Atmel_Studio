/* 
* EvaporatorAndCondesatorFans_Class.h
*
* Created: 10/21/2020 11:50:29 AM
* Author: GMateusDP
*/


#ifndef __EVAPORATORANDCONDESATORFANS_CLASS_H__
#define __EVAPORATORANDCONDESATORFANS_CLASS_H__
#include "EvaporatorFan_Class.h"
#include "CondesatorFan_Class.h"


class EvaporatorAndCondesatorFans_Class: public EvaporatorFan_Class,public CondesatorFan_Class
{
//variables
public:
	volatile bool isOK,changed;
	EvaporatorFan_Class*	evaporator[2];
	CondesatorFan_Class*	condesator;
protected:
private:

	MCP23008_Class* expanders[EVAPORATOR_AND_CONDESATOR_EXPANDERS];
	MCP23008_Class* currentExpander;
	I2C_Sync_Class*	i2c;
//functions
public:
	EvaporatorAndCondesatorFans_Class();
	~EvaporatorAndCondesatorFans_Class();
	bool	Init(void);
protected:
private:
	EvaporatorAndCondesatorFans_Class( const EvaporatorAndCondesatorFans_Class &c );
	EvaporatorAndCondesatorFans_Class& operator=( const EvaporatorAndCondesatorFans_Class &c );
	uint8_t InitExpanderArray(void);

}; //EvaporatorAndCondesatorFans_Class

//extern EvaporatorAndCondesatorFans_Class  fans;

#endif //__EVAPORATORANDCONDESATORFANS_CLASS_H__
