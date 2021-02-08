/* 
* CO_Driver_Class.cpp
*
* Created: 8/5/2020 3:44:14 PM
* Author: GMateusDP
*/


#include "CO_Driver_Class.h"
#include "string.h"
#include "CO_OD_Class.h"

CO_Driver_Class		*canopen_driver;
static tagSTATUSFLAGS	local_status_flags;
static tagERRORFLAGS	local_error_flags;
static uint8_t		local_error_flags_array[2];
static uint16_t	local_compressor_temperatures[3];


static void CO_rx_callback(struct can_async_descriptor *const descr)
{
	canopen_driver->receivedCOMsg.data=canopen_driver->co_rx_buffer_64;
	can_async_read(descr, &(canopen_driver->receivedCOMsg));
	if (canopen_driver->canopen_ready)
	{
		canopen_driver->ProcessInterrupt_Rx();
	}
	
	canopen_driver->receivedRxCOCallback=true;
// 	if(ptrCODriver->RxFifo_Callback_CanModule_p != NULL)
// 	{
// 		ProcessInterrupt_Rx();
// 	
}

static void CO_IRQ_Error_Callback(struct can_async_descriptor *const descr)
{

	canopen_driver->HalCanErrorCode= hri_can_read_IR_reg(descr->dev.hw);
	canopen_driver->receivedIntCOCallback=true;
}


static void CO_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
	canopen_driver->receivedtxCOCallback=true;
	
}

// default constructor
CO_Driver_Class::CO_Driver_Class()
{
	RxFifo_Callback_CanModule_p = NULL;
	canopen_driver=this;
	canopen_ready=false;
	status_flags=&local_status_flags;
	error_flags_array=local_error_flags_array;
} //CO_Driver_Class

CO_Driver_Class::~CO_Driver_Class()
{
// default destructor
} //~CO_Driver_Class



/*-----------------------------------------------------------------------------
 * LOCAL FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
//static void prepareTxHeader(struct can_message *msgHeader, CO_CANtx_t *buffer);

/*-----------------------------------------------------------------------------
 * LOCAL FUNCTIONS
 *----------------------------------------------------------------------------*/

/*!*****************************************************************************
 * \author  Andrii Shylenko
 * \date 	10.11.2018
 *
 * \brief prepares CAN Tx header based on the ID, RTR and data count.
 * \param [in]	TxHeader pointer to @CAN_TxHeaderTypeDef object
 * \param [in]	buffer ponyer to CO_CANtx_t with CANopen configuration data
 *
 * \ingroup CO_driver
 ******************************************************************************/
void CO_Driver_Class::PrepareTxMessage(struct can_message *msgHeader, CO_CANtx_t *buffer)
{
	/* Map buffer data to the HAL CAN tx header data*/
	//TxHeader->ExtId = 0u;
	msgHeader->id=0;
	//TxHeader->IDE = 0;
	msgHeader->fmt=CAN_FMT_STDID;
	
	//TxHeader->DLC = buffer->DLC;
	msgHeader->len=buffer->DLC;
	
	//TxHeader->StdId = ( buffer->ident >> 2 );
	msgHeader->id=( buffer->ident >> 2 );
	
	//TxHeader->RTR = ( buffer->ident & 0x2 );
	msgHeader->type=(can_type)( buffer->ident & 0x2 );
	msgHeader->data=buffer->data;
}

void CO_Driver_Class::ConvertCANMsg2CORxMsg(can_message& msgHeader, CO_CANrxMsg_t *rxMsg)
{
	rxMsg->RxHeader=msgHeader;
	rxMsg->DLC=msgHeader.len;
	rxMsg->ident=msgHeader.id;
	memcpy(&rxMsg->data[0],msgHeader.data,msgHeader.len);

	/* Map buffer data to the HAL CAN tx header data*/
	//TxHeader->ExtId = 0u;

}
/*-----------------------------------------------------------------------------
 * GLOBAL FUNCTIONS - see descriptions in header file
 *----------------------------------------------------------------------------*/

