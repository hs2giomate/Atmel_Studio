/* 
* CAN_Asyn_Class.h
*
* Created: 8/4/2020 4:43:27 PM
* Author: GMateusDP
*/


#ifndef __CAN_ASYN_CLASS_H__
#define __CAN_ASYN_CLASS_H__
#include "driver_init.h"
#include "hpl_can_config.h"
#define	FILTER_INDEXES	CONF_CAN1_SIDFC_LSS

class CAN_Asyn_Class
{
//variables
public:
	volatile	bool	rxCallBack;
	volatile	bool	txCallBack;
	volatile	bool	txSTDCallBack;
	uint8_t rx_buffer_64[64];
	can_message receivedMsg;
	can_async_descriptor  *canDescriptor;
protected:
private:
	
	struct can_message msg;
	struct can_filter  filter;
	uint8_t buffer_64[64];
	uint8_t tx_message_64[64];
	uint8_t tx_message_8[8];
	
	uint8_t rx_message_8[8];
	uint32_t	filterIndex[FILTER_INDEXES];
	uint8_t		i,j,k;
	

//functions
public:
	CAN_Asyn_Class();
	CAN_Asyn_Class(can_async_descriptor  *descr);
	~CAN_Asyn_Class();
	bool	Init(void);
	int32_t	Write(uint8_t	*p,uint32_t id,  uint8_t	len);
	int32_t	WriteStandard(uint8_t	*p,uint32_t id,  uint8_t	len);
	void	SetDescriptor(can_async_descriptor  *descr);
	
	
protected:
private:
	CAN_Asyn_Class( const CAN_Asyn_Class &c );
	CAN_Asyn_Class& operator=( const CAN_Asyn_Class &c );
	uint8_t	SetDefaultFilters(void);
	int32_t	SetFilter(uint32_t id, uint8_t	indice);
}; //CAN_Asyn_Class

#endif //__CAN_ASYN_CLASS_H__
