/* 
* DateTimeLog_Class.cpp
*
* Created: 7/1/2020 3:03:18 PM
* Author: GMateusDP
*/


#include "DateTime_Logger.h"
#include "math.h"
#include "time.h"
//#include "AT24MAC_Class.h"
#include "Calendar_Class.h"

DateTime_Logger::DateTime_Logger()
{
} //~D
// default destructor
DateTime_Logger::~DateTime_Logger()
{
} //~DateTimeLog_Class


uint32_t	DateTime_Logger::ReadLastDateTime(calendar_date_time *dt){
// 	stackValue=ReadStackValue();
// 	handler=(PTR_DATE_TIME)(&lastDateTime);
// 	uint32_t r=memory	ReadAddress((uint8_t *)handler,(uint8_t)(dataTimeLogsAddress+stackValue),(uint8_t)sizeof(lastDateTime));
 	return	0;
}
uint32_t	DateTime_Logger::ReadDateTime(calendar_date_time *dt){
	stackValue=ReadStackValue();
	handler=(PTR_DATE_TIME)(dt);
//	uint32_t r=	eeprom.ReadAddress((uint8_t *)handler,(uint8_t)(dataTimeLogsAddress+stackValue),(uint8_t)sizeof(lastDateTime));
	return	0;
}