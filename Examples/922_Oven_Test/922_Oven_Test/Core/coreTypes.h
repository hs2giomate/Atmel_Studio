/*--------------------------------------------------------------------------

types.h

This file is part of e.Development

Interface
basic types

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $

Copyright (c) 2006,2007,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
/** @file
	Grundlegende Datentypen
*/
#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#ifndef NULL
#define NULL 0
#endif
#else
typedef unsigned char	bool;
#define NULL ((void*)0)
#define true (0x01)
#define false (0x00)
#endif
		
/**
	Vorzeichenbehaftet 8 Bit
*/
typedef signed char		int8;
/**
	Vorzeichenbehaftet 16 Bit
*/
typedef int		int16;
/**
	Vorzeichenbehaftet 32 Bit
*/
typedef long int	int32;

/**
	Vorzeichenlos 8 Bit
*/
typedef unsigned char			uint8;
/**
	Vorzeichenlos 16 Bit
*/
typedef unsigned int			uint16;
/**
	Vorzeichenlos 32 Bit
*/
typedef unsigned long int	uint32;
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
	tick_t ist vorzeichenbehaftet, um auch negative Zeiten darstellen und mit diesen rechnen zu können
*/
typedef int32		tick_t;
typedef int32		time_t;

#ifdef __cplusplus
	}
#endif

#endif // TYPES_H
