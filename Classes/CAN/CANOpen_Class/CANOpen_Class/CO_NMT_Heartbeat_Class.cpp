/* 
* CO_NMT_Heartbeat.cpp
*
* Created: 8/5/2020 2:24:08 PM
* Author: GMateusDP
*/


#include "CO_NMT_Heartbeat_Class.h"
#include "CO_Driver_Class.h"



static void CO_NMT_receive(void *object, const CO_CANrxMsg_t *msg){
    CO_NMT_t *NMT;
    uint8_t nodeId;

    NMT = (CO_NMT_t*)object;   /* this is the correct pointer type of the first argument */

    nodeId = msg->data[1];

    if((msg->DLC == 2) && ((nodeId == 0) || (nodeId == NMT->nodeId))){
        uint8_t command = msg->data[0];
        uint8_t currentOperatingState = NMT->operatingState;

        switch(command){
            case CO_NMT_ENTER_OPERATIONAL:
                if((*NMT->emPr->errorRegister) == 0U){
                    NMT->operatingState = CO_NMT_OPERATIONAL;
                }
                break;
            case CO_NMT_ENTER_STOPPED:
                NMT->operatingState = CO_NMT_STOPPED;
                break;
            case CO_NMT_ENTER_PRE_OPERATIONAL:
                NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
                break;
            case CO_NMT_RESET_NODE:
                NMT->resetCommand = CO_RESET_APP;
                break;
            case CO_NMT_RESET_COMMUNICATION:
                NMT->resetCommand = CO_RESET_COMM;
                break;
        }

        if(NMT->ptrState!=NULL && currentOperatingState!=NMT->operatingState){
            NMT->ptrState((CO_NMT_internalState_t)NMT->operatingState);
        }
    }
}
// default constructor
CO_NMT_Heartbeat_Class::CO_NMT_Heartbeat_Class()
{
} //CO_NMT_Heartbeat

// default destructor
CO_NMT_Heartbeat_Class::~CO_NMT_Heartbeat_Class()
{
} //~CO_NMT_Heartbeat

/******************************************************************************/
CO_ReturnError_t CO_NMT_Heartbeat_Class::NMT_Init(
        CO_NMT_t               *NMT_Init,
        CO_EMpr_t              *emPr,
        uint8_t                 nodeId,
        uint32_t                firstHBTime,
        CO_CANmodule_t         *NMT_CANdev,
        uint32_t                NMT_rxIdx,
        uint32_t                CANidRxNMT,
        CO_CANmodule_t         *HB_CANdev,
        uint32_t                HB_txIdx,
        uint32_t                CANidTxHB)
{
    /* verify arguments */
    if(NMT_Init==NULL || emPr==NULL || NMT_CANdev==NULL || HB_CANdev==NULL){
        return CO_ERROR_ILLEGAL_ARGUMENT;
    }else{
		NMT=NMT_Init;
	}

    /* blinking bytes */
    NMT->LEDflickering          = 0;
    NMT->LEDblinking            = 0;
    NMT->LEDsingleFlash         = 0;
    NMT->LEDdoubleFlash         = 0;
    NMT->LEDtripleFlash         = 0;
    NMT->LEDquadrupleFlash      = 0;

    /* Configure object variables */
    NMT->operatingState         = CO_NMT_INITIALIZING;
    NMT->LEDgreenRun            = -1;
    NMT->LEDredError            = 1;
    NMT->nodeId                 = nodeId;
    NMT->firstHBTime            = firstHBTime;
    NMT->resetCommand           = 0;
    NMT->HBproducerTimer        = 0xFFFF;
    NMT->emPr                   = emPr;
    NMT->ptrState             = NULL;

    /* configure NMT CAN reception */
   ptrCODriverClass->CAN_Rx_BufferInit(
      
            NMT_rxIdx,          /* rx buffer index */
            CANidRxNMT,         /* CAN identifier */
            0x7FF,              /* mask */
            0,                  /* rtr */
            (void*)NMT,         /* object passed to receive function */
            CO_NMT_receive);    /* this function will process received message */

    /* configure HB CAN transmission */
    NMT->HB_CANdev = HB_CANdev;
    NMT->HB_TXbuff =ptrCODriverClass->CAN_Tx_BufferInit(
             HB_txIdx,           /* index of specific buffer inside CAN module */
            CANidTxHB,          /* CAN identifier */
            0,                  /* rtr */
            1,                  /* number of data bytes */
            0);                 /* synchronous message flag bit */

    return CO_ERROR_NO;
}


