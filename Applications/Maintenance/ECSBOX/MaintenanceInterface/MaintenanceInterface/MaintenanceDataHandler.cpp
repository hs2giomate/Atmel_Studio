/* 
* MaintenanceDataHandler.cpp
*
* Created: 11/17/2020 10:50:07 AM
* Author: GMateusDP
*/


#include "MaintenanceDataHandler.h"
#include "string.h"

// default constructor
MaintenanceDataHandler::MaintenanceDataHandler()
{
	sizeFullBufferMessage=FULL_MESSAGE_SIZE;
} //MaintenanceDataHandler

// default destructor
MaintenanceDataHandler::~MaintenanceDataHandler()
{
} //~MaintenanceDataHandler

uint8_t MaintenanceDataHandler::CreateFullBufferMessage(uint8_t *localBuff,  uint8_t *data){
	
	memcpy((void*)fullBufferMessage.content,(void*)data,sizeFullBufferMessage);
	memcpy((void*)localBuff,(void *)&fullBufferMessage,MAINTENANCE_TOOL_BUFFER_SIZE);
	return 0;
}
