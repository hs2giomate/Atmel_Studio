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
	uint8_t		checkResult,i;
		
	
//functions
public:
	Interfaces_Class();
	~Interfaces_Class();
	uint32_t	CheckCommunication(void);
	uint32_t	GetStatus(HVACStatus&);
	bool	CheckI2CExpander(uint8_t add);
	bool	checkLTC2983(void);
	bool CheckUSBInterface(void);
	bool	CheckArincInterface(void);
protected:
private:
	Interfaces_Class( const Interfaces_Class &c );
	Interfaces_Class& operator=( const Interfaces_Class &c );

}; //Interfaces_Class
extern	Interfaces_Class	interfaces;
#endif //__INTERFACES_CLASS_H__
