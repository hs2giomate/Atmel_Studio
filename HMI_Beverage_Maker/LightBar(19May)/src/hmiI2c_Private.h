/** \file		hmiI2c_Private.h
 *
 *  \brief		I2C master driver for communication with hardware components connected to the micro-controller via I2C
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef HMII2C_PRIVATE_H
#define HMII2C_PRIVATE_H


/*! \brief HMI I2C driver struct
 *
 *  Holds pointer to TWI module,
 *  buffers and necessary variables.
 */
typedef struct HmiI2c 
{
	TWI_t *pInterface;                  /*!< Pointer to the peripheral / interface to use */
	volatile uint8_t cAddress;                            /*!< Raw 7-bit Slave address. This is NOT shifted */
	uint8_t* pcWriteData;  /*!< pointer to Data to write */
	volatile uint8_t* pcReadBuf;    /*!< pointer to buffer for data read */
	volatile uint8_t cCountOfBytesToWrite;                       /*!< Number of bytes to write */
	volatile uint8_t cCountOfBytesToRead;                        /*!< Number of bytes to read */
	volatile uint8_t cCountOfBytesWritten;                       /*!< Number of bytes written */
	volatile uint8_t cCountOfBytesRead;                          /*!< Number of bytes read */
	volatile uint8_t cStatus;                             /*!< Status of transaction */
	volatile uint8_t cResult;                             /*!< Result of transaction */
} HmiI2c_t;

/*! Transaction status enumeration */
enum HmiI2cStatus_enum
{
	HmiI2cReady	= 0,
	HmiI2cBusy	= 1,
	
};


/*! Transaction result enumeration */
enum HmiI2cResult_enum 
{
	HmiI2cResultUnknown          = (0x00<<0),
	HmiI2cResultOK               = (0x01<<0),
/*	HmiI2cResultBUFFER_OVERFLOW  = (0x02<<0),*/
	HmiI2cResultArbitrationLost = (0x03<<0),
	HmiI2cResultBusError        = (0x04<<0),
	HmiI2cResultNackReceived    = (0x05<<0),
	HmiI2cResultFail             = (0x06<<0),
};

/* Function Prototypes */


#endif /* HMII2C_PRIVATE_H */
/*** END of FILE ***/

