/* 
* Event_Logger_Class.cpp
*
* Created: 12/11/2020 10:50:52 AM
* Author: GMateusDP
*/


#include "Event_Logger_Class.h"

	
static Calendar_Class	staticClock(&DATETIME_CLOCK);


// default constructor
Event_Logger_Class::Event_Logger_Class()
{
	cal=&staticClock;
} //Event_Logger_Class

// default destructor
Event_Logger_Class::~Event_Logger_Class()
{
} //~Event_Logger_Class

bool Event_Logger_Class::Init(void){
	MakeNullDateTime();
	cal->Init(&nullDateTime);
	stackLocalArray=0;
	SetStackValue(stackLocalArray);
}

void Event_Logger_Class::MakeNullDateTime(void){
	nullDateTime.date.year  = 0x0;
	nullDateTime.date.month = 0x0;
	nullDateTime.date.day   = 0x0;

	nullDateTime.time.hour = 0x0;
	nullDateTime.time.min  = 0x0;
	nullDateTime.time.sec  = 0x0;
}

void Event_Logger_Class::SaveEvent(uint8_t* text){
	
	GetDateTime(&currentDateTime);
	semiColonPosition=FindSemiColon(text);
	memcpy(&entry.code,text,(uint32_t)semiColonPosition);

	ChopStringDescription(text);
	memcpy(&entry.description,&text[semiColonPosition],44);
	currentTimestamp=convert_datetime_to_timestamp(&currentDateTime);
	entry.timestamp=currentTimestamp;
	StackEventEntry(&entry);
	
	if (usb.IsEnabled())
	{
	
		usb.println(text);
		
	} 
	else
	{
	}
}

 Event_Logger_Class	logger;

