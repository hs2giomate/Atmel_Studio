/*--------------------------------------------------------------------------

corePorts.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the ATmega644P, ATmega128, AT90CAN128

$Author: steffen $
$Date: 2017-03-01 13:01:46 +0100 (Mi, 01. Mrz 2017) $
$Revision: 7240 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/corePorts.c $

--------------------------------------------------------------------------*/
#include <avr/io.h>
#include "corePorts.h"

volatile uint8*	PINValue[7] = {&PINA,&PINB,&PINC,&PIND,&PINE,&PINF,&PING};
volatile uint8*	DDRValue[7] = {&DDRA,&DDRB,&DDRC,&DDRD,&DDRE,&DDRF,&DDRG};
volatile uint8*	PORTValue[7] = {&PORTA,&PORTB,&PORTC,&PORTD,&PORTE,&PORTF,&PORTG};

#define PIN(x)	(*PINValue[x])
#define DDR(x)	(*DDRValue[x])
#define PORT(x)	(*PORTValue[x])

void corePortsInitialize(void)
	{
	//	Configure ports:
	corePortsConfigurePort(kPortA, kPortBitAll, kPortBitNone, kPortBitNone);
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBitNone, kPortBitNone);
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBitNone, kPortBitNone);
	corePortsConfigurePort(kPortD, kPortBitAll, kPortBitNone, kPortBitNone);
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBitNone, kPortBitNone);
	corePortsConfigurePort(kPortF, kPortBitAll, kPortBitNone, kPortBitNone);
	corePortsConfigurePort(kPortG, kPortBitAll, kPortBitNone, kPortBitNone);

#if E_DEVICE_CAN == 0x0100
	//	Port A: Power out PA0...PA3, Expansion ports slave select/reset PA4...PA7
	corePortsConfigurePort(kPortA, kPortBitAll, kPortBitAll, kPortBitNone);
	//	Set /RESET Expansion high
	corePortsSetPortBit(kPortA, kPortBit5);
	//	Set /RESET Expansion high
	corePortsSetPortBit(kPortA, kPortBit6);
    //	Port B: Power out #4, #5, #6 PB4...PB6, Crumb LED PB7
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBit4|kPortBit5|kPortBit6|kPortBit7, kPortBitNone);
    //	Port C: GPIO
#ifdef OFFLINE
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBitNone, kPortBit7);
#else
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBitNone, kPortBitNone);
#endif
	//	Port E: Pullup for bootloader pin (PE6), Power out #7 (PE3), /SS (PE6)
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBit3|kPortBit7, kPortBit6);
	//	Set /SS high for Master SPI operation
	corePortsSetPortBit(kPortE, kPortBit7);
	//	Onboard LEDs: port G, pin 0 - 2
	corePortsConfigurePort(kPortG, kPortBitAll, kPortBit0|kPortBit1|kPortBit2, kPortBit0|kPortBit1|kPortBit2);
#elif E_HAPTIC_RADAR_Revision
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBit0|kPortBit1|kPortBit6|kPortBit7, kPortBitNone);
	// Pullup for button pins
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBitNone, kPortBit6|kPortBit7);
#elif E_BASIC_Revision == 0x0101
	//	Power out: port A, pin 0 - 7
	corePortsConfigurePort(kPortA, kPortBitAll, kPortBitAll, kPortBitNone);
    //	Port B: Crumb LED PB7
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBit7, kPortBitNone);
	// Pullup for bootloader pin
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBitNone, kPortBit2);
	// Onboard LEDs: port G, pin 0 - 2
	corePortsConfigurePort(kPortG, kPortBitAll, kPortBit0|kPortBit1|kPortBit2, kPortBitNone);
#elif E_BASIC_Revision == 0x0103
	//	Port A: Power out PA0...PA3, Expansion ports slave select/reset PA4...PA7
	corePortsConfigurePort(kPortA, kPortBitAll, kPortBitAll, kPortBitNone);
    //	Port B: Power out #4, #5, #6 PB4...PB6, Crumb LED PB7
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBit4|kPortBit5|kPortBit6|kPortBit7, kPortBitNone);
    //	Port C: GPIO
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBitNone, kPortBitNone);
	//	Port E: Pullup for bootloader pin (PE6), Power out #7 (PE3)
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBit3, kPortBit6);
	//	Onboard LEDs: port G, pin 0 - 2
	corePortsConfigurePort(kPortG, kPortBitAll, kPortBit0|kPortBit1|kPortBit2, kPortBit0|kPortBit1|kPortBit2);
