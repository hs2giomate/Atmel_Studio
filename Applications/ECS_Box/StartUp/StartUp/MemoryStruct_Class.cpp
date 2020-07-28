/* 
* MemoryStruct_Class.cpp
*
* Created: 7/8/2020 1:34:11 PM
* Author: GMateusDP
*/


#include "MemoryStruct_Class.h"
#include "N25Q256_Class.h"
#include "MemoryManagment_Class.h"

MemoryFlashStruct	*flashMap=NULL;

// default constructor
MemoryStruct_Class::MemoryStruct_Class()
{
} //MemoryStruct_Class

// default destructor
MemoryStruct_Class::~MemoryStruct_Class()
{
} //~MemoryStruct_Class

uint32_t	MemoryStruct_Class::ReadDefaultConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->defaultConfiguration;
	uint32_t	r=ReadConfigurationData(cd,add);
	return	r;
}
uint32_t	MemoryStruct_Class::ReadLastConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->lastConfiguracion;
	uint32_t	r=ReadConfigurationData(cd,add);
	return	r;
}
uint32_t	MemoryStruct_Class::ReadUpdatedConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->updatedConfiguration;
	uint32_t	r=ReadConfigurationData(cd,add);
	return	r;
}

uint32_t	MemoryStruct_Class::ReadConfigurationData(ConfigurationData& cd,uint32_t add){
	handlerConfigData=PTR_CONFIG_DATA(&cd);
	uint32_t	r=flash.ReadAddress((uint8_t*)handlerConfigData,add,sizeof(ConfigurationData));
	return	r;
}

uint32_t	MemoryStruct_Class::ReadCRCConfigurationData(void){
	uint32_t	crc;
	uint32_t	add=(uint32_t)&flashMap->crcConfigData;
	uint32_t	r=flash.ReadAddress((uint8_t*)(&crc),add,sizeof(uint32_t));
	return	crc;
}
uint32_t	MemoryStruct_Class::WriteConfigurationData(ConfigurationData& cd,uint32_t add){
	handlerConfigData=PTR_CONFIG_DATA(&cd);
	uint32_t	w=flash.WriteAddress((uint8_t*)handlerConfigData,add,sizeof(ConfigurationData));
	return	w;
}
uint32_t	MemoryStruct_Class::WriteDefaultConfiguration(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->defaultConfiguration;
	uint32_t	w=WriteConfigurationData(cd,add);
	return	w;
}

uint32_t	MemoryStruct_Class::WriteCRCConfigurationData(uint32_t crc){
	uint32_t	add=(uint32_t)&flashMap->crcConfigData;
	uint32_t	w=flash.WriteAddress((uint8_t*)(&crc),add,sizeof(uint32_t));
	return	w;
}

uint32_t	MemoryStruct_Class::WriteValidConfigurationData(ConfigurationData& cd){
	uint32_t	w=WriteLastConfigurationData(cd);
	uint32_t crc=memory.CalculateCRC((uint32_t*)PTR_CONFIG_DATA(&cd),sizeof(configuration));
	w=WriteCRCConfigurationData(crc);
	};
	
uint32_t	MemoryStruct_Class::WriteLastConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->lastConfiguracion;
	uint32_t	w=WriteConfigurationData(cd,add);
	return	w;
}