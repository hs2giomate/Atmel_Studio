/* 
* Scavenge_Fan.h
*
* Created: 1/18/2021 11:29:15 AM
* Author: GMateusDP
*/


#ifndef __SCAVENGE_FAN_H__
#define __SCAVENGE_FAN_H__
#include "MCP23008_Class.h"
#define MCP23008_SCAVENGE_ADDRESS 0x24

struct Scavenge_Data
{
	bool	tempFault;
	bool	SPDFault;
	bool	relayFault;
	
};

class Scavenge_Fan
{
//variables
public:
		volatile bool scanvengechanged;
		bool enabled,isOK;
		Scavenge_Data  scavengeData;
protected:
private:
	MCP23008_Class* expander;
	I2C_Sync_Class*	i2c;
	bool bool_result;
	


//functions
public:
	Scavenge_Fan();
	~Scavenge_Fan();
	bool Init(void);
	uint8_t	ReadStatus(void);
	uint8_t	SetEnable(bool state);
	bool IsEnabled(void);
protected:
private:
	Scavenge_Fan( const Scavenge_Fan &c );
	Scavenge_Fan& operator=( const Scavenge_Fan &c );
	bool SelfTest();

}; //Scavenge_Fan
extern Scavenge_Fan scavenge;
#endif //__SCAVENGE_FAN_H__
