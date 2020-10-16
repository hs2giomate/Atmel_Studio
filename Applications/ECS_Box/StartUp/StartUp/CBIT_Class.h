/* 
* CBIT_Class.h
*
* Created: 7/8/2020 10:31:45 AM
* Author: GMateusDP
*/


#ifndef __CBIT_CLASS_H__
#define __CBIT_CLASS_H__
#include "ConfigState_Class.h"

class CBIT_Class
{
//variables
public:
	volatile	bool	isOK;
	
protected:
private:
	
	StatusBits	statusBits;

//functions
public:
	CBIT_Class();
	~CBIT_Class();
	bool	IsOK(void);
	operator	bool();
protected:
private:
	CBIT_Class( const CBIT_Class &c );
	CBIT_Class& operator=( const CBIT_Class &c );

}; //CBIT_Class

static	CBIT_Class	cBit;

#endif //__CBIT_CLASS_H__
