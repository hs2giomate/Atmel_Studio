/*--------------------------------------------------------------------------

coreAsyncSerial.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the AT90CAN128 Asynchronous serial interfaces

$Author: steffen $
$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/coreAsyncSerial.c $

--------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "coreAsyncSerial.h"
#include "coreSystemTimer.h"
#include "coreController.h"

#ifdef __DEBUG__
#define kBufferSize	0x40
#define kBufferMask	0x3F
#else
#define kBufferSize	0x80
#define kBufferMask	0x7F
#endif

#if !defined(IPECO_PSC_Revision)
volatile uint8	usart0Data[kBufferSize];
volatile uint8	usart0DataReceived;
volatile uint8	usart0DataReceivedPointer;
volatile uint8	usart0DataReadPointer;
volatile uint8* usart0TransferData;
volatile uint16	usart0TransferRemaining;
#endif
volatile uint8	usart1Data[kBufferSize];
volatile uint8	usart1DataReceived;
volatile uint8	usart1DataReceivedPointer;
volatile uint8	usart1DataReadPointer;
volatile uint8* usart1TransferData;
volatile uint16	usart1TransferRemaining;

#if !defined(IPECO_PSC_Revision)
SIGNAL(SIG_USART0_RECV)
	{
	usart0Data[usart0DataReceivedPointer++] = UDR0;
	usart0DataReceivedPointer &= kBufferMask;
	if (usart0DataReceived < kBufferSize)
		usart0DataReceived++;
	}

SIGNAL(SIG_USART0_DATA)
	{
	if (--usart0TransferRemaining > 0)
		UDR0 = *usart0TransferData++;
	else
		UCSR0B &= ~_BV(UDRIE0);
	}
#endif

SIGNAL(SIG_USART1_RECV)
	{
	usart1Data[usart1DataReceivedPointer++] = UDR1;
	usart1DataReceivedPointer &= kBufferMask;
	if (usart1DataReceived < kBufferSize)
		usart1DataReceived++;
	}

SIGNAL(SIG_USART1_DATA)
	{
	if (--usart1TransferRemaining > 0)
		UDR1 = *usart1TransferData++;
	else
		UCSR1B &= ~_BV(UDRIE1);
	}

/** Initialize async serial subsystem
*/
int8 coreAsyncSerialInitialize(void)
	{
	int8	result = 0;
	
#if !defined(IPECO_PSC_Revision)
	//	Initialize USART 0
	usart0DataReceived = 0;
	usart0DataReceivedPointer = 0;
	usart0DataReadPointer = 0;
	usart0TransferData = NULL;
	usart0TransferRemaining = 0;
#endif
	//	Initialize USART 1
	usart1DataReceived = 0;
	usart1DataReceivedPointer = 0;
	usart1DataReadPointer = 0;
	usart1TransferData = NULL;
	usart1TransferRemaining = 0;
	
	return result;
	}

/** Open uart
*/
int8 coreAsyncSerialOpen(uint8 uart, int32 baudrate, uint8 bits, uint8 stop, bool parity)
	{
	int8	result = 0;
	
	switch (uart)
		{
		case kUART0:
		#if !defined(IPECO_PSC_Revision)
			UBRR0L = (uint8)(F_CPU / (baudrate * 16L) - 1);
			UBRR0H = (F_CPU / (baudrate * 16L) - 1) >> 8;
			UCSR0A = 0x00;
			UCSR0C = 0x06;

			UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
			break;
			
		case kUART1:
		#endif
			UBRR1L = (uint8)(F_CPU / (baudrate * 16L) - 1);
			UBRR1H = (F_CPU / (baudrate * 16L) - 1) >> 8;
			UCSR1A = 0x00;
			UCSR1C = 0x06;

			UCSR1B = _BV(TXEN1) | _BV(RXEN1) | _BV(RXCIE1);
			break;
		
		default:
			result = -1;
			break;
		}
	
	return result;
	}

/** Close uart
*/
int8 coreAsyncSerialClose(uint8 uart)
	{
	int8	result = 0;
	
	switch (uart)
		{
		case kUART0:
		#if !defined(IPECO_PSC_Revision)
			UCSR0B = 0x00;
			break;
			
		case kUART1:
		#endif
			UCSR1B = 0x00;
			break;
		
		default:
			result = -1;
			break;
		}
	
	return result;
	}

