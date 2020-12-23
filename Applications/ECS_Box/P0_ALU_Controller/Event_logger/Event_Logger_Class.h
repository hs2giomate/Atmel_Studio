/* 
* Event_Logger_Class.h
*
* Created: 12/11/2020 10:50:53 AM
* Author: GMateusDP
*/


#ifndef __EVENT_LOGGER_CLASS_H__
#define __EVENT_LOGGER_CLASS_H__
#include "Calendar_Class.h"
#include "DateTime_Logger.h"
#include "DataLoger_Class.h"
#include "Event_Logger_Struct.h"
#include "time.h"
class Event_Logger_Class:  public Calendar_Class, private Event_Logger_Struct, private DataLoger_Class
{
//variables
public:
protected:
private:
	Calendar_Class* cal;
	calendar_date_time lastDateTime,currentDateTime, nullDateTime;
	uint8_t	bufferText[128];
	tm timeStruct;
	time_t controllerTime;
	Event_Log_Entry	entry;
	uint8_t	semiColonPosition;
	uint32_t stackLocalArray;
	

//functions
public:
	Event_Logger_Class();
	~Event_Logger_Class();
	bool Init();
	void SaveEvent(uint8_t* text);
protected:
private:
	Event_Logger_Class( const Event_Logger_Class &c );
	Event_Logger_Class& operator=( const Event_Logger_Class &c );
	void MakeNullDateTime(void);
	void RemoveSpaces(char *str);

}; //Event_Logger_Class
extern Event_Logger_Class	logger;

#endif //__EVENT_LOGGER_CLASS_H__
