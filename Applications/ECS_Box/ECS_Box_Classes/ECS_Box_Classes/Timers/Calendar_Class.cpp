/* 
* Calendar_Class.cpp
*
* Created: 6/29/2020 11:25:42 AM
* Author: GMateusDP
*/


#include "Calendar_Class.h"

Calendar_Class	*ptrCalendarClass;
//AT24MAC_Class	Calendar_Class::eepromCal(&I2C_EEPROM);
static void alarm_calendar(struct calendar_descriptor *const descr)
{
	ptrCalendarClass->enlapsedTime=true;
	/* alarm expired */
}

// default constructor
Calendar_Class::Calendar_Class()
{
	ptrCalendarClass=this;
	//ptrMemory=&memory;
} //Calendar_Class

// default constructor
Calendar_Class::Calendar_Class(calendar_descriptor *d)
{
	ptrCalendar=d;
	ptrCalendarClass=this;
	//ptrMemory=&memory;
} //Calendar_Class

// default destructor
Calendar_Class::~Calendar_Class()
{
} //~Calendar_Class

void	Calendar_Class::Init(void){
	ptrMemory=NULL;
	calendar_enable(ptrCalendar);
	
	InitReferenceDates();
	
	//memory.ReadAddress((uint8_t*)handler,CALENDER_ADDRESS,QSPI_BUFFER_SIZE);
	ReadLastDateTime();
	lastTimestamp=convert_datetime_to_timestamp(&lastDateTime);
	
	maxTimestamp=convert_datetime_to_timestamp(&maxDateTime);
	erasedTimestamp=convert_datetime_to_timestamp(&erasedDateTime);
	nullTimestamp=convert_datetime_to_timestamp(&nullDateTime);
	SetCompilationDateTime();
	currentTimestamp=convert_datetime_to_timestamp(&compilationDateTime);
	if ((lastTimestamp>maxTimestamp)||(lastTimestamp==erasedTimestamp)||(lastTimestamp==nullTimestamp))
	{
		SetDateTime(&compilationDateTime);
	}
	else
	{
		if (lastTimestamp<currentTimestamp)
		{
			SetDateTime(&compilationDateTime);
		}
		else
		{
			if (lastTimestamp==nullTimestamp)
			{
				SetDateTime(&compilationDateTime);
			} 
			else
			{
				SetDateTime(&lastDateTime);
			}
		
		}
	}
	enlapsedTime=false;
	SaveCurrentDateTime();
	
}


bool	Calendar_Class::Init(calendar_date_time dt_memory){

	calendar_enable(ptrCalendar);
	
	InitReferenceDates();
	lastDateTime=dt_memory;
	//memory.ReadAddress((uint8_t*)handler,CALENDER_ADDRESS,QSPI_BUFFER_SIZE);
	lastTimestamp=convert_datetime_to_timestamp(&lastDateTime);
	
	maxTimestamp=convert_datetime_to_timestamp(&maxDateTime);
	erasedTimestamp=convert_datetime_to_timestamp(&erasedDateTime);
	nullTimestamp=convert_datetime_to_timestamp(&nullDateTime);
	SetCompilationDateTime();
	compilationTimestamp=convert_datetime_to_timestamp(&compilationDateTime);
	if ((lastTimestamp>maxTimestamp)||(lastTimestamp==erasedTimestamp)||(lastTimestamp==nullTimestamp))
	{
		SetDateTime(&compilationDateTime);
	}
	else
	{
		if (lastTimestamp<compilationTimestamp)
		{
			SetDateTime(&compilationDateTime);
		}
		else
		{
			if (lastTimestamp==nullTimestamp)
			{
				SetDateTime(&compilationDateTime);
			}
			else
			{
				SetDateTime(&lastDateTime);
			}
			
		}
	}
	enlapsedTime=false;
	
	return true;
}
void	Calendar_Class::SetSavingAlarm(void){
	
	alarm.cal_alarm.datetime.time.sec = 1;
	alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm.cal_alarm.mode              = REPEAT;
	calendar_set_alarm(ptrCalendar, &alarm, alarm_calendar);
}

	
	
