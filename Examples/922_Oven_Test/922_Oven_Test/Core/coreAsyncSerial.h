/*--------------------------------------------------------------------------

coreAsyncSerial.h

This file is part of e.Core

Interface
low level interface to the asynchronous serial controller

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $

Copyright (c) 2008, 2009, 2010, 2011 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_ASYNC_SERIAL_H
#define CORE_ASYNC_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/**
	Constants for on-chip UARTS
*/
enum
	{
#if defined(IPECO_PSC_Revision)
	kUART0 = 1,	//!	UART0
#else
	kUART0 = 0,	//!	UART0
	kUART1 = 1,	//!	UART1
#ifdef MGS_ZST_Revision
	kUART2 = 2,	//!	UART2
	kUART3 = 3,	//!	UART3
#endif
#endif
	kNumberOfUARTS
	};

#ifdef __DEBUG__
#if defined(IPECO_PSC_Revision)
	#define kUARTDebug	kUART0	
#else
	#define kUARTDebug	kUART1
#endif
#endif	
/** Initialize async serial subsystem
*/
int8 coreAsyncSerialInitialize(void);

/** Open uart
*/
int8 coreAsyncSerialOpen(uint8 uart, int32 baudrate, uint8 bits, uint8 stop, bool parity);

/** Close uart
*/
int8 coreAsyncSerialClose(uint8 uart);

/** Reset uart
*/
void coreAsyncSerialReset(uint8 uart);

/**
	Check if there is any data available.
	\returns Number of bytes available
*/
uint8 coreAsyncSerialDataAvailable(uint8 uart);

/**
	Peek for the next byte in stream.
	This function doesn't change the read pointer, so you have to call
	\ref coreAsyncSerialRead anyway.
	\returns Next byte if  data available, 0 else.
*/
uint8 coreAsyncSerialPeekNext(uint8 uart);

/**
	Peek for the byte at location offset in stream.
	This function doesn't change the read pointer, so you have to call
	\ref coreAsyncSerialRead anyway.
	\returns Byte at location offset if sufficient data available, 0 else.
*/
uint8 coreAsyncSerialPeek(uint8 uart, int16 offset);

/**
	Read <n> byte in stream.
	\returns Number of bytes read
*/
int16 coreAsyncSerialRead(uint8 uart, void* p, int16 n, tick_t timeout);

/**
	Read one byte from stream.
*/
uint8 coreAsyncSerialReadByte(uint8 uart, tick_t timeout);

/**
	Write one byte to stream.
*/
void coreAsyncSerialWriteByte(uint8 uart, uint8);

/**
	Writes <n> byte to stream.
	\returns Number of bytes written
*/
int16 coreAsyncSerialWriteAsync(uint8 uart, const void* p, int16 n);

/**
	Writes <n> byte to stream.
	\returns Number of bytes written
*/
int16 coreAsyncSerialWrite(uint8 uart, const void* p, int16 n);

/**
	Writes a C-string to stream.
	\returns Number of bytes written
*/
int16 coreAsyncSerialWriteString(uint8 uart, const char* p);
#ifdef __cplusplus
	}
#endif

#endif
