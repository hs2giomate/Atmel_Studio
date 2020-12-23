/* 
* Calendar_Class.h
*
* Created: 6/29/2020 11:25:42 AM
* Author: GMateusDP
*/


#ifndef __CALENDAR_CLASS_H__
#define __CALENDAR_CLASS_H__
#include "driver_init.h"
#include "hal_calendar.h"
#include <utils.h>
#include <utils_assert.h>
#include <hal_atomic.h>
#include "N25Q256_Class.h"

#include "string.h"
#include "DateTimeLog_Class.h"

#define		CALENDER_ADDRESS	0x0

#define CALENDAR_VERSION 0x00000001u
#define SECS_IN_LEAP_YEAR 31622400
#define SECS_IN_NON_LEAP_YEAR 31536000
#define SECS_IN_31DAYS 2678400
#define SECS_IN_30DAYS 2592000
#define SECS_IN_29DAYS 2505600
#define SECS_IN_28DAYS 2419200
#define SECS_IN_DAY 86400
#define SECS_IN_HOUR 3600
#define SECS_IN_MINUTE 60
#define DEFAULT_BASE_YEAR 1970
#define SAVING_ALARM	500

#define SET_ALARM_BUSY 1
#define PROCESS_ALARM_BUSY 2



class Calendar_Class: public DateTimeLog_Class
{
//variables
public:
	
	volatile	bool enlapsedTime;
	calendar_alarm alarm;
protected:
	friend class DateTimeLog_Class;
	calendar_date_time dateTime,lastDateTime;
	uint32_t  nullTimestamp;
private:
	calendar_date_time maxDateTime,compilationDateTime,erasedDateTime,nullDateTime;
	uint32_t  currentTimestamp,lastTimestamp,maxTimestamp,erasedTimestamp, compilationTimestamp;
	calendar_descriptor *ptrCalendar;
	N25Q256_Class	*ptrMemory;
	//AT24MAC_Class	*ptrMemory;
//	static AT24MAC_Class	eepromCal;
	PTR_DATE_TIME	handler;
	

//functions
public:
	Calendar_Class();
	Calendar_Class(calendar_descriptor *);
	~Calendar_Class();
	void Init(void);
	bool	Init(calendar_date_time dt_memory);

	uint32_t SetTimeStamp(uint32_t);
	uint32_t	GetDateTime(void);
	uint32_t	GetDateTime(calendar_date_time *dt);
	uint32_t	ReadDateTime(calendar_date_time *dt);
	uint32_t	convert_datetime_to_timestamp( calendar_date_time *dt);
	void		SetSavingAlarm(void);
	uint8_t		SetCalendarDataAdress(uint8_t);
protected:
private:
	Calendar_Class( const Calendar_Class &c );
	Calendar_Class& operator=( const Calendar_Class &c );
	
	bool leap_year(uint16_t year);
	uint32_t get_secs_in_month(uint32_t year, uint8_t month);
	uint32_t convert_timestamp_to_datetime( uint32_t ts,struct calendar_date_time *dt);
	void	SetCompilationDateTime(void);
	uint32_t	SetTime(calendar_time *p_calendar_time);
	uint32_t	SetDate(calendar_date *p_calendar_date);
	uint32_t	SetDateTime(calendar_date_time *cdt);
	void InitReferenceDates(void);
	
}; //Calendar_Class


extern	Calendar_Class	calendar;


#endif //__CALENDAR_CLASS_H__
