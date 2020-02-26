/*--------------------------------------------------------------------------

corePorts.h

This file is part of e.Core

Interface

$Date: 2016-10-24 09:50:09 +0200 (Mo, 24 Okt 2016) $
$Revision: 984 $

Copyright (c) Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_PORTS_H
#define CORE_PORTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

#define kNumberOfIOPins		2
#define kNumberOfMuxPins	2

/** Constants for ports
*/
typedef enum
	{
	kPortA = 0,	//!< PORT A
#if defined (__SAMD21G18A__)||defined (__SAMD21J18A__)
	kPortB = 1,	//!< PORT B
#endif
	kNumberOfPorts,
	kPortNone
	} controllerPort;

/** Constants for port bits
*/
typedef enum
	{
	kPortPin0 = 0,	//!< Port pin #0
	kPortPin1,	//!< Port pin #1
	kPortPin2,	//!< Port pin #2
	kPortPin3,	//!< Port pin #3
	kPortPin4,	//!< Port pin #4
	kPortPin5,	//!< Port pin #5
	kPortPin6,	//!< Port pin #6
	kPortPin7,	//!< Port pin #7
	kPortPin8,	//!< Port pin #8
	kPortPin9,	//!< Port pin #9
	kPortPin10,	//!< Port pin #10
	kPortPin11,	//!< Port pin #11
	kPortPin12,	//!< Port pin #12
	kPortPin13,	//!< Port pin #13
	kPortPin14,	//!< Port pin #14
	kPortPin15,	//!< Port pin #15
	kPortPin16,	//!< Port pin #16
	kPortPin17,	//!< Port pin #17
	kPortPin18,	//!< Port pin #18
	kPortPin19,	//!< Port pin #19
	kPortPin20,	//!< Port pin #20
	kPortPin21,	//!< Port pin #21
	kPortPin22,	//!< Port pin #22
	kPortPin23,	//!< Port pin #23
	kPortPin24,	//!< Port pin #24
	kPortPin25,	//!< Port pin #25
	kPortPin26,	//!< Port pin #26
	kPortPin27,	//!< Port pin #27
	kPortPin28,	//!< Port pin #28
	kPortPin29,	//!< Port pin #29
	kPortPin30,	//!< Port pin #30
	kPortPin31,	//!< Port pin #31
	kNumberOfPortPins
	} controllerPortPin;

/** Bit-Constants for port bits
	\sa analogIn()
*/
typedef enum
	{
	kPortBit0 = (1UL << kPortPin0),		//!< Port bit #0
	kPortBit1 = (1UL << kPortPin1),		//!< Port bit #1
	kPortBit2 = (1UL << kPortPin2),		//!< Port bit #2
	kPortBit3 = (1UL << kPortPin3),		//!< Port bit #3
	kPortBit4 = (1UL << kPortPin4),		//!< Port bit #4
	kPortBit5 = (1UL << kPortPin5),		//!< Port bit #5
	kPortBit6 = (1UL << kPortPin6),		//!< Port bit #6
	kPortBit7 = (1UL << kPortPin7),		//!< Port bit #7
	kPortBit8 = (1UL << kPortPin8),		//!< Port bit #8
	kPortBit9 = (1UL << kPortPin9),		//!< Port bit #9
	kPortBit10 = (1UL << kPortPin10),	//!< Port bit #10
	kPortBit11 = (1UL << kPortPin11),	//!< Port bit #11
	kPortBit12 = (1UL << kPortPin12),	//!< Port bit #12
	kPortBit13 = (1UL << kPortPin13),	//!< Port bit #13
	kPortBit14 = (1UL << kPortPin14),	//!< Port bit #14
	kPortBit15 = (1UL << kPortPin15),	//!< Port bit #15
	kPortBit16 = (1UL << kPortPin16),	//!< Port bit #16
	kPortBit17 = (1UL << kPortPin17),	//!< Port bit #17
	kPortBit18 = (1UL << kPortPin18),	//!< Port bit #18
	kPortBit19 = (1UL << kPortPin19),	//!< Port bit #19
	kPortBit20 = (1UL << kPortPin20),	//!< Port bit #20
	kPortBit21 = (1UL << kPortPin21),	//!< Port bit #21
	kPortBit22 = (1UL << kPortPin22),	//!< Port bit #22
	kPortBit23 = (1UL << kPortPin23),	//!< Port bit #23
	kPortBit24 = (1UL << kPortPin24),	//!< Port bit #24
	kPortBit25 = (1UL << kPortPin25),	//!< Port bit #25
	kPortBit26 = (1UL << kPortPin26),	//!< Port bit #26
	kPortBit27 = (1UL << kPortPin27),	//!< Port bit #27
	kPortBit28 = (1UL << kPortPin28),	//!< Port bit #28
	kPortBit29 = (1UL << kPortPin29),	//!< Port bit #29
	kPortBit30 = (1UL << kPortPin30),	//!< Port bit #30
	kPortBit31 = (1UL << kPortPin31),	//!< Port bit #31
	kNumberOfPortBits
	} controllerPortBit;

