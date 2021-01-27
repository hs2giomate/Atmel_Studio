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
#include "MemoryFlash_Class.h"
#include "FlashMemoryClass.h"
#include "HEATERS_HANDLER.h"
#include "Scavenge_Fan.h"
#include "Compresor_Controller.h"
#include "TemperatureSensors_Class.h"
#include "Event_Logger_Class.h"


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
			if (flapper.valve[i]->isOK)
			{
				data=message64.content[i];
				if ((data&(0x01))>0)
				{
					//fvc.fv->SetEnable(true);
					flapper.valve[i]->StartControlling() ;
				}
				else
				{
					//fvc.fv->SetEnable(false);
					flapper.valve[i]->StopControlling();
				}
			} 
			else
			{
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
			if (flapper.valve[i]->isOK)
			{
				data=message64.content[i];

				if (flapper.valve[i]->controllerEnabled)
				{
					flapper.valve[i]->StartControlling(data);
				}
				
				
				flapper.valve[i]->doPeriodicTask=false;
			} 
			else
			{
			}
			
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
			if (flapper.valve[i]->isOK)
			{
				flapper.valve[i]->UpdateFlapperValveData();
				fvds[i]= flapper.valve[i]->dataStruct;
			} 
			else
			{
				fvds[i]= flapper.valve[i]->dataStruct;
			}
				
		}
		
		#ifdef DEBUG
			uint8_t sizeStruct=sizeof(fvds);
		#endif
	
		CreateFullBufferMessage(usbMessageBuffer,(uint8_t*)&fvds[0]);
		
		
	
		usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);

		
	}
	
	return result;
}

bool Commands_Handler::CommandReadParameters(){
	int n=sizeof(HVACMessageHeader);
	HVACMessageHeader hm;

	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	bool	result(singleTaskMessage.header.task == kGAINCommandReadParameters);
	if (result){
		//
		uint32_t add=(uint32_t)&flashLayout->parameters;
		memory.ReadParameters(parameters);
		memcpy(usbMessageBuffer,(void*)&hm,n);
		memcpy((uint8_t*)&usbMessageBuffer[n],(void*)&parameters,sizeof(UserParameters));
		is_stand_alone=flapper.IsStandAlone();
		if (is_stand_alone)
		{
			usbMessageBuffer[n+sizeof(UserParameters)]=parameters.flapperValveStandAloneMinimumPosition;
		}
		else
		{
			usbMessageBuffer[n+sizeof(UserParameters)]=parameters.flapperValveMinimumPosition;
		}
		usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);

		
		
		}else{

	}
	
	return result;
}

