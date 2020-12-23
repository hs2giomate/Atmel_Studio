/* 
* Commands_Handler.h
*
* Created: 11/27/2020 3:56:46 PM
* Author: GMateusDP
*/


#ifndef __COMMANDS_HANDLER_H__
#define __COMMANDS_HANDLER_H__

#include "MaintenanceDataHandler.h"

class Commands_Handler: private virtual MaintenanceDataHandler
{
//variables
public:
protected:
private:
	SingleTaskMessage	singleTaskMessage;
	uint8_t		*usbMessageBuffer;
	FullBufferMessage message64;

//functions
public:
	Commands_Handler();
	~Commands_Handler();
	bool CommandSetEnableFans(void);
	bool	CommandSetPWMFans(void);
	void InitCommandHandler(uint8_t * buffer);
	bool CommandFansStatus(void);
	bool CommandReadFlapperData(void);
	bool  CommandSetFlapperPosition(void);
	bool  CommandSetFlapperValve(void);
protected:
private:
	Commands_Handler( const Commands_Handler &c );
	Commands_Handler& operator=( const Commands_Handler &c );
	uint8_t SetEnableCondesator(uint8_t data);
	uint8_t	SetEnableFan2(uint8_t data);
	uint8_t SetEnableFan1(uint8_t data);

}; //Commands_Handler

#endif //__COMMANDS_HANDLER_H__
