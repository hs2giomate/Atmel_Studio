/*--------------------------------------------------------------------------

eDeviceAudio.h

This file is part of e.Lib

Interface
low level interface to the ATmega128, AT90CAN128

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef E_DEVICE_ADC_H
#define E_DEVICE_ADC_H

#include "coreADConverter.h"

class eDeviceADC
	{
	public:
		/** \addtogroup AnalogIn
		*/
		/*@{*/
		int16	operator[](uint8 channel);
		/** Liest einen Analogwert aus.
			\param channel liegt im Bereich (0..7)
			\return Das Ergebniss der Analog/Digitalwandlung
			\param oversampling Dieser Parameter gibt an, wie oft der Eingang abgetastet wird.
			Damit erh�ht sich bei verrauschten Eingangsdaten die nutzbare Aufl�sung,
			indem der Eingang 2^oversampling-mal abgetastet wird und daraus der Mittelwert gebildet wird.
			Der maximal zul�ssige Wert ist 5, dies entspricht 2^5 = 32 Messungen je R�ckgabewert.
			Ein Wert von 0 liest genau eine Messung aus (2^0 = 1).
			\sa ledPins
		*/
		int16	in(uint8 channel);
		/**
			Liest alle Analogeing�nge aus, deren Bit gesetzt ist.
			\param bits liegt im Bereich 0 (bin�r 00000000, kein Eingang)
			bis 255 (bin�r 11111111, alle Eing�nge).
			Gez�hlt wird von Pin 7 (h�chstwertiges Bit, MSB)
			bis Pin 0 (niedrigstwertiges Bit, LSB). Pin 7 ist bin�r 10000000 oder dezimal 128,
			Pin 0 ist bin�r 00000001 oder dezimal 1.
			\param results Ein Array mit acht Elementen f�r die Ergebnisse der Wandlungen.
			Nicht verwendete Eing�nge werden auf 0 gesetzt.
			\param oversampling Dieser Parameter gibt an, wie oft der Eingang abgetastet wird.
			Damit erh�ht sich bei verrauschten Eingangsdaten die nutzbare Aufl�sung,
			indem der Eingang 2^oversampling-mal abgetastet wird und daraus der Mittelwert gebildet wird.
			Der maximal zul�ssige Wert ist 5, dies entspricht 2^5 = 32 Messungen je R�ckgabewert.
			Ein Wert von 0 liest genau eine Messung aus (2^0 = 1).
			\sa analogBitValues
		*/
		void	in(uint8 bits, int16 results[8]);
		
		/**
			Startet die automatische Konvertierung aller Analogeing�nge, deren Bit gesetzt ist.
			\param bits liegt im Bereich 0 (bin�r 00000000, kein Eingang)
			bis 255 (bin�r 11111111, alle Eing�nge).
			Gez�hlt wird von Pin 7 (h�chstwertiges Bit, MSB)
			bis Pin 0 (niedrigstwertiges Bit, LSB). Pin 7 ist bin�r 10000000 oder dezimal 128,
			Pin 0 ist bin�r 00000001 oder dezimal 1.
			\param oversampling Dieser Parameter gibt an, wie oft der Eingang abgetastet wird.
			Damit erh�ht sich bei verrauschten Eingangsdaten die nutzbare Aufl�sung,
			indem der Eingang 2^oversampling-mal abgetastet wird und daraus der Mittelwert gebildet wird.
			Der maximal zul�ssige Wert ist 5, dies entspricht 2^5 = 32 Messungen je R�ckgabewert.
			Ein Wert von 0 liest genau eine Messung aus (2^0 = 1).
			\sa analogBitValues
		*/
		void	enableAutoTrigger(analogAutoTriggerSource trigger, uint8 bits, uint8 oversampling);

		/**
			Beendet die automatische Konvertierung.
		*/
		void	disableAutoTrigger(void);

		/**
			Gibt true zur�ck wenn die automatische Konvertierung aktiviert ist, false sonst.
		*/
		bool	isAutoTriggerEnabled(void);

		/**
			Liest alle Analogeing�nge aus, deren Bits in \ref analogEnableAutoTrigger gesetzt wurden.
			\param results Ein Array mit acht Elementen f�r die Ergebnisse der Wandlungen.
			Nicht verwendete Eing�nge werden auf 0 gesetzt.
			\sa analogEnableAutoTrigger, \sa analogDisableAutoTrigger
		*/
		void	readAutoTrigger(int16 results[8]);
		/*@}*/
	};

inline int16 eDeviceADC::operator[](uint8 channel)
	{
	return coreADConverterReadSingle(channel);
	}

inline int16 eDeviceADC::in(uint8 channel)
	{
	return coreADConverterReadSingle(channel);
	}

inline void	eDeviceADC::in(uint8 bits, int16 results[8])
	{
	coreADConverterReadArray(bits, results);
	}

inline void	eDeviceADC::enableAutoTrigger(analogAutoTriggerSource trigger, uint8 bits, uint8 oversampling)
	{
	coreADConverterEnableAutoTrigger(trigger, bits, oversampling);
	}

inline void	eDeviceADC::disableAutoTrigger(void)
	{
	coreADConverterDisableAutoTrigger();
	}

inline bool	eDeviceADC::isAutoTriggerEnabled(void)
	{
	return coreADConverterIsAutoTriggerEnabled();
	}

inline void	eDeviceADC::readAutoTrigger(int16 results[8])
	{
	coreADConverterReadAutoTriggerArray(results);
	}

#endif

