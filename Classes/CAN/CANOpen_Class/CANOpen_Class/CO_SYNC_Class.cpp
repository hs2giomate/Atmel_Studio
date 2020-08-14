/* 
* CO_SYNC_Class.cpp
*
* Created: 8/5/2020 2:58:37 PM
* Author: GMateusDP
*/


#include "CO_SYNC_Class.h"
#include "CO_Driver_Class.h"

// default constructor
CO_SYNC_Class::CO_SYNC_Class()
{
} //CO_SYNC_Class

// default destructor
CO_SYNC_Class::~CO_SYNC_Class()
{
} //~CO_SYNC_Class

/*
 * Read received message from CAN module.
 *
 * Function will be called (by CAN receive interrupt) every time, when CAN
 * message with correct identifier will be received. For more information and
 * description of parameters see file CO_driver.h.
 */
static void CO_SYNC_receive(void *object, const CO_CANrxMsg_t *msg){
	
    CO_SYNC_t *SYNC;
    uint8_t operState;

    SYNC = (CO_SYNC_t*)object;   /* this is the correct pointer type of the first argument */
    operState = *SYNC->operatingState;

    if((operState == CO_NMT_OPERATIONAL) || (operState == CO_NMT_PRE_OPERATIONAL)){
        if(SYNC->counterOverflowValue == 0){
            if(msg->DLC == 0U){
                SYNC->CANrxNew = true;
            }
            else{
                SYNC->receiveError = (uint32_t)msg->DLC | 0x0100U;
            }
        }
        else{
            if(msg->DLC == 1U){
                SYNC->counter = msg->data[0];
                SYNC->CANrxNew = true;
            }
            else{
                SYNC->receiveError = (uint32_t)msg->DLC | 0x0200U;
            }
        }
        if(SYNC->CANrxNew) {
            SYNC->CANrxToggle = SYNC->CANrxToggle ? false : true;
        }
    }
}


/*
 * Function for accessing _COB ID SYNC Message_ (index 0x1005) from SDO server.
 *
 * For more information see file CO_SDO.h.
 */
static CO_SDO_abortCode_t CO_ODF_1005(CO_ODF_arg_t *ODF_arg){
    CO_SYNC_t *SYNC;
    uint32_t value;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    SYNC = (CO_SYNC_t*) ODF_arg->object;
    value =ptrCODriverClass->CO_getUint32(ODF_arg->data);

    if(!ODF_arg->reading){
        uint8_t configureSyncProducer = 0;

        /* only 11-bit CAN identifier is supported */
        if(value & 0x20000000UL){
            ret = CO_SDO_AB_INVALID_VALUE;
        }
        else{
            /* is 'generate Sync messge' bit set? */
            if(value & 0x40000000UL){
                /* if bit was set before, value can not be changed */
                if(SYNC->isProducer){
                    ret = CO_SDO_AB_DATA_DEV_STATE;
                }
                else{
                    configureSyncProducer = 1;
                }
            }
        }

        /* configure sync producer and consumer */
        if(ret == CO_SDO_AB_NONE){
            SYNC->COB_ID = (uint32_t)(value & 0x7FFU);

            if(configureSyncProducer){
                uint8_t len = 0U;
                if(SYNC->counterOverflowValue != 0U){
                    len = 1U;
                    SYNC->counter = 0U;
                    SYNC->timer = 0U;
                }
                SYNC->CANtxBuff = ptrCODriverClass->CAN_Tx_BufferInit(
                   
                        SYNC->CANdevTxIdx,      /* index of specific buffer inside CAN module */
                        SYNC->COB_ID,           /* CAN identifier */
                        0,                      /* rtr */
                        len,                    /* number of data bytes */
                        0);                     /* synchronous message flag bit */
                SYNC->isProducer = true;
            }
            else{
                SYNC->isProducer = false;
            }

            ptrCODriverClass->CAN_Rx_BufferInit(
                    SYNC->CANdevRxIdx,      /* rx buffer index */
                    SYNC->COB_ID,           /* CAN identifier */
                    0x7FF,                  /* mask */
                    0,                      /* rtr */
                    (void*)SYNC,            /* object passed to receive function */
                    CO_SYNC_receive);       /* this function will process received message */
        }
    }

    return ret;
}


