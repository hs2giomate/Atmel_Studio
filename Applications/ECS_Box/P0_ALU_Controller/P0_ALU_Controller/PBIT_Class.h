/* 
* PBIT_Class.h
*
* Created: 6/22/2020 6:21:14 PM
* Author: GMateusDP
*/


#ifndef __PBIT_CLASS_H__
#define __PBIT_CLASS_H__
#include "ARINC_Interface.h"
#include "Protocol_Class.h"
#include "ConfigState_Class.h"

class PBIT_Class
{
//variables
public:
	volatile bool	isOK;
protected:
private:
	uint32_t label371;
	Datagram	datagram;
	uint32_t	datafield;
	
//functions
public:
	PBIT_Class();
	~PBIT_Class();
	uint8_t	GetStatus(void);
	bool ISPBITOK();
	uint32_t	CheckCurrentStatus(HVACStatus&);
	operator	bool();
protected:
private:
	PBIT_Class( const PBIT_Class &c );
	PBIT_Class& operator=( const PBIT_Class &c );

}; //PBIT_Class

static	PBIT_Class	pBit;
#endif //__PBIT_CLASS_H__
