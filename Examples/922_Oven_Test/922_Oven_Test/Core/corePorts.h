/*--------------------------------------------------------------------------

corePorts.h

This file is part of e.Development

Interface
low level interface to the ATmega128/AT90CAN128 ports

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2006,2007, 2009, 2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_PORTS_H
#define CORE_PORTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <coreTypes.h>

/** Constants for ports
	\sa corePortsConfigurePort()
*/
typedef enum
	{
	kPortA = 0,	//!< PORTA
	kPortB = 1,	//!< PORTB
	kPortC = 2,	//!< PORTC
	kPortD = 3,	//!< PORTD
	kPortE = 4,	//!< PORTE
	kPortF = 5,	//!< PORTF
	kPortG = 6	//!< PORTG
	} controllerPort;

/** Bit-Constants for port bits
	\sa analogIn()
*/
typedef enum
	{
	kPortPin0 = 0,	//!< Port pin #0
	kPortPin1 = 1,	//!< Port pin #1
	kPortPin2 = 2,	//!< Port pin #2
	kPortPin3 = 3,	//!< Port pin #3
	kPortPin4 = 4,	//!< Port pin #4
	kPortPin5 = 5,	//!< Port pin #5
	kPortPin6 = 6,	//!< Port pin #6
	kPortPin7 = 7	//!< Port pin #7
	} controllerPortPin;

/** Bit-Constants for port bits
	\sa analogIn()
*/
typedef enum
	{
	kPortBit0 = (1 << kPortPin0),	//!< Port bit #0
	kPortBit1 = (1 << kPortPin1),	//!< Port bit #1
	kPortBit2 = (1 << kPortPin2),	//!< Port bit #2
	kPortBit3 = (1 << kPortPin3),	//!< Port bit #3
	kPortBit4 = (1 << kPortPin4),	//!< Port bit #4
	kPortBit5 = (1 << kPortPin5),	//!< Port bit #5
	kPortBit6 = (1 << kPortPin6),	//!< Port bit #6
	kPortBit7 = (1 << kPortPin7)	//!< Port bit #7
	} controllerPortBit;

#define kPortBitNone	0x00	//!< No bit set
#define kPortBitAll		0xFF	//!< All bits set

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
void	corePortsConfigurePort(controllerPort port, uint8 portMask, uint8 directionMask, uint8 pullupMask);

/** Get I/O port.
*/
uint8	corePortsGetPort(controllerPort port);

/** Get port direction.
*/
uint8	corePortsGetDirection(controllerPort port);

/** Get I/O port masked.
*/
uint8	corePortsGetPortMasked(controllerPort port, uint8 mask);

/** Set I/O port.
*/
void	corePortsSetPort(controllerPort port, uint8 value);

/** Set I/O port masked.
*/
void	corePortsSetPortMasked(controllerPort port, uint8 mask, uint8 value);

/** Logical OR I/O port.
*/
void	corePortsORPort(controllerPort port, uint8 value);

/** Logical OR I/O port masked.
*/
void	corePortsORPortMasked(controllerPort port, uint8 mask, uint8 value);

/** Logical AND I/O port.
*/
void	corePortsANDPort(controllerPort port, uint8 value);

/** Logical AND I/O port masked.
*/
void	corePortsANDPortMasked(controllerPort port, uint8 mask, uint8 value);

/** Logical XOR I/O port.
*/
void	corePortsXORPort(controllerPort port, uint8 value);

/** Logical XOR I/O port masked.
*/
void	corePortsXORPortMasked(controllerPort port, uint8 mask, uint8 value);

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