/******************************************************************************/
void CO_NMT_Heartbeat_Class::CO_NMT_initCallback(
        ptrFuncNMT         ptrState)
{
    if(NMT != NULL){
        NMT->ptrState = ptrState;
        if(NMT->ptrState != NULL){
            NMT->ptrState((CO_NMT_internalState_t)(NMT->operatingState));
        }
    }
}


/******************************************************************************/
void CO_NMT_Heartbeat_Class::CO_NMT_blinkingProcess50ms(void){

    if(++NMT->LEDflickering >= 1) NMT->LEDflickering = -1;

    if(++NMT->LEDblinking >= 4) NMT->LEDblinking = -4;

    if(++NMT->LEDsingleFlash >= 4) NMT->LEDsingleFlash = -20;

    switch(++NMT->LEDdoubleFlash){
        case    4:  NMT->LEDdoubleFlash = -104; break;
        case -100:  NMT->LEDdoubleFlash =  100; break;
        case  104:  NMT->LEDdoubleFlash =  -20; break;
    }

    switch(++NMT->LEDtripleFlash){
        case    4:  NMT->LEDtripleFlash = -104; break;
        case -100:  NMT->LEDtripleFlash =  100; break;
        case  104:  NMT->LEDtripleFlash = -114; break;
        case -110:  NMT->LEDtripleFlash =  110; break;
        case  114:  NMT->LEDtripleFlash =  -20; break;
    }

    switch(++NMT->LEDquadrupleFlash){
        case    4:  NMT->LEDquadrupleFlash = -104; break;
        case -100:  NMT->LEDquadrupleFlash =  100; break;
        case  104:  NMT->LEDquadrupleFlash = -114; break;
        case -110:  NMT->LEDquadrupleFlash =  110; break;
        case  114:  NMT->LEDquadrupleFlash = -124; break;
        case -120:  NMT->LEDquadrupleFlash =  120; break;
        case  124:  NMT->LEDquadrupleFlash =  -20; break;
    }
}