typedef enum
	{
	kFunctionA = 0,		//!< Port Function A
	kFunctionB = 1,		//!< Port Function B
	kFunctionC = 2,		//!< Port Function C
	kFunctionD = 3,		//!< Port Function D
	kFunctionE = 4,		//!< Port Function E
	kFunctionF = 5,		//!< Port Function F
	kFunctionG = 6,		//!< Port Function G
	kFunctionH = 7,		//!< Port Function H
	} controllerMuxFunction;

typedef enum
	{
	kOutput,
	kOutputLo,
	kOutputHi,

	kInput,
	kInputPullDown,
	kInputPullUp,
	} controllerIOFunction;

typedef struct
	{
	controllerPort			port;
	controllerPortPin		pin;
	controllerMuxFunction	function;
	} controllerMuxPin;

typedef struct
	{
	controllerPort			port;
	controllerPortPin		pin;
	controllerIOFunction	function;
	} controllerIOPin;

#define kPortBitNone	0x00		//!< No bit set
#define kPortBitAll		0xFFFFFFFF	//!< All bits set

typedef struct
	{
	controllerPort		port;	
	controllerPortPin	pin;	
	} controllerPin;

/*@}*/

/** Initialize controller.
*/
void	corePortsInitialize(void);

/** Configure I/O ports.
*/
void	corePortsConfigurePort(controllerPort port, uint32 portMask, uint32 directionMask, uint32 pullupMask, uint32 pulldownMask);

/** Configure  I/O fuction.
*/
void corePortsConfigureIOPinFunction(const controllerIOPin pin);
void corePortsConfigureMUXPinFunction(const controllerMuxPin pin);

/** Configure I/O fuctions.
*/
void	corePortsConfigureIOPinFunctions(uint16 count, const controllerIOPin[]);
void	corePortsConfigureMUXPinFunctions(uint16 count, const controllerMuxPin[]);

/** Get I/O port.
*/
uint32	corePortsGetPort(controllerPort port);

/** Get port direction.
*/
uint32	corePortsGetDirection(controllerPort port);

/** Get I/O port masked.
*/
uint32	corePortsGetPortMasked(controllerPort port, uint32 mask);

/** Set I/O port.
*/
void	corePortsSetPort(controllerPort port, uint32 value);

/** Set I/O port masked.
*/
void	corePortsSetPortMasked(controllerPort port, uint32 mask, uint32 value);

/** Logical OR I/O port.
*/
void	corePortsORPort(controllerPort port, uint32 value);

/** Logical OR I/O port masked.
*/
void	corePortsORPortMasked(controllerPort port, uint32 mask, uint32 value);

/** Logical AND I/O port.
*/
void	corePortsANDPort(controllerPort port, uint32 value);

/** Logical AND I/O port masked.
*/
void	corePortsANDPortMasked(controllerPort port, uint32 mask, uint32 value);

/** Logical XOR I/O port.
*/
void	corePortsXORPort(controllerPort port, uint32 value);

/** Logical XOR I/O port masked.
*/
void	corePortsXORPortMasked(controllerPort port, uint32 mask, uint32 value);

/** Get one bit.
*/
bool	corePortsGetPortPin(controllerPort port, controllerPortPin pin);

/** Set one bit.
*/
void	corePortsSetPortPin(controllerPort port, controllerPortPin pin);

/** Clear one bit.
*/
void	corePortsClearPortPin(controllerPort port, controllerPortPin pin);

/** Toggle one bit.
*/
void	corePortsTogglePortPin(controllerPort port, controllerPortPin pin);

/** Get one bit.
*/
bool	corePortsGetPortBit(controllerPort port, controllerPortBit bit);

/** Set one bit.
*/
void	corePortsSetPortBit(controllerPort port, controllerPortBit bit);

/** Clear one bit.
*/
void	corePortsClearPortBit(controllerPort port, controllerPortBit bit);

/** Toggle one bit.
*/
void	corePortsTogglePortBit(controllerPort port, controllerPortBit bit);

#ifdef __cplusplus
	}
#endif

#endif

