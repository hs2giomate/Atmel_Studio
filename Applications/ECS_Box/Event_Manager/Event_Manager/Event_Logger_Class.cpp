/* 
* Event_Logger_Class.cpp
*
* Created: 12/11/2020 10:50:52 AM
* Author: GMateusDP
*/


#include "Event_Logger_Class.h"
#include "CDC_Class.h"
#include "string.h"


static uint8_t text_buffer_local[64];
static uint8_t module_buffer_local[32];
static uint8_t number_buffer_local[8];	

static Calendar_Class	staticClock(&DATETIME_CLOCK);

uint8_t local_info_text[]="Info   :";

// default constructor
Event_Logger_Class::Event_Logger_Class()
{
	cal=&staticClock;
	logger_text=(char*)text_buffer_local;
	logger_module=(char*)module_buffer_local;
	logger_number=(char*)number_buffer_local;
	info_text=local_info_text;
	
} //Event_Logger_Class

// default destructor
Event_Logger_Class::~Event_Logger_Class()
{
} //~Event_Logger_Class

bool Event_Logger_Class::Init(void){
	MakeNullDateTime();
	cal->Init(nullDateTime);
	stackLocalArray=0;
	SetStackValue(stackLocalArray);
	memset(event_buffer_arrray,0,QSPI_ERBLK);
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
	
	cal->GetDateTime(&currentDateTime);
	
	memcpy(&entry.code,info_text,8);
	semiColonPosition=FindSemiColon(text);
	if (semiColonPosition>0)
	{
		if (semiColonPosition<8)
		{
				memcpy(&entry.code,text,(uint32_t)semiColonPosition);
		} 
		else
		{
				memcpy(&entry.code,text,8);
		}
	
	}
		

	

	
	text_length=(uint8_t)strlen((char*)text);
	if (text_length>DESCRIPTION_LENGHT_TEXT)
	{
		ChopStringDescription(text);
		memcpy(&entry.description,&text[semiColonPosition],DESCRIPTION_LENGHT_TEXT);
		text_length=DESCRIPTION_LENGHT_TEXT;
	} 
	else
	{
		memcpy(&entry.description,&text[semiColonPosition],(uint32_t)text_length);
		entry.description[text_length]=END__OF_ENTRY;
	}
	
	currentTimestamp= cal->convert_datetime_to_timestamp(&currentDateTime);
	if (currentTimestamp>0)
	{
		entry.timestamp=currentTimestamp;
	} 
	else
	{
		entry.timestamp=0xffffffff;
	}
	
	StackEventEntry(&entry,text_length+1);
	

}

void Event_Logger_Class::SaveEventIndexResult(char *text,uint8_t index, uint8_t result){
	task_result	=(TaskResult)result;

	itoa(index,logger_number,10);
	strcpy(logger_text,text);
	logger_module=strcat(logger_text,logger_number);
	switch (task_result)
	{
		case SUCCESS:
			logger_module=strcat(logger_module," SUCCESS");
			
		break;
		case FAILLED :
			logger_module=strcat(logger_module," FAILLED");
		
		break;
		default:
			logger_module=strcat(logger_module," UNKNOWN");
		/* Your code here */
		break;
	}
	SaveEvent(logger_module);
	
}

void Event_Logger_Class::SaveEvent(char const * text){
	
	SaveEvent((uint8_t*)text);
}

 Event_Logger_Class	logger;

