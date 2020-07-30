/* 
* DateTime_Class.cpp
*
* Created: 7/10/2020 4:33:55 PM
* Author: GMateusDP
*/


#include "DateTime_Class.h"
#include "FastStorage_Class.h"

// default constructor
DateTime_Class::DateTime_Class()
{
} //DateTime_Class

// default destructor
DateTime_Class::~DateTime_Class()
{
} //~DateTime_Class

void	DateTime_Class::Init(){
	uint32_t	add=(uint32_t)&eepromMap->stack;
	calendar.SetCalendarStackAddress(add);
	uint32_t	addTimeLogs=(uint32_t)&eepromMap->timeLogs[0];
	calendar.SetCalendarAddress(addTimeLogs);
	calendar.Init();
	
}

DateTime_Class	uhr;