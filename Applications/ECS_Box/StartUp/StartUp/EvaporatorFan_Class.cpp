/* 
* EvaporatorFan_Class.cpp
*
* Created: 10/21/2020 10:56:29 AM
* Author: GMateusDP
*/


#include "EvaporatorFan_Class.h"


EvaporatorFan_Class*	ptrEvaFanClass;




// default constructor
EvaporatorFan_Class::EvaporatorFan_Class()
{
	
	ptrEvaFanClass=this;
	
} //EvaporatorFan_Class

// default destructor
EvaporatorFan_Class::~EvaporatorFan_Class()
{
} //~EvaporatorFan_Class

bool EvaporatorFan_Class::InitEvaporator(uint8_t number, I2C_Sync_Class* i2cClass,MCP23008_Class** expandersArray){
	i2c=i2cClass;
	fanNumber=number-1;
		if (i2c->isOK)
		{
			InitExpanderArray(expandersArray);

			isOK=SelfTest();
		}
		else
		{
		}
		return isOK;
	
	
}
uint8_t EvaporatorFan_Class::InitExpanderArray(MCP23008_Class** expanArray){
	
			for (int i = 0; i < EVAPORATOR_EXPANDERS; i++)
			{
				expanders[i]=expanArray[i];
			}
}

uint8_t	EvaporatorFan_Class::ReadStatus(void){
	uint8_t r=expanders[1]->ReadGPIORegister();
	
	
	evaporatorFansStatus.inputs[fanNumber].niAlcEvaFanExtFault=r&(0x01<<(4*fanNumber));
	evaporatorFansStatus.inputs[fanNumber].niAlcEvaFanEnableFault=r&(0x02<<(4*fanNumber));
	evaporatorFansStatus.inputs[fanNumber].iAlcEvaFanPwmFault=r&(0x04<<(4*fanNumber));
	
		
}
uint8_t	EvaporatorFan_Class::Enable(void){
	enabled=expanders[0]->WriteDigit(fanNumber,true);
	return uint8_t(enabled);
}
uint8_t	EvaporatorFan_Class::Disable(void){
	enabled=expanders[0]->WriteDigit(fanNumber,true)?false:true;
	return uint8_t(enabled);
}

uint8_t	EvaporatorFan_Class::SetPWM(uint8_t pwm){
	pwmValue= expanders[2+fanNumber]->WriteGPIORegister(pwm);
	return pwmValue;
}

bool EvaporatorFan_Class::SelfTest(void){
	return true;
}
