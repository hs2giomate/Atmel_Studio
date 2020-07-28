/* 
* DateTimeLog_Class.cpp
*
* Created: 7/1/2020 3:03:18 PM
* Author: GMateusDP
*/


#include "DateTimeLog_Class.h"
#include "math.h"
#include "time.h"
#include "AT24MAC_Class.h"


DateTimeLog_Class::DateTimeLog_Class()
{
} //~D
// default destructor
DateTimeLog_Class::~DateTimeLog_Class()
{
} //~DateTimeLog_Class

tick_t	DateTimeLog_Class::GetPowerInterrupTime(void){

	GetDateTime(&dateTime);
	currentTimestamp=convert_datetime_to_timestamp(&dateTime);
	ReadLastDateTime();
	lastTimestamp=convert_datetime_to_timestamp(&lastDateTime);
	timeDifference=difftime((time_t)currentTimestamp,(time_t)lastTimestamp);
	tick_t diferencia=(uint32_t)timeDifference;
	return	diferencia;
}

bool	DateTimeLog_Class::WasInterrpted(void){
	if (abs(GetPowerInterrupTime()-UPDATE_TIME_MAX)>UPDATE_ERROR)
	{
		powerInterrupt=true;
	} 
	else
	{
		powerInterrupt=false;
	}
	return	powerInterrupt;
}

uint32_t	DateTimeLog_Class::IncrementMemoryStack(){
	
	uint32_t r=	eeprom.ReadAddress((uint8_t*)&stackValue,stackAdress,1);
	stackValue=(uint16_t)RollStack();
	r=	eeprom.WriteAddress((uint8_t*)&stackValue,stackAdress,1);
	return r;
}
 uint8_t DateTimeLog_Class::RollStack(void)
 {
	 return (uint8_t)(stackValue +1 ) % AT24MAC_BUFFER_SIZE;
 }
 
 uint8_t	DateTimeLog_Class::ReadStackValue(void){
	 uint8_t s;
	eeprom.ReadAddress(&s,stackAdress,1);
	 return	s;
 }
 
 uint16_t	DateTimeLog_Class::SetCalendarStackAddress(uint32_t add){
	 stackAdress=(uint16_t)(add&0xffff);
	 return	stackAdress;
 }
uint16_t	DateTimeLog_Class::SetCalendarAddress(uint32_t add){
	dataTimeLogsAddress=(uint16_t)(add&0xffff);
	return	dataTimeLogsAddress;
}
uint32_t	DateTimeLog_Class::SaveCurrentDateTime(void){
	stackValue=ReadStackValue();
	GetDateTime(&dateTime);
	handler=(PTR_DATE_TIME)(&dateTime);
	uint32_t	w= eeprom.WriteAddress((uint8_t*)handler,(uint8_t)(dataTimeLogsAddress+stackValue),(uint8_t)sizeof(lastDateTime));
	IncrementMemoryStack();
	return	w;
}

uint32_t	DateTimeLog_Class::ReadLastDateTime(void){
	stackValue=ReadStackValue();
	handler=(PTR_DATE_TIME)(&lastDateTime);
	uint32_t r=	eeprom.ReadAddress((uint8_t *)handler,(uint8_t)(dataTimeLogsAddress+stackValue),(uint8_t)sizeof(lastDateTime));
	return	r;
}
uint32_t	DateTimeLog_Class::ReadDateTime(calendar_date_time *dt){
	stackValue=ReadStackValue();
	handler=(PTR_DATE_TIME)(dt);
	uint32_t r=	eeprom.ReadAddress((uint8_t *)handler,(uint8_t)(dataTimeLogsAddress+stackValue),(uint8_t)sizeof(lastDateTime));
	return	r;
}