/* 
* MemoryStruct_Class.cpp
*
* Created: 7/8/2020 1:34:11 PM
* Author: GMateusDP
*/


#include "FlashHandler_Class.h"
#include "N25Q256_Class.h"
#include "MemoryFlash_Class.h"


MemoryFlashStruct	*flashMap=NULL;

// default constructor
FlashHandler_Class::FlashHandler_Class()
{
} //MemoryStruct_Class

// default destructor
FlashHandler_Class::~FlashHandler_Class()
{
} //~MemoryStruct_Class

uint32_t	FlashHandler_Class::ReadDefaultConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->configurationSector.defaultConfiguration;
	uint32_t	r=ReadConfigurationData(cd,add);
	return	r;
}
uint32_t	FlashHandler_Class::ReadDefaultParameters(UserParameters& up){
	uint32_t	add=(uint32_t)&flashMap->parametersSector.defaultParameters;
	uint32_t	r=ReadParameters(up,add);
	return	r;
}
uint32_t	FlashHandler_Class::ReadParameters(UserParameters& up){
	uint32_t	add=(uint32_t)&flashMap->parametersSector.parameters;
	uint32_t	r=ReadParameters(up,add);
	return	r;
}
uint32_t	FlashHandler_Class::ReadEventLogSector(uint32_t add, uint8_t *buffer){
	if (add>(uint32_t)&flashMap->EventsLogStateSector)
	{
		read_result=qspiFlash.ReadAddress(buffer,add,64);
	} 
	else
	{
		read_result=1;
	}

	return	read_result;
}
uint32_t	FlashHandler_Class::ReadLastConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->configurationSector.lastConfiguracion;
	uint32_t	r=ReadConfigurationData(cd,add);
	return	r;
}
uint32_t	FlashHandler_Class::ReadUpdatedConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->configurationSector.updatedConfiguration;
	uint32_t	r=ReadConfigurationData(cd,add);
	return	r;
}

uint32_t	FlashHandler_Class::ReadConfigurationData(ConfigurationData& cd,uint32_t add){
	handlerConfigData=PTR_CONFIG_DATA(&cd);
	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)handlerConfigData,add,sizeof(ConfigurationData));
	return	r;
}

uint32_t	FlashHandler_Class::ReadParameters(UserParameters& up,uint32_t add){

	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)&up,add,sizeof(UserParameters));
	return	r;
}
uint32_t	FlashHandler_Class::ReadCRCConfigurationData(void){
	uint32_t	crc;
	uint32_t	add=(uint32_t)&flashMap->configurationSector.crcConfigData;
	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)(&crc),add,sizeof(uint32_t));
	return	crc;
}
uint32_t	FlashHandler_Class::WriteConfigurationData(ConfigurationData& cd,uint32_t add){
	handlerConfigData=PTR_CONFIG_DATA(&cd);
	uint32_t	w=qspiFlash.WriteAddress((uint8_t*)handlerConfigData,add,sizeof(ConfigurationData));
	return	w;
}
uint32_t	FlashHandler_Class::WriteParameters(UserParameters& up,uint32_t add){

	uint32_t	w=qspiFlash.WriteAddress((uint8_t*)&up,add,sizeof(UserParameters));
	return	w;
}
uint32_t	FlashHandler_Class::WriteApplicationState(HVACState& hs,uint32_t add){
	handlerAppState=PTR_HVAC_STATE(&hs);
	uint32_t	w=qspiFlash.WriteAddress((uint8_t*)handlerAppState,add,sizeof(HVACState));
	return	w;
}
uint32_t	FlashHandler_Class::WriteDefaultConfiguration(void){
	ConfigurationData cd=factoryDefaultsConfiguration;
	uint32_t	add=(uint32_t)&flashMap->configurationSector.defaultConfiguration;
	uint32_t	w=WriteConfigurationData(cd,add);
	return	w;
}
uint32_t	FlashHandler_Class::WriteDefaultParameters(void){
	UserParameters up=defaultParameters;
	uint32_t	add=(uint32_t)&flashMap->parametersSector.defaultParameters;
	uint32_t	w=WriteParameters(up,add);
	return	w;
}

uint32_t	FlashHandler_Class::WriteDefaultState(void){
	HVACState hs=defaultHVACState;
	uint32_t	add=(uint32_t)&flashMap->hvacStateSector.hvacDefaultState;
	uint32_t	w=WriteApplicationState(hs,add);
	return	w;
}

uint32_t	FlashHandler_Class::WriteCurrentState(HVACState& hs){
	uint32_t	add=(uint32_t)&flashMap->hvacStateSector.hvacState;
	uint32_t	w=WriteApplicationState(hs,add);
	return	w;
}

uint32_t	FlashHandler_Class::WriteCRCConfigurationData(uint32_t crc){
	uint32_t	add=(uint32_t)&flashMap->configurationSector.crcConfigData;
	uint32_t	w=qspiFlash.WriteAddress((uint8_t*)(&crc),add,sizeof(uint32_t));
	return	w;
}
uint32_t	FlashHandler_Class::WriteCRCAppState(uint32_t crc){
	uint32_t	add=(uint32_t)&flashMap->hvacStateSector.crcAppState;
	uint32_t	w=qspiFlash.WriteAddress((uint8_t*)(&crc),add,sizeof(uint32_t));
	return	w;
}


	
uint32_t	FlashHandler_Class::WriteCurrentConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->configurationSector.currentConfiguration;
	uint32_t	w=WriteConfigurationData(cd,add);
	return	w;
}
uint32_t	FlashHandler_Class::WriteLastConfigurationData(ConfigurationData& cd){
	uint32_t	add=(uint32_t)&flashMap->configurationSector.lastConfiguracion;
	uint32_t	w=WriteConfigurationData(cd,add);
	return	w;
}
uint32_t	FlashHandler_Class::ReadApplicationState(HVACState& as){
	flashAddress=(uint32_t)&flashMap->hvacStateSector.hvacState;
	handlerAppState=PTR_HVAC_STATE(&as);
	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)handlerAppState,flashAddress,sizeof(HVACState));
	return	r;
}
uint32_t	FlashHandler_Class::ReadCRCApplicationState(void){
	uint32_t	crc;
	flashAddress=(uint32_t)&flashMap->hvacStateSector.crcAppState;
	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)(&crc),flashAddress,sizeof(uint32_t));
	return	crc;
}