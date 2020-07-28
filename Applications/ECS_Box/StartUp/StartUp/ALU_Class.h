/* 
* ALU_Class.h
*
* Created: 6/19/2020 4:21:20 PM
* Author: GMateusDP
*/


#ifndef __ALU_CLASS_H__
#define __ALU_CLASS_H__
#include "driver_init.h"
#include "ARINC_Interface.h"
#include "PBIT_Class.h"
#include "ConfigurationData.h"
#include "States_Class.h"
#include "Calendar_Class.h"
#include "Error_Labelling_Class.h"

class ALU_Class
{
//variables
public:
	ECSConfiguration	controllerState;
	PBIT_Class		*ptrPbit;
protected:
private:
	ARINC_Interface	*ptrARINC;
	ConfigurationData	configuration;
	HVACState	lastState;
	HVACStatus	status;
	uint8_t	activeLine;
	
	
//functions
public:
	ALU_Class();
	~ALU_Class();
	uint8_t Run(void);
	uint32_t Init(void);
	uint8_t	GetSelectedAMMC(void);
	int32_t	EnableWatchDog(uint32_t clk_rate  = 1000,uint32_t timeout_period = 4096);
	int32_t	FeedWatchDog(void);
	bool	ValidateCyclus(void);
	uint32_t SetInitialConfiguration(ConfigurationData& cd);
protected:
private:
	ALU_Class( const ALU_Class &c );
	ALU_Class& operator=( const ALU_Class &c );
	uint8_t	StartLivePulse(void);
	void NotifyError( Fault_List fl,const eventData& data=0);
	void NotifyError(Internal_Fault_List ifl,const eventData& data=0);

}; //ALU_Class

extern	ALU_Class alu;

#endif //__ALU_CLASS_H__
