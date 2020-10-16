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
#include "CBIT_Class.h"
#include "ConfigurationData.h"
#include "States_Class.h"
#include "Calendar_Class.h"
#include "Error_Labelling_Class.h"
#include "TaskHandler_Class.h"




class ALU_Class: public TaskHandler_Class
{
//variables
public:
	ECSConfiguration	controllerState;
	PBIT_Class		*ptrPbit;
	volatile bool arincTXTimeUP,isOK;
protected:
private:
	ARINC_Interface	*ptrARINC;
	ConfigurationData	configuration;
	HVACState	lastState;
	HVACStatus	status;
	UserParameters userParameters;
	uint8_t	activeLine;
	uint32_t callingTask,currentTask,taskEntryTime;
	ControllerTask	task;
	bool allTasksDone;
	uint32_t clk_rate;
	uint32_t timeout_period;
	
//functions
public:
	ALU_Class();
	~ALU_Class();
	uint8_t Run(void);
	uint32_t RunController(void);
	uint32_t Init(void);
	uint8_t	GetSelectedAMMC(void);
	int32_t	EnableWatchDog(void);
	int32_t	FeedWatchDog(void);
	bool	ValidateCyclus(void);
	uint32_t SetInitialConfiguration(ConfigurationData& cd);
	void NotifyError( Fault_List fl,const eventData& data=0);
	void NotifyError(Internal_Fault_List ifl,const eventData& data=0);
	void PrepareNewTask(ALUTaskEnum newTask, uint32_t data=0);
	void PrepareNewEvent( uint16 newState, uint16 data=0);

protected:
private:
	ALU_Class( const ALU_Class &c );
	ALU_Class& operator=( const ALU_Class &c );
	uint8_t	StartLivePulse(void);
	void HandleControllerEvent(event& e);
	bool	ExecutePendingTask(void);
	void HandleTasks(ControllerTask& ct);
	static void CheckPeriodicTask(void);
	void	RunPeriodicTasks(void);
	


}; //ALU_Class

extern	ALU_Class alu;

/*static	ALU_Class alu;*/
#endif //__ALU_CLASS_H__
