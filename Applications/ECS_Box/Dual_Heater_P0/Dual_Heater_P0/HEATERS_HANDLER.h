/* 
* HEATERS_HANDLER.h
*
* Created: 1/12/2021 10:30:23 AM
* Author: GMateusDP
*/


#ifndef __HEATERS_HANDLER_H__
#define __HEATERS_HANDLER_H__
#include "SingleHeater_Class.h"
#define NUMBER_OF_HEATERS	2

class HEATERS_HANDLER: private SingleHeater_Class
{
//variables
public:
	SingleHeater_Class *module[NUMBER_OF_HEATERS];
	bool volatile	 isOK;
protected:
private:
	

//functions
public:
	HEATERS_HANDLER();
	~HEATERS_HANDLER();
	bool Init(void);
	
protected:
private:
	HEATERS_HANDLER( const HEATERS_HANDLER &c );
	HEATERS_HANDLER& operator=( const HEATERS_HANDLER &c );

}; //HEATERS_HANDLER

extern HEATERS_HANDLER heaters;

#endif //__HEATERS_HANDLER_H__
