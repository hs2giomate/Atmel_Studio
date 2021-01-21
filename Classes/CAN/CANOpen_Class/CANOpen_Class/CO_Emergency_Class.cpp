/* 
* CO_Emergency_Class.cpp
*
* Created: 8/5/2020 3:22:28 PM
* Author: GMateusDP
*/


#include "CO_Emergency_Class.h"
#include "CO_Driver_Class.h"

CO_Emergency_Class	*ptrCOEMClass;

// default constructor
CO_Emergency_Class::CO_Emergency_Class()
{
	ptrCOEMClass=this;
} //CO_Emergency_Class

// default destructor
CO_Emergency_Class::~CO_Emergency_Class()
{
} //~CO_Emergency_Class


/*
 * Function for accessing _Pre-Defined Error Field_ (index 0x1003) from SDO server.
 *
 * For more information see file CO_SDO.h.
 */

static   CO_SDO_abortCode_t CO_ODF_1003(CO_ODF_arg_t *ODF_arg){
    
    uint8_t value;
	CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    CO_EMpr_t *emPr = (CO_EMpr_t*) ODF_arg->object;
    value = ODF_arg->data[0];

    if(ODF_arg->reading){
        uint8_t noOfErrors;
        noOfErrors = emPr->preDefErrNoOfErrors;

        if(ODF_arg->subIndex == 0U){
            ODF_arg->data[0] = noOfErrors;
        }
        else if(ODF_arg->subIndex > noOfErrors){
            ret = CO_SDO_AB_NO_DATA;
        }
        else{
            ret = CO_SDO_AB_NONE;
        }
    }
    else{
        /* only '0' may be written to subIndex 0 */
        if(ODF_arg->subIndex == 0U){
            if(value == 0U){
                emPr->preDefErrNoOfErrors = 0U;
            }
            else{
                ret = CO_SDO_AB_INVALID_VALUE;
            }
        }
        else{
            ret = CO_SDO_AB_READONLY;
        }
    }

    return ret;
}


/*
 * Function for accessing _COB ID EMCY_ (index 0x1014) from SDO server.
 *
 * For more information see file CO_SDO.h.
 */

static  CO_SDO_abortCode_t CO_ODF_1014(CO_ODF_arg_t *ODF_arg){
    uint8_t *nodeId;
    uint32_t value;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    nodeId = (uint8_t*) ODF_arg->object;
    value =ptrCOEMClass->CO_getUint32(ODF_arg->data);

    /* add nodeId to the value */
    if(ODF_arg->reading){
        ptrCOEMClass->CO_setUint32(ODF_arg->data, value + *nodeId);
    }

    return ret;
}


/******************************************************************************/
CO_ReturnError_t CO_Emergency_Class::EM_Init(
        CO_EM_t                *em,
        CO_EMpr_t              *emPr,
        CO_SDO_t               *SDO,
        uint8_t                *errorStatusBits,
        uint8_t                 errorStatusBitsSize,
        uint8_t                *errorRegister,
        uint32_t               *preDefErr,
        uint8_t                 preDefErrSize,
        CO_CANmodule_t         *CANdev,
        uint32_t                CANdevTxIdx,
        uint32_t                CANidTxEM)
{
    uint8_t i;

    /* verify arguments */
    if(em==NULL || emPr==NULL || SDO==NULL || errorStatusBits==NULL ||
        errorStatusBitsSize<6U || errorRegister==NULL || preDefErr==NULL || CANdev==NULL){
        return CO_ERROR_ILLEGAL_ARGUMENT;
    }

    /* Configure object variables */
    em->errorStatusBits         = errorStatusBits;
    em->errorStatusBitsSize     = errorStatusBitsSize;
    em->bufEnd                  = em->buf + (CO_EM_INTERNAL_BUFFER_SIZE * 8);
    em->bufWritePtr             = em->buf;
    em->bufReadPtr              = em->buf;
    em->bufFull                 = 0U;
    em->wrongErrorReport        = 0U;
    em->handler            = NULL;
    emPr->em                    = em;
    emPr->errorRegister         = errorRegister;
    emPr->preDefErr             = preDefErr;
    emPr->preDefErrSize         = preDefErrSize;
    emPr->preDefErrNoOfErrors   = 0U;
    emPr->inhibitEmTimer        = 0U;
 
    /* clear error status bits */
    for(i=0U; i<errorStatusBitsSize; i++){
        em->errorStatusBits[i] = 0U;
    }

    /* Configure Object dictionary entry at index 0x1003 and 0x1014 */
    CO_OD_configure( OD_H1003_PREDEF_ERR_FIELD,CO_ODF_1003, (void*)emPr, 0, 0U);
    CO_OD_configure( OD_H1014_COBID_EMERGENCY,CO_ODF_1014, (void*)&SDO->nodeId, 0, 0U);

    /* configure emergency message CAN transmission */
    emPr->CANdev = CANdev;
    emPr->CANdev->em = (void*)em; /* update pointer inside CAN device. */
    emPr->CANtxBuff =canopen_driver->CAN_Tx_BufferInit(
            CANdevTxIdx,        /* index of specific buffer inside CAN module */
            CANidTxEM,          /* CAN identifier */
            0,                  /* rtr */
            8U,                  /* number of data bytes */
            0);                 /* synchronous message flag bit */

    return CO_ERROR_NO;
}


