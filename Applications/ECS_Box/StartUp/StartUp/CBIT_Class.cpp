/* 
* CBIT_Class.cpp
*
* Created: 7/8/2020 10:31:45 AM
* Author: GMateusDP
*/


#include "CBIT_Class.h"

// default constructor
CBIT_Class::CBIT_Class()
{
} //CBIT_Class

// default destructor
CBIT_Class::~CBIT_Class()
{
} //~CBIT_Class

bool	CBIT_Class::IsOK(void){
	return	isOK;
}
CBIT_Class::operator bool(){
	return	isOK;
}

CBIT_Class	cBit;