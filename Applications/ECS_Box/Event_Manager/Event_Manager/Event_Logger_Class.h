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

enum TaskResult
{
	SUCCESS=0,
	FAILLED,
	ABORT
};
class Event_Logger_Class:  public Calendar_Class, private Event_Logger_Struct, private DataLoger_Class
{
//variables
public:
	uint32_t *memory_event_stack;
	uint8_t *event_buffer_arrray;
protected:
private:
	Calendar_Class* cal;
	calendar_date_time lastDateTime,currentDateTime, nullDateTime;
	uint8_t	bufferText[128];
	tm timeStruct;
	time_t controllerTime;
	Event_Log_Entry	entry;
	uint8_t	semiColonPosition;
	uint32_t stackLocalArray,currentTimestamp;
	TaskResult task_result;
	char *logger_text;
	char *logger_module;
	char *logger_number;
	uint8_t *info_text;
	uint8_t  text_length;
	

//functions
public:
	Event_Logger_Class();
	~Event_Logger_Class();
	bool Init();
	void SaveEvent(uint8_t* text);
	void SaveEvent(char const *text);
	void SaveEventIndexResult(char *text,uint8_t index, uint8_t result);
	uint32_t GetCurrentTimeStamp(void);
	void SaveInstationLog(void);
protected:
private:
	Event_Logger_Class( const Event_Logger_Class &c );
	Event_Logger_Class& operator=( const Event_Logger_Class &c );
	void MakeNullDateTime(void);
	void RemoveSpaces(char *str);

}; //Event_Logger_Class
extern Event_Logger_Class	logger;

#endif //__EVENT_LOGGER_CLASS_H__
