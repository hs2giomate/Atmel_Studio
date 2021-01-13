/* 
* DateTimeLog_Class.h
*
* Created: 7/1/2020 3:03:18 PM
* Author: GMateusDP
*/


#ifndef __DATETIMELOGGER_CLASS_H__
#define __DATETIMELOGGER_CLASS_H__
#include "coreTypes.h"
#include "hpl_calendar.h"
#include "Calendar_Class.h"

#define		UPDATE_TIME_MAX	500
#define		UPDATE_ERROR	50



class DateTime_Logger: private virtual  Calendar_Class
{
//variables
public:
	volatile	bool powerInterrupt;
	
protected:
private:
	uint32_t  currentTimestamp,lastTimestamp,maxTimestamp,timestamp;
	double	timeDifference;
	uint16_t		stackAdress,stackValue;
	uint16_t	dataTimeLogsAddress;
	calendar_date_time		dateTime,lastDateTime;
	PTR_DATE_TIME	handler;
	friend class Calendar_Class;
	

//functions
public:
	DateTime_Logger();
	~DateTime_Logger();
	
	uint32_t	ReadDateTime(calendar_date_time *dt);
	uint32_t	ReadLastDateTime(calendar_date_time *dt);
protected:
private:
	DateTime_Logger( const DateTime_Logger &c );
	DateTime_Logger& operator=( const DateTime_Logger &c );
	 uint8_t RollStack(void);
	 uint32_t	IncrementMemoryStack();


}; //DateTimeLog_Class

#endif //__DATETIMELOG_CLASS_H__
