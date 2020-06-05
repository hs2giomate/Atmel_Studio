/*--------------------------------------------------------------------------

coreI2C.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the ATMega128 I2C controller

$Author: steffen $
$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/coreI2C.c $

--------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "coreI2C.h"
#include "coreController.h"

#if __DEBUG__ > 4
#define I2C_DEBUG 4
//__DEBUG__
#endif	

#ifdef I2C_DEBUG
void writeI2CDebugString(const char* p)
	{
	while (*p)
		{
		UDR1 = *p++;
		// wait until UART1 send register is empty
		while (!(UCSR1A & _BV(UDRE1)))
			;
		}
	}
#endif	

// TWSR values (not bits)
// (taken from avr-libc twi.h - thank you Marek Michalkiewicz)
// Master
#define TW_START					0x08
#define TW_REP_START				0x10
// Master Transmitter
#define TW_MT_SLA_ACK				0x18
#define TW_MT_SLA_NACK				0x20
#define TW_MT_DATA_ACK				0x28
#define TW_MT_DATA_NACK				0x30
#define TW_MT_ARB_LOST				0x38
// Master Receiver
#define TW_MR_ARB_LOST				0x38
#define TW_MR_SLA_ACK				0x40
#define TW_MR_SLA_NACK				0x48
#define TW_MR_DATA_ACK				0x50
#define TW_MR_DATA_NACK				0x58
// Slave Transmitter
#define TW_ST_SLA_ACK				0xA8
#define TW_ST_ARB_LOST_SLA_ACK		0xB0
#define TW_ST_DATA_ACK				0xB8
#define TW_ST_DATA_NACK				0xC0
#define TW_ST_LAST_DATA				0xC8
// Slave Receiver
#define TW_SR_SLA_ACK				0x60
#define TW_SR_ARB_LOST_SLA_ACK		0x68
#define TW_SR_GCALL_ACK				0x70
#define TW_SR_ARB_LOST_GCALL_ACK	0x78
#define TW_SR_DATA_ACK				0x80
#define TW_SR_DATA_NACK				0x88
#define TW_SR_GCALL_DATA_ACK		0x90
#define TW_SR_GCALL_DATA_NACK		0x98
#define TW_SR_STOP					0xA0
// Misc
#define TW_NO_INFO					0xF8
#define TW_BUS_ERROR				0x00

// defines and constants
#define TWCR_CMD_MASK		0x0F
#define TWSR_STATUS_MASK	0xF8

// return values
#define I2C_OK				0x00
#define I2C_ERROR_NODEV		0x01

// functions

// high-level I2C transaction commands

//! send I2C data to a device on the bus
int16 coreI2CMasterSend(uint8 deviceAddr, int16 length, uint8 *data);
//! send I2C data to a device on the bus
int16 coreI2CMasterSendToRegister(uint8 deviceAddr, uint8 registerAddr, int16 length, uint8 *data);
//! receive I2C data from a device on the bus
int16 coreI2CMasterReceive(uint8 deviceAddr, int16 length, uint8* data);
//! receive I2C data from a device on the bus
int16 coreI2CMasterReceiveFromRegister(uint8 deviceAddr, uint8 registerAddr, int16 length, uint8* data);

//! Get the current high-level state of the I2C interface
i2cStates coreI2CGetState(void);

// I2C state and address variables
static volatile i2cStates I2cState;
static uint8	I2cDeviceAddrRW;
static bool		i2c0RegisterPending = false;
static uint8	i2c0RegisterAddress;

// send/transmit buffer (outgoing data)
static uint8*	I2cSendData;
static int16	I2cSendDataIndex;
static int16	I2cSendDataLength;

// receive buffer (incoming data)
static uint8*	I2cReceiveData;
static int16	I2cReceiveDataIndex;
static int16	I2cReceiveDataLength;

static uint8	coreI2CResult;

// function pointer to coreI2C receive routine
//! I2cSlaveReceive is called when this processor
// is addressed as a slave for writing
static uint8* (*coreI2CSlaveReceive)(int16* receiveDataLength);
//! I2cSlaveTransmit is called when this processor
// is addressed as a slave for reading
static uint8* (*coreI2CSlaveTransmit)(int16* transmitDataLength);

int8 coreI2CInitialize(void)
	{
	// clear SlaveReceive and SlaveTransmit handler to null
	coreI2CSlaveReceive = NULL;
	coreI2CSlaveTransmit = NULL;

	I2cSendDataIndex = 0;
	I2cSendDataLength = 0;
    I2cSendData = NULL;

	I2cReceiveDataIndex = 0;
	I2cReceiveDataLength = 0;
    I2cReceiveData = NULL;

	I2cState = kI2CStateIdle;
	// set coreI2C bit rate to 100KHz
	coreI2CSetLocalDeviceAddress(0x42, 1);

	return 0xFF;
	}

int8 coreI2COpen(uint8 i2c, uint32 bitrate)
	{
	int8	result = 0;
	uint16	FDIV;
#ifdef I2C_DEBUG
	writeI2CDebugString("coreI2COpen\r");
#endif
	if (bitrate < 1000)
		bitrate = 1000;
	else if (bitrate > 400000)
		bitrate = 400000;

	switch (i2c)
		{
		case kI2C0:
		#ifdef I2C_DEBUG
			writeI2CDebugString("coreI2COpen kI2C0\r");
		#endif
			// set I2C bitrate:
			// SCL freq = F_CPU/(16+2*TWBR*4^TWPS)
			// set TWPS to zero for bitrates > ~30000 bps
 			TWSR &= ~((1 << TWPS1)|(1 << TWPS0));

			FDIV = (F_CPU/bitrate);
		 	FDIV = (FDIV - 16)/2;
		 	if (FDIV < 10)
		 		FDIV = 10;
			TWBR = FDIV;
			TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWIE)|(1 << TWEA);
			break;
		
		default:
			result = -1;
			break;
		}

	return result;
	}

int8 coreI2CClose(uint8 i2c)
	{
	int8	result = 0;
#ifdef I2C_DEBUG
	writeI2CDebugString("coreI2CClose\r");
#endif
	switch (i2c)
		{
		case kI2C0:
		#ifdef I2C_DEBUG
			writeI2CDebugString("coreI2CClose kI2C0\r");
		#endif
			TWCR = 0;
			break;
		
		default:
			result = -1;
			break;
		}

	return result;
	}

void coreI2CAbortOperation(uint8 i2c)
	{
#ifdef I2C_DEBUG
	writeI2CDebugString("coreI2CAbortOperation\r");
#endif
	switch (i2c)
		{
		case kI2C0:
			TWCR = 0;
			I2cSendDataIndex = 0;
			I2cSendDataLength = 0;
			I2cSendData = NULL;

			I2cReceiveDataIndex = 0;
			I2cReceiveDataLength = 0;
			I2cReceiveData = NULL;

			I2cState = kI2CStateIdle;
			TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWIE)|(1 << TWEA);
			break;

		default:
			break;
		}
	}

int16 coreI2CRead(uint8 i2c, uint8 adr, void* p, int16 n, tick_t timeout)
	{
	int16	nRead;
	
	coreI2CResult = I2C_OK;
	nRead = coreI2CMasterReceive(adr, n, (uint8*)p);
	
	return nRead;
	}

int16 coreI2CReadFromRegister(uint8 i2c, uint8 adr, uint8 reg, void* p, int16 n, tick_t timeout)
	{
	int16	nRead;
	
	coreI2CResult = I2C_OK;
	nRead = coreI2CMasterReceiveFromRegister(adr, reg, n, (uint8*)p);
	
	return nRead;
	}

int16 coreI2CWrite(uint8 i2c, uint8 adr, const void* p, int16 n, tick_t timeout)
	{
	int16	nWritten;
	
	coreI2CResult = I2C_OK;
	nWritten = coreI2CMasterSend(adr, n, (uint8*)p);

	return nWritten;
	}

int16 coreI2CWriteToRegister(uint8 i2c, uint8 adr, uint8 reg, const void* p, int16 n, tick_t timeout)
	{
	int16	nWritten;
	
	coreI2CResult = I2C_OK;
	nWritten = coreI2CMasterSendToRegister(adr, reg, n, (uint8*)p);
	
	return nWritten;
	}

void coreI2CSetLocalDeviceAddress(uint8 deviceAddress, uint8 genCallEn)
	{
	// set local device address (used in slave mode only)
	TWAR = ((deviceAddress&0xFE) | (genCallEn?1:0));
	}

void coreI2CSetSlaveReceiveHandler(uint8* (*I2CSlaveReceiveFunction)(int16* receiveDataLength))
	{
	coreI2CSlaveReceive = I2CSlaveReceiveFunction;
	}

void coreI2CSetSlaveTransmitHandler(uint8* (*I2CSlaveTransmitFunction)(int16* transmitDataLength))
	{
	coreI2CSlaveTransmit = I2CSlaveTransmitFunction;
	}

static volatile bool	coreI2CSignalHandlerFired = false;

int16 coreI2CMasterSend(uint8 deviceAddr, int16 length, uint8* data)
	{
#if I2C_DEBUG > 3
	writeI2CDebugString("coreI2CMasterSend...\r");
#endif
	// wait for interface to be ready
	while(I2cState != kI2CStateIdle)
		coreControllerIdle();

	// set state
	I2cState = kI2CStateMasterTransmit;

	I2cDeviceAddrRW = (deviceAddr & 0xFE);	// RW cleared: write operation
	I2cSendData = data;
	I2cSendDataIndex = 0;
	I2cSendDataLength = length;
	i2c0RegisterPending = false;
	
	coreI2CSignalHandlerFired = false;
	// send start condition
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN)|(1 << TWIE);

	while(I2cState == kI2CStateMasterTransmit)
		{
		coreControllerIdle();
		if (!coreI2CSignalHandlerFired)
			{
		#if I2C_DEBUG > 2
			writeI2CDebugString("coreI2CMasterSend failed\r");
		#endif
        	coreI2CAbortOperation(kI2C0);
        	break;
        	}
  		//else
  		//	coreI2CSignalHandlerFired = false;
  	    }
#if I2C_DEBUG > 3
	writeI2CDebugString("...done\r");
#endif
	
	return I2cSendDataIndex;
	}

int16 coreI2CMasterSendToRegister(uint8 deviceAddr, uint8 registerAddr, int16 length, uint8 *data)
	{
#if I2C_DEBUG > 3
	writeI2CDebugString("coreI2CMasterSendToRegister...\r");
#endif
	// wait for interface to be ready
	while(I2cState != kI2CStateIdle)
		coreControllerIdle();

	// set state
	I2cState = kI2CStateMasterTransmit;

	I2cDeviceAddrRW = (deviceAddr & 0xFE);	// RW cleared: write operation
	I2cSendData = data;
	I2cSendDataIndex = 0;
	I2cSendDataLength = length;
	i2c0RegisterPending = true;
	i2c0RegisterAddress = registerAddr;
	
	coreI2CSignalHandlerFired = false;
	// send start condition
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN)|(1 << TWIE);

	while(I2cState == kI2CStateMasterTransmit)
		{
		coreControllerIdle();

		if (!coreI2CSignalHandlerFired)
			{
		#if I2C_DEBUG > 2
			writeI2CDebugString("coreI2CMasterSendToRegister failed\r");
		#endif
        	coreI2CAbortOperation(kI2C0);
        	break;
        	}
  		//else
  		//	coreI2CSignalHandlerFired = false;
  	    }
#if I2C_DEBUG > 3
	writeI2CDebugString("...done\r");
#endif
	
	return I2cSendDataIndex;
	}

int16 coreI2CMasterReceive(uint8 deviceAddr, int16 length, uint8* data)
	{
#if I2C_DEBUG > 3
	writeI2CDebugString("coreI2CMasterReceive...\r");
#endif
	// wait for interface to be ready
	while(I2cState != kI2CStateIdle)
		coreControllerIdle();

	// set state
	I2cState = kI2CStateMasterReceive;
	// set receive address
	I2cDeviceAddrRW = (deviceAddr|0x01);	// RW set: read operation

	I2cReceiveData = data;
	I2cReceiveDataIndex = 0;
	I2cReceiveDataLength = length;
	i2c0RegisterPending = false;
	coreI2CSignalHandlerFired = false;

	// send start condition
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN)|(1 << TWIE);

	// wait for data
	while (I2cState == kI2CStateMasterReceive)
		{
		coreControllerIdle();

		if (!coreI2CSignalHandlerFired)
			{
		#if I2C_DEBUG > 2
			writeI2CDebugString("coreI2CMasterReceive failed\r");
		#endif
        	coreI2CAbortOperation(kI2C0);
        	break;
        	}
  		//else
  		//	coreI2CSignalHandlerFired = false;
  	    }

#if I2C_DEBUG > 3
	writeI2CDebugString("...done\r");
#endif
	
	return I2cReceiveDataIndex;
	}

int16 coreI2CMasterReceiveFromRegister(uint8 deviceAddr, uint8 registerAddr, int16 length, uint8* data)
	{
#if I2C_DEBUG > 3
	writeI2CDebugString("coreI2CMasterReceiveFromRegister...\r");
#endif
	// wait for interface to be ready
	while(I2cState != kI2CStateIdle)
		coreControllerIdle();

	// set state
	I2cState = kI2CStateMasterReceive;
	// set receive address
	I2cDeviceAddrRW = (deviceAddr|0x01);	// RW set: read operation

	I2cReceiveData = data;
	I2cReceiveDataIndex = 0;
	I2cReceiveDataLength = length;
	i2c0RegisterPending = true;
	i2c0RegisterAddress = registerAddr;
	coreI2CSignalHandlerFired = false;

	// send start condition
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN)|(1 << TWIE);

	// wait for data
	while (I2cState == kI2CStateMasterReceive)
		{
		coreControllerIdle();
		if (!coreI2CSignalHandlerFired)
			{
		#if I2C_DEBUG > 2
			writeI2CDebugString("coreI2CMasterReceiveFromRegister failed\r");
		#endif
        	coreI2CAbortOperation(kI2C0);
        	break;
        	}
  		//else
  		//	coreI2CSignalHandlerFired = false;
  	    }

#if I2C_DEBUG > 3
	writeI2CDebugString("...done\r");
#endif
	
	return I2cReceiveDataIndex;
	}

#ifdef I2C_DEBUG
static uint8	charBuffer[5] = "'X' ";
#endif
//! I2C (TWI) interrupt service routine
SIGNAL(SIG_2WIRE_SERIAL)
	{
	// read status bits
	uint8	status = (TWSR & TWSR_STATUS_MASK);
	uint8	nextTWCRValue = 0;
	
	coreI2CSignalHandlerFired = true;
	
	switch(status)
		{
		// Master General
		case TW_START:						// 0x08: Sent start condition
		case TW_REP_START:					// 0x10: Sent repeated start condition
		#if I2C_DEBUG > 4
			writeI2CDebugString("TW_START\r");
		#endif
			// send device address
			if (i2c0RegisterPending)
				TWDR = I2cDeviceAddrRW & 0xFE;
			else
				TWDR = I2cDeviceAddrRW;
			break;
	
		// Master Transmitter & Receiver status codes
		case TW_MT_SLA_ACK:					// 0x18: Slave address acknowledged
		case TW_MT_DATA_ACK:				// 0x28: Data acknowledged
		#if I2C_DEBUG > 4
			writeI2CDebugString("TW_MT_SLA_ACK\r");
		#endif
			if (i2c0RegisterPending)
				{
				if (I2cState == kI2CStateMasterReceive)
					{
					if (status == TW_MT_DATA_ACK)
						{
						i2c0RegisterPending = false;
						nextTWCRValue = (1 << TWSTA);
						TWDR = I2cDeviceAddrRW|1;
						}
					else
						{
						// send I2C device register
						TWDR = i2c0RegisterAddress;
						}
					}
				else
					{
					i2c0RegisterPending = false;
					// send I2C device register
					TWDR = i2c0RegisterAddress;
					}
				}
			else if (I2cSendDataIndex < I2cSendDataLength)
				{
				// send data
				TWDR = I2cSendData[I2cSendDataIndex++];
				}
			else
				{
				// transmit stop condition, enable SLA ACK
				nextTWCRValue = (1 << TWSTO)|(1 << TWEA);
				// set state
				I2cState = kI2CStateIdle;
				}
			break;

		case TW_MR_DATA_NACK:				// 0x58: Data received, NACK reply issued
#if I2C_DEBUG > 4
	charBuffer[1] = TWDR;
	writeI2CDebugString((char*)charBuffer);
	writeI2CDebugString("TW_MR_DATA_NACK\r");
#endif
			// store final received data byte
			I2cReceiveData[I2cReceiveDataIndex++] = TWDR;
			// transmit stop condition, enable SLA ACK
			nextTWCRValue = (1 << TWSTO)|(1 << TWEA);
			// set state
			I2cState = kI2CStateIdle;
			break;

		case TW_MR_SLA_NACK:				// 0x48: Slave address not acknowledged
		case TW_MT_SLA_NACK:				// 0x20: Slave address not acknowledged
		case TW_MT_DATA_NACK:				// 0x30: Data not acknowledged
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_MR_SLA_NACK\r");
#endif
			// transmit stop condition, enable SLA ACK
			nextTWCRValue = (1 << TWSTO)|(1 << TWEA);
			// set state
			I2cState = kI2CStateIdle;
			break;

		case TW_MT_ARB_LOST:				// 0x38: Bus arbitration lost
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_MT_ARB_LOST\r");
#endif
			// release bus
			nextTWCRValue = (1 << TWEA);
			// set state
			I2cState = kI2CStateIdle;
			break;

		case TW_MR_DATA_ACK:				// 0x50: Data acknowledged
#if I2C_DEBUG > 4
	charBuffer[1] = TWDR;
	writeI2CDebugString((char*)charBuffer);
	writeI2CDebugString("TW_MR_DATA_ACK\r");
#endif
			// store received data byte
			I2cReceiveData[I2cReceiveDataIndex++] = TWDR;
			// fall-through to see if more bytes will be received
		case TW_MR_SLA_ACK:					// 0x40: Slave address acknowledged
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_MR_SLA_ACK\r");
#endif
			if (I2cReceiveDataIndex < (I2cReceiveDataLength-1))
				// data byte will be received, reply with ACK (more bytes in transfer)
				nextTWCRValue = (1 << TWEA);
			break;

		// Slave Receiver status codes
		case TW_SR_SLA_ACK:					// 0x60: own SLA+W has been received, ACK has been returned
		case TW_SR_ARB_LOST_SLA_ACK:		// 0x68: own SLA+W has been received, ACK has been returned
		case TW_SR_GCALL_ACK:				// 0x70:     GCA+W has been received, ACK has been returned
		case TW_SR_ARB_LOST_GCALL_ACK:		// 0x78:     GCA+W has been received, ACK has been returned
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_SR_SLA_ACK\r");
#endif
			// we are being addressed as slave for writing (data will be received from master)
			// set state
			I2cState = kI2CStateSlaveReceive;
			I2cReceiveDataIndex = 0;
			I2cReceiveDataLength = 0;
			I2cReceiveData = NULL;
			// prepare buffer
			if(coreI2CSlaveReceive)
				{
				I2cReceiveData = coreI2CSlaveReceive(&I2cReceiveDataLength);
				}
			// Return ACK
			nextTWCRValue = (1 << TWEA);
			break;

		case TW_SR_DATA_ACK:				// 0x80: data byte has been received, ACK has been returned
		case TW_SR_GCALL_DATA_ACK:			// 0x90: data byte has been received, ACK has been returned
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_SR_DATA_ACK\r");
#endif
			// get previously received data byte
			if (I2cReceiveData != NULL)
				I2cReceiveData[I2cReceiveDataIndex++] = TWDR;
			// check receive buffer status
			if(I2cReceiveDataIndex < I2cReceiveDataLength)
				{
				// Return ACK
				nextTWCRValue = (1 << TWEA);
				}
			break;

		case TW_SR_DATA_NACK:				// 0x88: data byte has been received, NACK has been returned
		case TW_SR_GCALL_DATA_NACK:			// 0x98: data byte has been received, NACK has been returned
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_SR_DATA_NACK\r");
#endif
			// Return NACK
			break;

		case TW_SR_STOP:					// 0xA0: STOP or REPEATED START has been received while addressed as slave
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_SR_STOP\r");
#endif
			nextTWCRValue = (1 << TWEA);
			// coreI2C receive is complete, call coreI2CSlaveReceive
			if(coreI2CSlaveReceive)
				coreI2CSlaveReceive(&I2cReceiveDataIndex);
			// set state
			I2cState = kI2CStateIdle;
			break;

		// Slave Transmitter
		case TW_ST_SLA_ACK:					// 0xA8: own SLA+R has been received, ACK has been returned
		case TW_ST_ARB_LOST_SLA_ACK:		// 0xB0:     GCA+R has been received, ACK has been returned
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_ST_SLA_ACK\r");
#endif
			// we are being addressed as slave for reading (data must be transmitted back to master)
			// set state
			I2cState = kI2CStateSlaveTransmit;
			// request data from application
			if(coreI2CSlaveTransmit)
				I2cSendData = coreI2CSlaveTransmit(&I2cSendDataLength);
			else
				{
				I2cSendDataLength = 0;
				I2cSendData = NULL;
				}
			// reset data index
			I2cSendDataIndex = 0;
			//	fall through
		case TW_ST_DATA_ACK:				// 0xB8: data byte has been transmitted, ACK has been received
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_ST_DATA_ACK\r");
#endif
			TWDR = I2cSendData[I2cSendDataIndex++];
			if (I2cSendDataIndex < I2cSendDataLength)
				// expect ACK to data byte
				nextTWCRValue = (1 << TWEA);
			break;

		case TW_ST_DATA_NACK:				// 0xC0: data byte has been transmitted, NACK has been received
		case TW_ST_LAST_DATA:				// 0xC8:
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_ST_DATA_NACK\r");
#endif
			// all done
			// switch to open slave
			nextTWCRValue = (1 << TWEA);
			// set state
			I2cState = kI2CStateIdle;
			break;

		// Misc
		case TW_NO_INFO:					// 0xF8: No relevant state information
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_NO_INFO\r");
#endif
			// do nothing
			break;

		case TW_BUS_ERROR:					// 0x00: Bus error due to illegal start or stop condition
#if I2C_DEBUG > 4
	writeI2CDebugString("TW_BUS_ERROR\r");
#endif
			// reset internal hardware and release bus
			nextTWCRValue = (1 << TWSTO)|(1 << TWEA);
			// set state
			I2cState = kI2CStateIdle;
			break;
		
		default:
#if I2C_DEBUG > 4
	writeI2CDebugString("UNKNOWN\r");
#endif
			// reset internal hardware and release bus
			nextTWCRValue = (1 << TWSTO)|(1 << TWEA);
			// set state
			I2cState = kI2CStateIdle;
			break;
		}

	nextTWCRValue |= (1 << TWINT)|(1 << TWIE)|(1 << TWEN);
	TWCR = nextTWCRValue;
	}

i2cStates coreI2CGetState(void)
	{
	return I2cState;
	}

