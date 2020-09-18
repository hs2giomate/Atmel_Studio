/*--------------------------------------------------------------------------
Copyright (c)  2019 hs2-engineering. All rights reserved.
coreTypes.h



Interface
basic types

$Date: 2016-09-28 14:13:45 +0200 (Mi, 28 Sep 2016) $


Author: Steffen Simon / Giovanny Mateus

--------------------------------------------------------------------------*/
/** @file
	Grundlegende Datentypen
*/


//#include <stdint.h>
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>



#ifdef __cplusplus
extern "C" {
#endif

// #ifdef __cplusplus
// 	#ifndef NULL
// 		#define NULL 0
// 	#endif
// #else
// 	#ifndef bool
// 		typedef unsigned char	bool;
// 		#define true (0x01)
// 		#define false (0x00)
// 	#endif
// 	#ifndef NULL
// 		#define NULL ((void*)0)
// 	#endif
// 		
// 		
// 	
// #endif
		
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

typedef int32_t		tick_t;

#ifdef __cplusplus
	}
#endif

#endif // TYPES_H

