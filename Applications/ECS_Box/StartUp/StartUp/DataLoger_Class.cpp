/* 
* DataLoger_Class.cpp
*
* Created: 7/1/2020 4:04:29 PM
* Author: GMateusDP
*/


#include "DataLoger_Class.h"
#include "ConfigurationData.h"
#include "MemoryStruct_Class.h"

// default constructor
DataLoger_Class::DataLoger_Class()
{
} //DataLoger_Class

// default destructor
DataLoger_Class::~DataLoger_Class()
{
} //~DataLoger_Class

 

uint32_t	DataLoger_Class::WriteConfigurationData(ConfigurationData *cd){
	uint32_t	add=(uint32_t )(&flashMap->currentConfiguration);
	uint32_t	r=flash.ReadAddress((uint8_t*)PTR_CONFIG_DATA(cd),add,sizeof(dataOnMemory));
	
	return	r;
}

uint32_t	DataLoger_Class::LogResetEvent(void){
	return 0;
}