//TODO move callbacks to the CO_driver.c and implement callback init routine

/* \brief 	Cube MX callbacks for Fifo0 and Fifo1
 * \details It is assumed that only one CANmodule is (CO->CANmodule[0]) is used.
 */


void CO_Driver_Class::CAN_RxFifo1MsgPendingCallback(void)
{
	if(RxFifo_Callback_CanModule_p != NULL)
	{
		ProcessInterrupt_Rx();
	}
	else
	{
		;//TODO add assert here
	}
}

void CO_Driver_Class::CO_CANsetConfigurationMode(can_async_descriptor *descr){
	
	canDescr=descr;
	/* Put CAN module in configuration mode */
	/* HAL is responsible for that */
}

/******************************************************************************/
CO_ReturnError_t CO_Driver_Class::CO_CANsetNormalMode(void){
	/* Put CAN module in normal mode */
	int32_t		error_CAN_hal;
	Error = CO_ERROR_NO;
	error_CAN_hal=can_async_enable(CAN_Module->CANBaseDescriptor);

	/* Enable CAN interrupts */

	//    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 1, 0);
	//    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	//    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
	//    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	//    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 1, 0);
	//    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);

	if (error_CAN_hal != CO_ERROR_NO)
	{
		//_Error_Handler(__FILE__, __LINE__);
		return CO_ERROR_HAL;
	}
	CAN_Module->CANnormal = true;
	return Error;
}

/******************************************************************************/
CO_ReturnError_t CO_Driver_Class::CANmodule_init(
		CO_CANmodule_t         *CANmodule,
		can_async_descriptor    *HALCanObject,
		CO_CANrx_t              rxArray[],
		uint32_t                rxSize,
		CO_CANtx_t              txArray[],
		uint32_t                txSize,
		uint32_t                CANbitRate)
{
	canDescr=HALCanObject;
	
	int32_t		error_CAN_hal;

	/* verify arguments */
	if(CANmodule==NULL || rxArray==NULL || txArray==NULL)
	{
		return CO_ERROR_ILLEGAL_ARGUMENT;
	}
	else
	{
		;//do nothing
	}

	RxFifo_Callback_CanModule_p = CANmodule;

	/* Configure object variables */
	CANmodule->CANBaseDescriptor = (can_async_descriptor*)HALCanObject;
	CANmodule->rxArray = rxArray;
	CANmodule->rxSize = rxSize;
	CANmodule->txArray = txArray;
	CANmodule->txSize = txSize;
	CANmodule->CANnormal = false;
	CANmodule->useCANrxFilters = false;
	CANmodule->bufferInhibitFlag = false;
	CANmodule->firstCANtxMessage = true;
	CANmodule->CANtxCount = 0U;
	CANmodule->errOld = 0U;
	CANmodule->em = NULL;

	for(i=0U; i<rxSize; i++)
	{
		rxArray[i].ident = 0U;
		rxArray[i].pFunct = NULL;
	}

	for(i=0U; i<txSize; i++)
	{
		txArray[i].bufferFull = false;
	}


	CANmodule_Disable();
	
	can_async_register_callback(canDescr, CAN_ASYNC_RX_CB, (FUNC_PTR)CO_rx_callback);
	can_async_register_callback(canDescr, CAN_ASYNC_IRQ_CB, (FUNC_PTR)CO_IRQ_Error_Callback);
	error_CAN_hal=can_async_enable(HALCanObject);
	if (error_CAN_hal != CO_ERROR_NO)
	{
		//_Error_Handler(__FILE__, __LINE__);
		return CO_ERROR_HAL;
	}
	CAN_Module=CANmodule;
	compressor_temperatures=local_compressor_temperatures;
	return CO_ERROR_NO;
}


