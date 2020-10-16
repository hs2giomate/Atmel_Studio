/* 
* FastStorage_Class.cpp
*
* Created: 7/8/2020 1:18:16 PM
* Author: GMateusDP
*/


#include "FastStorage_Class.h"
#include "ConfigState_Class.h"
#include "N25Q256_Class.h"
#include "MemoryManagment_Class.h"

EEPROMMemoryStruct		*eepromMap=NULL;

// default constructor
FastStorage_Class::FastStorage_Class()
{
} //FastStorage_Class

// default destructor
FastStorage_Class::~FastStorage_Class()
{
} //~FastStorage_Class


uint32_t	FastStorage_Class::ReadFastCRCApplicationState(void){
	uint32_t	crc,add32;
	add32=(uint32_t)&eepromMap->crcAppState;
	eepromAddress=(uint16_t)(0xffff&add32);
	uint32_t	r=eeprom.ReadAddress((uint8_t*)(&crc),eepromAddress,sizeof(uint32_t));
	return	crc;
}
uint32_t	FastStorage_Class::ReadFastApplicationState(HVACState& hs){
	uint32_t	crc,add32,r;
	add32=(uint32_t)&eepromMap->appState;
	eepromAddress=(uint16_t)(0xffff&add32);
	handlerAppState=PTR_HVAC_STATE(&hs);
	r=eeprom.ReadAddress((uint8_t*)handlerAppState,eepromAddress,sizeof(HVACState));
	return	r;
}
uint32_t	FastStorage_Class::ReadFastParameters(UserParameters& up){
	uint32_t	crc,add32,r;
	add32=(uint32_t)&eepromMap->parameters;
	eepromAddress=(uint16_t)(0xffff&add32);
	r=eeprom.ReadAddress((uint8_t*)&up,eepromAddress,sizeof(UserParameters));
	return	r;
}
uint32_t	FastStorage_Class::GetTimeLogs(calendar_date_time& cdt){
	uint32_t	add32=(uint32_t)&eepromMap->timeLogs[0];
	eepromAddress=(uint16_t)(0xffff&add32);
	handlerCalendar=(PTR_DATE_TIME)(&cdt);
	uint32_t	r=eeprom.ReadAddress((uint8_t*)(handlerCalendar),(uint8_t)eepromAddress,sizeof(calendar_date_time)*AT24MAC_BUFFER_SIZE);
	return	r;
}
uint32_t	FastStorage_Class::WriteFastCurrentState(HVACState& hs){
	uint32_t	add32=(uint32_t)&eepromMap->appState;
	eepromAddress=(uint16_t)(0xffff&add32);
	uint32_t	w=WriteFastAppState(hs,eepromAddress);
	crc32=		memory.CalculateCRC((uint32_t *)PTR_HVAC_STATE(&hs),sizeof(HVACState));
	w=WriteFastCRCAppState(crc32);
	return	w;
}
uint32_t	FastStorage_Class::WriteFastAppState(HVACState& hs,uint32_t add){
	handlerAppState=PTR_HVAC_STATE(&hs);
	uint32_t	w=eeprom.WriteAddress((uint8_t*)handlerAppState,(uint16_t)add,sizeof(HVACState));
	return	w;
}
uint32_t	FastStorage_Class::WriteFastCRCAppState(uint32_t crc){
	uint16_t	add=(uint32_t)&eepromMap->crcAppState;
	uint32_t	w=eeprom.WriteAddress((uint8_t*)(&crc),(uint16_t)add,sizeof(uint32_t));
	return	w;
}

uint32_t	FastStorage_Class::WriteFastParameters(UserParameters& up){
	uint32_t	add32=(uint32_t)&eepromMap->parameters;
	uint8_t		add8=(uint8_t)(0xff&&add32);
	uint32_t	w=eeprom.WriteAddress((uint8_t*)(&up),add8,sizeof(UserParameters));
	return	w;
}
uint32_t	FastStorage_Class::WriteFastDefaultParameters(void){
	UserParameters up=defaultParameters;
	uint32_t	add32=(uint32_t)&eepromMap->parameters;
	uint8_t		add8=(uint8_t)(0xff&&add32);
	uint32_t	w=eeprom.WriteAddress((uint8_t*)(&up),add8,sizeof(UserParameters));
	return	w;
}