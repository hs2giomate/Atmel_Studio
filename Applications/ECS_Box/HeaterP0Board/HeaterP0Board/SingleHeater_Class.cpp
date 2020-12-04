/* 
* SingleHeater_Class.cpp
*
* Created: 22.10.2020 14:48:14
* Author: technik
*/


#include "SingleHeater_Class.h"
SingleHeater_Class*	ptrSingleHeaterClass;
static I2C_Sync_Class	i2cSharedStatic(&I2C_HEATERS);
static	MCP23008_Class expandersStatic[SINGLE_HEATER_EXPANDERS];



// default constructor
SingleHeater_Class::SingleHeater_Class()
{
	ptrSingleHeaterClass=this;
	i2c=&i2cSharedStatic;
	heaterStatusChanged=false;
} //SingleHeater_Class

// default constructor
SingleHeater_Class::SingleHeater_Class(I2C_Sync_Class *i2cInput)
{
	i2c=i2cInput;
	ptrSingleHeaterClass=this;
	heaterStatusChanged=false;
} //SingleHeater_Class


// default destructor
SingleHeater_Class::~SingleHeater_Class()
{
} //~SingleHeater_Class

static void	HeaterStatusChanged(void){
	
	ptrSingleHeaterClass->heaterStatusChanged=true;
}

bool SingleHeater_Class::Init(void){
		i2c->Init();
		if (i2c->isOK)
		{
			
			//ext_irq_register(PIN_PA03,FUNC_PTR(HeaterStatusChanged));
			InitExpanderArray();
			expanders[0]->SetPortInput();
			expanders[1]->SetPortOutput();
			isOK=SelfTest();
		}
		else
		{
			asm("nop");
		}
		isOK=i2c->isOK;
		return isOK;
}

uint8_t SingleHeater_Class::InitExpanderArray(void){
	
	for (int i = 0; i < SINGLE_HEATER_EXPANDERS; i++)
	{
		expanders[i]=&expandersStatic[i];
		currentExpander=(expanders[i]);
		currentExpander->Init(i|MCP23008_ADDRESS,i2c);
	}
	
}

uint8_t	SingleHeater_Class::ReadStatus(void){
	uint8_t r=expanders[0]->ReadGPIORegister();
	heaterGPIO.inputs.niAlcHeaterOverTemp=r&0x01;
	for (uint8_t i = 0; i < 4; i++)
	{
		heaterGPIO.inputs.niAlcHeaterRelayFault[i]=r&(0x01<<(i+1));
	}
	return r;
}

uint8_t	SingleHeater_Class::ReadEnableGIPO(void){
	uint8_t r=expanders[1]->ReadGPIORegister();

	for (uint8_t i = 0; i < 4; i++)
	{
		enables[i]=!(r&(0x01<<i));
	}
	return r;
}
uint8_t	SingleHeater_Class::EnableIndex(uint8_t indexHeater){
	enabled=expanders[1]->WriteDigit(indexHeater,false);
	return uint8_t(enabled);
}
uint8_t	SingleHeater_Class::SetRelay(uint8_t indexHeater, bool state){
	enabled=expanders[1]->WriteDigit(indexHeater,!state);
	return uint8_t(enabled);
}

uint8_t	SingleHeater_Class::DisableIndex(uint8_t indexHeater){
	enabled=expanders[1]->WriteDigit(indexHeater,true)?false:true;
	return uint8_t(enabled);
}

uint8_t SingleHeater_Class::GetHeaterPowerLevel(void){
	ReadEnableGIPO();
	powerLevel=0;
	uint8_t aux=0;
	for (uint8_t i = 0; i < 4; i++)
	{
		aux=enables[i]?1:0;
		powerLevel+=aux;
	}
	return powerLevel;
}


bool SingleHeater_Class::SelfTest(void){
	bool result;
	for (uint8_t i = 0; i < 4; i++)
	{
			EnableIndex(i);
			delay_ms(100);
			DisableIndex(i);
			ReadStatus();
			if (heaterGPIO.inputs.niAlcHeaterRelayFault[i])
			{
				result=true;
			} 
			else
			{
				result=false;
				break;
			}
			
			
	}
	

	return result;
}

SingleHeater_Class heater;