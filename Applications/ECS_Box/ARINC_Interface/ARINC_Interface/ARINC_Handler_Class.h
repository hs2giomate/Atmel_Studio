/* 
* ARINC_Handler_Class.h
*
* Created: 11/30/2020 11:24:32 AM
* Author: GMateusDP
*/


#ifndef __ARINC_HANDLER_CLASS_H__
#define __ARINC_HANDLER_CLASS_H__
#include "ARINC_Interface.h"


class ARINC_Handler_Class: private  ARINC_Interface
{
//variables
public:
	volatile bool isOK,r1GotMessage;
	ARINC_Interface		*ar;
protected:
private:
	
	

//functions
public:
	ARINC_Handler_Class();
	~ARINC_Handler_Class();
	bool	Init_ARINC(void);
	bool R1GotMessage(void);
	bool HandleCommunication(void);
	bool PollData(void);
protected:
private:
	ARINC_Handler_Class( const ARINC_Handler_Class &c );
	ARINC_Handler_Class& operator=(const ARINC_Handler_Class &c );

}; //ARINC_Handler_Class

extern ARINC_Handler_Class arinc;
#endif //__ARINC_HANDLER_CLASS_H__