/*
 * Function for accessing _Communication cycle period_ (index 0x1006) from SDO server.
 *
 * For more information see file CO_SDO.h.
 */
static CO_SDO_abortCode_t CO_ODF_1006(CO_ODF_arg_t *ODF_arg){
    CO_SYNC_t *SYNC;
    uint32_t value;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    SYNC = (CO_SYNC_t*) ODF_arg->object;
    value = ptrCODriverClass->CO_getUint32(ODF_arg->data);

    if(!ODF_arg->reading){
        /* period transition from 0 to something */
        if((SYNC->periodTime == 0) && (value != 0)){
            SYNC->counter = 0;
        }

        SYNC->periodTime = value;
        SYNC->periodTimeoutTime = (value / 2U) * 3U;
        /* overflow? */
        if(SYNC->periodTimeoutTime < value){
            SYNC->periodTimeoutTime = 0xFFFFFFFFUL;
        }

        SYNC->timer = 0;
    }

    return ret;
}


/**
 * Function for accessing _Synchronous counter overflow value_ (index 0x1019) from SDO server.
 *
 * For more information see file CO_SDO.h.
 */
static CO_SDO_abortCode_t CO_ODF_1019(CO_ODF_arg_t *ODF_arg){
    CO_SYNC_t *SYNC;
    uint8_t value;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    SYNC = (CO_SYNC_t*) ODF_arg->object;
    value = ODF_arg->data[0];

    if(!ODF_arg->reading){
        uint8_t len = 0U;

        if(SYNC->periodTime){
            ret = CO_SDO_AB_DATA_DEV_STATE;
        }
        else if((value == 1) || (value > 240 && value <= 255)){
            ret = CO_SDO_AB_INVALID_VALUE;
        }
        else{
            SYNC->counterOverflowValue = value;
            if(value != 0){
                len = 1U;
            }

            SYNC->CANtxBuff =ptrCODriverClass->CAN_Tx_BufferInit(
             
                    SYNC->CANdevTxIdx,      /* index of specific buffer inside CAN module */
                    SYNC->COB_ID,           /* CAN identifier */
                    0,                      /* rtr */
                    len,                    /* number of data bytes */
                    0);                     /* synchronous message flag bit */
        }
    }

    return ret;
}


/******************************************************************************/
CO_ReturnError_t CO_SYNC_Class::SYNC_Init(
        CO_SYNC_t              *SYNC,
        CO_EM_t                *em,
        CO_SDO_t               *SDO,
        uint8_t                *operatingState,
        uint32_t                COB_ID_SYNCMessage,
        uint32_t                communicationCyclePeriod,
        uint8_t                 synchronousCounterOverflowValue,
        CO_CANmodule_t         *CANdevRx,
        uint32_t                CANdevRxIdx,
        CO_CANmodule_t         *CANdevTx,
        uint32_t                CANdevTxIdx)
{
    uint8_t len = 0;

    /* verify arguments */
    if(SYNC==NULL || em==NULL || SDO==NULL || operatingState==NULL ||
        CANdevRx==NULL || CANdevTx==NULL){
        return CO_ERROR_ILLEGAL_ARGUMENT;
    }

    /* Configure object variables */
    SYNC->isProducer = (COB_ID_SYNCMessage&0x40000000L) ? true : false;
    SYNC->COB_ID = COB_ID_SYNCMessage&0x7FF;

    SYNC->periodTime = communicationCyclePeriod;
    SYNC->periodTimeoutTime = communicationCyclePeriod / 2 * 3;
    /* overflow? */
    if(SYNC->periodTimeoutTime < communicationCyclePeriod) SYNC->periodTimeoutTime = 0xFFFFFFFFL;

    SYNC->counterOverflowValue = synchronousCounterOverflowValue;
    if(synchronousCounterOverflowValue) len = 1;

    SYNC->curentSyncTimeIsInsideWindow = true;

    SYNC->CANrxNew = false;
    SYNC->CANrxToggle = false;
    SYNC->timer = 0;
    SYNC->counter = 0;
    SYNC->receiveError = 0U;

    SYNC->em = em;
    SYNC->operatingState = operatingState;

    SYNC->CANdevRx = CANdevRx;
    SYNC->CANdevRxIdx = CANdevRxIdx;

    /* Configure Object dictionary entry at index 0x1005, 0x1006 and 0x1019 */
    ptrCODriverClass->CO_OD_configure( OD_H1005_COBID_SYNC,        CO_ODF_1005, (void*)SYNC, 0, 0);
    ptrCODriverClass->CO_OD_configure( OD_H1006_COMM_CYCL_PERIOD,  CO_ODF_1006, (void*)SYNC, 0, 0);
    ptrCODriverClass->CO_OD_configure( OD_H1019_SYNC_CNT_OVERFLOW, CO_ODF_1019, (void*)SYNC, 0, 0);

    /* configure SYNC CAN reception */
    ptrCODriverClass->CAN_Rx_BufferInit(
            CANdevRxIdx,            /* rx buffer index */
            SYNC->COB_ID,           /* CAN identifier */
            0x7FF,                  /* mask */
            0,                      /* rtr */
            (void*)SYNC,            /* object passed to receive function */
            CO_SYNC_receive);       /* this function will process received message */

    /* configure SYNC CAN transmission */
    SYNC->CANdevTx = CANdevTx;
    SYNC->CANdevTxIdx = CANdevTxIdx;
    SYNC->CANtxBuff =  ptrCODriverClass->CAN_Tx_BufferInit(
            CANdevTxIdx,            /* index of specific buffer inside CAN module */
            SYNC->COB_ID,           /* CAN identifier */
            0,                      /* rtr */
            len,                    /* number of data bytes */
            0);                     /* synchronous message flag bit */

    return CO_ERROR_NO;
}


