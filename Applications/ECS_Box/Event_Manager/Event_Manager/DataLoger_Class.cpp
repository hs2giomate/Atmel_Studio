/* 
* DataLoger_Class.cpp
*
* Created: 7/1/2020 4:04:29 PM
* Author: GMateusDP
*/


#include "DataLoger_Class.h"
#include "ConfigurationData.h"
#include "FlashHandler_Class.h"
#include "MemoryFlash_Class.h"


static uint8_t staticEventLogs[QSPI_ERBLK];

// default constructor
DataLoger_Class::DataLoger_Class()
{
	stackPointer=&staticEventLogs[0];
} //DataLoger_Class

// default destructor
DataLoger_Class::~DataLoger_Class()
{
} //~DataLoger_Class

 

uint32_t	DataLoger_Class::WriteConfigurationData(ConfigurationData *cd){
	uint32_t	add=(uint32_t )(&flashMap->configurationSector);
	uint32_t	r=qspiFlash.ReadAddress((uint8_t*)PTR_CONFIG_DATA(cd),add,sizeof(dataOnMemory));
	
	return	r;
}

uint32_t	DataLoger_Class::LogResetEvent(void){
	return 0;
}

void	DataLoger_Class::SetStackValue(uint32_t stk){
	
	if (stk==0)
	{
		stackPointer=&staticEventLogs[stk];
	} 
	else
	{
	}
	stackValue=stk;
}

uint32_t DataLoger_Class::StackEventEntry(Event_Log_Entry *ent){
	
	if (IsStackFull())
	{
		memory.SaveEventLog(&staticEventLogs[0]);
		SetStackValue(0);
		
	} 
	else
	{
		
	}
	memcpy(&stackPointer[stackValue],ent,sizeof(Event_Log_Entry));
	stackValue+=sizeof(Event_Log_Entry);
	return stackValue;
}

bool	DataLoger_Class::IsStackFull(void){
	if ((stackPointer+(sizeof(Event_Log_Entry))>&staticEventLogs[QSPI_ERBLK-1]))
	{
		return true;
	} 
	else
	{
		return false;
	}
}