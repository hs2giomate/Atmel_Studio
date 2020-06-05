/*--------------------------------------------------------------------------

corePWM.h

This file is part of e.Development

Interface
low level interface to the ATMega128 pwm

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2006,2007,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef CORE_PWM_H
#define CORE_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "coreTypes.h"

/**
 * \defgroup PWMOut PWM-Ausgänge
*/
/*@{*/
/** Konstanten für die PWM-Ausgänge
*/
#if defined (E_BASIC_Revision)
typedef enum
	{
	kPWM0 = 0,		//!< PWM-Ausgang 1
	kPWM1 = 1,		//!< PWM-Ausgang 2
	kPWM2 = 2,		//!< PWM-Ausgang 3
	kPWM3 = 3,		//!< PWM-Ausgang 4
#if E_BASIC_Revision != 0x0103  && E_BASIC_Revision != 0x0100
	kPWM4 = 4,		//!< PWM-Ausgang 5
#if E_BASIC_Revision == 0x0104
	kPWMAudio = 5,	//!< PWM-Ausgang 6 (Audio)
#endif
#endif
	kPWMPinCount
	} pwmPin;
#elif defined (E_HAPTIC_RADAR_Revision)
typedef enum
	{
	kPWM0 = 0,		//!< PWM-Ausgang 1
	kPWM1 = 1,		//!< PWM-Ausgang 2
	kPWM2 = 2,		//!< PWM-Ausgang 3
	kPWM3 = 3,		//!< PWM-Ausgang 4
	kPWM4 = 4,		//!< PWM-Ausgang 5
	kPWM5 = 5,		//!< PWM-Ausgang 6
	kPWMPinCount
	} pwmPin;
#elif defined (E_PANEL_Revision)
typedef enum
	{
	kPWM0 = 0,		//!< PWM-Ausgang 1
	kPWM1 = 1,		//!< PWM-Ausgang 2
	kPWM2 = 2,		//!< PWM-Ausgang 3
	kPWMPinCount
	} pwmPin;
#elif defined (E_DEVICE_CAN)
typedef enum
	{
	kPWM0 = 0,		//!< PWM-Ausgang 1
	kPWM1 = 1,		//!< PWM-Ausgang 2
	kPWM2 = 2,		//!< PWM-Ausgang 3
	kPWM3 = 3,		//!< PWM-Ausgang 4
	kPWMPinCount
	} pwmPin;
#elif defined (IPECO_PSC_Revision)
typedef enum
	{
	kPWM0 = 0,		//!< PWM-Ausgang 1
#if IPECO_PSC_Revision == 0x0103
	kPWM1 = 1,		//!< PWM-Ausgang 2
#endif
	kPWMPinCount
	} pwmPin;
#elif defined (IPECO_Panel_Revision)
typedef enum
	{
	kPWM0 = 0,	//!< PWM-Ausgang 1
	kPWM1 = 1,	//!< PWM-Ausgang 2
	kPWM2 = 2,	//!< PWM-Ausgang 3
	kPWMPinCount
	} pwmPin;
#else
	#error "Unknown device"
#endif

/** Konstanten für die Frequenz der PWM-Ausgänge
*/
typedef enum
	{
	kPWM7Hz = 0,		//!< 7,03125 Hz (nur kCorePWM1 bis kCorePWM5)
	kPWM14Hz = 1,		//!< 14,0625 Hz (nur kCorePWM1 bis kCorePWM5, 16-Bit Auflösung)
	kPWM28Hz = 2,		//!< 28,125 Hz
	kPWM56Hz = 3,		//!< 56,25 Hz ("single slope")
	kPWM112Hz = 4,		//!< 112,5 Hz (8-Bit Auflösung auf kCorePWM1 bis kCorePWM5!)
	kPWM225Hz = 5,		//!< 225 Hz (kCorePWM1 bis kCorePWM5: "single slope")
	kPWM450Hz = 6,		//!< 450 Hz (kCorePWM1 bis kCorePWM5: 9-Bit Auflösung und "single slope")
	kPWM900Hz = 7,		//!< 900 Hz
	kPWM1800Hz = 8,		//!< 1800 Hz ("single slope")
	kPWM3600Hz = 9,		//!< 3600 Hz (kCorePWM1 bis kCorePWM5: "single slope")
	kPWM7200Hz = 10,	//!< 7200 Hz (kCorePWM0: "single slope")
	kPWM14400Hz = 11,	//!< 14400 Hz (nur kCorePWM1 bis kCorePWM5
	kPWM28800Hz = 12,	//!< 28800 Hz (kCorePWM1 bis kCorePWM5: nur 9-Bit Auflösung)
	kPWM57600Hz = 13	//!< 57600 Hz ("single slope", kCorePWM1 bis kCorePWM5: nur 8-Bit Auflösung)
	} pwmFrequency;
/*@}*/

void corePWMInitialize(void);

/** \addtogroup PWMOut
*/
/*@{*/
/** Konfiguriert den entsprechenden PWM-Ausgang. Diese Methode kann auch aufgerufen
	werden, wenn der enstprechende PWM-Ausgang aktiv ist.
	\param pin liegt im Bereich (0..4). Als Konstanten sind die Werte kPWM0 bis kPWM4 definiert.
	\param frequency liegt im Bereich (1..5) und legt die Frequenz des PWM-Signals fest.
	Die möglichen Frequenzen sind in \ref pwmFrequency definiert.
	\sa pwmBitValues, pwmFrequency, corePWMSetPWMOn, corePWMSetPWMOff, corePWMSetPWMDutyCycle
*/
void corePWMSetPWMFrequency(pwmPin pin, pwmFrequency frequency);

/** Setzt die Pulsbreite (duty cycle) des entsprechenden PWM-Ausgangs.
	Diese Methode kann aufgerufen werden, wenn der enstprechende PWM-Ausgang aktiv ist.
	\param pin liegt im Bereich (0..4). Als Konstanten sind die Werte kPWM0 bis kPWM4 definiert.
	\param dutyCycle setzt die Pulsbreite des  PWM-Signals. Dieser Parameter ist ein vorzeichenloser
	16-Bit-Wert: ein Wert von 0 entspricht einer Pulsbreite von 0%, ein Wert von 65635 einer Pulsbreite
	von 100%.
	Bitte beachten: Die tatsächliche Auflösung der Pulsbreite des Mikrocontrollers ist u.U. geringer: PWM Ausgang 4
	hat eine interne Auflösung von 8 Bit. PWM 0 bis 3 haben, je nach Frequenz, eine Auflösung von 8 bis 16 Bit.
	\sa pwmBitValues, pwmFrequency, corePWMSetPWMFrequency, corePWMSetPWMBits
*/
void corePWMSetPWMDutyCycle(pwmPin pin, uint16 dutyCycle);

uint16 corePWMGetPWMDutyCycle(pwmPin pin);

/**
Schaltet den entsprechende PWM-Ausgang an.
	\param pwm liegt im Bereich (0..4),
	als Konstanten sind die Werte kPWM0 bis kPWM4 definiert.
*/
void corePWMSetPWMOn(pwmPin pwm);

/**
Schaltet den entsprechende PWM-Ausgang aus.
	\param pwm liegt im Bereich (0..4),
	als Konstanten sind die Werte kPWM0 bis kPWM4 definiert.
*/
void corePWMSetPWMOff(pwmPin pwm);

bool corePWMGetPWMState(pwmPin pwm);
/*@}*/

#ifdef __cplusplus
	}
#endif

#endif