bool Commands_Handler::CommandReadDataLogger(){
	//	usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);

	memcpy(&dataLogMessage,usbMessageBuffer,sizeof(DataLogMessage));
	bool	result(dataLogMessage.header.task == kHVACCommandReadDataLog);
	if (result){
			uint32_t add=*logger.memory_event_stack;
			
			if (add>=dataLogMessage.address)
			{
				memory_flash_address= dataLogMessage.address;
				uint8_t memory_block[QSPI_ERBLK];
				//	CreateFullBufferMessage(usbMessageBuffer,(uint8_t*)&add);
				//	usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
				/*	uint32_t add=*logger.memory_event_stack;*/
				
				read_result=memory.ReadEventLogSector(memory_flash_address,memory_block);
				if (read_result==0)
				{
					CreateFullBufferMessage(usbMessageBuffer,(uint8_t*)&memory_flash_address);
					usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
					delay_ms(100);
					write_result=usb.write(memory_block,QSPI_ERBLK);
					result=write_result==0;
					
					
				}
				else
				{
					memory_flash_address=0;
					CreateFullBufferMessage(usbMessageBuffer,(uint8_t*)&memory_flash_address);
					usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
					result=false;
					
				}
			} 
			else
			{
				memory_flash_address=0;
				CreateFullBufferMessage(usbMessageBuffer,(uint8_t*)&add);
				usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
				result=false;
			}
			
			
		}
		


	
	return result;
}
bool Commands_Handler::CommandWriteParameters(void){

	uint32_t	w,r;
	int n=sizeof(HVACMessageHeader);
	HVACMessageHeader hm;
	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	
	bool	result(singleTaskMessage.header.task == kGAINCommandWriteParameters);
	if (result){
		
		
		
		
		memcpy((uint8_t*)&parameters,&usbMessageBuffer[n],sizeof(UserParameters));
		uint32_t add=(uint32_t)&flashLayout->parameters;
		r=memory.SaveParameters(parameters);
		result=(bool)(r==0);
		
		
		}else{

	}
	
	return result;
}
bool Commands_Handler::CommandReadHeaterStatus(){

	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	bool	result(singleTaskMessage.header.task == kHVACCommandReadHeaterStatus);
	if (result){
		
		heaters.ReadAllGPIOs(heater_data_array);
		
		CreateFullBufferMessage(usbMessageBuffer,heater_data_array);
		usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
		
	}
	return result;
}
bool Commands_Handler::CommandSetHeaters(void){


	
	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(singleTaskMessage.header.task == kHVACCommandSetHeaters);
	if (result){
		lastEnableHeaters=enableHeaters;
		enableHeaters=singleTaskMessage.description;
		if (enableHeaters!=lastEnableHeaters)
		{
			for (uint8_t j = 0; j < 2; j++)
			{
				for (uint8_t i = 0; i < 4; i++)
				{
					powerOn=enableHeaters&(0x01<<(i+4*j));
					heaters.module[j]->SetRelay(i,powerOn);

			
					
					

				}
			}
			
		}
	}
	
	return result;
}
bool Commands_Handler::CommandReadScavengeStatus(void){

	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	bool	result(singleTaskMessage.header.task == kHVACCommandReadScavengeStatus);
	if (result){
		data_byte=scavenge.ReadStatus();
	
		
		CreateFullBufferMessage(usbMessageBuffer,&data_byte);
		usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
		
	}
	return result;
}
bool Commands_Handler::CommandSetScavenge(void){


	
	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(singleTaskMessage.header.task == kHVACCommandSetScavenge);
	if (result){
		data_byte=singleTaskMessage.description;
		powerOn=(data_byte&(0x02))>0;
		scavenge.SetEnable(powerOn);
	
	}
	
	return result;
}

bool Commands_Handler::CommandReadCompressorStatus(void){

	memcpy(&singleTaskMessage,usbMessageBuffer,sizeof(SingleTaskMessage));
	bool	result(singleTaskMessage.header.task == kHVACCommandReadCompressorStatus);
	if (result){
		ccu.GetCompressorStatus(compressor_data_array);
		memcpy(&compressor_data_array[4],(void*)&temperatures.values[0][0],4);
		memcpy(&compressor_data_array[8],(void*)&temperatures.values[0][1],4);
		CreateFullBufferMessage(usbMessageBuffer,compressor_data_array);
		usb.write(usbMessageBuffer,MAINTENANCE_TOOL_BUFFER_SIZE);
		
	}
	return result;
}
bool Commands_Handler::CommandSetCompressor(void){
	
	memcpy(&message64,usbMessageBuffer,64);
	
	//	singleTaskMessage.description=localBuffer[0x06];
	bool	result(message64.header.task == kHVACCommandSetCompressor);
	if (result){
		
		data_byte=message64.content[0] ;
		compressor_speed=message64.content[1]*256+message64.content[2];
		if ((data_byte&0x01)>0)
		{
			if (ccu.IsEnabled())
			{
			//	memcpy(&compressor_speed,&message64.content[1],2);
				if ( compressor_speed>0)
				{
					ccu.Set_Motor_Speed(compressor_speed);
				} 
				else
				{
					ccu.SetEnable(false);
				}
			} 
			else
			{
				ccu.SetEnable(true);
			}
		}else{
			if (ccu.IsEnabled())
			{
				ccu.SetEnable(false);
			} 
			else
			{
			//	memcpy(&compressor_speed,&message64.content[1],2);
				if ( compressor_speed>0)
				{
					ccu.Set_Motor_Speed(compressor_speed);
				}
			}
		}
	
		
	}
	
	return result;
}