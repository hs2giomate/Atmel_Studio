/* 
* DateTime_Class.h
*
* Created: 7/10/2020 4:33:56 PM
* Author: GMateusDP
*/


#ifndef __DATETIME_CLASS_H__
#define __DATETIME_CLASS_H__
#include "FastStorage_Class.h"
#include "Calendar_Class.h"

class DateTime_Class: public FastStorage_Class, public Calendar_Class
{
//variables
public:
protected:
private:

//functions
public:
	DateTime_Class();
	~DateTime_Class();
	void Init();
protected:
private:
	DateTime_Class( const DateTime_Class &c );
	DateTime_Class& operator=( const DateTime_Class &c );

}; //DateTime_Class
extern	DateTime_Class	uhr;
#endif //__DATETIME_CLASS_H__
