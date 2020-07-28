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



class DataLoger_Class: public	N25Q256_Class
{
//variables
public:
protected:
private:
	ConfigurationData	dataOnMemory;
	RingBuffer	ringBuffer;
	uint8_t		calendarStack;
	PTR_CONFIG_DATA		handlerConfData;

//functions
public:
	DataLoger_Class();
	~DataLoger_Class();
	uint32_t	WriteConfigurationData(ConfigurationData *cd);
	uint32_t	LogResetEvent(void);
protected:
private:
	DataLoger_Class( const DataLoger_Class &c );
	DataLoger_Class& operator=( const DataLoger_Class &c );

}; //DataLoger_Class

#endif //__DATALOGER_CLASS_H__
