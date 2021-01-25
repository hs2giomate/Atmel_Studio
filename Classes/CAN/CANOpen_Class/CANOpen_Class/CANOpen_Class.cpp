/* 
* CANOpen_Class.cpp
*
* Created: 8/5/2020 10:24:01 AM
* Author: GMateusDP
*/


#include "CANOpen_Class.h"
#include "CO_Class.h"

CANOpen_Class	*ptrCANOpenClass;

// default constructor
CANOpen_Class::CANOpen_Class()
{
	ptrCANOpenClass=this;
} //CANOpen_Class

CANOpen_Class::CANOpen_Class(can_async_descriptor  *descr)
{
	ptrCANOpenClass=this;
	canDescr=descr;
} //CANOpen_Class

// default destructor
CANOpen_Class::~CANOpen_Class()
{
} //~CANOpen_Class

int	CANOpen_Class::Init_CO_CLASS(uint8_t node){
	coNode=node;
	if (node==1)
	{
			//RedefineMacros();
	}
	//can.SetDescriptor(canDescr);
	err=CO_init(canDescr,node,1000);
	
	if (err==0)
	{
		canopen_driver->canopen_ready=true;
		err=(CO_ReturnError_t)SetInitialMode();
		if (node==1)
		{
			CO_sendNMTcommand(1,0);
		}
			

		
	}else{
		return	(int)err;
	}
	return	(int)err;
}

int	CANOpen_Class::SetInitialMode(void){
	/* CAN module address, NodeID, Bitrate */
	/* We do not use CAN registers directly, so address here is a pointer to the CAN_HandleTypeDef object. */
	
	
	/* start CAN */
	err=CO_CANsetNormalMode();
	//err=CO_CANsetNormalMode(CO->CANmodule[0]);

	reset = CO_RESET_NOT;
	return	(int)err;
}

int	CANOpen_Class::Periodic_Process(uint32_t timeDifference){
	if (ptrCO->CANmodule[0]->CANnormal)
	{
		syncWas=CO_process_SYNC_RPDO(timeDifference);
		CO_process_TPDO(syncWas,timeDifference);
		CAN_Polling_Tx();
		
	}
	
	return	int(syncWas);
}


bool	CANOpen_Class::StartNMTService(void){
	bool operational=false;
//	while ((!operational))
//	{
		CO_sendNMTcommand(1,2);
		delay_ms(100);
		operational=(ptrCO->HBcons->monitoredNodes[2].NMTstate==5);
//0	}
	return	operational;
}

bool	CANOpen_Class::StartNMTService(uint8_t nodeId){
	bool localOperational=false;
// 	while ((!localOperational))
// 	{
		CO_sendNMTcommand(1,nodeId);
		delay_ms(100);
		localOperational=(ptrCO->HBcons->monitoredNodes[nodeId].NMTstate==5);
		slaveIsOperational=localOperational;
//	}
	return	localOperational;
}
void	CANOpen_Class::RedefineMacros(void){
			#undef CO_NO_NMT_MASTER               
			#undef CCS_DOWNLOAD_INITIATE           
			#undef CCS_DOWNLOAD_SEGMENT            
			#undef CCS_UPLOAD_INITIATE             
			#undef CCS_UPLOAD_SEGMENT              
			#undef CCS_DOWNLOAD_BLOCK              
			#undef CCS_UPLOAD_BLOCK                
			#undef CCS_ABORT
		    
			
			
			
			#define CO_NO_NMT_MASTER                1
			#define CCS_DOWNLOAD_INITIATE           1
			#define CCS_DOWNLOAD_SEGMENT            0
			#define CCS_UPLOAD_INITIATE             2
			#define CCS_UPLOAD_SEGMENT              3
			#define CCS_DOWNLOAD_BLOCK              6
			#define CCS_UPLOAD_BLOCK                5
			#define CCS_ABORT                       4 
                    
}