#elif E_BASIC_Revision == 0x0104
	//	Port A: Power out PA0...PA2, Expansion ports slave select/reset PA4...PA7
	corePortsConfigurePort(kPortA, kPortBitAll, kPortBit0|kPortBit1|kPortBit2|kPortBit4|kPortBit5|kPortBit6|kPortBit7, kPortBitNone);
    //	Port B: Audio Out PB4, power out #3, #4 PB5, PB6, Crumb LED PB7
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBit4|kPortBit5|kPortBit6|kPortBit7, kPortBitNone);
    //	Port C: GPIO
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBitNone, kPortBitNone);
    //	Port D: Audio Volume controll PD4...PD7
	corePortsConfigurePort(kPortD, kPortBitAll, kPortBit4|kPortBit5|kPortBit6|kPortBit7, kPortBitNone);
	//	Port E: Pullup for bootloader pin (PE2), Power out #5 (PE3), Power out #6 (PE4), Power out #7 (PE5)
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBit3|kPortBit4|kPortBit5, kPortBit2);
	// Onboard LEDs: port G, pin 0 - 2
	corePortsConfigurePort(kPortG, kPortBitAll, kPortBit0|kPortBit1|kPortBit2, kPortBitNone);
#elif IPECO_PSC_Revision == 0x0101
	//	Port B: PB5 /CS EEPROM, PB6 Onboard LED
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBit5|kPortBit6, kPortBitNone);
	//	Set /CS EEPROM high
	corePortsSetPortBit(kPortB, kPortBit5);
	// Port E: PE2 Micropump, PE3 3-way valve, PE4/PE5/PE6 relays
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBit2|kPortBit3|kPortBit4|kPortBit5|kPortBit6, kPortBitNone);
#elif IPECO_PSC_Revision == 0x0102
	//	Port B: PB5 /CS EEPROM, PB6 Onboard LED
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBit5|kPortBit6, kPortBitNone);
	//	Set /CS EEPROM high
	corePortsSetPortBit(kPortB, kPortBit5);
	// Port E: PE2 Micropump, PE3 3-way valve, PE4/PE5/PE6 relays
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBit2|kPortBit3|kPortBit4|kPortBit5|kPortBit6, kPortBitNone);
#elif IPECO_PSC_Revision == 0x0103
	//	Port A: PA7 Safety relais, PA6 Relay #2, PA5 Relay #1, PA3 diverter valve as outputs
	corePortsConfigurePort(kPortA, kPortBitAll, kPortBit3|kPortBit5|kPortBit6|kPortBit7, kPortBitNone);
	//	Port B: PB0 24V on/off, PB4 Fan #3, PB5 /CS EEPROM, PB6 onboard LED, PB7 proportional valve
	corePortsConfigurePort(kPortB, kPortBitAll, kPortBit0|kPortBit4|kPortBit5|kPortBit6|kPortBit7, kPortBitNone);
	//	Set /CS EEPROM high
	corePortsSetPortBit(kPortB, kPortBit5);
#ifdef E_DEVICE
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBitNone, kPortBit0|kPortBit1|kPortBit2|kPortBit3);
#else
	//	Port C: PC0 /CS FRAM, PC1 micropump timer short, PC2 3 way valve timer short, PC3 Diverter Valve Timer long
	corePortsConfigurePort(kPortC, kPortBitAll, kPortBit0|kPortBit1|kPortBit2|kPortBit3, kPortBitNone);
	//	Set /CS FRAM high
	corePortsSetPortBit(kPortC, kPortBit0);
#endif
	//	Port E: PE2 micro pump outputs, PE3 Three Way Valve
	corePortsConfigurePort(kPortE, kPortBitAll, kPortBit2|kPortBit3, kPortBitNone);
	//	Port G: PG3 CP Power on/off, PG4 fan #2
#ifdef NO_RTC
	corePortsConfigurePort(kPortG, kPortBitAll,  kPortBit0|kPortBit1|kPortBit2|kPortBit3|kPortBit4, kPortBitNone);
	corePortsSetPort(kPortG, 0x07);
#else
	corePortsConfigurePort(kPortG, kPortBitAll, kPortBit3|kPortBit4, kPortBitNone);
	corePortsSetPort(kPortG, 0x00);
#endif
#else
	#warning "Unknown device"
#endif
	}

