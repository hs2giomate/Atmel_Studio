/* 
* SingleHeater_Class.h
*
* Created: 22.10.2020 14:48:15
* Author: technik
*/


#ifndef __SINGLEHEATER_CLASS_H__
#define __SINGLEHEATER_CLASS_H__
#include "MCP23008_Class.h"
#define SINGLE_HEATER_EXPANDERS 2

struct IC29_Inputs
{
	bool	niAlcHeaterOverTemp;
	bool	niAlcHeaterRelayFault[4];
};

struct IC30_Outputs
{
	bool	niALCHeaterRelay[4];
};

struct SingleHeaterGPIO
{
	IC29_Inputs	inputs;
	IC30_Outputs outputs;
};

class SingleHeater_Class
{
//variables
public:
	volatile bool isOK,heaterStatusChanged;
	SingleHeaterGPIO heaterGPIO;
	uint8_t enabled;
protected:
private:
	MCP23008_Class* expanders[SINGLE_HEATER_EXPANDERS];
	MCP23008_Class* currentExpander;
	I2C_Sync_Class*	i2c;
	bool	enables[4];
	uint8_t	powerLevel,heater_id;
	uint8_t	gpio_array[SINGLE_HEATER_EXPANDERS];
	
//functions
public:
	SingleHeater_Class();
	SingleHeater_Class(I2C_Sync_Class *i2c);
	~SingleHeater_Class();
	bool Init(void);
	bool Init(uint8_t id,MCP23008_Class*);
	uint8_t	ReadStatus(void);
	uint8_t	EnableIndex(uint8_t indexHeater);
	uint8_t	DisableIndex(uint8_t indexHeater);
	uint8_t	SetRelay(uint8_t indexHeater, bool state);
	uint8_t	ReadEnableGIPO(void);
	uint8_t GetHeaterPowerLevel(void);
	void DisableAll(void);
	void	ReadGPIOs(uint8_t *buffer);
		
protected:
private:
	SingleHeater_Class( const SingleHeater_Class &c );
	SingleHeater_Class& operator=( const SingleHeater_Class &c );
	uint8_t InitExpanderArray(void);
	uint8_t InitExpanderArray(MCP23008_Class *heater_expanders);
	bool SelfTest(void);
}; //SingleHeater_Class

extern SingleHeater_Class heater;

#endif //__SINGLEHEATER_CLASS_H__
