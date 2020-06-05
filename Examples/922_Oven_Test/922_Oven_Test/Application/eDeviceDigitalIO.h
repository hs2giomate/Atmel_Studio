/*--------------------------------------------------------------------------

eDeviceDigitalIO.h

This file is part of e.Lib

Interface
Application interface to the ATmega128, AT90CAN128

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

--------------------------------------------------------------------------*/
#ifndef E_DEVICE_DIGITAL_IO_H
#define E_DEVICE_DIGITAL_IO_H

#if E_BASIC_Revision == 0x0101
	#error "Serial Expansion Hub: Digital IO not supported/Digital IO nicht unterstützt!"
#else
#include <coreTypes.h>
#include <corePorts.h>

/**
 * \defgroup DigitalIO Digitale Ein- und Ausgänge
*/

/** \addtogroup DigitalIO
*/
/*@{*/
class eDeviceDigitalIO
	{
	public:
		/** Initialize I/O port.
		*/
		eDeviceDigitalIO(void);

		/** Konfiguriert den digitalen I/O-Port.
			\param outbits jedes in outbits gesetzte Bit schaltet das entsprechende Pin als Ausgang.
			\param pullups jedes in pullups gesetzte Bit schaltet den Pullupwiderstand des entsprechenden, als Eingang konfiguriertes, Pins ein.
			Gezählt wird von Pin 0  (niedrigstwertiges Bit, LSB)
			bis Pin 7 (höchstwertiges Bit, MSB). Pin 0 ist binär 00000001 oder dezimal 1, Pin 7 ist binär 10000000 oder dezimal 128.
			
			Nach der Initialisierung des Controllers sind alle Pins als Eingänge ohne Pullup konfiguriert!
			
			\example
			configurePort(240, 3) schalten Pin 4 bis Pin 7 als Ausgang, Pin 2 und 3 als Eingang ohne Pullup-Widerstand und Pin 0 und 1 als Eingang mit Pullup-Widerstand.
			Dezimal 240 entspricht binär 11110000, dezimal 3 entspricht binär 00000011.
		*/
		void	configurePort(uint8 directionMask, uint8 pullupMask);

		/** Gibt den Zustand des digitalen Ports zurück.
			\return der Rückgabewert liegt im Bereich 0 (binär 00000000, alle Pins aus)
			bis 255 (binär 11111111, alle Pins an).
			Gezählt wird von Pin 0  (niedrigstwertiges Bit, LSB)
			bis Pin 7 (höchstwertiges Bit, MSB). Pin 0 ist binär 00000001 oder dezimal 1, Pin 7 ist binär 10000000 oder dezimal 128.
		*/
		uint8	getPort(void);

		/** Get I/O port masked.
		*/
		uint8	getPortMasked(uint8 mask);

		/** Setzt alle digitalen Ausgänge mit einem Befehl. Jedes Bit im Parameter pins
			entspricht genau einem Ausgang.
			\param bits liegt im Bereich 0 (binär 00000000, alle Pins aus)
			bis 255 (binär 11111111, alle Pins an).
			Gezählt wird von Pin 0  (niedrigstwertiges Bit, LSB)
			bis Pin 7 (höchstwertiges Bit, MSB). Pin 0 ist binär 00000001 oder dezimal 1, Pin 7 ist binär 10000000 oder dezimal 128.
		*/
		void	setPort(uint8 value);

		/** Set I/O port masked.
		*/
		void	setPortMasked(uint8 mask, uint8 value);

		/** Get one bit.
		*/
		bool	getPortBit(controllerPortPin bit);

		/** Schaltet den entsprechenden Pin an.
			\param pin liegt im Bereich (0..7).
		*/
		void	setPortBit(controllerPortPin pin);

		/** Schaltet den entsprechenden Pin aus.
			\param pin liegt im Bereich (0..7).
		*/
		void	clearPortBit(controllerPortPin pin);

		/** Schaltet den entsprechenden Pin um.
			\param pin liegt im Bereich (0..7).
		*/
		void	togglePortBit(controllerPortPin pin);
	};
/*@}*/

inline void eDeviceDigitalIO::configurePort(uint8 direction, uint8 pullup)
	{
	corePortsConfigurePort(kPortC, kPortBitAll, direction, pullup);
	}

/** Get I/O port.
*/
inline uint8 eDeviceDigitalIO::getPort(void)
	{
	return corePortsGetPort(kPortC);
	}

inline uint8 eDeviceDigitalIO::getPortMasked(uint8 mask)
	{
	return corePortsGetPortMasked(kPortC, mask);
	}

/** Set I/O port.
*/
inline void eDeviceDigitalIO::setPort(uint8 value)
	{
	corePortsSetPort(kPortC, value);
	}

inline void eDeviceDigitalIO::setPortMasked(uint8 mask, uint8 value)
	{
	corePortsSetPortMasked(kPortC, mask, value);
	}


/** Get one bit.
*/
inline bool eDeviceDigitalIO::getPortBit(controllerPortPin pin)
	{
	return corePortsGetPortPin(kPortC, pin) != 0;
	}

inline void eDeviceDigitalIO::setPortBit(controllerPortPin pin)
	{
	corePortsSetPortPin(kPortC, pin);
	}

inline void eDeviceDigitalIO::clearPortBit(controllerPortPin pin)
	{
	corePortsClearPortPin(kPortC, pin);
	}

inline void eDeviceDigitalIO::togglePortBit(controllerPortPin pin)
	{
	corePortsTogglePortPin(kPortC, pin);
	}

#endif
#endif

