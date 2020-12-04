/** \file		pscEvent.c
 *
 *  \brief		Provide a collection point for HMI data to be passed to the HALT test box
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
#include "pscEvent.h"
#include "pscEvent_Private.h"
#include "pscI2c.h"
#include "buttons.h"
#include "pscHALTCommand.h"
#include "beeper.h"



void pscSetSecondsAlive(uint32_t secondsAlive)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	pHaltEvent->secondsAlive = secondsAlive;
	
	EnableInterrupts();
}

void pscSetHmiSupplyVoltageAdcResult(uint16_t adcResult)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	pHaltEvent->hmiSupplyVoltageAdcResult = adcResult;
	
	EnableInterrupts();
}


void pscSetTemperatureAdcResult(uint16_t adcResult)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	pHaltEvent->temperatureAdcResult = adcResult;
	
	EnableInterrupts();
}

void pscSetVccAdcResult(uint16_t adcResult)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	pHaltEvent->vccAdcResult = adcResult;
	
	EnableInterrupts();
}

void pscSetLastReasonForReset(uint8_t lastReasonForReset)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	pHaltEvent->lastReasonForReset = lastReasonForReset;
	
	EnableInterrupts();
}

void pscSetSequenceCount(uint32_t sequenceCount)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	pHaltEvent->sequenceCount = sequenceCount;
	
	EnableInterrupts();
}

void pscOnExternalFlashResult(bool bSuccess, uint32_t externalFlashMemSuccessCount, uint32_t externalFlashMemFailedCount)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	pHaltEvent->externalFlashMemSuccessCount = externalFlashMemSuccessCount;
	pHaltEvent->externalFlashMemFailedCount = externalFlashMemFailedCount;
	
	EnableInterrupts();
}

void pscSetPublicButtonState(uint8_t cButton, bool bActive)
{
	PscHALTEvent_t* pHaltEvent = (PscHALTEvent_t*)pscI2cGetTxBuffer();
	
	/* Disable interrupts while editing the buffer the PSC I2C interrupt also uses */
	DisableInterrupts();
	
	if(cButton == BtnLeft)
	{
		pHaltEvent->leftButtonStatus = bActive;
	}
	else if(cButton == BtnCentre)
	{
		pHaltEvent->centreButtonStatus = bActive;
	}
	else if(cButton == BtnRight)
	{
		pHaltEvent->rightButtonStatus = bActive;
	}
	else
	{}
		
	EnableInterrupts();
}

/*** END of FILE ***/
