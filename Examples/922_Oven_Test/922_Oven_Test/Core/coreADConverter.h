/*--------------------------------------------------------------------------

coreADConverter.h

This file is part of e.Development

Interface
low level interface to the ATmega128/AT90CAN128 internal A/D converter

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2006,2007,2008,2009 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_AD_CONVERTER_H
#define CORE_AD_CONVERTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/**
 * \defgroup AnalogIn A/D conversion
*/
/*@{*/
/**	Constants for auto trigger source
	\sa coreADConverterEnableAutoTrigger
*/
typedef enum
	{
	kNoAutoTrigger = 0,				//!<	Auto trigger off
	kFreeRunningMode,				//!<	Free Running mode
#if defined (__AVR_AT90CAN128__)
	kAnalogComparator,				//!<	Analog Comparator
	kExternalInterruptRequest0,		//!<	External Interrupt Request 0
	kTimerCounter0CompareMatch,		//!<	Timer/Counter0 Compare Match
	kTimerCounter0Overflow,			//!<	Timer/Counter0 Overflow
	kTimerCounter1CompareMatchB,	//!<	Timer/Counter1 Compare Match B
	kTimerCounter1Overflow,			//!<	Timer/Counter1 Overflow
	kTimerCounter1CaptureEvent		//!<	Timer/Counter1 Capture Event
#endif
	} analogAutoTriggerSource;

/**	Constants for oversampling of the analogue inputs
	\sa coreADConverterReadSingle, coreADConverterReadArray, coreADConverterEnableAutoTrigger
*/
typedef enum
	{
	kNoOversampling = 0,	//!< Single conversion, no oversampling
	kOversampling4 = 1,		//!< 4-times oversampling => 1 bit enhancement (4^1)
	kOversampling16 = 2,	//!< 16-times oversampling => 2 bit enhancement (4^2)
	kOversampling64 = 3,	//!< 64-times oversampling => 3 bit enhancement (4^3)
	kOversampling256 = 4	//!< 256-times oversampling => 4 bit enhancement (4^4)
/*
	kOversampling16 = 4,	//!< 16-times oversampling
	kOversampling32 = 5,	//!< 32-times oversampling
	kOversampling64 = 6,	//!< 64-times oversampling
	kOversampling128 = 7,	//!< 128-times oversampling
	kOversampling256 = 8	//!< 256-times oversampling
*/
	} analogOversampling;

/** Konstanten f�r die digitalen Ein-/Ausg�nge
	\sa analogIn()
*/
typedef enum
	{
	kAnalogPort0 = 0,	//!< Analogeingang 1
	kAnalogPort1 = 1,	//!< Analogeingang 2
	kAnalogPort2 = 2,	//!< Analogeingang 3
	kAnalogPort3 = 3,	//!< Analogeingang 4
	kAnalogPort4 = 4,	//!< Analogeingang 5
	kAnalogPort5 = 5,	//!< Analogeingang 6
	kAnalogPort6 = 6,	//!< Analogeingang 7
	kAnalogPort7 = 7	//!< Analogeingang 8
	} analogPinValues;
/** Bit-Konstanten für die Analogeingänge
	\sa analogIn()
*/
typedef enum
	{
	kBitAnalog0 = (1 << 0),	//!< Analogeingang 1
	kBitAnalog1 = (1 << 1),	//!< Analogeingang 2
	kBitAnalog2 = (1 << 2),	//!< Analogeingang 3
	kBitAnalog3 = (1 << 3),	//!< Analogeingang 4
	kBitAnalog4 = (1 << 4),	//!< Analogeingang 5
	kBitAnalog5 = (1 << 5),	//!< Analogeingang 6
	kBitAnalog6 = (1 << 6),	//!< Analogeingang 7
	kBitAnalog7 = (1 << 7),	//!< Analogeingang 8
	kBitAnalogAll = 0xFF		//!< Alle Analogeingänge
	} analogBitValues;
/*@}*/

/** \addtogroup AnalogIn
*/
/*@{*/
/** A/D converter raw data array
*/
extern volatile int16	analogResultArray[8];
extern volatile uint8	analogConversionReadyMask;

/** A/D converter interface initializer
*/
void coreADConverterInitialize(void);

/** A/D converter interface initializer
*/
void coreADConverterRelease(void);

/** Liest einen Analogwert aus.
	\param channel liegt im Bereich (0..7)
	\return Das Ergebniss der Analog/Digitalwandlung
*/
int16 coreADConverterReadSingle(uint8 channel);

/**
	Liest alle Analogeingänge aus, deren Bit gesetzt ist.
	\param bits liegt im Bereich 0 (binär 00000000, kein Eingang)
	bis 255 (binär 11111111, alle Eingänge).
	Gezählt wird von Pin 7 (höchstwertiges Bit, MSB)
	bis Pin 0 (niedrigstwertiges Bit, LSB). Pin 7 ist binär 10000000 oder dezimal 128,
	Pin 0 ist binär 00000001 oder dezimal 1.
	\param results Ein Array mit acht Elementen für die Ergebnisse der Wandlungen.
	Nicht verwendete Eingänge werden auf 0 gesetzt.
	\sa analogBitValues
*/
void coreADConverterReadArray(uint8 bits, int16 results[8]);

/**
	Liest alle Analogeingänge aus, deren Bit gesetzt ist.
	\param bits liegt im Bereich 0 (binär 00000000, kein Eingang)
	bis 255 (binär 11111111, alle Eingänge).
	Gezählt wird von Pin 7 (höchstwertiges Bit, MSB)
	bis Pin 0 (niedrigstwertiges Bit, LSB). Pin 7 ist binär 10000000 oder dezimal 128,
	Pin 0 ist binär 00000001 oder dezimal 1.
	\param results Ein Array mit acht Elementen für die Ergebnisse der Wandlungen.
	Nicht verwendete Eingänge werden auf 0 gesetzt.
	\param oversampling Dieser Parameter gibt an, wie oft der Eingang abgetastet wird.
	Damit erhöht sich bei verrauschten Eingangsdaten die nutzbare Auflösung,
	indem der Eingang 2^oversampling-mal abgetastet wird und daraus der Mittelwert gebildet wird.
	Der maximal zulässige Wert ist 5, dies entspricht 2^5 = 32 Messungen je Rückgabewert.
	Ein Wert von 0 liest genau eine Messung aus (2^0 = 1).
	\sa analogBitValues
*/
void coreADConverterEnableAutoTrigger(analogAutoTriggerSource trigger, uint8 bits, uint8 oversampling);
	
void coreADConverterDisableAutoTrigger(void);

bool coreADConverterIsAutoTriggerEnabled(void);

void coreADConverterReadAutoTriggerArray(int16 results[8]);

uint8 coreADConverterGetResolution(void);

int16 coreADConverterGetMaximumValue(void);
/*@}*/

#ifdef __cplusplus
	}
#endif

#endif

