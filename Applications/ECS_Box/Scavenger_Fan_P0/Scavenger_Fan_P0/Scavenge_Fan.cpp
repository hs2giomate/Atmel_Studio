/* 
* Scavenge_Fan.cpp
*
* Created: 1/18/2021 11:29:14 AM
* Author: GMateusDP
*/


#include "Scavenge_Fan.h"
static MCP23008_Class local_expander;
static bool low_speed_array[TACHO_SIGNAL_BUFFER_SIZE];

// default constructor
Scavenge_Fan::Scavenge_Fan()
{
	isOK=false;
} //Scavenge_Fan

// default destructor
Scavenge_Fan::~Scavenge_Fan()
{
} //~Scavenge_Fan

bool Scavenge_Fan::Init(void){
	i2c=&i2cSharedStatic;
	expander=&local_expander;
	expander->Init(MCP23008_SCAVENGE_ADDRESS,i2c);
	if (i2c->initiated)
	{
		
	} 
	else
	{
		i2c->Init();
	}
	if (i2c->isOK)
	{
		expander->SetPortInput(0xf0);
		isOK=SelfTest();
	} 
	else
	{
	}
	return isOK;
}

uint8_t	Scavenge_Fan::ReadStatus(void){
	uint8_t r=expander->ReadGPIORegister();
	
	
	scavengeData.tempFault=r&(0x01<<(7));
	scavengeData.SPDFault=r&(0x01<<(6));
	scavengeData.relayFault=r&(0x01<<(4));
	r&=0xfe;
	if (IsLowSpeed())
	{
		r|=0x01;
	} 
	else
	{
		
	}
	

	return r;
	
}
uint8_t	Scavenge_Fan::SetEnable(bool state){

	enabled=expander->WriteDigit(1,!state);
	return uint8_t(enabled);
}

bool Scavenge_Fan::IsLowSpeed(void){
	low_speed=false;
	for (uint8_t i = 0; i < TACHO_SIGNAL_BUFFER_SIZE-1; i++)
	{	
		if (low_speed_array[i])
		{
			low_speed=true;
			break;
		} 
		else
		{
			if (low_speed_array[i]==low_speed_array[i+1])
			{
				low_speed=false;
			}
			else
			{
				low_speed=true;
				break;
			}
		}
		
		
	}
	return low_speed;
}
	
void Scavenge_Fan::FillLowSpeedFIFO(void){
	uint8_t r=expander->ReadGPIORegister();
	for (uint8_t i = 0; i < TACHO_SIGNAL_BUFFER_SIZE-1; i++)
	{
		low_speed_array[TACHO_SIGNAL_BUFFER_SIZE-1-i]=low_speed_array[TACHO_SIGNAL_BUFFER_SIZE-2-i];
	}
	low_speed_array[0]=(!(r>>6))&0x01;
}

bool Scavenge_Fan::IsEnabled(void){

	uint8_t value=expander->ReadGPIORegister();
	enabled=!(value&0x02);
	return enabled;
}
bool Scavenge_Fan::SelfTest(){
	if ((ReadStatus()&0xd0)<0xd)
	{
		bool_result=false;
	} 
	else
	{
		bool_result=true;
	}
	return bool_result;
}
Scavenge_Fan scavenge;