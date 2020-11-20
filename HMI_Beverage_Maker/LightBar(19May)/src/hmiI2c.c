/** \file		hmiI2chmiI2c.c
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

#include "bevMakerHmi.h"
#include "hmiI2c.h"
#include "hmiI2c_Private.h"
#include "mainTimer.h"



/* Global variables */
HmiI2c_t gHmiI2c;
static const uint32_t gdwBaudSetting = 35;
uint32_t gMarkLastHmiI2cActivity;

/* static function prototypes */

/** \brief .
 *
 *  \return  Nothing
 */
inline static void hmiI2cArbitrationLostBusErrorHandler(void);

/** \brief .
 *
 *  \return  Nothing
 */
inline static void hmiI2cWriteHandler(void);

/** \brief .
 *
 *  \return  Nothing
 */
inline static uint8_t i2cFormatSlaveAddressRead(uint8_t cAddress);

/** \brief .
 *
 *  \return  Nothing
 */
inline static uint8_t i2cFormatSlaveAddressWrite(uint8_t cAddress);

/** \brief .
 *
 *  \return  Nothing
 */
inline static void hmiI2cReadHandler(void);

/** \brief .
 *
 *  \return  Nothing
 */
inline static void hmiI2cTransactionFinished(uint8_t cResult);



/** Initialise the I2C bus for communication with ICs on the HMI internal bus
	\return Nothing
*/   
void hmiI2cInit(void)
{
	/* Configuration of I2C peripheral overrides the normal port pin function */
	/* M-SDA */
	/* M-SCL */
	
	
	TWIE.MASTER.CTRLA = 0;
	gHmiI2c.pInterface = &TWIE;
	TWIE.MASTER.CTRLA = TWI_MASTER_INTLVL_LO_gc | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;
	
	/* Baud-Setting =((gdwCpuFrequency /(2 * desired-baudrate)) - 5) */
	TWIE.MASTER.BAUD = gdwBaudSetting;
	TWIE.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;

	gHmiI2c.cResult = HmiI2cResultUnknown;	/* indicate a transaction is not finished */
	gHmiI2c.cStatus = HmiI2cReady;
}

/*! \brief Returns true if transaction is ready.
 *
 *  This function returns a boolean whether the TWI Master is ready
 *  for a new transaction.
 *
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool hmiI2cReady(void)
{
	/* Restart the I2C peripheral if an error appears to be stopping communication */
	if(gMarkLastHmiI2cActivity > gdwSecondsAlive)
	{
		gMarkLastHmiI2cActivity = gdwSecondsAlive;
	}
	uint32_t dwDiff = gdwSecondsAlive -gMarkLastHmiI2cActivity;
	if(dwDiff > 5)
	{
		gMarkLastHmiI2cActivity = gdwSecondsAlive;
		
		hmiI2cInit();
	}

	bool twi_status = (gHmiI2c.cStatus == HmiI2cReady);
	return twi_status;
}


/*! \brief TWI write transaction.
 *
 *  This function is TWI Master wrapper for a write-only transaction.
 *
 *  \param cAddress      Slave address.
 *  \param writeData    Pointer to data to write.
 *  \param cCountOfBytesToWrite Number of data bytes to write.
 *
 *  \retval 
 */
void hmiI2cWrite(uint8_t cAddress
                     ,uint8_t *pcWriteData
                     ,uint8_t cCountOfBytesToWrite)
{
	hmiI2cWriteRead(cAddress, pcWriteData, cCountOfBytesToWrite, 0, 0);
}


/*! \brief TWI write and/or read transaction.
 *
 *  This function is a TWI Master write and/or read transaction. The function
 *  can be used to both write and/or read bytes to/from the TWI Slave in one
 *  transaction.
 *
 *  \param cAddress        The slave address.
 *  \param pcWriteData      Pointer to data to write.
 *  \param cCountOfBytesToWrite   Number of bytes to write.
 *  \param cCountOfBytesToRead    Number of bytes to read.
 *
 *  \retval 
 */
void hmiI2cWriteRead(uint8_t cAddress,
                         uint8_t *pcWriteData,
                         uint8_t cCountOfBytesToWrite,
								 uint8_t *pcReadBuffer,
                         uint8_t cCountOfBytesToRead)
{
	/*Initiate transaction */
	gHmiI2c.cStatus = HmiI2cBusy;	/* indicate a transaction is in progress */
	gHmiI2c.cResult = HmiI2cResultUnknown;	/* indicate a transaction is not finished */

	gHmiI2c.cAddress = cAddress;

	/* Note the start of the data to be written */
	gHmiI2c.pcWriteData = pcWriteData;
	
	gHmiI2c.pcReadBuf = pcReadBuffer;

	gHmiI2c.cCountOfBytesToWrite = cCountOfBytesToWrite;
	gHmiI2c.cCountOfBytesToRead = cCountOfBytesToRead;
	gHmiI2c.cCountOfBytesRead = 0;

	if(gHmiI2c.cCountOfBytesToWrite > 0) 
	{	/* write request */
		/* send the START condition + Address +Write ('R/_W = 0') */
		gHmiI2c.pInterface->MASTER.ADDR = i2cFormatSlaveAddressWrite(gHmiI2c.cAddress);
	}
	else if(gHmiI2c.cCountOfBytesToRead > 0) 
	{	/* read request */
		/* send the START condition + Address +Read('R/_W = 1') */
		gHmiI2c.pInterface->MASTER.ADDR = i2cFormatSlaveAddressRead(gHmiI2c.cAddress);
	}
	else
	{}	/* Should never be a request which neither writes nor reads anything */
}


