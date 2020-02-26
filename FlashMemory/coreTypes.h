/*--------------------------------------------------------------------------

coreTypes.h

This file is part of e.Core

Interface
basic types

$Date: 2016-09-28 14:13:45 +0200 (Mi, 28 Sep 2016) $
$Revision: 967 $

Copyright (c) 2006,2007,2010, 2013 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
/** @file
	Grundlegende Datentypen
*/
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
	#ifndef NULL
	#define NULL 0
	#endif
#else
	typedef unsigned char	bool;
	#ifndef NULL	
		#define NULL ((void*)0)
	#endif
	
	#define true (0x01)
	#define false (0x00)
#endif
		
/**
	Signed 8 Bit
*/
typedef int8_t		int8;
/**
	Signed 16 Bit
*/
typedef int16_t		int16;
/**
	Signed 32 Bit
*/
typedef int32_t		int32;

/**
	Unsigned 8 Bit
*/
typedef uint8_t		uint8;
/**
	Unsigned 16 Bit
*/
typedef uint16_t	uint16;
/**
	Unsigned 32 Bit
*/
typedef uint32_t	uint32;
/**
	Ein Byte
*/
typedef uint8		byte;
/**
	Ein Word = zwei Byte
*/
typedef uint16		word;
/**
	Datentyp, der eine Zeit in Millisekunden repräsentiert.
	time_t ist vorzeichenbehaftet, um auch negative Zeiten darstellen und mit diesen rechnen zu können
*/
typedef int32_t		time_t;

#ifdef __cplusplus
	}
#endif

#endif // TYPES_H