/******************************************************************************/
void CO_Driver_Class::CANmodule_Disable(void){
	/* turn off the module */
	/* handled by CubeMX HAL*/
	/*
	HAL_CAN_DeactivateNotification(CANmodule->CANbaseAddress ,
			CAN_IT_RX_FIFO0_MSG_PENDING |
			CAN_IT_RX_FIFO1_MSG_PENDING |
			CAN_IT_TX_MAILBOX_EMPTY);
	HAL_CAN_Stop(CANmodule->CANbaseAddress);
	*/
	//can_async_register_callback(canDescr, CAN_ASYNC_RX_CB, (FUNC_PTR)CO_rx_callback);
	//can_async_register_callback(canDescr, CAN_ASYNC_IRQ_CB, (FUNC_PTR)CO_IRQ_Error_Callback);
	//can_async_disable(CAN_Module->CANBaseDescriptor);
}
uint8_t CO_Driver_Class::Set_Slave_Node(uint8_t sn){
	slave_node=sn;
	return slave_node;
}

/******************************************************************************/
uint32_t CO_Driver_Class::CANrxMsg_ReadIdent(struct can_message *msg){
	return (uint32_t) msg->id;
}


/******************************************************************************/
CO_ReturnError_t CO_Driver_Class::CAN_Rx_BufferInit(
		uint32_t                index,
		uint32_t                ident,
		uint32_t                mask,
		bool_t                  rtr,
		void                   *object,
		void                  (*pFunct)(void *object, const CO_CANrxMsg_t *message))
{
	ret = CO_ERROR_NO;

	if((CAN_Module!=NULL) && (object!=NULL) && (pFunct!=NULL) && (index < CAN_Module->rxSize)){
		/* buffer, which will be configured */
		CO_CANrx_t *buffer = &CAN_Module->rxArray[index];

		/* Configure object variables */
		buffer->object = object;
		buffer->pFunct = pFunct;

		/* CAN identifier and CAN mask, bit aligned with CAN module. Different on different microcontrollers. */
		buffer->ident = (ident & 0x07FF) << 2;
		if (rtr)
		{
			buffer->ident |= 0x02;
		}
		buffer->mask = (mask & 0x07FF) << 2;
		buffer->mask |= 0x02;

		/* Set CAN hardware module filter and mask. */
		if(CAN_Module->useCANrxFilters)
		{
			/* TODO Configure CAN module hardware filters */
		}
		else
		{
			/*no hardware filters*/
			//CAN_FilterTypeDef FilterConfig;
		
			CO_Filter.id   = buffer->ident;
			CO_Filter.mask = buffer->mask;
			ret= (CO_ReturnError_t)can_async_set_filter(CAN_Module->CANBaseDescriptor,(uint8_t)index, CAN_FMT_STDID, &CO_Filter);
		}
	}
	else
	{
		ret = CO_ERROR_ILLEGAL_ARGUMENT;
	}
	return ret;
}


/******************************************************************************/
CO_CANtx_t* CO_Driver_Class::CAN_Tx_BufferInit(
	
		uint32_t                index,
		uint32_t                ident,
		bool_t                  rtr,
		uint8_t                 noOfBytes,
		bool_t                  syncFlag)
{
	buffer = NULL;

	if((CAN_Module != NULL) && (index < CAN_Module->txSize)){
		/* get specific buffer */
		buffer = &CAN_Module->txArray[index];

		/* CAN identifier, DLC and rtr, bit aligned with CAN module transmit buffer.*/

		buffer->ident &= 0x7FF;
		buffer->ident = ident << 2;
		if (rtr) buffer->ident |= 0x02;

		buffer->DLC = noOfBytes;
		buffer->bufferFull = false;
		buffer->syncFlag = syncFlag;
	}

	return buffer;
}