/*! \brief Common TWI master interrupt service routine.
 *
 *  Check current status and calls the appropriate handler.
 *
 */
/*! TWIE Master Interrupt vector. */
ISR(TWIE_TWIM_vect)
{
	uint8_t currentStatus = gHmiI2c.pInterface->MASTER.STATUS;
	
	if(currentStatus & (TWI_MASTER_ARBLOST_bm | TWI_MASTER_BUSERR_bm))
	{	/* arbitration lost or bus error */
		hmiI2cArbitrationLostBusErrorHandler();
	}
	else if(currentStatus & TWI_MASTER_WIF_bm) 
	{	/* write interrupt */
		gMarkLastHmiI2cActivity = gdwSecondsAlive;
		
		hmiI2cWriteHandler();
	}	
	else if(currentStatus & TWI_MASTER_RIF_bm) 
	{	/* read interrupt */
		hmiI2cReadHandler();
	}
	else 
	{	/* unexpected state */
		hmiI2cTransactionFinished(HmiI2cResultFail);
	}
}


/*! \brief TWI master arbitration lost and bus error interrupt handler.
 *
 *  Handles TWI responses to lost arbitration and bus error.
 *
 */
inline static void hmiI2cArbitrationLostBusErrorHandler(void)
{
	uint8_t currentStatus = gHmiI2c.pInterface->MASTER.STATUS;

	/* If bus error. */
	if(currentStatus & TWI_MASTER_BUSERR_bm) 
	{
		gHmiI2c.cResult = HmiI2cResultBusError;
	}
	/* If arbitration lost. */
	else 
	{
		gHmiI2c.cResult = HmiI2cResultArbitrationLost;
	}

	/* Clear interrupt flag. */
	gHmiI2c.pInterface->MASTER.STATUS =(TWI_MASTER_ARBLOST_bm | TWI_MASTER_BUSERR_bm);

	gHmiI2c.cStatus = HmiI2cReady;
}


/*! \brief TWI master write interrupt handler.
 *
 *  Handles TWI transactions(master write) and responses to(N)ACK.
 *
 */
inline static void hmiI2cWriteHandler(void)
{
	if(gHmiI2c.pInterface->MASTER.STATUS & TWI_MASTER_RXACK_bm) 
	{	/* NOT acknowledged(NACK) by slave */
		/* cancel the transaction */
		gHmiI2c.pInterface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		gHmiI2c.cResult = HmiI2cResultNackReceived;
		gHmiI2c.cStatus = HmiI2cReady;
	}
	else if(gHmiI2c.cCountOfBytesToWrite != 0) 
	{	/* more data to write */
		/* send next byte */
		gHmiI2c.cCountOfBytesToWrite--;
	
		uint8_t data = *(gHmiI2c.pcWriteData);
		gHmiI2c.pInterface->MASTER.DATA = data;
		gHmiI2c.pcWriteData++;
	}
	else if(gHmiI2c.cCountOfBytesRead < gHmiI2c.cCountOfBytesToRead) 
	{	/* setup data written, target data to be read */
		/* send repeated START condition + Address + Read ('R/_W = 1') */
		gHmiI2c.pInterface->MASTER.ADDR = i2cFormatSlaveAddressRead(gHmiI2c.cAddress);
	}
	else 
	{	/* transaction finished */
		/* send STOP condition and set RESULT OK. */
		gHmiI2c.pInterface->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		hmiI2cTransactionFinished(HmiI2cResultOK);
	}
}

inline static uint8_t i2cFormatSlaveAddressRead(uint8_t cAddress)
{
	uint8_t cSlaveAddress = cAddress;
	cSlaveAddress <<= 1;	/* shift the slave address into position */
	cSlaveAddress |= 0x01;	/* include the READ bit */

	return cSlaveAddress;
}

inline static uint8_t i2cFormatSlaveAddressWrite(uint8_t cAddress)
{
	uint8_t cSlaveAddress = cAddress;
	cSlaveAddress <<= 1;	/* shift the slave address into position */

	return cSlaveAddress;
}


/*! \brief TWI master read interrupt handler.
 *
 *  This is the master read interrupt handler that takes care of
 *  reading bytes from the TWI slave.
 *
 */
inline static void hmiI2cReadHandler(void)
{
	uint8_t data = gHmiI2c.pInterface->MASTER.DATA;
	*(gHmiI2c.pcReadBuf)++ = data;
	gHmiI2c.cCountOfBytesRead++;

	/* Local variable used in if test to avoid compiler warning. */
	uint8_t cCountOfBytesToRead = gHmiI2c.cCountOfBytesToRead;

	
	if(gHmiI2c.cCountOfBytesRead < cCountOfBytesToRead) 
	{	/* more bytes to read */
		/* issue ACK to start next byte read */
		gHmiI2c.pInterface->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
	}
	else 
	{	/* transaction finished */
		/* issue NACK and STOP condition. */
		gHmiI2c.pInterface->MASTER.CTRLC = (TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc);
		hmiI2cTransactionFinished(HmiI2cResultOK);
	}
}


/*! \brief TWI transaction finished handler.
 *
 *  Prepares module for new transaction.
 *
 *  \param twi     The hmiI2c_t struct instance.
 *  \param result  The result of the operation.
 */
static inline void hmiI2cTransactionFinished(uint8_t result)
{
	gHmiI2c.cResult = result;
	gHmiI2c.cStatus = HmiI2cReady;
}



/*** END of FILE ***/
