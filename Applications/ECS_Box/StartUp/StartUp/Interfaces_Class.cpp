/* 
* Interfaces_Class.cpp
*
* Created: 7/6/2020 9:05:06 AM
* Author: GMateusDP
*/


#include "Interfaces_Class.h"
#include "CDC_Class.h"
#include "States_Class.h"
#include "ARINC_Interface.h"
#include "LTC2983_Class.h"
#include "MCP23017_Class.h"

// default constructor
Interfaces_Class::Interfaces_Class()
{
} //Interfaces_Class

// default destructor
Interfaces_Class::~Interfaces_Class()
{
} //~Interfaces_Class

uint32_t Interfaces_Class::CheckCommunication(void)
{
	uint32_t	r=0;
	r=(uint32_t)CheckI2CExpander(1);
	
	


	
	return	0;
}
bool	Interfaces_Class::CheckI2CExpander(uint8_t add){
		if (mcp.hasChanged)
		{
			mcp.SavePorts();
			mcp.hasChanged=false;
		}
		return	mcp.hasChanged;
}
bool	Interfaces_Class::CheckUSBInterface(void){
		if (usb.terminalStarted)
		{
			hvac.PrintState();
		}else{
						
		}
		return	usb.terminalStarted;
}

bool	Interfaces_Class::checkLTC2983(void){
	if (temperatures.conversionFinished)
	{
		for (i = 0; i <NUMBER_TEMPERATURE_CHANNELS ; i++)
		{
			temperatures.SaveChannelValue(temperatures.activeChannels[i]);
		}
		temperatures.conversionFinished=0;
	} 
	else
	{
	}
	return	temperatures.conversionFinished;
}
bool	Interfaces_Class::CheckArincInterface(void){
	if (arinc.newMessageR1||arinc.newMessageR2)
	{
		if (arinc.newMessageR1)
		{
			arinc.FetchAllMessagesReceiver1();
	
		} 
		else
		{
			arinc.FetchAllMessagesReceiver2();
	
		}
		
	} 
	else
	{
		
	}
	return	arinc.newMessageR1||arinc.newMessageR2;
}
uint32_t	Interfaces_Class::GetStatus(HVACStatus& s){
	s=status;
	uint32_t	*p=(uint32_t*)(&s.statusBits);
	for (int i=0;i<sizeof(status.statusBits);i++)
	{
		s.rawStatus+=(*p)&(0x01<<i);
		p++;
	}
	return	status.rawStatus;
}

Interfaces_Class	interfaces;