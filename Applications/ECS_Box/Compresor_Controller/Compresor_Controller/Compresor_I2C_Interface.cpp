/* 
* Compresor_I2C_Interface.cpp
*
* Created: 12/16/2020 10:42:12 AM
* Author: GMateusDP
*/


#include "Compresor_I2C_Interface.h"

static	MCP23008_Class expandersCompresorStatic;
// default constructor
Compresor_I2C_Interface::Compresor_I2C_Interface()
{
	i2c=&i2cSharedStatic;
} //Compresor_I2C_Interface

// default destructor
Compresor_I2C_Interface::~Compresor_I2C_Interface()
{
} //~Compresor_I2C_Interface

bool Compresor_I2C_Interface::InitExpander(void){
	if (i2c->initiated)
	{
	} 
	else
	{
		
		i2c->Init();
	}
	if (i2c->isOK)
	{
		expander=&expandersCompresorStatic;
		expander->Init(COMPRESOR_MCP23008_ADDRESS,i2c);
		expander->SetPortInput(0xf0);
	//	expander->SetPortOutput();
		isOK=SelfTest();
	}
	else
	{
		asm("nop");
	}
	isOK=i2c->isOK;
		return isOK;
}

uint8_t	Compresor_I2C_Interface::ReadStatus(void){
	uint8_t r=expander->ReadGPIORegister();
	
	return r;
	
}
uint8_t	Compresor_I2C_Interface::SetEnable(bool state){

	enabled=expander->WriteDigit(0,state);
	return uint8_t(enabled);
}
uint8_t	Compresor_I2C_Interface::SetRelay(bool state){

	enabled=expander->WriteDigit(1,!state);
	return uint8_t(enabled);
}
uint8_t	Compresor_I2C_Interface::SetClutch(bool state){

	enabled=expander->WriteDigit(2,!state);
	return uint8_t(enabled);
}

bool Compresor_I2C_Interface::IsEnabled(void){

	uint8_t value=expander->ReadGPIORegister();
	enabled=(value&0x01);
	return enabled;
}
bool* Compresor_I2C_Interface::IsEnabledPointer(void){

	uint8_t value=expander->ReadGPIORegister();
	enabled=(value&0x01);
	return &enabled;
}

bool Compresor_I2C_Interface::SelfTest(void){
	bool result=true;
	for (uint8_t i = 0; i < 4; i++)
	{
// 		EnableIndex(i);
// 		delay_ms(100);
// 		DisableIndex(i);
// 		ReadStatus();
// 		if (heaterGPIO.inputs.niAlcHeaterRelayFault[i])
// 		{
// 			result=true;
// 		}
// 		else
// 		{
// 			result=false;
// 			break;
// 		}
		
		
	}
	

	return result;
}