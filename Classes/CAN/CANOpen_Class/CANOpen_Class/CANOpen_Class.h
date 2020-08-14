/* 
* CANOpen_Class.h
*
* Created: 8/5/2020 10:24:01 AM
* Author: GMateusDP
*/


#ifndef __CANOPEN_CLASS_H__
#define __CANOPEN_CLASS_H__
#include "driver_init.h"
#include "CO_Class.h"
#ifdef NMT_MASTER_NODE
	#define NMT_NODE	1
#else
	#define NMT_NODE	2
#endif
	
class CANOpen_Class: public	CO_Class
{
//variables
public:
	volatile bool isOK,synchronized;
	
protected:
private:
	can_async_descriptor  *canDescr;
	CO_NMT_reset_cmd_t reset;	
	CO_ReturnError_t err;
	bool_t	syncWas;
	uint8_t	coNode;
	
//functions
public:
	CANOpen_Class();
	~CANOpen_Class();
	CANOpen_Class(can_async_descriptor  *descr);
	int	Init(uint8_t node=NMT_NODE);
	int RunallProcesses();
	int	Periodic_Process(uint32_t timeDifference=500);
	bool	StartNMTService(void);
protected:
private:
	CANOpen_Class( const CANOpen_Class &c );
	CANOpen_Class& operator=( const CANOpen_Class &c );
	int	SetInitialMode(void);
	void	RedefineMacros(void);
}; //CANOpen_Class

#endif //__CANOPEN_CLASS_H__
