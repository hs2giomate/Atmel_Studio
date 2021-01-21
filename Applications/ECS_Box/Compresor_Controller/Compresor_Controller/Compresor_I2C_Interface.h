/* 
* Compresor_I2C_Interface.h
*
* Created: 12/16/2020 10:42:13 AM
* Author: GMateusDP
*/


#ifndef __COMPRESOR_I2C_INTERFACE_H__
#define __COMPRESOR_I2C_INTERFACE_H__

#define COMPRESOR_MCP23008_ADDRESS 0x25

#include "compiler.h"
#include "MCP23008_Class.h"
#include "SingleHeater_Class.h"
struct Compresor_Status_Inputs
{
	bool	niAlcCcuClutchFault;
	bool	nicAlcCcuRelayFault;
	bool	nicAlcCcuPowerFault;
	bool	niAlcCcuExtFault;
	
};

struct Compresor_Status_Outputs
{
	
	bool	niAlcCcuClutchEnable;
	bool	niAlcCcuRelayEnable;
	bool	niAlcCcuEnable;
	
};

struct Compresor_Status
{
	Compresor_Status_Inputs	inputs;
	Compresor_Status_Outputs outputs;
	
};
class Compresor_I2C_Interface
{
//variables
public:
	Compresor_Status compresor_Status;
	MCP23008_Class	*expander;
protected:
private:
	
	I2C_Sync_Class*	i2c;
	bool  isOK,enabled;

//functions
public:
	Compresor_I2C_Interface();
	~Compresor_I2C_Interface();
	
	bool	InitExpander(void);
		uint8_t	ReadStatus(void);
		uint8_t	SetEnable(bool state);
		uint8_t	SetRelay(bool state);
		uint8_t	SetClutch(bool state);
		bool IsEnabled(void);
		bool* IsEnabledPointer(void);
protected:
private:
	Compresor_I2C_Interface( const Compresor_I2C_Interface &c );
	Compresor_I2C_Interface& operator=( const Compresor_I2C_Interface &c );
	bool SelfTest(void);

}; //Compresor_I2C_Interface

#endif //__COMPRESOR_I2C_INTERFACE_H__
