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
#include "Event_Logger_Class.h"


static uint8_t staticEventLogs[QSPI_ERBLK];
static uint32_t local_memory_event_stack;

// default constructor
DataLoger_Class::DataLoger_Class()
{
	stackPointer=&staticEventLogs[0];
	logger.memory_event_stack=&local_memory_event_stack;
	logger.event_buffer_arrray=staticEventLogs;
	logger.event_buffer_arrray_stack=&stackValue;
	
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
		local_memory_event_stack=memory.SaveEventLog(staticEventLogs);
		SetStackValue(0);
		memset(staticEventLogs,0,QSPI_ERBLK);
		
	} 
	else
	{
		
	}
	memcpy(&stackPointer[stackValue],ent,sizeof(Event_Log_Entry));
	stackValue+=sizeof(Event_Log_Entry);
	return stackValue;
}
uint32_t DataLoger_Class::StackEventEntry(Event_Log_Entry *ent, uint8_t len){
	if (len==0xff)
	{
		uint8_t length;
		uint8_t text[]="Initialization Finished";
		
		length=(uint8_t)strlen((char*)text);
		
			memcpy(&stackPointer[stackValue],ent,12+length+1);
			local_memory_event_stack=memory.SaveEventLog(staticEventLogs);
			SetStackValue(0);
			memset(staticEventLogs,0,QSPI_ERBLK);
		
	}
	else
	{
		if (IsStackFull())
		{
			local_memory_event_stack=memory.SaveEventLog(staticEventLogs);
			SetStackValue(0);
			memset(staticEventLogs,0,QSPI_ERBLK);
			
		}
		else
		{
			
		}
		memcpy(&stackPointer[stackValue],ent,12+len);
		stackValue+=12+len;
		
	}
	
	return stackValue;
}

bool	DataLoger_Class::IsStackFull(void){
	if (stackValue+(sizeof(Event_Log_Entry))>QSPI_ERBLK)
	{
		return true;
	} 
	else
	{
		return false;
	}
}