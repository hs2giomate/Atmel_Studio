/** \file		pscI2c.h
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


#ifndef PSCI2C_H
#define PSCI2C_H
#include "pscI2c_Private.h"
#include "mainTimer.h"
#include "bevMakerHmi.h"
#ifdef __cplusplus
extern "C"{
	#endif
	
	extern PscI2c_t gPscI2c;      /* Used to buffer incoming data from the PSC */

extern uint32_t gMarkLastI2cActivity;

/* static function prototypes */

/** \brief Initalizes PSC I2C driver structure.
 *
 *  Initialize the instance of the TWI Slave and set the appropriate values.
 *
 *  \param ppscI2c                  The PscI2c_t struct instance.
 *
 *  \return  Nothing
 */
static void pscI2cInitialiseDriver(PscI2c_t *ppscI2c);

/** \brief Initialize the I2C peripheral.
 *
 *  Enables interrupts on address recognition and data available.
 *  Remember to enable interrupts globally from the main application.
 *
 *  \param ppscI2c        The PscI2c_t struct instance.
 *  \param address    Slave address
 *  \param intLevel   Interrupt level
 *
 *  \return  Nothing
 */
static void pscI2cInitialiseModule(PscI2c_t *ppscI2c, uint8_t address, TWI_SLAVE_INTLVL_t intLevel);

/** \brief PSC I2C interrupt service routine.
 *
 *  Handles all TWI transactions and responses to address match, data reception,
 *  data transmission, bus error and data collision.
 *
 *  \param ppscI2c The PscI2c_t struct instance.
 *
 *  \return  Nothing
 */
static void pscI2cInterruptHandler(PscI2c_t *ppscI2c);

/** \brief Called upon an I2C address-match. The PSC is starting a transaction.
 *
 *  \return  Nothing
 */
static void pscI2cAddressMatchHandler(PscI2c_t *ppscI2c);

/** \brief Called after the I2C stop event during a PSC transaction.
 *
 *  \return  Nothing
 */
static void pscI2cStopHandler(PscI2c_t *ppscI2c);

/** \brief .
 *
 *  \return  Nothing
 */
static void pscI2cDataHandler(PscI2c_t *ppscI2c);

/** \brief .
 *
 *  \return  Nothing
 */
static void pscI2cReadHandler(PscI2c_t *ppscI2c);

/** \brief .
 *
 *  \return  Nothing
 */
static void pscI2cWriteHandler(PscI2c_t *ppscI2c);

/** \brief .
 *
 *  \return  Nothing
 */
static void pscI2cTransactionFinished(PscI2c_t *ppscI2c, uint8_t result);

/** \brief Initialise the I2C bus for communication with the PSC.
 *
 *  \return  Nothing
 */
void pscI2cInit(void);

/** \brief Check if a command has been received from the PSC and is pending processing.
 *
 *  \return true if a command is pending processing
 *  \return false if a command isn't pending processing
 */
bool pscI2cIsCommandPendingProcessing(void);

/** \brief Copy the latest command data sent from the PSC.
 *
 *  \return  Nothing
 */
void pscI2cReadCommandData(uint8_t* pDest, uint8_t cLength);

/** \brief Access the buffer used to transfer data back to the PSC / test-box.
 *
 *  \return  a pointer to the start of the buffer
 */
uint8_t* pscI2cGetTxBuffer(void);

#ifdef __cplusplus
}
#endif

#endif /* PSCI2C_H */
/*** END of FILE ***/