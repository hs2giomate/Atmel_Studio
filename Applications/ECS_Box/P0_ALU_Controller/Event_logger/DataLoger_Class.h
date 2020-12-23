/* 
* DataLoger_Class.h
*
* Created: 7/1/2020 4:04:30 PM
* Author: GMateusDP
*/


#ifndef __DATALOGER_CLASS_H__
#define __DATALOGER_CLASS_H__
#include "N25Q256_Class.h"
#include "ConfigurationData.h"
#include "Event_Logger_Struct.h"



class DataLoger_Class: public	N25Q256_Class
{
//variables
public:
	uint8_t	*ptrLocalEventsStack;
protected:
private:
	ConfigurationData	dataOnMemory;
	RingBuffer	ringBuffer;
	uint8_t		calendarStack;
	PTR_CONFIG_DATA		handlerConfData;
	uint32_t stackValue;
	Event_Log_Entry	entry;	
	uint8_t		*stackPointer;

//functions
public:
	DataLoger_Class();
	~DataLoger_Class();
	uint32_t	WriteConfigurationData(ConfigurationData *cd);
	uint32_t	LogResetEvent(void);
	void	SetStackValue(uint32_t stk);
	uint32_t StackEventEntry(Event_Log_Entry *ent);
protected:
private:
	DataLoger_Class( const DataLoger_Class &c );
	DataLoger_Class& operator=( const DataLoger_Class &c );

}; //DataLoger_Class

#endif //__DATALOGER_CLASS_H__
