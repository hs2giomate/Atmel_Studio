/* 
* MemoryStruct_Class.h
*
* Created: 7/8/2020 1:34:11 PM
* Author: GMateusDP
*/


#ifndef __MEMORYSTRUCT_CLASS_H__
#define __MEMORYSTRUCT_CLASS_H__
#include "ConfigState_Class.h"
#include "Parameters.h"
#include "N25Q256_Class.h"
#define		FLASH_SUBSECTOR_ERASE 4*1024

struct __attribute__((__packed__))  FlashPartition {
	bool supported;
	bool supportedMan;
	bool sfdpAvailable;
	uint8_t manufacturerID;
	uint8_t memoryTypeID;
	uint8_t capacityID;
	uint32_t capacity;
	uint32_t eraseTime;
};
struct __attribute__((__packed__)) HeaderSector
{
	uint32_t		status;
	uint32_t		stack;
	FlashPartition			logDetails;
	uint8_t			partitionSector[FLASH_SUBSECTOR_ERASE- sizeof(FlashPartition)-8];
};
struct __attribute__((__packed__)) ParametersSector
{
	UserParameters			parameters;
	UserParameters			defaultParameters;
	uint8_t					ParametersSector[FLASH_SUBSECTOR_ERASE-2*sizeof(UserParameters)];
};

struct __attribute__((__packed__)) ConfigurationSector
{
	ConfigurationData			defaultConfiguration;
	ConfigurationData			lastConfiguracion;
	ConfigurationData			currentConfiguration;
	ConfigurationData			updatedConfiguration;
	uint32_t					crcConfigData;
	uint8_t		configurationDataSector[FLASH_SUBSECTOR_ERASE- 4*sizeof(ConfigurationData)-4];
};

struct __attribute__((__packed__)) ECSSector
{
	ECSConfiguration		aluState;
	uint8_t		ecsSector[FLASH_SUBSECTOR_ERASE-sizeof(ECSConfiguration)];
};

struct __attribute__((__packed__)) HVACStateSector
{
	PowerLevel			powerLevel;
	HVACState		hvacDefaultState;
	HVACState		hvacState;
	uint32_t		crcAppState;
	uint8_t		stateSector[FLASH_SUBSECTOR_ERASE-sizeof(PowerLevel)-2*sizeof(HVACState)-4];
};

struct __attribute__((__packed__)) EventLoggerSector
{
	calendar_date_time	compilationTime;
	calendar_date_time	lastDateTime;
	uint32_t		stackPointerEventLogger;
	uint8_t		stateSector[1*FLASH_SUBSECTOR_ERASE-2*sizeof(calendar_date_time)-4];
};

struct __attribute__((__packed__)) MemoryFlashStruct
{
	union
	{
		HeaderSector headerSector;
	};
	union
	{
		ParametersSector parametersSector;
	};
	union
	{
		ConfigurationSector configurationSector;
	};
	union
	{
		ECSSector	ecsSector;
	};
	union
	{
		HVACStateSector hvacStateSector;
	};
 	union
 	{
 		EventLoggerSector EventsLogStateSector;
 	};

};

typedef struct MemoryFlashStruct		MemoryFlashStruct;
extern			MemoryFlashStruct*		flashMap;



class FlashHandler_Class
{
//variables
public:
protected:
private:
	ConfigurationData	configuration;
	PTR_CONFIG_DATA		handlerConfigData;
	PTR_HVAC_STATE	handlerAppState;
	uint32_t	flashAddress,read_result,write_result;
//functions
public:
	FlashHandler_Class();
	~FlashHandler_Class();
	
	uint32_t	ReadDefaultConfigurationData(ConfigurationData& cd);
	uint32_t	ReadLastConfigurationData(ConfigurationData& cd);
	uint32_t	ReadUpdatedConfigurationData(ConfigurationData& cd);
	uint32_t	WriteDefaultConfiguration(void);
	uint32_t	ReadCRCConfigurationData(void);
	uint32_t	WriteCRCConfigurationData(uint32_t crc);

	uint32_t	ReadApplicationState(HVACState& hs);
	uint32_t	ReadCRCApplicationState(void);
	uint32_t	WriteDefaultState(void);
	uint32_t	WriteCurrentState(HVACState& hs);
	uint32_t	WriteCRCAppState(uint32_t crc);
	uint32_t	WriteCurrentConfigurationData(ConfigurationData& cd);
	uint32_t	ReadDefaultParameters(UserParameters& up);
	uint32_t	WriteDefaultParameters(void);
	uint32_t	ReadParameters(UserParameters& up);
	uint32_t	ReadEventLogSector(uint32_t add, uint8_t *buffer);
protected:
private:
	FlashHandler_Class( const FlashHandler_Class &c );
	FlashHandler_Class& operator=( const FlashHandler_Class &c );
	uint32_t	ReadConfigurationData(ConfigurationData& cd,uint32_t	add);
	uint32_t	WriteConfigurationData(ConfigurationData& cd,uint32_t add);
	uint32_t	WriteLastConfigurationData(ConfigurationData& cd);
	uint32_t	WriteApplicationState(HVACState& hs,uint32_t add);
	uint32_t	ReadParameters(UserParameters& up,uint32_t add);
	uint32_t	WriteParameters(UserParameters& up,uint32_t add);
}; //MemoryStruct_Class



#endif //__MEMORYSTRUCT_CLASS_H__
