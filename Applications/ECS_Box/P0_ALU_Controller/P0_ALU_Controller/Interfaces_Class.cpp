/* 
* Interfaces_Class.cpp
*
* Created: 7/6/2020 9:05:06 AM
* Author: GMateusDP
*/


#include "Interfaces_Class.h"
#include "CDC_Class.h"
#include "States_Class.h"
#include "ARINC_Handler_Class.h"
#include "TemperatureSensors_Class.h"
#include "ALU_Class.h"
#include "Error_Labelling_Class.h"
#include "Maintenance_Tool.h"
#include "FlapperValveController.h"


// default constructor
Interfaces_Class::Interfaces_Class()
{
	isOK=false;
} //Interfaces_Class

// default destructor
Interfaces_Class::~Interfaces_Class()
{
} //~Interfaces_Class

CommunicationRequest	Interfaces_Class::request;

bool	Interfaces_Class::Init(){

	gpio_set_pin_level(LED0,true);
	size=sizeof(CommunicationRequest);
	while (!arinc.Init_ARINC())
	{
		delay_ms(200);
		gpio_toggle_pin_level(LED0);
	}
	
	while (!toolApp.Init())
	{
		delay_ms(200);
		gpio_toggle_pin_level(LED0);
	}
	isOK=arinc.isOK;
	
	if (result==0x01)
	{
		//arinc.TrasmitSingleLabel();
// 		if (toolApp.IsAppConnected())
// 		{
// 			
// 		} 
// 		else
// 		{
// 		}
	} 
	else
	{
		//alu.NotifyError(kARINCINnterfaceError,result);
	}
	gpio_set_pin_level(LED0,true);
	return isOK;
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
//	request.internRequest.flapperValvesMessage=CheckFlapperValveInterface(1);
	request.internRequest.temperatureSensorsMessage=CheckTemperatures();
	return	0;
}
uint32_t Interfaces_Class::CheckExternalCommunication(void)
{
	uint32_t	r=0;


	if (arinc.ar->newMessageR1)
	{
		request.externRequest.arinc1GotMessage=true;
		alu.PrepareNewTask(kALUTaskReadARINCR1);
	}
	if (arinc.ar->newMessageR2)
	{
		request.externRequest.arinc2GotMessage=true;
		alu.PrepareNewTask(kALUTaskReadARINCR2);
	}
	if (toolApp.IsAppConnected())
	{
		toolApp.handleCommunication();
		
	}
	//request.externRequest.arinc1GotMessage=arinc.newMessageR2;
	
	return	0;
}
bool	Interfaces_Class::CheckFlapperValveInterface(uint8_t add){
		if (fvc.fv->fv1StatusChanged)
		{
			fvc.fv->ReadActualPosition();
			alu.PrepareNewTask(kALUTaskUpdateFlapperValves);
			fvc.fv->fv1StatusChanged=false;
		}
		return	fvc.fv1StatusChanged;
}
bool	Interfaces_Class::CheckUSBInterface(void){
		if (usb.connected)
		{
			if (toolApp.IsAppConnected())
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
			if (lastTemperature!=temperatures.values[0][1])
			{
				lastTemperature=temperatures.values[0][1];
				alu.PrepareNewTask(kALUTaskUpdateTemperatures);
				result=true;
			}
				
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
	if (arinc.ar->newMessageR1||arinc.ar->newMessageR2)
	{
		if (arinc.ar->newMessageR1)
		{
			arinc.ar->ReadRXBuffer(1);
	
		} 
		else
		{
			arinc.ar->ReadRXBuffer(2);
	
		}
		
	} 
	else
	{
		
	}
	return	arinc.ar->newMessageR1||arinc.ar->newMessageR2;
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