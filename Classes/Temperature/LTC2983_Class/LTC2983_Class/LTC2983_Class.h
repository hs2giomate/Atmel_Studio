/* 
* LTC2983_Class.h
*
* Created: 4/23/2020 9:06:37 AM
* Author: GMateusDP
*/


#ifndef __LTC2983_CLASS_H__
#define __LTC2983_CLASS_H__
#include "SPI_Syn_Class.h"

class LTC2983_Class
{
//variables
public:
	static	SPI_Syn_Class	spiLTC;
protected:
private:

//functions
public:
	LTC2983_Class();
	~LTC2983_Class();
protected:
private:
	LTC2983_Class( const LTC2983_Class &c );
	LTC2983_Class& operator=( const LTC2983_Class &c );

}; //LTC2983_Class

#endif //__LTC2983_CLASS_H__
