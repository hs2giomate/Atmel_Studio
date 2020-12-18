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
	
protected:
private:
	CANOpen_Class	*cano;
	bool boolResult;
	int int_result,syncWas;
	uint32_t timeDifference_ms;
	volatile	bool *syncCANOpenTimeout;
//functions
public:
	Compressor_CANOpen_Interface();
	~Compressor_CANOpen_Interface();
	
	bool	InitCANOpen(void);
	int	Comporesor_Process(void);
	void	Periodic_Task(void);
protected:
private:
	Compressor_CANOpen_Interface( const Compressor_CANOpen_Interface &c );
	Compressor_CANOpen_Interface& operator=( const Compressor_CANOpen_Interface &c );

}; //Compressor_Can_Interface


#endif //__COMPRESSOR_CAN_INTERFACE_H__