/******************************************************************************/
CO_ReturnError_t CO_Driver_Class::CAN_Send(CO_CANtx_t *buffer)
{
	err = CO_ERROR_NO;

	if(buffer->bufferFull){
		if(!CAN_Module->firstCANtxMessage){
			/* don't set error, if bootup message is still on buffers */
			EM_ErrorReport((CO_EM_t*)CAN_Module->em, CO_EM_CAN_TX_OVERFLOW, CO_EMC_CAN_OVERRUN, buffer->ident);
		}
		err = CO_ERROR_TX_OVERFLOW;
	}

	

	PrepareTxMessage(&msgHeader, buffer);

	if ((CAN_Module->CANtxCount == 0) ) {
	//	((uint32_t)hri_can_read_TXFQS_TFFL_bf(canHandler->dev.hw) > 0 )) {
		CAN_Module->bufferInhibitFlag = buffer->syncFlag;
		
		can_async_register_callback(CAN_Module->CANBaseDescriptor, CAN_ASYNC_TX_CB, (FUNC_PTR)CO_tx_callback);
		can_async_enable(CAN_Module->CANBaseDescriptor);
		receivedtxCOCallback=false;
		CO_LOCK_CAN_SEND();
		HalCanErrorCode=can_async_write(CAN_Module->CANBaseDescriptor, &msgHeader);
		CO_UNLOCK_CAN_SEND();
		if(HalCanErrorCode != CO_ERROR_NO)
		{
			err = CO_ERROR_HAL;
		}
		else
		{
		//	buffer->bufferFull = true;
			//CANmodule->CANtxCount++;
		}
	}	/* if no buffer is free, message will be sent in the task */
	else
	{
		buffer->bufferFull = true;
		CAN_Module->CANtxCount++;
	}
	

	return err;
}


/******************************************************************************/
void CO_Driver_Class::CANClearPendingSyncPDOs(void)
{
	tpdoDeleted = 0U;

	CO_LOCK_CAN_SEND();
	/* Abort message from CAN module, if there is synchronous TPDO.
	 * Take special care with this functionality. */
	/* TODO */

	/*
      if ((state = HAL_CAN_IsTxMessagePending(CANmodule->CANbaseAddress) && (CANmodule->bufferInhibitFlag))
      {
    	HAL_CAN_AbortTxRequest(CANmodule->);
      }
	 */

	if(/*messageIsOnCanBuffer && */CAN_Module->bufferInhibitFlag){
		/* clear TXREQ */
		CAN_Module->bufferInhibitFlag = false;
		tpdoDeleted = 1U;
	}
	/* delete also pending synchronous TPDOs in TX buffers */
	if(CAN_Module->CANtxCount != 0U){
	
		CO_CANtx_t *buffer = &CAN_Module->txArray[0];
		for(i = CAN_Module->txSize; i > 0U; i--){
			if(buffer->bufferFull){
				if(buffer->syncFlag){
					buffer->bufferFull = false;
					CAN_Module->CANtxCount--;
					tpdoDeleted = 2U;
				}
			}
			buffer++;
		}
	}
	CO_UNLOCK_CAN_SEND();


	if(tpdoDeleted != 0U){
		EM_ErrorReport((CO_EM_t*)CAN_Module->em, CO_EM_TPDO_OUTSIDE_WINDOW, CO_EMC_COMMUNICATION, tpdoDeleted);
	}
}