/******************************************************************************/
CO_NMT_reset_cmd_t CO_NMT_Heartbeat_Class::NMT_Process(
        uint32_t                timeDifference_ms,
        uint32_t                HBtime,
        uint32_t                NMTstartup,
        uint8_t                 errorRegister,
        const uint8_t           errorBehavior[],
        uint32_t               *timerNext_ms)
{
    uint8_t CANpassive;

    uint8_t currentOperatingState = NMT->operatingState;

    NMT->HBproducerTimer += timeDifference_ms;

    /* Heartbeat producer message & Bootup message */
    if((HBtime != 0 && NMT->HBproducerTimer >= HBtime) || NMT->operatingState == CO_NMT_INITIALIZING){

        /* Start from the beginning. If OS is slow, time sliding may occur. However, heartbeat is
         * not for synchronization, it is for health report. */
        NMT->HBproducerTimer = 0;

        NMT->HB_TXbuff->data[0] = NMT->operatingState;
        ptrCODriverClass->CAN_Send(NMT->HB_TXbuff);

        if(NMT->operatingState == CO_NMT_INITIALIZING){
            if(HBtime > NMT->firstHBTime) NMT->HBproducerTimer = HBtime - NMT->firstHBTime;
            else                          NMT->HBproducerTimer = 0;

            if((NMTstartup & 0x04) == 0) NMT->operatingState = CO_NMT_OPERATIONAL;
            else                         NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
        }
    }


    /* Calculate, when next Heartbeat needs to be send and lower timerNext_ms if necessary. */
    if(HBtime != 0 && timerNext_ms != NULL){
        if(NMT->HBproducerTimer < HBtime){
            uint32_t diff = HBtime - NMT->HBproducerTimer;
            if(*timerNext_ms > diff){
                *timerNext_ms = diff;
            }
        }else{
            *timerNext_ms = 0;
        }
    }


    /* CAN passive flag */
    CANpassive = 0;
    if(ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_CAN_TX_BUS_PASSIVE) || ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_CAN_RX_BUS_PASSIVE))
        CANpassive = 1;


    /* CANopen green RUN LED (DR 303-3) */
    switch(NMT->operatingState){
        case CO_NMT_STOPPED:          NMT->LEDgreenRun = NMT->LEDsingleFlash;   break;
        case CO_NMT_PRE_OPERATIONAL:  NMT->LEDgreenRun = NMT->LEDblinking;      break;
        case CO_NMT_OPERATIONAL:      NMT->LEDgreenRun = 1;                     break;
    }


    /* CANopen red ERROR LED (DR 303-3) */
    if(ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_CAN_TX_BUS_OFF))
        NMT->LEDredError = 1;

    else if(ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_SYNC_TIME_OUT))
        NMT->LEDredError = NMT->LEDtripleFlash;

    else if(ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_HEARTBEAT_CONSUMER) || ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_HB_CONSUMER_REMOTE_RESET))
        NMT->LEDredError = NMT->LEDdoubleFlash;

    else if(CANpassive ||  ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_CAN_BUS_WARNING))
        NMT->LEDredError = NMT->LEDsingleFlash;

    else if(errorRegister)
        NMT->LEDredError = (NMT->LEDblinking>=0)?-1:1;

    else
        NMT->LEDredError = -1;


    /* in case of error enter pre-operational state */
    if(errorBehavior && (NMT->operatingState == CO_NMT_OPERATIONAL)){
        if(CANpassive && (errorBehavior[2] == 0 || errorBehavior[2] == 2)) errorRegister |= 0x10;

        if(errorRegister){
            /* Communication error */
            if(errorRegister & CO_ERR_REG_COMM_ERR){
                if(errorBehavior[1] == 0){
                    NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
                }
                else if(errorBehavior[1] == 2){
                    NMT->operatingState = CO_NMT_STOPPED;
                }
                else if( ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_CAN_TX_BUS_OFF)
                     ||  ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_HEARTBEAT_CONSUMER)
                     ||  ptrCODriverClass->EM_IsError(NMT->emPr->em, CO_EM_HB_CONSUMER_REMOTE_RESET))
                {
                    if(errorBehavior[0] == 0){
                        NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
                    }
                    else if(errorBehavior[0] == 2){
                        NMT->operatingState = CO_NMT_STOPPED;
                    }
                }
            }

            /* Generic error */
            if(errorRegister & CO_ERR_REG_GENERIC_ERR){
                if      (errorBehavior[3] == 0) NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
                else if (errorBehavior[3] == 2) NMT->operatingState = CO_NMT_STOPPED;
            }

            /* Device profile error */
            if(errorRegister & CO_ERR_REG_DEV_PROFILE){
                if      (errorBehavior[4] == 0) NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
                else if (errorBehavior[4] == 2) NMT->operatingState = CO_NMT_STOPPED;
            }

            /* Manufacturer specific error */
            if(errorRegister & CO_ERR_REG_MANUFACTURER){
                if      (errorBehavior[5] == 0) NMT->operatingState = CO_NMT_PRE_OPERATIONAL;
                else if (errorBehavior[5] == 2) NMT->operatingState = CO_NMT_STOPPED;
            }

            /* if operational state is lost, send HB immediately. */
            if(NMT->operatingState != CO_NMT_OPERATIONAL)
                NMT->HBproducerTimer = HBtime;
        }
    }

    if(NMT->ptrState!=NULL && currentOperatingState!=NMT->operatingState){
        NMT->ptrState((CO_NMT_internalState_t)NMT->operatingState);
    }

    return (CO_NMT_reset_cmd_t)NMT->resetCommand;
}


/******************************************************************************/
CO_NMT_internalState_t CO_NMT_Heartbeat_Class::CO_NMT_getInternalState(void)
{
    if(NMT != NULL){
        return (CO_NMT_internalState_t)NMT->operatingState;
    }
    return CO_NMT_INITIALIZING;
}