void	Calendar_Class::InitReferenceDates(void){
	

		maxDateTime.date.year  = 2050;
		maxDateTime.date.month = 12;
		maxDateTime.date.day   = 31;

		maxDateTime.time.hour = 12;
		maxDateTime.time.min  = 59;
		maxDateTime.time.sec  = 59;
		
		erasedDateTime.date.year  =0xffff;
		erasedDateTime.date.month = 0xff;
		erasedDateTime.date.day   = 0xff;

		erasedDateTime.time.hour = 0xff;
		erasedDateTime.time.min  = 0xff;
		erasedDateTime.time.sec  = 0xff;
		
		nullDateTime.date.year  = 0x0;
		nullDateTime.date.month = 0x0;
		nullDateTime.date.day   = 0x0;

		nullDateTime.time.hour = 0x0;
		nullDateTime.time.min  = 0x0;
		nullDateTime.time.sec  = 0x0;
	}
	
void	Calendar_Class::SetCompilationDateTime(void){
		

		compilationDateTime.date.year  =((((__DATE__ [7]-'0')*10+(__DATE__[8]-'0'))*10+(__DATE__ [9]-'0'))*10+(__DATE__ [10]-'0')) ;
		compilationDateTime.date.month = ((  __DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 0 : 5)   \
                                : __DATE__ [2] == 'b' ? 1                               \
                                : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ?  2 : 3)  \
                                : __DATE__ [2] == 'y' ? 4                               \
                                : __DATE__ [2] == 'l' ? 6                               \
                                : __DATE__ [2] == 'g' ? 7                               \
                                : __DATE__ [2] == 'p' ? 8                               \
                                : __DATE__ [2] == 't' ? 9                               \
                                : __DATE__ [2] == 'v' ? 10 : 11) +1);
		compilationDateTime.date.day   = ((__DATE__ [4]==' ' ? 0 : __DATE__  [4]-'0')*10+(__DATE__[5]-'0'));

		compilationDateTime.time.hour = (((__TIME__[0]-'0')*10) + (__TIME__[1]-'0'));
		compilationDateTime.time.min  = (((__TIME__[3]-'0')*10) + (__TIME__[4]-'0'));
		compilationDateTime.time.sec  = (((__TIME__[6]-'0')*10) + (__TIME__[7]-'0'));
		compiled_time=__TIME__;
	}
	

uint32_t Calendar_Class::SetTimeStamp(uint32_t ts){
	uint32_t result;
	result=convert_timestamp_to_datetime(ts,&dateTime);
	return (uint32_t)result;
}
uint32_t	Calendar_Class::GetDateTime(void){
	return (uint32_t)calendar_get_date_time(ptrCalendar,&dateTime);
}
uint32_t	Calendar_Class::GetDateTime(calendar_date_time *cdt){
	
	return (uint32_t)calendar_get_date_time(ptrCalendar,cdt);
}

bool Calendar_Class::leap_year(uint16_t year)
{
	if (year & 3) {
		return false;
	} else {
		return true;
	}
}

uint32_t	Calendar_Class::SetTime(calendar_time *p_calendar_time){
	return	(uint32_t)calendar_set_time(ptrCalendar,p_calendar_time);
}
uint32_t	Calendar_Class::SetDate(calendar_date *p_calendar_date){
	return	(uint32_t)calendar_set_date(ptrCalendar,p_calendar_date);
}


uint32_t	Calendar_Class::SetDateTime(calendar_date_time *cdt){
	calendar_date	cd=cdt->date;
	SetDate(&cd);
	//GetDateTime();
	calendar_time ct=cdt->time;
	return SetTime(&ct);
			
}


