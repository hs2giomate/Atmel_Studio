/* 
* MemoryStruct_Class.h
*
* Created: 7/8/2020 1:34:11 PM
* Author: GMateusDP
*/


#ifndef __MEMORYSTRUCT_CLASS_H__
#define __MEMORYSTRUCT_CLASS_H__
#include "ConfigState_Class.h"
struct      FlashPartition {
	bool supported;
	bool supportedMan;
	bool sfdpAvailable;
	uint8_t manufacturerID;
	uint8_t memoryTypeID;
	uint8_t capacityID;
	uint32_t capacity;
	uint32_t eraseTime;
};


struct MemoryFlashStruct
{
	union
	{
		uint32_t		status;
		uint32_t		stack;
		FlashPartition			logDetails;
	};
	union
	{
		ConfigurationData			defaultConfiguration;
		ConfigurationData			lastConfiguracion;
		ConfigurationData			currentConfiguration;
		ConfigurationData			updatedConfiguration;
		uint32_t					crcConfigData;
	};
	union
	{
		ECSConfiguration		aluState;
		
	};
	union
	{
		PowerLevel			powerLevel;
		HVACState		hvacDefaultState;
		HVACState		hvacState;
		
	};

};

typedef struct MemoryFlashStruct		MemoryFlashStruct;
extern			MemoryFlashStruct*		flashMap;



class MemoryStruct_Class
{
//variables
public:
protected:
private:
	ConfigurationData	configuration;
	PTR_CONFIG_DATA		handlerConfigData;

//functions
public:
	MemoryStruct_Class();
	~MemoryStruct_Class();
	
	uint32_t	ReadDefaultConfigurationData(ConfigurationData& cd);
	uint32_t	ReadLastConfigurationData(ConfigurationData& cd);
	uint32_t	ReadUpdatedConfigurationData(ConfigurationData& cd);
	uint32_t	WriteDefaultConfiguration(ConfigurationData& cd);
	uint32_t	ReadCRCConfigurationData(void);
	uint32_t	WriteCRCConfigurationData(uint32_t crc);
	uint32_t	WriteValidConfigurationData(ConfigurationData& cd);
protected:
private:
	MemoryStruct_Class( const MemoryStruct_Class &c );
	MemoryStruct_Class& operator=( const MemoryStruct_Class &c );
	uint32_t	ReadConfigurationData(ConfigurationData& cd,uint32_t	add);
	uint32_t	WriteConfigurationData(ConfigurationData& cd,uint32_t add);
	uint32_t	WriteLastConfigurationData(ConfigurationData& cd);
}; //MemoryStruct_Class



#endif //__MEMORYSTRUCT_CLASS_H__
