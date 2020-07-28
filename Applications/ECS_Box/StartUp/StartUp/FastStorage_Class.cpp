/* 
* FastStorage_Class.cpp
*
* Created: 7/8/2020 1:18:16 PM
* Author: GMateusDP
*/


#include "FastStorage_Class.h"
#include "ConfigState_Class.h"
#include "N25Q256_Class.h"

EEPROMMemoryStruct		*eepromMap=NULL;

// default constructor
FastStorage_Class::FastStorage_Class()
{
} //FastStorage_Class

// default destructor
FastStorage_Class::~FastStorage_Class()
{
} //~FastStorage_Class

uint32_t	FastStorage_Class::ReadApplicationState(HVACState& as){
	handlerAppState=PTR_HVAC_STATE(&as);
	uint32_t	r=flash.ReadAddress((uint8_t*)handlerAppState,APP_STATE_ADDRESS,sizeof(HVACState));
	return	r;
}
uint32_t	FastStorage_Class::ReadCRCApplicationState(void){
	uint32_t	crc;
	uint32_t	r=flash.ReadAddress((uint8_t*)(&crc),APP_STATE_ADDRESS+sizeof(HVACState),sizeof(uint32_t));
	return	crc;
}