/******************************************************************************/
void CO_Driver_Class::CAN_VerifyErrors(void){
	em = (CO_EM_t*)CAN_Module->em;
	//uint32_t HalCanErrorCode = canHandler->dev.cb->irq_handler();

	if(CAN_Module->errOld != HalCanErrorCode)
	{
		CAN_Module->errOld = HalCanErrorCode;
		if(HalCanErrorCode & CAN_IRQ_BO)
		{                               /* bus off */
			EM_ErrorReport(em, CO_EM_CAN_TX_BUS_OFF, CO_EMC_BUS_OFF_RECOVERED, HalCanErrorCode);
		}
		else{                                               /* not bus off */
			EM_ErrorReset(em, CO_EM_CAN_TX_BUS_OFF, HalCanErrorCode);

			if(HalCanErrorCode & CAN_IRQ_EW)
			{     											/* bus warning */
				EM_ErrorReport(em, CO_EM_CAN_BUS_WARNING, CO_EMC_NO_ERROR, HalCanErrorCode);
			}
			else
			{
				//do nothing
			}
			if(HalCanErrorCode & CAN_IRQ_EP)
			{      											/* TX/RX bus passive */
				if(!CAN_Module->firstCANtxMessage)
				{
					EM_ErrorReport(em, CO_EM_CAN_TX_BUS_PASSIVE, CO_EMC_CAN_PASSIVE, HalCanErrorCode);
				}
				else
				{
					//do nothing
				}
			}
			else{
				bool_t isError = EM_IsError(em, CO_EM_CAN_TX_BUS_PASSIVE);
				if(isError)
				{
					EM_ErrorReset(em, CO_EM_CAN_TX_BUS_PASSIVE, HalCanErrorCode);
					EM_ErrorReset(em, CO_EM_CAN_TX_OVERFLOW, HalCanErrorCode);
				}
				else
				{
					//do nothing
				}
			}

			if(HalCanErrorCode & CAN_IRQ_EA)
			{      											 /* no error */
				EM_ErrorReset(em, CO_EM_CAN_BUS_WARNING, HalCanErrorCode);
			}
			else
			{
				//do nothing
			}

		}

		if((HalCanErrorCode & CAN_IRQ_DO) )
		{                                 					/* CAN RX bus overflow */
			EM_ErrorReport(em, CO_EM_CAN_RXB_OVERFLOW, CO_EMC_CAN_OVERRUN, HalCanErrorCode);
		}
		else
		{
			//do nothing
		}
	}
		receivedIntCOCallback=false;
}

/*Interrupt handlers*/
/******************************************************************************/
void CO_Driver_Class::ProcessInterrupt_Rx(void)
{

	msgMatched = false;

	ConvertCANMsg2CORxMsg(receivedCOMsg,&CANmessage);
	MsgBuff=CAN_Module->rxArray;
	/* Search rxArray form CANmodule for the same CAN-ID. */
	uint32_t msgCO = (((uint32_t)(CANmessage.ident << 2)) | (uint32_t)(msg.type));
	for (index = 0; index < RxFifo_Callback_CanModule_p->rxSize; index++)
	{
		
		if (((msgCO ^ MsgBuff->ident) & MsgBuff->mask) == 0)
		{
			msgMatched = true;
			break;
		}
		MsgBuff++;
	}

	/* Call specific function, which will process the message */
	if(msgMatched && (MsgBuff != NULL) && (MsgBuff->pFunct != NULL))
	{
			//printf("  %X", CANmessage.ident);
			MsgBuff->pFunct(MsgBuff->object, &CANmessage);
	}else{
		
		if (CANmessage.ident==(CO_CAN_ID_TPDO_1+CCU_CANOPEN_NODE))
	//	if (CANmessage.ident==(CO_CAN_ID_TPDO_1+2))
		{
			memcpy((void*)CO_OD_RAM.voltage,(void*)&CANmessage.data[6],2);
// 			error_flags->b.ptfault_mot=CANmessage.data[32];
// 			error_flags->b.ptfault_pwr=CANmessage.data[33];
// 			error_flags->b.ptfault_hs=CANmessage.data[34];
// 			error_flags->b.ptfault_atru=CANmessage.data[35];
// 			
// 				//error_flags->b.ptfault_mot=CANmessage.data[32];
// 				error_flags->b.faulta=CANmessage.data[37];
// 				error_flags->b.faultb=CANmessage.data[38];
// 				error_flags->b.undervoltage=CANmessage.data[39];
// 				
// 					error_flags->b.overvoltage=CANmessage.data[40];
// 					error_flags->b.overcurrent=CANmessage.data[41];
// 					error_flags->b.overheat=CANmessage.data[42];
// 					error_flags->b.overheat_motor=CANmessage.data[43];
// 					
// 						error_flags->b.overheat_trafo=CANmessage.data[44];
// 						error_flags->b.disabled=CANmessage.data[45];
// 						error_flags->b.crc_bad=CANmessage.data[46];
// 						error_flags->b.sync_fault=CANmessage.data[47];
			memcpy(error_flags_array,(void*)&CANmessage.data[4],2);
			//CO_OD_RAM.temperature=
		}
		else
		{
			if (CANmessage.ident==(CO_CAN_ID_TPDO_2+CCU_CANOPEN_NODE)){
				memcpy((void*)&current_IQFilt,(void*)&CANmessage.data[0],2);
				memcpy((void*)&current_IDFilt,(void*)&CANmessage.data[2],2);
			}else{
				if (CANmessage.ident==(CO_CAN_ID_TPDO_4+CCU_CANOPEN_NODE)){
						memcpy((void*)&compressor_temperatures[0],(void*)&CANmessage.data[0],2);
						memcpy((void*)&compressor_temperatures[1],(void*)&CANmessage.data[4],2);
						memcpy((void*)&compressor_temperatures[2],(void*)&CANmessage.data[6],2);
				}
			}
		}
	}

	//TODO filters handing

	//        if(CANmodule->useCANrxFilters){
	//            /* CAN module filters are used. Message with known 11-bit identifier has */
	//           /* been received */
	//            index = rcvMsg;  /* get index of the received message here. Or something similar */
	//            if(index < CANmodule->rxSize){
	//                buffer = &CANmodule->rxArray[index];
	//                /* verify also RTR */
	//                if(( RxHeader.RTR ) == CAN_RTR_DATA){
	//                    msgMatched = true;
	//                }
	//                if((( RxHeader. rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U){
	//                    msgMatched = true;
	//                }
	//            }
	//        }
	//        else{
	//            /* CAN module filters are not used, message with any standard 11-bit identifier */
	//            /* has been received. Search rxArray form CANmodule for the same CAN-ID. */
	//            buffer = &CANmodule->rxArray[0];
	//            for(index = CANmodule->rxSize; index > 0U; index--){
	//                if(((rcvMsgIdent ^ buffer->ident) & buffer->mask) == 0U){
	//                    msgMatched = true;
	//                    break;
	//                }
	//                buffer++;
	//            }
	//        }

	/*CubeMx HAL is responsible for clearing interrupt flags and all the dirty work. */
}


