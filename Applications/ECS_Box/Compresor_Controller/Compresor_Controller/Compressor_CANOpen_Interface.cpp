/* 
* Compressor_Can_Interface.cpp
*
* Created: 12/16/2020 1:50:24 PM
* Author: GMateusDP
*/


#include "Compressor_CANOpen_Interface.h"
static	CANOpen_Class canoStatic(&CAN_CCU);
static bool syncCANOpenTimeoutStatic=false;

static void CANOpen_Sync_Timeout(const struct timer_task *const timer_task)
{
	syncCANOpenTimeoutStatic=true;
	asm("nop" );
}

// default constructor
Compressor_CANOpen_Interface::Compressor_CANOpen_Interface()
{
	syncCANOpenTimeout=&syncCANOpenTimeoutStatic;
	timeDifference_ms=CCU_PROCESS_TIME_DIFFERENCE;
} //Compressor_Can_Interface

// default destructor
Compressor_CANOpen_Interface::~Compressor_CANOpen_Interface()
{
} //~Compressor_Can_Interface

bool	Compressor_CANOpen_Interface::InitCANOpen(void){
	syncCANOpenTimeout=&syncCANOpenTimeoutStatic;
	timeDifference_ms=CCU_PROCESS_TIME_DIFFERENCE;
	cano=&canoStatic;
	int_result=cano->Init(NMT_MASTER_NODE);
	if (int_result==0)
	{
	//	cano->SetInitialMode();
		boolResult=	cano->StartNMTService(CCU_CANOPEN_NODE);
	} 
	else
	{
		
	}
	
	canoTimer.Start_oneShot_task(FUNC_PTR(CANOpen_Sync_Timeout),1000);
	
	return boolResult;
	
}

int	Compressor_CANOpen_Interface::Comporesor_Process(void){
	

		bool_t NMTisPreOrOperational = false;
		CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
// 
// 		if(CO->NMT->operatingState == CO_NMT_PRE_OPERATIONAL || CO->NMT->operatingState == CO_NMT_OPERATIONAL)
// 		NMTisPreOrOperational = true;

		reset = Process(1000);

	

		return (int)reset;
}

void Compressor_CANOpen_Interface::Periodic_Task(void){
	
	if (syncCANOpenTimeoutStatic)
	{
		Comporesor_Process();
		if (ptrCO->CANmodule[0]->CANnormal)
		{
				syncWas=CO_process_SYNC_RPDO(timeDifference_ms);
				CO_process_TPDO(syncWas,timeDifference_ms);
				CAN_Polling_Tx();
		} 
		else
		{
		}
			syncCANOpenTimeoutStatic=false;
			canoTimer.Start_oneShot_task(FUNC_PTR(CANOpen_Sync_Timeout),1000);
	} 
	else
	{
	}

}

