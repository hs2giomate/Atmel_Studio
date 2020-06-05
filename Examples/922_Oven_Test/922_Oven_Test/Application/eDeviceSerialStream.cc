/*--------------------------------------------------------------------------

serialStream.cc

This file is part of e.Development

Implementation
Serial IO using low level routines provided in asyncSerial

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $
$Author: steffen $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/ApplicationLayer/PSC/eDeviceSerialStream.cc $

Copyright (c) 2006,2007 Steffen Simon.
All rights reserved.
--------------------------------------------------------------------------*/

#include <avr/io.h>
#include <eDeviceSerialStream.h>

serialStream::serialStream(uint8 uart)
	:selfUART(uart)
	{
	}

bool serialStream::open(void)
	{
	return coreAsyncSerialOpen(selfUART, kBaud57600, 8, 1, false) == 0;
	}

bool serialStream::open(int32 baud, uint8 bits, uint8 stop, bool parity)
	{
	return coreAsyncSerialOpen(selfUART, baud, bits, stop, parity) == 0;
	}

bool serialStream::close(void)
	{
	return coreAsyncSerialClose(selfUART) == 0;
	}

uint8 serialStream::dataAvailable(void)
	{
	return coreAsyncSerialDataAvailable(selfUART);
	}

int16 serialStream::readData(void* p, int16 n, tick_t timeout)
	{
	int16	nRead(coreAsyncSerialRead(selfUART, (byte*)p, n, timeout));
	return nRead;
	}

int16 serialStream::writeData(const void* p, int16 n)
	{
	int16	nWritten(coreAsyncSerialWrite(selfUART, (byte*)p, n));
	return nWritten;
	}

