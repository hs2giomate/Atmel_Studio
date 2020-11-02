/* 
* CondesatorFan_Class.cpp
*
* Created: 10/21/2020 11:49:20 AM
* Author: GMateusDP
*/


#include "CondesatorFan_Class.h"

// default constructor
CondesatorFan_Class::CondesatorFan_Class()
{
} //CondesatorFan_Class

// default destructor
CondesatorFan_Class::~CondesatorFan_Class()
{
} //~CondesatorFan_Class

bool CondesatorFan_Class::InitCondesator(I2C_Sync_Class* i2cClass,MCP23008_Class** expanderArray){
	i2c=i2cClass;

	if (i2c->isOK)
	{
		InitExpanderArray(expanderArray);
		expanders[EVAPORATOR_AND_CONDESATOR_EXPANDERS-1]->SetPortOutput();
		isOK=SelfTest();
	}
	else
	{
	}
	return isOK;
	
	
}

uint8_t CondesatorFan_Class::InitExpanderArray(MCP23008_Class** expandersArray){
		for (int i = 0; i < EVAPORATOR_AND_CONDESATOR_EXPANDERS; i++)
		{
			expanders[i]=expandersArray[i];
		}
		return 0;
}
	


uint8_t	CondesatorFan_Class::ReadStatus(void){
	uint8_t r=expanders[0]->ReadGPIORegister();
	
	
	condesatorStatus.niAlcCdsFanExtFault=r&(0x01<<(4));
	condesatorStatus.niAlcCdsFanEnableFault=r&(0x02<<(4));
	condesatorStatus.niAlcCdsFanPwmFault=r&(0x04<<(4));
	
	return r;
}
uint8_t	CondesatorFan_Class::Enable(void){
	enabled=expanders[0]->WriteDigit(2,true);
	return uint8_t(enabled);
}
uint8_t	CondesatorFan_Class::SetEnable(bool state){
	enabled=expanders[0]->WriteDigit(2,state);
	return uint8_t(enabled);
}
uint8_t	CondesatorFan_Class::Disable(void){
	enabled=expanders[0]->WriteDigit(2,false)?false:true;
	return uint8_t(enabled);
}

uint8_t	CondesatorFan_Class::SetPWM(uint8_t pwm){
	pwmValue= expanders[6]->WriteGPIORegister(pwm);
	return pwmValue;
}

bool CondesatorFan_Class::SelfTest(void){
	return true;
}