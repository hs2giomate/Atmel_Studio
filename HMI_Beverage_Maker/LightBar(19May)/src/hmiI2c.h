/** \file		hmiI2c.h
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


#ifndef HMII2C_H
#define HMII2C_H

/* function prototypes */

/** \brief Initialise the I2C bus for communication with ICs on the HMI internal bus.
 *
 *  \return  Nothing
 */
void hmiI2cInit(void);

/** \brief Returns true if transaction is ready.
 *
 *  This function returns a boolean whether the TWI Master is ready
 *  for a new transaction.
 *
 *
 *  \return true  If transaction could be started.
 *  \return false If transaction could not be started.
 */
bool hmiI2cReady(void);

/** \brief TWI write transaction.
 *
 *  This function is TWI Master wrapper for a write-only transaction.
 *
 *  \param cAddress      Slave address.
 *  \param writeData    Pointer to data to write.
 *  \param cCountOfBytesToWrite Number of data bytes to write.
 *
 *  \return	Nothing 
 */
void hmiI2cWrite(uint8_t cAddress, uint8_t *pcWriteData, uint8_t cCountOfBytesToWrite);

/** \brief TWI write and/or read transaction.
 *
 *  This function is a TWI Master write and/or read transaction. The function
 *  can be used to both write and/or read bytes to/from the TWI Slave in one
 *  transaction.
 *
 *  \param cAddress					The slave address.
 *  \param pcWriteData				Pointer to data to write.
 *  \param cCountOfBytesToWrite   Number of bytes to write.
 *  \param  pcReadBuffer			Buffer to store data read
 *  \param cCountOfBytesToRead    Number of bytes to read.
 *
 *  \return  Nothing
 */
void hmiI2cWriteRead(uint8_t cAddress,uint8_t *pcWriteData,uint8_t cCountOfBytesToWrite,uint8_t *pcReadBuffer,uint8_t cCountOfBytesToRead);




#endif /* HMII2C_H */
/*** END of FILE ***/