void CO_Driver_Class::CAN_Polling_Tx(void)
{
	
	//struct can_async_descriptor canStruct= (* struct can_async_descriptor )canHandler;
	if (hri_can_read_TXFQS_TFFL_bf(CAN1) > 0)
	{
		/* First CAN message (bootup) was sent successfully */
		CAN_Module->firstCANtxMessage = false;
		/* Clear flag from previous message */
		CAN_Module->bufferInhibitFlag = false;
		/* Are there any new messages waiting to be send */
		if(CAN_Module->CANtxCount > 0U)
		{
			uint32_t i;             /* index of transmitting message */

			/* first buffer */
			CO_CANtx_t *buffer = &CAN_Module->txArray[0];
			/* search through whole array of pointers to transmit message buffers. */
			for(i = CAN_Module->txSize; i > 0U; i--)
			{
				/* if message buffer is full, send it. */
				if(buffer->bufferFull)
				{

					/* Copy message to CAN buffer */
					CAN_Module->bufferInhibitFlag = buffer->syncFlag;

					

					PrepareTxMessage(&msgHeader, buffer);
					can_async_register_callback(canDescr, CAN_ASYNC_TX_CB, (FUNC_PTR)CO_tx_callback);
					can_async_enable(canDescr);
					receivedtxCOCallback=false;
					HalCanErrorCode=can_async_write(canDescr, &msgHeader);
					if( HalCanErrorCode != CO_ERROR_NO)
					{
						;//do nothing
					}
					else
					{
						buffer->bufferFull = false;
						CAN_Module->CANtxCount--;
					}

					break;                      /* exit for loop */
				}
				else
				{
					/*do nothing*/;
				}
				buffer++;
			}/* end of for loop */

			/* Clear counter if no more messages */
			if(i == 0U)
			{
				CAN_Module->CANtxCount = 0U;
			}
			else
			{
				/*do nothing*/;
			}
		}
	}
}
