/* 
* PCS_I2C_Interface.h
*
* Created: 11/5/2020 5:15:56 PM
* Author: GMateusDP
*/


#ifndef __PCS_I2C_INTERFACE_H__
#define __PCS_I2C_INTERFACE_H__
#include "compiler.h"
#include "pscI2c.h"

class PSC_I2C_Interface
{
//variables
public:
	volatile bool isReady;
	uint8_t pscCommand;
protected:
private:
	PscI2c_t *psc;
	
//functions
public:
	PSC_I2C_Interface();
	~PSC_I2C_Interface();
	bool Init(void);
	bool GotData();
	uint8_t ReadCommand(void);
protected:
private:
	PSC_I2C_Interface( const PSC_I2C_Interface &c );
	PSC_I2C_Interface& operator=( const PSC_I2C_Interface &c );

}; //PCS_I2C_Interface

#endif //__PCS_I2C_INTERFACE_H__
