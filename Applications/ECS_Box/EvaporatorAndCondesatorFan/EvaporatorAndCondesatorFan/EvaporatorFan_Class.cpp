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
	fanNumber=number;
		if (i2c->isOK)
		{
			InitExpanderArray(expandersArray);
			if (fanNumber==0)
			{
				
				expanders[0]->SetPortInput(0x70);
				expanders[1]->SetPortInput();
			}
		
			expanders[4+fanNumber]->SetPortOutput();
			
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
			return 0;
}

uint8_t	EvaporatorFan_Class::ReadStatus(void){
	uint8_t r=expanders[1]->ReadGPIORegister();
	
	
	evaporatorFansStatus.inputs[fanNumber].niAlcEvaFanExtFault=r&(0x01<<(4*fanNumber));
	evaporatorFansStatus.inputs[fanNumber].niAlcEvaFanEnableFault=r&(0x02<<(4*fanNumber));
	evaporatorFansStatus.inputs[fanNumber].iAlcEvaFanPwmFault=r&(0x04<<(4*fanNumber));
	return r;
		
}
uint8_t	EvaporatorFan_Class::Enable(void){
	enabled=expanders[0]->WriteDigit(fanNumber,true);
	return uint8_t(enabled);
}
uint8_t	EvaporatorFan_Class::SetEnable(bool state){
	enabled=expanders[0]->WriteDigit(1-fanNumber,!state);
	return uint8_t(enabled);
}
uint8_t	EvaporatorFan_Class::Disable(void){
	enabled=expanders[0]->WriteDigit(fanNumber,false)?false:true;
	return uint8_t(enabled);
}

uint8_t	EvaporatorFan_Class::SetPWM(uint8_t pwm){
	pwmValue= expanders[4+fanNumber]->WriteGPIORegister(0xff-pwm);
	return pwmValue;
}

bool EvaporatorFan_Class::SelfTest(void){
	return true;
}
