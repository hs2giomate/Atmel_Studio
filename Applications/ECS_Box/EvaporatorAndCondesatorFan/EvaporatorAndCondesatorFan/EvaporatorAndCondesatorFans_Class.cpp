/* 
* EvaporatorAndCondesatorFans_Class.cpp
*
* Created: 10/21/2020 11:50:28 AM
* Author: GMateusDP
*/


#include "EvaporatorAndCondesatorFans_Class.h"
static I2C_Sync_Class	i2cFansStatic(&I2C_FANS);
static EvaporatorFan_Class	evaporatorFan[2];
static CondesatorFan_Class	condesatorFan;
static	MCP23008_Class expandersStatic[EVAPORATOR_AND_CONDESATOR_EXPANDERS];

EvaporatorAndCondesatorFans_Class*	ptrFansClass;
static void	FansStatusChanged(void){
	
	ptrFansClass->somethingChanged=true;


}

// default constructor
EvaporatorAndCondesatorFans_Class::EvaporatorAndCondesatorFans_Class()
{
	i2c=&i2cFansStatic;
	evaporator[0]=&evaporatorFan[0];
	evaporator[1]=&evaporatorFan[1];
	condesator=&condesatorFan;
	ptrFansClass=this;
	somethingChanged=false;
	
} //EvaporatorAndCondesatorFans_Class

// default destructor
EvaporatorAndCondesatorFans_Class::~EvaporatorAndCondesatorFans_Class()
{
} //~EvaporatorAndCondesatorFans_Class

bool	EvaporatorAndCondesatorFans_Class::Init(void){
	i2c->Init();
	if (i2c->isOK)
	{
		
		//ext_irq_register(PIN_PA03,FUNC_PTR(FansStatusChanged));
		InitExpanderArray();
		evaporator[0]->InitEvaporator(0,i2c,expanders);
		evaporator[1]->InitEvaporator(1,i2c,expanders);
		condesator->InitCondesator(i2c,expanders);
		isOK=(evaporator[0]->isOK)&(evaporator[1]->isOK);
	}
	else
	{
	}
	isOK=i2c->isOK;
	return isOK;
	
}
uint8_t EvaporatorAndCondesatorFans_Class::InitExpanderArray(void){
	
	for (int i = 0; i < EVAPORATOR_AND_CONDESATOR_EXPANDERS; i++)
	{
		expanders[i]=&expandersStatic[i];
		currentExpander=expanders[i];
		currentExpander->Init(i|MCP23008_ADDRESS,i2c);
	}
	return 0;
}
void EvaporatorAndCondesatorFans_Class::GetGPIOSValues(uint8_t * gpios){
	for (uint8_t  i= 0; i < EVAPORATOR_AND_CONDESATOR_EXPANDERS; i++)
	{
		gpios[i]=expanders[i]->ReadGPIORegister();
	}
	

}

EvaporatorAndCondesatorFans_Class  fans;
	