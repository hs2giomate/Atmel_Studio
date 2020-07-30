/* 
* DateTimeLog_Class.h
*
* Created: 7/1/2020 3:03:18 PM
* Author: GMateusDP
*/


#ifndef __DATETIMELOG_CLASS_H__
#define __DATETIMELOG_CLASS_H__
#include "coreTypes.h"
#include "hpl_calendar.h"

#define		UPDATE_TIME_MAX	500
#define		UPDATE_ERROR	50

typedef		void 	(*PTR_DATE_TIME)(calendar_date_time *);
typedef	tick_t (CAST2TICK)(double);

class DateTimeLog_Class
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
	

//functions
public:
	DateTimeLog_Class();
	~DateTimeLog_Class();
	tick_t	GetPowerInterrupTime(void);
	bool	WasInterrupted(void);
	uint8_t	ReadStackValue(void);
	
	uint16_t	SetCalendarStackAddress(uint32_t);
	uint16_t	SetCalendarAddress(uint32_t);
	virtual uint32_t	GetDateTime(calendar_date_time *dt)=0;
	virtual	uint32_t	convert_datetime_to_timestamp( calendar_date_time *dt)=0;
	uint32_t	ReadLastDateTime(void);
	uint32_t	SaveCurrentDateTime(void);
	uint32_t	ReadDateTime(calendar_date_time *dt);
protected:
private:
	DateTimeLog_Class( const DateTimeLog_Class &c );
	DateTimeLog_Class& operator=( const DateTimeLog_Class &c );
	 uint8_t RollStack(void);
	 uint32_t	IncrementMemoryStack();


}; //DateTimeLog_Class

#endif //__DATETIMELOG_CLASS_H__
