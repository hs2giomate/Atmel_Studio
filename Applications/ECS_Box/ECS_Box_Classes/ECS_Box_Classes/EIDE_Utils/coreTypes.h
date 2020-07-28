/*--------------------------------------------------------------------------

types.h

This file is part of e.Development

Interface
basic types
--------------------------------------------------------------------------*/
/** @file
	Grundlegende Datentypen
*/
#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "compiler.h"
		
/**
	Vorzeichenbehaftet 8 Bit
*/
typedef signed char		int8;
/**
	Vorzeichenbehaftet 16 Bit
*/
typedef short int	int16;
/**
	Vorzeichenbehaftet 32 Bit
*/
typedef long int	int32;

/**
	Vorzeichenlos 8 Bit
*/
typedef unsigned char		uint8;
/**
	Vorzeichenlos 16 Bit
*/
typedef unsigned short int	uint16;
/**
	Vorzeichenlos 32 Bit
*/
typedef long unsigned int	uint32;
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
typedef long int	tick_t;

#ifdef __cplusplus
	}
#endif

#endif // TYPES_H
