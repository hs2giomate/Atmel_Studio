/* 
* CAN_Asyn_Class.cpp
*
* Created: 8/4/2020 4:43:27 PM
* Author: GMateusDP
*/


#include "CAN_Asyn_Class.h"

CAN_Asyn_Class	*ptrCANAsynClass;
const	uint32_t defaultFilterIndex[]={0x45a,0x469};

static void CAN_CCU_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
	ptrCANAsynClass->txCallBack=true;
	
}

static void CAN_std_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;

	/* Enable the FDOE and BRSE in register configuration*/
	hri_can_set_CCCR_INIT_bit(ptrCANAsynClass->canDescriptor->dev.hw);
	while (hri_can_get_CCCR_INIT_bit(ptrCANAsynClass->canDescriptor->dev.hw) == 0)
	;
	hri_can_set_CCCR_CCE_bit(ptrCANAsynClass->canDescriptor->dev.hw);
	hri_can_set_CCCR_FDOE_bit(ptrCANAsynClass->canDescriptor->dev.hw);
	hri_can_set_CCCR_BRSE_bit(ptrCANAsynClass->canDescriptor->dev.hw);
	ptrCANAsynClass->txSTDCallBack=true;
	
}

static void CAN_CCU_rx_callback(struct can_async_descriptor *const descr)
{
	ptrCANAsynClass->receivedMsg.data=ptrCANAsynClass->rx_buffer_64;
	can_async_read(descr, &(ptrCANAsynClass->receivedMsg));
	ptrCANAsynClass->rxCallBack=true;
	return;
}

// default constructor
CAN_Asyn_Class::CAN_Asyn_Class()
{
	ptrCANAsynClass=this;
} //CAN_Asyn_Class

CAN_Asyn_Class::CAN_Asyn_Class(can_async_descriptor  *descr)
{
	canDescriptor=descr;
	ptrCANAsynClass=this;
} //CAN_Asyn_Class

// default destructor
CAN_Asyn_Class::~CAN_Asyn_Class()
{
} //~CAN_Asyn_Class

bool	CAN_Asyn_Class::Init(void){
	msg.data=buffer_64;
	SetDefaultFilters();
	return	1;
}
void	CAN_Asyn_Class::SetDescriptor(can_async_descriptor  *descr){
	canDescriptor=descr;
}

uint8_t	CAN_Asyn_Class::SetDefaultFilters(void){
		for (i = 0; i < FILTER_INDEXES; i++)
		{
			filterIndex[i]=defaultFilterIndex[i];
			SetFilter(filterIndex[i],i);
		}
		return	0;
}

int32_t	CAN_Asyn_Class::Write(uint8_t *p,uint32_t id, uint8_t len){
		msg.id   = id;
		msg.type = CAN_TYPE_DATA;
		msg.data = p;
		msg.len  = len;
		msg.fmt  = CAN_FMT_STDID;

		can_async_register_callback(canDescriptor, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_CCU_tx_callback);
		can_async_enable(canDescriptor);
		txCallBack=false;
		return  can_async_write(canDescriptor, &msg);
}
int32_t	CAN_Asyn_Class::SetFilter(uint32_t id,uint8_t	indice){
		
		can_async_register_callback(canDescriptor, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_CCU_rx_callback);
		filter.id   = id;
		filter.mask = 0;
		return	can_async_set_filter(canDescriptor, indice, CAN_FMT_STDID, &filter);
}
int32_t	CAN_Asyn_Class::WriteStandard(uint8_t *p,uint32_t id, uint8_t len){
		msg.id   = id;
		msg.type = CAN_TYPE_DATA;
		msg.data = p;
		msg.len  = len;
		msg.fmt  = CAN_FMT_STDID;

		/* Disable the FDOE and BRSE from register configuration
			* and enable them again in callback */
		hri_can_set_CCCR_INIT_bit(canDescriptor->dev.hw);
		while (hri_can_get_CCCR_INIT_bit(canDescriptor->dev.hw) == 0)
			;
		hri_can_set_CCCR_CCE_bit(canDescriptor->dev.hw);

		hri_can_clear_CCCR_FDOE_bit(canDescriptor->dev.hw);
		hri_can_clear_CCCR_BRSE_bit(canDescriptor->dev.hw);

		can_async_register_callback(canDescriptor, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_std_tx_callback);
		can_async_enable(canDescriptor);
		txSTDCallBack=false;
		return  can_async_write(canDescriptor, &msg);
}