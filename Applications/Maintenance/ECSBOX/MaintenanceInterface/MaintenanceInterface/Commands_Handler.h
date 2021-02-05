/* 
* Commands_Handler.h
*
* Created: 11/27/2020 3:56:46 PM
* Author: GMateusDP
*/


#ifndef __COMMANDS_HANDLER_H__
#define __COMMANDS_HANDLER_H__

#define HEATER_DATA_SIZE	4
#define COMPRESSOR_DATA_SIZE	14
#define EVENT_LOGGER_MASK 0xaaaa0000
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
	uint8_t	data_fans_array[3];
	uint8_t	heater_data_array[HEATER_DATA_SIZE];
	uint8_t	*compressor_data_array;
	UserParameters	parameters;
	bool is_stand_alone;
	uint8_t	 lastEnableHeaters,enableHeaters;
	bool powerOn;
	uint8_t data_byte;
	uint16_t compressor_speed;
	uint32_t read_result,write_result;
	uint32_t memory_flash_address;
	uint32_t ucontroller_timestamp;
	uint8_t logger_buffer[8];
	DataLogMessage  dataLogMessage;
	uint8_t *memory_block;
	uint32_t *last_logger_array_stack;
	uint32_t current_logger_stack_value;
	uint32_t logger_data_buffer_temp_size;
	
	

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
	bool CommandReadParameters(void);
	bool CommandWriteParameters(void);
	bool	CommandSetHeaters(void);
	bool	CommandReadHeaterStatus();
	bool CommandSetScavenge(void);
	bool CommandReadScavengeStatus(void);
	bool CommandSetCompressor(void);
	bool CommandReadCompressorStatus(void);
	bool CommandReadDataLogger(void);
protected:
private:
	Commands_Handler( const Commands_Handler &c );
	Commands_Handler& operator=( const Commands_Handler &c );
	uint8_t SetEnableCondesator(uint8_t data);
	uint8_t	SetEnableFan2(uint8_t data);
	uint8_t SetEnableFan1(uint8_t data);

}; //Commands_Handler

#endif //__COMMANDS_HANDLER_H__