void coreAsyncSerialFlush(uint8 uart)
	{
	}

uint8 coreAsyncSerialDataAvailable(uint8 uart)
	{
	int8	result = 0;
	
	switch (uart)
		{
		case kUART0:
		#if !defined(IPECO_PSC_Revision)
			result = usart0DataReceived;
			break;
			
		case kUART1:
		#endif
			result = usart1DataReceived;
			break;
		
		default:
			break;
		}
	
	return result;
	}

uint8 coreAsyncSerialPeekNext(uint8 uart)
	{
	uint8	result = 0x00;
	
	if (uart == kUART0)
#if !defined(IPECO_PSC_Revision)
		result = usart0Data[usart0DataReadPointer];
	else if (uart == kUART1)
#endif
		result = usart1Data[usart1DataReadPointer];
	
	return result;
	}

int16 coreAsyncSerialRead(uint8 uart, void* p, int16 n, tick_t timeout)
	{
	int16	nRead = 0;
	byte*	bytePtr = (byte*)p;
	tick_t	timeoutTime = timeout;
	uint8	timeoutOccured = 0x00;
	
	if (timeoutTime != forever)
		timeoutTime += coreSystemTimerTicks();

	if (uart == kUART0)
#if !defined(IPECO_PSC_Revision)
		{
		while ((nRead < n) && !timeoutOccured)
			{
			//	If read buffer is empty: Wait until character is in UART receive register
			while (!usart0DataReceived && !timeoutOccured)
				{
				sleep_mode();
				timeoutOccured = (coreSystemTimerTicks() > timeoutTime);
				}
			// read character
			while (usart0DataReceived && (nRead < n))
				{
				coreControllerEnterCritical();
				usart0DataReceived--;
				coreControllerLeaveCritical();
				*bytePtr++ = usart0Data[usart0DataReadPointer++];
				usart0DataReadPointer &= kBufferMask;
				nRead++;
				}
			}
		}
	else if (uart == kUART1)
#endif
		{
		while ((nRead < n) && !timeoutOccured)
			{
			//	wait until character is in UART receive register
			while (!usart1DataReceived && !timeoutOccured)
				{
				sleep_mode();
				timeoutOccured = (coreSystemTimerTicks() > timeoutTime);
				}
			// read character(s)
			while (usart1DataReceived && (nRead < n))
				{
				coreControllerEnterCritical();
				//	UCSR1B &= ~_BV(RXCIE1);
				usart1DataReceived--;
				//	UCSR1B |= _BV(RXCIE1);
				coreControllerLeaveCritical();
				*bytePtr++ = usart1Data[usart1DataReadPointer++];
				usart1DataReadPointer &= kBufferMask;
				nRead++;
				}
			}
		}

	return nRead;
	}

int16 coreAsyncSerialWrite(uint8 uart, const void* p, int16 n)
	{
	int16	nWritten = n;
	uint8	data;
		
	if (uart == kUART0)
#if !defined(IPECO_PSC_Revision)
		{
		// wait until UART0 send register is empty
		while (!(UCSR0A & _BV(UDRE0)))
			;

		usart0TransferData = (byte*)p;
		usart0TransferRemaining = n;
		data = *usart0TransferData++;
		UDR0 = data;
		UCSR0B |= _BV(UDRIE0);

		while (usart0TransferRemaining > 0)
			coreControllerIdle();
		}
	else if (uart == kUART1)
#endif
		{
		// wait until UART1 send register is empty
		while (!(UCSR1A & _BV(UDRE1)))
			;

		usart1TransferData = (byte*)p;
		usart1TransferRemaining = n;
		data = *usart1TransferData++;
		UDR1 = data;
		UCSR1B |= _BV(UDRIE1);

		while (usart1TransferRemaining > 0)
			coreControllerIdle();
		}

	return nWritten;
	}

int16 coreAsyncSerialWriteString(uint8 uart, const char* p)
	{
	int16	length = 0;
	const char*	pChar = p;
	
	while (*p++)
		length++;
	
	return coreAsyncSerialWrite(uart, pChar, length);
	}

