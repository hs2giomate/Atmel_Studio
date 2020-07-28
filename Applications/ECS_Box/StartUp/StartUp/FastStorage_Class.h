/* 
* FastStorage_Class.h
*
* Created: 7/8/2020 1:18:17 PM
* Author: GMateusDP
*/




#ifndef __FASTSTORAGE_CLASS_H__
#define __FASTSTORAGE_CLASS_H__
#include "AT24MAC_Class.h"
#include "ConfigState_Class.h"
#include "ConfigurationData.h"
struct      EEpromProperties {

	uint8_t manufacturerID;
	uint8_t memoryTypeID;
	uint8_t capacityID;
	uint32_t capacity;
	uint32_t eraseTime;
};


struct EEPROMMemoryStruct
{
	union
	{
		uint8_t		status;
		EEpromProperties	properties;
	};
	union
	{
		uint32_t		stack;
		calendar_date_time			timeLogs[AT24MAC_BUFFER_SIZE];
	};
	union
	{
		CycleData			cyclicData;
		
	};
	union
	{
		HVACState	appState;
		uint32_t	crcAppState;
		
	};
	union
	{
		ECSConfiguration	controllerState;
		uint32_t	crcCtrlState;
		
	};

};

typedef struct EEPROMMemoryStruct		EEPROMMemoryStruct;
extern			EEPROMMemoryStruct*		eepromMap;


#define CALENDAR_STACK_ADDRESS	sizeof(uint8_t) + sizeof(EEpromProperties)
#define CALENDAR_DATA_ADDRESS	sizeof(uint8_t) + CALENDAR_STACK_ADDRESS
#define CYCLIC_DATA_ADDRESS	  CALENDAR_DATA_ADDRESS +AT24MAC_BUFFER_SIZE* sizeof(calendar_date_time)
#define APP_STATE_ADDRESS	 sizeof(CycleData)+CYCLIC_DATA_ADDRESS
#define CTR_STATE_ADDRESS	 sizeof(HVACState)+APP_STATE_ADDRESS+sizeof(uint32_t)

class FastStorage_Class
{
//variables
public:
	uint32_t	ReadApplicationState(HVACState& hs);
	uint32_t	ReadCRCApplicationState(void);
protected:
private:
	PTR_HVAC_STATE	handlerAppState;
//functions
public:
	FastStorage_Class();
	~FastStorage_Class();
protected:
private:
	FastStorage_Class( const FastStorage_Class &c );
	FastStorage_Class& operator=( const FastStorage_Class &c );

}; //FastStorage_Class

#endif //__FASTSTORAGE_CLASS_H__
