/* 
* EvaporatorAndCondesatorFans_Class.cpp
*
* Created: 10/21/2020 11:50:28 AM
* Author: GMateusDP
*/


#include "EvaporatorAndCondesatorFans_Class.h"
static I2C_Sync_Class	i2cSharedStatic(&I2C_SHARED);
static EvaporatorFan_Class	evaporatorFan[2];
static CondesatorFan_Class	condesatorFan;
static	MCP23008_Class expandersStatic[EVAPORATOR_AND_CONDESATOR_EXPANDERS];

EvaporatorAndCondesatorFans_Class*	ptrFansClass;
static void	FansStatusChanged(void){
	
	ptrFansClass->changed=true;


}

// default constructor
EvaporatorAndCondesatorFans_Class::EvaporatorAndCondesatorFans_Class()
{
	i2c=&i2cSharedStatic;
	evaporator[0]=&evaporatorFan[0];
	evaporator[1]=&evaporatorFan[1];
} //EvaporatorAndCondesatorFans_Class

// default destructor
EvaporatorAndCondesatorFans_Class::~EvaporatorAndCondesatorFans_Class()
{
} //~EvaporatorAndCondesatorFans_Class

bool	EvaporatorAndCondesatorFans_Class::Init(void){
	i2c->Init();
	if (i2c->isOK)
	{
		
		ext_irq_register(PIN_PA04,FUNC_PTR(FansStatusChanged));
		InitExpanderArray();
		evaporator[0]->InitEvaporator(1,i2c,expanders);
		evaporator[1]->InitEvaporator(2,i2c,expanders);
		isOK=(evaporator[0]->isOK)&(evaporator[1]->isOK);
	}
	else
	{
	}
	return isOK;
	
}
uint8_t EvaporatorAndCondesatorFans_Class::InitExpanderArray(void){
	
	for (int i = 0; i < EVAPORATOR_AND_CONDESATOR_EXPANDERS; i++)
	{
		expanders[i]=&expandersStatic[i];
		currentExpander=(expanders[i]);
		currentExpander->Init(i,i2c);
	}
}

EvaporatorAndCondesatorFans_Class  fans;