/******************************************************************************/
void CO_Emergency_Class::CO_EM_initCallback(CO_EM_t *em,	ptrFunctSignal                  handler)
{
    if(em != NULL){
        em->handler = handler;
    }
}


/******************************************************************************/
void CO_Emergency_Class::EM_process(
        CO_EMpr_t              *emPr,
        bool_t                  NMTisPreOrOperational,
        uint32_t                timeDifference_100us,
        uint32_t                emInhTime)
{

    CO_EM_t *em = emPr->em;
    uint8_t errorRegister;

    /* verify errors from driver and other */
   canopen_driver->CAN_VerifyErrors();
    if(em->wrongErrorReport != 0U){
        EM_ErrorReport(em, CO_EM_WRONG_ERROR_REPORT, CO_EMC_SOFTWARE_INTERNAL, (uint32_t)em->wrongErrorReport);
        em->wrongErrorReport = 0U;
    }


    /* calculate Error register */
    errorRegister = 0U;
    /* generic error */
    if(em->errorStatusBits[5]){
        errorRegister |= CO_ERR_REG_GENERIC_ERR;
    }
    /* communication error (overrun, error state) */
    if(em->errorStatusBits[2] || em->errorStatusBits[3]){
      //  errorRegister |= CO_ERR_REG_COMM_ERR;
    }
    *emPr->errorRegister = (*emPr->errorRegister & 0xEEU) | errorRegister;

    /* inhibit time */
    if(emPr->inhibitEmTimer < emInhTime){
        emPr->inhibitEmTimer += timeDifference_100us;
    }

    /* send Emergency message. */
    if(     NMTisPreOrOperational &&
            !emPr->CANtxBuff->bufferFull &&
            emPr->inhibitEmTimer >= emInhTime &&
            (em->bufReadPtr != em->bufWritePtr || em->bufFull))
    {
        uint32_t preDEF;    /* preDefinedErrorField */
        
        /* add error register */
        em->bufReadPtr[2] = *emPr->errorRegister;

        /* copy data to CAN emergency message */
        CO_memcpy(emPr->CANtxBuff->data, em->bufReadPtr, 8U);
        CO_memcpy((uint8_t*)&preDEF, em->bufReadPtr, 4U);
        em->bufReadPtr += 8;

        /* Update read buffer pointer and reset inhibit timer */
        if(em->bufReadPtr == em->bufEnd){
            em->bufReadPtr = em->buf;
        }
        emPr->inhibitEmTimer = 0U;

        /* verify message buffer overflow, then clear full flag */
        if(em->bufFull == 2U){
            em->bufFull = 0U;    /* will be updated below */
            EM_ErrorReport(em, CO_EM_EMERGENCY_BUFFER_FULL, CO_EMC_GENERIC, 0U);
        }
        else{
            em->bufFull = 0;
        }

        /* write to 'pre-defined error field' (object dictionary, index 0x1003) */
        if(emPr->preDefErr){
            uint8_t i;

            if(emPr->preDefErrNoOfErrors < emPr->preDefErrSize)
                emPr->preDefErrNoOfErrors++;
            for(i=emPr->preDefErrNoOfErrors-1; i>0; i--)
                emPr->preDefErr[i] = emPr->preDefErr[i-1];
            emPr->preDefErr[0] = preDEF;
        }

        /* send CAN message */
        canopen_driver->CAN_Send(emPr->CANtxBuff);
    }

    return;
}