void corePortsConfigurePort(controllerPort port, uint8 portMask, uint8 direction, uint8 pullup)
	{
	DDR(port) = (DDR(port) & ~portMask)|(direction & portMask);
	PORT(port) = (PORT(port) & ~portMask)|((~direction & pullup) & portMask);
	}

/** Get I/O port.
*/
uint8 corePortsGetPort(controllerPort port)
	{
	uint8	portValue;
	
	portValue = PIN(port) & ~DDR(port);
	portValue |= PORT(port) & DDR(port);
	
	return portValue;
	}

uint8 corePortsGetDirection(controllerPort port)
	{
	uint8	portValue;
	
	portValue = DDR(port);
	
	return portValue;
	}

uint8 corePortsGetPortMasked(controllerPort port, uint8 mask)
	{
	uint8	portValue;
	
	portValue = PIN(port) & ~DDR(port);
	portValue |= PORT(port) & DDR(port);
	portValue &= mask;
	
	return portValue;
	}

/** Set I/O port.
*/
void corePortsSetPort(controllerPort port, uint8 value)
	{
	uint8	portValue;
	uint8	mask;
	
	/* Create bitmap for all pins configured as output... */
	mask = DDR(port);
	portValue = PIN(port) & ~mask;
	portValue |= (mask & value);
	PORT(port) = portValue;
	}

void corePortsSetPortMasked(controllerPort port, uint8 mask, uint8 value)
	{
	uint8	portValue;
	
	mask = mask & DDR(port);
	portValue = PORT(port) & ~mask;
	portValue |= (mask & value);
	PORT(port) = portValue;
	}

void corePortsORPort(controllerPort port, uint8 value)
	{
	PORT(port) |= value;
	}

void corePortsORPortMasked(controllerPort port, uint8 mask, uint8 value)
	{
	PORT(port) |= (mask & value);
	}

void corePortsANDPort(controllerPort port, uint8 value)
	{
	PORT(port) &= value;
	}

void corePortsANDPortMasked(controllerPort port, uint8 mask, uint8 value)
	{
	uint8	portValue;
	
	portValue = PORT(port) & ~mask;
	portValue |= (PORT(port) & mask & value);
	PORT(port) = portValue;
	}

/** Logical XOR I/O port.
*/
void corePortsXORPort(controllerPort port, uint8 value)
	{
	uint8	portValue;
	
	portValue = PORT(port) ^ value;
	PORT(port) = portValue;
	}

/** Logical XOR I/O port masked.
*/
void corePortsXORPortMasked(controllerPort port, uint8 mask, uint8 value)
	{
	uint8	portValue;
	
	portValue = PORT(port) & ~mask;
	portValue ^= (mask & value);
	PORT(port) = portValue;
	}

/** Get one bit.
*/
bool corePortsGetPortPin(controllerPort port, controllerPortPin pin)
	{
	bool	result;
	
	if (DDR(port) & (1 << pin))
		result = ((PORT(port) & (1 << pin))?true:false);
	else
		result = ((PIN(port) & (1 << pin))?true:false);
	
	return result;
	}

void corePortsSetPortPin(controllerPort port, controllerPortPin pin)
	{
	if (DDR(port) & (1 << pin))
		PORT(port) |= (1 << pin);
	}

void corePortsClearPortPin(controllerPort port, controllerPortPin pin)
	{
	if (DDR(port) & (1 << pin))
		PORT(port) &= ~(1 << pin);
	}

void corePortsTogglePortPin(controllerPort port, controllerPortPin pin)
	{
	if (DDR(port) & (1 << pin))
		PORT(port) ^= (1 << pin);
	}

/** Get one bit.
*/
bool corePortsGetPortBit(controllerPort port, controllerPortBit bit)
	{
	bool	result;
	
	if (DDR(port) & bit)
		result = ((PORT(port) & bit)?true:false);
	else
		result = ((PIN(port) & bit)?true:false);
	
	return result;
	}

void corePortsSetPortBit(controllerPort port, controllerPortBit bit)
	{
	if (DDR(port) & bit)
		PORT(port) |= bit;
	}

void corePortsClearPortBit(controllerPort port, controllerPortBit bit)
	{
	if (DDR(port) & bit)
		PORT(port) &= ~bit;
	}

void corePortsTogglePortBit(controllerPort port, controllerPortBit bit)
	{
	if (DDR(port) & bit)
		PORT(port) ^= bit;
	}

