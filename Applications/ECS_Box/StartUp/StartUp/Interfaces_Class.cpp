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
#include "TemperatureSensors_Class.h"
#include "MCP23017_Class.h"
#include "ALU_Class.h"
#include "Error_Labelling_Class.h"
#include "Maintenance_Tool.h"


// default constructor
Interfaces_Class::Interfaces_Class()
{
	
} //Interfaces_Class

// default destructor
Interfaces_Class::~Interfaces_Class()
{
} //~Interfaces_Class

CommunicationRequest	Interfaces_Class::request;

bool	Interfaces_Class::Init(){
	size=sizeof(CommunicationRequest);
	result=arinc.Init();
	if (result==0x01)
	{
		arinc.TrasmitSingleLabel();
		if (maintenance.IsAppConnected())
		{
		} 
		else
		{
		}
	} 
	else
	{
		alu.NotifyError(kARINCINnterfaceError,result);
	}
	return result;
}

CommunicationRequest Interfaces_Class::CheckCommunication(void)
{
	event e;
	CommunicationRequest	r;
	CheckInternalCommunication();
	CheckExternalCommunication();
	r=request;
	
	return	r;
}

bool	Interfaces_Class::IsCommunicationRequest(void){
	bool arr[sizeof(CommunicationRequest)];
	CommunicationRequest r=CheckCommunication();
	uint8_t result=0;
	memcpy((void*)arr,(void*)&r,size);
	for (uint8_t ii = 0; ii < size; ii++)
	{
		result|=arr[ii];
	}
	return	result>0;
}

uint32_t Interfaces_Class::CheckInternalCommunication(void)
{
	uint32_t	r=0;
	request.internRequest.I2CExpanderGotMessage=CheckI2CExpander(1);
	request.internRequest.LTC2983GotMessage=CheckTemperatures();
	return	0;
}
uint32_t Interfaces_Class::CheckExternalCommunication(void)
{
	uint32_t	r=0;


	if (arinc.newMessageR1)
	{
		request.externRequest.arinc1GotMessage=true;
		alu.PrepareNewTask(kALUTaskReadARINCR1);
	}
	if (arinc.newMessageR2)
	{
		request.externRequest.arinc2GotMessage=true;
		alu.PrepareNewTask(kALUTaskReadARINCR2);
	}
	if (CheckUSBInterface())
	{
		request.externRequest.usbGotMessage=true;
		//alu.PrepareNewTask(kALUTaskReadARINCR2);
	}
	//request.externRequest.arinc1GotMessage=arinc.newMessageR2;
	
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
		if (usb.connected)
		{
			if (maintenance.IsAppConnected())
			{
				usb<<"Maintenance Tool Locked"<<NEWLINE;
			} 
			else
			{
				usb<<"Remote operation Disabled"<<NEWLINE;
			}
			hvac.PrintState();
		}else{
						
		}
		return	usb.rxReady;
}

bool	Interfaces_Class::CheckTemperatures(void){
	bool result(false);
	if (temperatures.IsConversionFinished())
	{
		
		temperatures.GetConversionResult();
		if (temperatures.faultData==VALID_TEMPERATURE)
		{
				alu.PrepareNewTask(kALUTaskUpdateTemperatures);
				result=true;
		}
			temperatures.StartOneConversion();
			asm("nop");
	
	} 
	else
	{
	}
	return	result;
}
bool	Interfaces_Class::CheckArincInterface(void){
	if (arinc.newMessageR1||arinc.newMessageR2)
	{
		if (arinc.newMessageR1)
		{
			arinc.ReadRXBuffer(1);
	
		} 
		else
		{
			arinc.ReadRXBuffer(2);
	
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
//static Interfaces_Class	interfaces;