/******************************************************************************/
void CO_Emergency_Class::EM_ErrorReport(CO_EM_t *em, const uint8_t errorBit, const uint32_t errorCode, const uint32_t infoCode){
    uint8_t index = errorBit >> 3;
    uint8_t bitmask = 1 << (errorBit & 0x7);
    uint8_t *errorStatusBits = 0;
    bool_t sendEmergency = true;

    if(em == NULL){
        sendEmergency = false;
    }
    else if(index >= em->errorStatusBitsSize){
        /* if errorBit value not supported, send emergency 'CO_EM_WRONG_ERROR_REPORT' */
        em->wrongErrorReport = errorBit;
        sendEmergency = false;
    }
    else{
        errorStatusBits = &em->errorStatusBits[index];
        /* if error was already reported, do nothing */
        if((*errorStatusBits & bitmask) != 0){
            sendEmergency = false;
        }
    }

    if(sendEmergency){
        /* set error bit */
        if(errorBit){
            /* any error except NO_ERROR */
            *errorStatusBits |= bitmask;
        }

        /* verify buffer full, set overflow */
        if(em->bufFull){
            em->bufFull = 2;
        }
        else{
            uint8_t bufCopy[8];

            /* prepare data for emergency message */
            CO_memcpySwap2(&bufCopy[0], &errorCode);
            bufCopy[2] = 0; /* error register will be set later */
            bufCopy[3] = errorBit;
            CO_memcpySwap4(&bufCopy[4], &infoCode);

            /* copy data to the buffer, increment writePtr and verify buffer full */
            CO_LOCK_EMCY();
            CO_memcpy(em->bufWritePtr, &bufCopy[0], 8);
            em->bufWritePtr += 8;

            if(em->bufWritePtr == em->bufEnd) em->bufWritePtr = em->buf;
            if(em->bufWritePtr == em->bufReadPtr) em->bufFull = 1;
            CO_UNLOCK_EMCY();

            /* Optional signal to RTOS, which can resume task, which handles CO_EM_process */
            if(em->handler != NULL) {
                em->handler();
            }
        }
    }
}


/******************************************************************************/
void CO_Emergency_Class::EM_ErrorReset(CO_EM_t *em, const uint8_t errorBit, const uint32_t infoCode){
    uint8_t index = errorBit >> 3;
    uint8_t bitmask = 1 << (errorBit & 0x7);
    uint8_t *errorStatusBits = 0;
    bool_t sendEmergency = true;

    if(em == NULL){
        sendEmergency = false;
    }
    else if(index >= em->errorStatusBitsSize){
        /* if errorBit value not supported, send emergency 'CO_EM_WRONG_ERROR_REPORT' */
        em->wrongErrorReport = errorBit;
        sendEmergency = false;
    }
    else{
        errorStatusBits = &em->errorStatusBits[index];
        /* if error was allready cleared, do nothing */
        if((*errorStatusBits & bitmask) == 0){
            sendEmergency = false;
        }
    }

    if(sendEmergency){
        /* erase error bit */
        *errorStatusBits &= ~bitmask;

        /* verify buffer full */
        if(em->bufFull){
            em->bufFull = 2;
        }
        else{
            uint8_t bufCopy[8];

            /* prepare data for emergency message */
            bufCopy[0] = 0;
            bufCopy[1] = 0;
            bufCopy[2] = 0; /* error register will be set later */
            bufCopy[3] = errorBit;
            CO_memcpySwap4(&bufCopy[4], &infoCode);

            /* copy data to the buffer, increment writePtr and verify buffer full */
            CO_LOCK_EMCY();
            CO_memcpy(em->bufWritePtr, &bufCopy[0], 8);
            em->bufWritePtr += 8;

            if(em->bufWritePtr == em->bufEnd) em->bufWritePtr = em->buf;
            if(em->bufWritePtr == em->bufReadPtr) em->bufFull = 1;
            CO_UNLOCK_EMCY();

            /* Optional signal to RTOS, which can resume task, which handles CO_EM_process */
            if(em->handler != NULL) {
                em->handler();
            }
        }
    }
}


/******************************************************************************/
bool_t CO_Emergency_Class::EM_IsError(CO_EM_t *em, const uint8_t errorBit){
    uint8_t index = errorBit >> 3;
    uint8_t bitmask = 1 << (errorBit & 0x7);
    bool_t ret = false;

    if(em != NULL && index < em->errorStatusBitsSize){
        if((em->errorStatusBits[index] & bitmask) != 0){
            ret = true;
        }
    }

    return ret;
}