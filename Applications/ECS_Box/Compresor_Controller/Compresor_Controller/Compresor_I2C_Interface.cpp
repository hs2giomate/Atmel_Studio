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
	if (i2c->i2c_initiated)
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
		//expander->SetPortInput(0xf0);
		expander->SetPortOutput();
		isOK=SelfTest();
	}
	else
	{
		asm("nop");
	}
	isOK=i2c->isOK;
		return isOK;
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