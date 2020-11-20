/** \file		pscI2c_Private.h
 *
 *  \brief		I2C slave driver for the communication with the HALT test box
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef PSCI2C_PRIVATE_H
#define PSCI2C_PRIVATE_H
#include "compiler.h"
#ifdef __cplusplus
extern "C"{
	#endif

/*! Transaction status enumeration */
enum PscI2cStatus_enum
{
	PscI2cReady	= 0,
	PscI2cBusy	= 1,
	
};

/* Transaction result enumeration */
enum PscI2cResult_enum {
	PscI2cResultUnknown          = (0x00<<0),
	PscI2cResultOk               = (0x01<<0),
	PscI2cResultBufferOverflow  = (0x02<<0),
	PscI2cResultTransmitCollision = (0x03<<0),
	PscI2cResultBusError        = (0x04<<0),
	PscI2cResultFail             = (0x05<<0),
	PscI2cResultAborted             = (0x06<<0)
};



enum PscI2cBufferSize_enum
{
	PscI2cRxBufferSize = 100,
	PscI2cTxBufferSize = 100,
	
};



/*! \brief PSC I2C driver object type
 *
 *  Holds buffers and necessary variables
 */
typedef struct PscI2c 
{
	bool bCommandPendingProcessing;	/* Set if buffer contains a PSC command ready for processing */
	uint8_t cRxData[PscI2cRxBufferSize]; /* Data received by the HMI from the PSC */
	uint8_t cDataForPscToCollect[PscI2cTxBufferSize];        /* Data setup by HMI ready for PSC to collect */
	uint8_t cCountOfRxBytes;                          /* Number of bytes sent by PSC in any transfer  */
	uint8_t cCountOfBytesCollectedByPsc;                          /* Number of bytes collected by PSC in any transfer  */
	bool bAbort;                                     /*!< flag is set if the transfer should be aborted */
	uint8_t cCountOfBytesToReturn;                       /*!< Number of bytes for PSC to read */
	uint8_t cCountOfBytesToRead;                        /*!< Number of bytes to read */
	uint8_t cStatus;                             /*!< Status of transaction */
	uint8_t cResult;                             /*!< Result of transaction */
} PscI2c_t;



#ifdef __cplusplus
}
#endif




#endif /* PSCI2C_PRIVATE_H */
/*** END of FILE ***/
