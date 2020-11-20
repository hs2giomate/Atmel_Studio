/** \file		pscI2c.c
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


#include "pscI2c.h"


/* Global variables */
PscI2c_t gPscI2c;      /* Used to buffer incoming data from the PSC */
static const uint8_t SLAVE_ADDRESS = 0x42;		/* HMI slave address. See EDES-01384-EDD-0001-937-ISS_1, section 4.1 */
uint32_t gMarkLastI2cActivity;







/** Initialise the I2C bus for communication with the PSC
	\return Nothing
*/   
void pscI2cInit(void)
{
	/* Configuration of I2C peripheral overrides the normal port pin function */
	/* PSC-I2C-SDA */
	/* PSC-I2C-SCL */
	
	/* Initialise I2C */
	TWIC.SLAVE.CTRLA = 0;
	pscI2cInitialiseDriver(&gPscI2c);
	pscI2cInitialiseModule(&gPscI2c, SLAVE_ADDRESS, TWI_SLAVE_INTLVL_LO_gc);
}

/** Check if a command has been received from the PSC and is pending processing
	\return true if a command is pending processing
	\return false if a command isn't pending processing
*/   
bool pscI2cIsCommandPendingProcessing(void)
{
	/* Restart the I2C peripheral if an error appears to be stopping communication */
	if(gMarkLastI2cActivity > gdwSecondsAlive)
	{
		gMarkLastI2cActivity = gdwSecondsAlive;
	}
	uint32_t dwDiff = gdwSecondsAlive -gMarkLastI2cActivity;
	if(dwDiff > 5)
	{
		gMarkLastI2cActivity = gdwSecondsAlive;
	
		uint8_t i2cStatus = TWIC.SLAVE.STATUS & (TWI_SLAVE_DIF_bm | TWI_SLAVE_APIF_bm | TWI_SLAVE_CLKHOLD_bm | TWI_SLAVE_COLL_bm | TWI_SLAVE_BUSERR_bm);
		if(i2cStatus != 0)
		{
			pscI2cInit();
		}
	}

	return(gPscI2c.bCommandPendingProcessing);
}

/** Copy the latest command data sent from the PSC.
	Also clears the flag indicating a command is pending processing
	\return Nothing
*/   
void pscI2cReadCommandData(uint8_t* pDest, uint8_t cLength)
{
	uint8_t* pSrc;
	uint8_t cMaxLength = cLength;
	
	/* Ensure the count of bytes copied does not exceed the maximum source length */
	if(cMaxLength > PscI2cRxBufferSize)
	{
		cMaxLength = PscI2cRxBufferSize;
	}
		
	/* Copy the data to the destination buffer */
	pSrc = (uint8_t*)gPscI2c.cRxData;
	for(uint8_t c = 0; c < cMaxLength; c++)
	{
		*pDest++ = *pSrc++;
	}
		
	/* Reset / Clear the flag which indicates a PSC command has been received and is ready for processing */
	gPscI2c.bCommandPendingProcessing = false;
}





/*! TWIC Slave Interrupt vector. */
ISR(TWIC_TWIS_vect)
{
	pscI2cInterruptHandler(&gPscI2c);
}






void pscI2cInitialiseDriver(PscI2c_t *ppscI2c)
{
	ppscI2c->cCountOfRxBytes = 0;
	ppscI2c->cCountOfBytesCollectedByPsc = 0;
	ppscI2c->cStatus = PscI2cReady;
	ppscI2c->cResult = PscI2cResultUnknown;
	ppscI2c->bAbort = false;
	ppscI2c->bCommandPendingProcessing = false;
}


void pscI2cInitialiseModule(PscI2c_t *ppscI2c,
                               uint8_t address,
                               TWI_SLAVE_INTLVL_t intLevel)
{
	TWIC.SLAVE.CTRLA = intLevel |
	                              TWI_SLAVE_DIEN_bm |
	                              TWI_SLAVE_APIEN_bm |
											TWI_SLAVE_PIEN_bm |
	                              TWI_SLAVE_ENABLE_bm;
	TWIC.SLAVE.ADDR = (address<<1);
}


