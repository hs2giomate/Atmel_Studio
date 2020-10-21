/* 
* HVAC_Commando_Class.h
*
* Created: 10/20/2020 12:00:05 PM
* Author: GMateusDP
*/


#ifndef __HVAC_COMMANDO_CLASS_H__
#define __HVAC_COMMANDO_CLASS_H__
#include "ConfigState_Class.h"

class HVAC_Commando_Class: private virtual ConfigState_Class
{
//variables
public:
	AcknoledgeStatus commando270;
protected:
private:
	
	uint32_t actualCommando, lastCommando;
	uint8_t	flapperValveSetpoint,lastflapperValveSetpoint;

//functions
public:
	HVAC_Commando_Class();
	~HVAC_Commando_Class();
	// virtual ~HVAC_Commando_Class();
	 bool IsCommando270Changed(void);
	 uint32_t GetCommando270(void);
	 bool IsNewFlapperValveSetpoint(void);
	 uint8_t GetFlapperValveSetpoint(void);
	 
protected:
private:
	HVAC_Commando_Class( const HVAC_Commando_Class &c );
	HVAC_Commando_Class& operator=( const HVAC_Commando_Class &c );
	 uint32_t UpdateCommando(void);
	 uint8_t UpadateFlapperValveSetpoint(void);

}; //HVAC_Commando_Class

#endif //__HVAC_COMMANDO_CLASS_H__
