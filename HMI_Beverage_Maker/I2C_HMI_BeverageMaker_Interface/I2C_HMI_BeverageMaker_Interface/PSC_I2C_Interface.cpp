/* 
* PCS_I2C_Interface.cpp
*
* Created: 11/5/2020 5:15:55 PM
* Author: GMateusDP
*/


#include "PSC_I2C_Interface.h"

// default constructor
PSC_I2C_Interface::PSC_I2C_Interface()
{
} //PCS_I2C_Interface

// default destructor
PSC_I2C_Interface::~PSC_I2C_Interface()
{
} //~PCS_I2C_Interface

bool PSC_I2C_Interface::Init(void){
	psc=&gPscI2c;
	return isReady;
}

bool PSC_I2C_Interface::GotData(){
		/* check if a command has been sent from the PSC */
	isReady=pscI2cIsCommandPendingProcessing();
	return isReady;
}

uint8_t PSC_I2C_Interface::ReadCommand(void){
	isReady=false;
	pscI2cReadCommandData((uint8_t*)&pscCommand,1);
	return pscCommand;
	
}