void pscI2cInterruptHandler(PscI2c_t *ppscI2c)
{
	uint8_t i2cStatus = TWIC.SLAVE.STATUS;

	if(i2cStatus & TWI_SLAVE_BUSERR_bm) 
	{	/* bus error */
		ppscI2c->cCountOfRxBytes = 0;
		ppscI2c->cCountOfBytesCollectedByPsc = 0;
		ppscI2c->cResult = PscI2cResultBusError;
		ppscI2c->cStatus = PscI2cReady;
	}
	else if(i2cStatus & TWI_SLAVE_COLL_bm) 
	{	/* transmit collision */
		ppscI2c->cCountOfRxBytes = 0;
		ppscI2c->cCountOfBytesCollectedByPsc = 0;
		ppscI2c->cResult = PscI2cResultTransmitCollision;
		ppscI2c->cStatus = PscI2cReady;
	}
	else if((i2cStatus & TWI_SLAVE_APIF_bm) && (i2cStatus & TWI_SLAVE_AP_bm)) 
	{	/* address match */
		pscI2cAddressMatchHandler(ppscI2c);
	}
	else if(i2cStatus & TWI_SLAVE_APIF_bm) 
	{	/* stop (only enabled through slave read transaction) */
		pscI2cStopHandler(ppscI2c);
	}
	else if(i2cStatus & TWI_SLAVE_DIF_bm) 
	{	/* data interrupt */
		pscI2cDataHandler(ppscI2c);
	}
	else 
	{	/* unexpected state */
		pscI2cTransactionFinished(ppscI2c, PscI2cResultFail);
	}
}

/*! \brief TWI address match handler
 *
 *  Prepares TWI module for transaction upon address match
 *
 *  \param ppscI2c The PscI2c_t struct instance.
 */
void pscI2cAddressMatchHandler(PscI2c_t *ppscI2c)
{
	bool bOK;
	bool bRead = ((TWIC.SLAVE.STATUS & TWI_SLAVE_DIR_bm) != 0);
	
	
	/* If application signalling need to abort (error occured). */
	if(ppscI2c->bAbort) 
	{
		TWIC.SLAVE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
		pscI2cTransactionFinished(ppscI2c, PscI2cResultAborted);
		ppscI2c->bAbort = false;
	} 
	else 
	{
		bOK = true;
		if(bRead)
		{}
		else
		{	/* PSC trying to write data */
			if(ppscI2c->bCommandPendingProcessing)
			{	/* Buffer is already in use */
				/* HMI not able to accept data from the PSC at this time */
				/* Terminate the transfer */
				bOK = false;
			}
			else
			{	/* buffer is free */
				/* OK to continue with transfer */
			}
		}

		if(bOK)
		{
			ppscI2c->cStatus = PscI2cBusy;
			ppscI2c->cResult = PscI2cResultUnknown;

			ppscI2c->cCountOfRxBytes = 0;
			ppscI2c->cCountOfBytesCollectedByPsc = 0;

			/* Send ACK, wait for data interrupt. */
			TWIC.SLAVE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
		}
		else
		{	
			/* Send Nack */
			/* Terminate the transfer */
			TWIC.SLAVE.CTRLB = (/*TWI_SLAVE_ACKACT_bm | */TWI_SLAVE_CMD_COMPTRANS_gc);
			ppscI2c->cStatus = PscI2cReady;
		}
	}
	
	gMarkLastI2cActivity = gdwSecondsAlive;
}


/*! \brief TWI stop condition interrupt handler.
 *
 *  \param ppscI2c The PscI2c_t struct instance.
 */
void pscI2cStopHandler(PscI2c_t *ppscI2c)
{
	bool bRead = ((TWIC.SLAVE.STATUS & TWI_SLAVE_DIR_bm) != 0);
	
	/* Clear APIF, according to flowchart don't ACK or NACK */
	uint8_t i2cStatus = TWIC.SLAVE.STATUS;
	TWIC.SLAVE.STATUS = i2cStatus | TWI_SLAVE_APIF_bm;

	pscI2cTransactionFinished(ppscI2c, PscI2cResultOk);
	
	if(bRead)
	{
		
	}
	else
	{	/* PSC writing bytes to the HMI */
		if(ppscI2c->cCountOfRxBytes > 0)
		{	/* PSC has successfully sent some bytes */
			/* Does it amount to a complete command ?
				no way to check without a CRC */
			ppscI2c->bCommandPendingProcessing = true;
		}
	}
}


