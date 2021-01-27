/* 
* Event_Logger_Struct.cpp
*
* Created: 12/14/2020 12:33:31 PM
* Author: GMateusDP
*/


#include "Event_Logger_Struct.h"
#include "string.h"

// default constructor
Event_Logger_Struct::Event_Logger_Struct()
{
} //Event_Logger_Struct

// default destructor
Event_Logger_Struct::~Event_Logger_Struct()
{
} //~Event_Logger_Struct

void	Event_Logger_Struct::ChopStringDescription(uint8_t* str){
	//RemoveSpaces((char *)str);
	sizeText=strlen((char *)str);
	if (sizeText>44)
	{
		str[44]=END__OF_ENTRY;
	} 
	else
	{
		
	}
}

void Event_Logger_Struct::RemoveSpaces(char *str)
{
	// To keep track of non-space character count
	int count = 0;
	
	// Traverse the given string. If current character
	// is not space, then place it at index 'count++'
	for (int i = 0; str[i]; i++){
		if (str[i] != ' '){
			str[count++] = str[i+1]; // here count is
		}
	}
	

	// incremented
	str[count] = '\0';
}

uint8_t Event_Logger_Struct::FindSemiColon(uint8_t *str)
{
	
	// Traverse the given string. If current character
	// is not space, then place it at index 'count++'
	for (uint8_t i = 0; str[i]; i++){
		if (str[i] == ':'){
			return i;
		}
	}
	
	return 0;
}
