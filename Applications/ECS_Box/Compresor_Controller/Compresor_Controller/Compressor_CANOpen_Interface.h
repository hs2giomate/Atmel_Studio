/* 
* Compressor_Can_Interface.h
*
* Created: 12/16/2020 1:50:25 PM
* Author: GMateusDP
*/


#ifndef __COMPRESSOR_CAN_INTERFACE_H__
#define __COMPRESSOR_CAN_INTERFACE_H__
#include "compiler.h"
#include "CANOpen_Class.h"
#include "Timer_Class.h"
#define NMT_MASTER_NODE	1
#define CCU_CANOPEN_NODE 2
#define CCU_PROCESS_TIME_DIFFERENCE 1

class Compressor_CANOpen_Interface: public CANOpen_Class
{
//variables
public:
	volatile	bool *syncCANOpenTimeout;
protected:
private:
	CANOpen_Class	*cano;
	bool *enable;
	bool boolResult,is_enabled;
	int int_result,syncWas;
	uint32_t timeDifference_ms;
	
	uint16_t	*speed;
	uint8_t		*tx_can_buffer;
//functions
public:
	Compressor_CANOpen_Interface();
	~Compressor_CANOpen_Interface();
	
	bool	InitCANOpen(void);
	int	Comporesor_Process(void);
	void	Periodic_Task(void);
	bool	Set_CAN_Enable(bool *st);
	void	Set_Motor_Speed(uint16_t sp);
protected:
private:
	Compressor_CANOpen_Interface( const Compressor_CANOpen_Interface &c );
	Compressor_CANOpen_Interface& operator=( const Compressor_CANOpen_Interface &c );
	void	Convert_Uint16_Array(uint16_t sp, uint8_t * arr);

}; //Compressor_Can_Interface


#endif //__COMPRESSOR_CAN_INTERFACE_H__
