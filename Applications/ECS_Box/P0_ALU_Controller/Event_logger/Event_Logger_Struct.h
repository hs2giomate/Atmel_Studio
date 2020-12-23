/* 
* Event_Logger_Struct.h
*
* Created: 12/14/2020 12:33:31 PM
* Author: GMateusDP
*/


#ifndef __EVENT_LOGGER_STRUCT_H__
#define __EVENT_LOGGER_STRUCT_H__

struct __attribute__((__packed__))  Event_Log_Entry
{
	uint32_t	timestamp;
	uint8_t		code[16];
	char		description[44];
};



class Event_Logger_Struct
{
//variables
public:
	
protected:
private:
	uint8_t	sizeText;

//functions
public:
	Event_Logger_Struct();
	~Event_Logger_Struct();
	void	ChopStringDescription(uint8_t* str);
	void	RemoveSpaces(char *str);
	uint8_t	FindSemiColon(uint8_t *str);
protected:
private:
	Event_Logger_Struct( const Event_Logger_Struct &c );
	Event_Logger_Struct& operator=( const Event_Logger_Struct &c );

}; //Event_Logger_Struct

#endif //__EVENT_LOGGER_STRUCT_H__