/******************************************************************************/
uint8_t CO_SYNC_Class::CO_SYNC_process(
        CO_SYNC_t              *SYNC,
        uint32_t                timeDifference_us,
        uint32_t                ObjDict_synchronousWindowLength)
{
    uint8_t ret = 0;
    uint32_t timerNew;

    if(*SYNC->operatingState == CO_NMT_OPERATIONAL || *SYNC->operatingState == CO_NMT_PRE_OPERATIONAL){
        /* update sync timer, no overflow */
        timerNew = SYNC->timer + timeDifference_us;
        if(timerNew > SYNC->timer) SYNC->timer = timerNew;

        /* was SYNC just received */
        if(SYNC->CANrxNew){
            SYNC->timer = 0;
            ret = 1;
            SYNC->CANrxNew = false;
        }

        /* SYNC producer */
        if(SYNC->isProducer && SYNC->periodTime){
            if(SYNC->timer >= SYNC->periodTime){
                if(++SYNC->counter > SYNC->counterOverflowValue) SYNC->counter = 1;
                SYNC->timer = 0;
                ret = 1;
                SYNC->CANrxToggle = SYNC->CANrxToggle ? false : true;
                SYNC->CANtxBuff->data[0] = SYNC->counter;
                ptrCODriverClass->CAN_Send(SYNC->CANtxBuff);
            }
        }

        /* Synchronous PDOs are allowed only inside time window */
        if(ObjDict_synchronousWindowLength){
            if(SYNC->timer > ObjDict_synchronousWindowLength){
                if(SYNC->curentSyncTimeIsInsideWindow){
                    ret = 2;
                }
                SYNC->curentSyncTimeIsInsideWindow = false;
            }
            else{
                SYNC->curentSyncTimeIsInsideWindow = true;
            }
        }
        else{
            SYNC->curentSyncTimeIsInsideWindow = true;
        }

        /* Verify timeout of SYNC */
        if(SYNC->periodTime && SYNC->timer > SYNC->periodTimeoutTime && *SYNC->operatingState == CO_NMT_OPERATIONAL)
             ptrCODriverClass->EM_ErrorReport(SYNC->em, CO_EM_SYNC_TIME_OUT, CO_EMC_COMMUNICATION, SYNC->timer);
    }
    else {
        SYNC->CANrxNew = false;
    }

    /* verify error from receive function */
    if(SYNC->receiveError != 0U){
        ptrCODriverClass->EM_ErrorReport(SYNC->em, CO_EM_SYNC_LENGTH, CO_EMC_SYNC_DATA_LENGTH, (uint32_t)SYNC->receiveError);
        SYNC->receiveError = 0U;
    }

    return ret;
}