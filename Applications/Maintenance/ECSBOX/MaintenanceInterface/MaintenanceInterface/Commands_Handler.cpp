/* 
* Commands_Handler.cpp
*
* Created: 11/27/2020 3:56:45 PM
* Author: GMateusDP
*/


#include "Commands_Handler.h"
#include "string.h"
#include "EvaporatorAndCondesatorFans_Class.h"
#include "CDC_Class.h"
#include "Dual_Flapper_Valve_Controller.h"

// default constructor
Commands_Handler::Commands_Handler()
{
	
} //Commands_Handler

// default destructor
Commands_Handler::~Commands_Handler()
{
} //~Commands_Handler

void Commands_Handler::InitCommandHandler(uint8_t * buffer){
	usbMessageBuffer=buffer;
}

bool Commands_Handler::CommandSetEnableFans(void){

	uint8_t	data=0;
	
	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(singleTaskMessage.header.task == kHVACCommandSetEnableFans);
	if (result){
		data=singleTaskMessage.description;
		
		SetEnableCondesator(data);
		
		}else{

	}
	
	return result;
}

uint8_t Commands_Handler::SetEnableCondesator(uint8_t data){
	if ((data&(0x04))>0)
	{
		fans.condesator->SetEnable(true);
		
	}
	else
	{
		fans.condesator->SetEnable(false);
	}
	return SetEnableFan2(data);
}

uint8_t	Commands_Handler::SetEnableFan2(uint8_t data){
	if ((data&(0x02))>0){
		fans.evaporator[1]->SetEnable(true);
		}else{
		fans.evaporator[1]->SetEnable(false);
		
		
	}
	return SetEnableFan1(data);
	
}

uint8_t Commands_Handler::SetEnableFan1(uint8_t data){
	if ((data&(0x01))>0)
	{
		fans.evaporator[0]->SetEnable(true);
	}
	else
	{
		fans.evaporator[0]->SetEnable(false);
	}
	return data;
}

bool Commands_Handler::CommandSetPWMFans(void){

	uint32_t	w,r;
	
	uint8_t		*data=data_fans_array;
	
	
	//memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	memcpy(&message64,usbMessageBuffer,64);
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(message64.header.task ==	kHVACCommandSetPWMFans);
	if (result){
		data=message64.content;
		if (data[0]>0)
		{
			fans.evaporator[0]->SetPWM(data[0]);
			
		}

		if (data[1]>0){
			fans.evaporator[1]->SetPWM(data[1]);
		}
		if (data[2]>0){
			fans.condesator->SetPWM(data[2]);
		}

		
		

	}
	
	return result;
}
bool Commands_Handler::CommandFansStatus(){
	uint8_t gpios[EVAPORATOR_AND_CONDESATOR_EXPANDERS];

	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	bool	result(singleTaskMessage.header.task == kHVACCommandRedStatusFans);
	if (result){
		fans.GetGPIOSValues(gpios);
		CreateFullBufferMessage(usbMessageBuffer,gpios);
		usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);

		
	}
	
	return result;
}

bool  Commands_Handler::CommandSetFlapperValve(void){


	uint8_t	data=0;
	
	memcpy(&message64,usbMessageBuffer,64);
	
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(message64.header.task == kHVACCommandFlapperValve);
	if (result){
		for (int i = 0; i < FLAPPER_VALVE_QUANTITY; i++)
		{
			data=message64.content[i];
			if ((data&(0x01))>0)
			{
				//fvc.fv->SetEnable(true);
				fv.fvx[i]->StartControlling() ;
			}
			else
			{
				//fvc.fv->SetEnable(false);
				fv.fvx[i]->StopControlling();
			}
		}
	

		
	}else{

	}
	
	return result;
}

bool  Commands_Handler::CommandSetFlapperPosition(void){


	uint8_t	data=0;
	
	memcpy(&message64,usbMessageBuffer,64);
	

	bool	result(message64.header.task == kHVACWriteSetpointFlapperValve);
	if (result){
		for ( int i = 0; i < FLAPPER_VALVE_QUANTITY; i++)
		{
			data=message64.content[i];

			if (fv.fvx[i]->controllerEnabled)
			{
				fv.fvx[i]->StartControlling(data);
			}
			
			
			fv.fvx[i]->doPeriodicTask=false;
		}
		
		// 			fans.evaporator[1]->SetEnable(data&0x02);
		// 		}

		
	}else{

	}
	
	return result;
}
bool Commands_Handler::CommandReadFlapperData(void){

	
	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	bool	result(singleTaskMessage.header.task == kHVACReadPositionFlapperValve);
	if (result){
		FVDataStruct	fvds[2];
		
		for (int i = 0; i < FLAPPER_VALVE_QUANTITY; i++)
		{
				fv.fvx[i]->UpdateFlapperValveData();	
				fvds[i]= fv.fvx[i]->dataStruct;
		}
		
		#ifdef DEBUG
			uint8_t sizeStruct=sizeof(fvds);
		#endif
	
		CreateFullBufferMessage(usbMessageBuffer,(uint8_t*)&fvds[0]);
		
		
	
		usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);

		
	}
	
	return result;
}