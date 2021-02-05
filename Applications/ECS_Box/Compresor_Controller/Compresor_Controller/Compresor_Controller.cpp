/* 
* Compresor_Controller.cpp
*
* Created: 12/16/2020 12:17:32 PM
* Author: GMateusDP
*/


#include "Compresor_Controller.h"
#include "CO_Driver_Class.h"
static uint8_t local_data_array[COMPRESSOR_CONTROLLER_DATA_SIZE];

// default constructor
Compresor_Controller::Compresor_Controller()
{
} //Compresor_Controller

// default destructor
Compresor_Controller::~Compresor_Controller()
{
} //~Compresor_Controller

bool Compresor_Controller::Init(void){
	data_array=local_data_array;
	isOK=InitExpander();
	if (isOK)
	{
		if (InitCANOpen())
		{
			canopen_driver->canopen_ready=true;
			Set_CAN_Enable(IsEnabledPointer());
			Set_Slave_ID(CCU_CANOPEN_NODE);
			isOK=true;
		} 
		else
		{
		}
	} 
	else
	{
	}
	return isOK;
}
uint8_t Compresor_Controller::Set_Slave_ID (uint8_t sn){
	return	canopen_driver->Set_Slave_Node(sn);
}
void Compresor_Controller::GetCompressorStatus(uint8_t *arr){
	data_array[0]=ReadStatus();
	current_speed=GetCurrentSpeed();
	memcpy(&data_array[1],&current_speed,2);
	data_array[3]=ReadPressureLimits();
	memcpy(&data_array[4],(void*)canopen_driver->error_flags_array,2);
	memcpy(&data_array[6],(void*)&canopen_driver->current_IQFilt,2);
	memcpy(&data_array[8],(void*)&canopen_driver->current_IDFilt,2);
	memcpy(&data_array[10],(void*)canopen_driver->compressor_temperatures,6);
	memcpy(arr,data_array,16);
}
Compresor_Controller ccu;