uint32_t Calendar_Class::convert_datetime_to_timestamp(struct calendar_date_time *dt)
{
	uint32_t tmp = 0;
	uint32_t i   = 0;
	uint8_t  year, month, day, hour, minutes, seconds;

	year    = dt->date.year - ptrCalendar->base_year;
	month   = dt->date.month;
	day     = dt->date.day;
	hour    = dt->time.hour;
	minutes = dt->time.min;
	seconds = dt->time.sec;

	/* tot up year field */
	for (i = 0; i < year; ++i) {
		if (leap_year(ptrCalendar->base_year + i)) {
			tmp += SECS_IN_LEAP_YEAR;
			} else {
			tmp += SECS_IN_NON_LEAP_YEAR;
		}
	}

	/* tot up month field */
	for (i = 1; i < month; ++i) {
		tmp += get_secs_in_month(dt->date.year, i);
	}

	/* tot up day/hour/minute/second fields */
	tmp += (day - 1) * SECS_IN_DAY;
	tmp += hour * SECS_IN_HOUR;
	tmp += minutes * SECS_IN_MINUTE;
	tmp += seconds;

	return tmp;
}

uint32_t Calendar_Class::get_secs_in_month(uint32_t year, uint8_t month)
{
	uint32_t sec_in_month = 0;

	if (leap_year(year)) {
		switch (month) {
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
			sec_in_month = SECS_IN_31DAYS;
			break;
			case 2:
			sec_in_month = SECS_IN_29DAYS;
			break;
			case 4:
			case 6:
			case 9:
			case 11:
			sec_in_month = SECS_IN_30DAYS;
			break;
			default:
			break;
		}
		} else {
		switch (month) {
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
			sec_in_month = SECS_IN_31DAYS;
			break;
			case 2:
			sec_in_month = SECS_IN_28DAYS;
			break;
			case 4:
			case 6:
			case 9:
			case 11:
			sec_in_month = SECS_IN_30DAYS;
			break;
			default:
			break;
		}
	}

	return sec_in_month;
}

uint32_t Calendar_Class::convert_timestamp_to_datetime( uint32_t ts,struct calendar_date_time *dt)
{
	uint32_t tmp, sec_in_year, sec_in_month;
	uint32_t tmp_year    = ptrCalendar->base_year;
	uint8_t  tmp_month   = 1;
	uint8_t  tmp_day     = 1;
	uint8_t  tmp_hour    = 0;
	uint8_t  tmp_minutes = 0;

	tmp = ts;

	/* Find year */
	while (true) {
		sec_in_year = leap_year(tmp_year) ? SECS_IN_LEAP_YEAR : SECS_IN_NON_LEAP_YEAR;

		if (tmp >= sec_in_year) {
			tmp -= sec_in_year;
			tmp_year++;
			} else {
			break;
		}
	}
	/* Find month of year */
	while (true) {
		sec_in_month = get_secs_in_month(tmp_year, tmp_month);

		if (tmp >= sec_in_month) {
			tmp -= sec_in_month;
			tmp_month++;
			} else {
			break;
		}
	}
	/* Find day of month */
	while (true) {
		if (tmp >= SECS_IN_DAY) {
			tmp -= SECS_IN_DAY;
			tmp_day++;
			} else {
			break;
		}
	}
	/* Find hour of day */
	while (true) {
		if (tmp >= SECS_IN_HOUR) {
			tmp -= SECS_IN_HOUR;
			tmp_hour++;
			} else {
			break;
		}
	}
	/* Find minute in hour */
	while (true) {
		if (tmp >= SECS_IN_MINUTE) {
			tmp -= SECS_IN_MINUTE;
			tmp_minutes++;
			} else {
			break;
		}
	}

	dt->date.year  = tmp_year;
	dt->date.month = tmp_month;
	dt->date.day   = tmp_day;
	dt->time.hour  = tmp_hour;
	dt->time.min   = tmp_minutes;
	dt->time.sec   = tmp;

	return ERR_NONE;
}

 Calendar_Class calendar(&DATETIME_CLOCK);