/*! \brief TWI data interrupt handler.
 *
 *  Calls the appropriate slave read or write handler.
 *
 *  \param ppscI2c The PscI2c_t struct instance.
 */
void pscI2cDataHandler(PscI2c_t *ppscI2c)
{
	bool bRead = ((TWIC.SLAVE.STATUS & TWI_SLAVE_DIR_bm) != 0);
	
	if(bRead)
	{	/* PSC I2C master is reading data from the HMI */
		pscI2cReadHandler(ppscI2c);
	} 
	else 
	{	/* PSC I2C master is writing data to the HMI */
		pscI2cWriteHandler(ppscI2c);
	}
}


/*! \brief PSC I2C slave read interrupt handler.
 *
 *  \param ppscI2c The PscI2c_t struct instance.
 */
void pscI2cWriteHandler(PscI2c_t *ppscI2c)
{
	uint8_t data;
	
	/* Enable stop interrupt. */
	///uint8_t currentCtrlA = TWIC.SLAVE.CTRLA;
	///TWIC.SLAVE.CTRLA = currentCtrlA | TWI_SLAVE_PIEN_bm;


	if(ppscI2c->cCountOfRxBytes < PscI2cRxBufferSize) 
	{	/* Space in buffer for another byte */
		data = TWIC.SLAVE.DATA;
		ppscI2c->cRxData[ppscI2c->cCountOfRxBytes] = data;

		ppscI2c->cCountOfRxBytes++;

		/* If application signalling need to abort (error occured),
		 * complete transaction and wait for next START. Otherwise
		 * send ACK and wait for data interrupt.
		 */
		if(ppscI2c->bAbort) 
		{
			TWIC.SLAVE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
			pscI2cTransactionFinished(ppscI2c, PscI2cResultAborted);
			ppscI2c->bAbort = false;
		} 
		else 
		{
			TWIC.SLAVE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
		}
	}
	/* If buffer overflow, send NACK and wait for next START. Set
	 * result buffer overflow.
	 */
	else 
	{
		TWIC.SLAVE.CTRLB = TWI_SLAVE_ACKACT_bm | TWI_SLAVE_CMD_COMPTRANS_gc;
		pscI2cTransactionFinished(ppscI2c, PscI2cResultBufferOverflow);
	}
}

uint8_t* pscI2cGetTxBuffer(void)
{
	return(gPscI2c.cDataForPscToCollect);
}

/*! \brief PSC I2C slave write interrupt handler.
 *
 *  \param ppscI2c The PscI2c_t struct instance.
 */
void pscI2cReadHandler(PscI2c_t *ppscI2c)
{
	/* If NACK, slave write transaction finished. */
	if((ppscI2c->cCountOfBytesCollectedByPsc > 0) && (TWIC.SLAVE.STATUS & TWI_SLAVE_RXACK_bm)) 
	{
		TWIC.SLAVE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
		pscI2cTransactionFinished(ppscI2c, PscI2cResultOk);
	}
	/* If ACK, master expects more data. */
	else 
	{
		if(ppscI2c->cCountOfBytesCollectedByPsc < PscI2cTxBufferSize) 
		{
			uint8_t data = ppscI2c->cDataForPscToCollect[ppscI2c->cCountOfBytesCollectedByPsc];
			TWIC.SLAVE.DATA = data;
			ppscI2c->cCountOfBytesCollectedByPsc++;

			/* Send data, wait for data interrupt. */
			TWIC.SLAVE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
		}
		/* If buffer overflow. */
		else 
		{
			TWIC.SLAVE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
			pscI2cTransactionFinished(ppscI2c, PscI2cResultBufferOverflow);
		}
	}
}


/*! \brief TWI transaction finished function.
 *
 *  Prepares module for new transaction.
 *
 *  \param ppscI2c    The PscI2c_t struct instance.
 *  \param result The result of the transaction.
 */
void pscI2cTransactionFinished(PscI2c_t *ppscI2c, uint8_t result)
{
	ppscI2c->cResult = result;
	ppscI2c->cStatus = PscI2cReady;
}

/*** END of FILE ***/
