/* 
* Compressor_Can_Interface.cpp
*
* Created: 12/16/2020 1:50:24 PM
* Author: GMateusDP
*/

#include "Compresor_Controller.h"
#include "Compressor_CANOpen_Interface.h"
static	CANOpen_Class canoStatic(&CAN_CCU);
static	uint8_t local_tx_data[8];
static bool syncCANOpenTimeoutStatic=false;
static uint16_t local_speed;

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
	tx_can_buffer=local_tx_data;
	speed=&local_speed;
	syncCANOpenTimeout=&syncCANOpenTimeoutStatic;
	timeDifference_ms=CCU_PROCESS_TIME_DIFFERENCE;
	cano=&canoStatic;
	int_result=cano->Init_CO_CLASS(NMT_MASTER_NODE);
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

		reset =cano->Process(1000);

	

		return (int)reset;
}

void Compressor_CANOpen_Interface::Periodic_Task(void){
	
	if (syncCANOpenTimeoutStatic)
	{
		//Comporesor_Process();
		if ( cano->ptrCO->CANmodule[0]->CANnormal)
		{
				syncWas=cano->Send_SYNC_Signal();
				if (syncWas==0)
				{
					//cano->Pass_Temperature_To_voltage();
					memcpy((void*)&current_speed,(void*)CO_OD_RAM.voltage,2);
				//	current_speed=(uint16_t)(CO_OD_RAM.voltage)
				} 
				else
				{
					memcpy((void*)&current_speed,(void*)CO_OD_RAM.voltage,2);
				}
				is_enabled=ccu.IsEnabled();
				if ((is_enabled))
				{
					Convert_Uint16_Array(*speed,tx_can_buffer);
					
				}else{
					
					Convert_Uint16_Array(*speed,tx_can_buffer);
				}
				cano->Send_Simple_Run_Command(tx_can_buffer,CCU_CANOPEN_NODE);
			//	cano->CO_process_TPDO(syncWas,timeDifference_ms);
			//	cano->CAN_Polling_Tx();
		} 
		else
		{
		}
			syncCANOpenTimeoutStatic=false;
			canoTimer.Start_oneShot_task(FUNC_PTR(CANOpen_Sync_Timeout),100);
	} 
	else
	{
	}

}

void	Compressor_CANOpen_Interface::Convert_Uint16_Array(uint16_t sp, uint8_t * arr){
	for (uint8_t i = 0; i < 8; i++)
	{
		if (i<2)
		{
			local_tx_data[i]=(uint8_t)(((*speed)>>8*i)&0x00ff);
		}else{
			local_tx_data[i]=0;
		}
		
	}
	arr=local_tx_data;
}

bool Compressor_CANOpen_Interface::Set_CAN_Enable(bool *st){
	enable=st;
	return  *enable;
}

void	Compressor_CANOpen_Interface::Set_Motor_Speed(uint16_t sp){
	if (sp>7000)
	{
		*speed=7000;
	} 
	else
	{
		*speed=sp;
	}
	
	
}

