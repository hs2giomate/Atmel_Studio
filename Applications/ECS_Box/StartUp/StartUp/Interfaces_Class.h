/* 
* Interfaces_Class.h
*
* Created: 7/6/2020 9:05:06 AM
* Author: GMateusDP
*/


#ifndef __INTERFACES_CLASS_H__
#define __INTERFACES_CLASS_H__
#include "coreTypes.h"
#include "EventHandler_Class.h"
#include "ConfigState_Class.h"

class Interfaces_Class
{
//variables
public:
protected:
private:
	HVACState	state;
	HVACStatus	status;
	
//functions
public:
	Interfaces_Class();
	~Interfaces_Class();
	uint32_t	CheckCommunication(void);
	uint32_t	GetStatus(HVACStatus&);
protected:
private:
	Interfaces_Class( const Interfaces_Class &c );
	Interfaces_Class& operator=( const Interfaces_Class &c );

}; //Interfaces_Class
extern	Interfaces_Class	communicator;
#endif //__INTERFACES_CLASS_H__
