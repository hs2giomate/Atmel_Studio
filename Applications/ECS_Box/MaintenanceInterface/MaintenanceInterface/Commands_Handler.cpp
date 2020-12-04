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

	uint32_t	w,r;
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
	uint8_t	data=0;
	
	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(singleTaskMessage.header.task == 	kHVACCommandSetPWMFans);
	if (result){
		data=singleTaskMessage.description;
		if (data>0)
		{
			fans.evaporator[0]->SetPWM(data);
			fans.evaporator[1]->SetPWM(data);
			fans.condesator->SetPWM(data);
		}
		else
		{
		}

		
		}else{

	}
	
	return result;
}
bool Commands_Handler::CommandFansStatus(){
	uint8_t gpios[2];
	SingleTaskMessage	singleTask;
	memcpy(&singleTask,usbMessageBuffer,sizeof(SingleTaskMessage));
	bool	result(singleTask.header.task == kHVACCommandRedStatusFans);
	if (result){
		fans.GetGPIOSValues(gpios);
		CreateFullBufferMessage(usbMessageBuffer,gpios);

	
			usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);

		
	}
	
	return result;
}