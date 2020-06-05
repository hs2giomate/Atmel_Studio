/*--------------------------------------------------------------------------

coreController.h

This file is part of e.Lib

Interface
Application interface eDevice PWM

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef E_DEVICE_PWM_H
#define E_DEVICE_PWM_H

#include "corePWM.h"

class eDevicePWMChannel
	{
	public:
		/** Setzt die Grundfrequenz des entsprechenden PWM-Ausgang. Diese Methode kann auch aufgerufen
			werden, wenn der enstprechende PWM-Ausgang aktiv ist.
			\param pin liegt im Bereich (0..3). Als Konstanten sind die Werte kPWM0 bis kPWM3 definiert.
			\param frequency legt die Frequenz des PWM-Signals fest.
			Die möglichen Frequenzen sind in \ref pwmFrequency definiert.
			
			\sa pwmPin, pwmFrequency, setPWMOn, setPWMOff, setPWMDutyCycle
		*/
		void setFrequency(pwmFrequency frequency);

		/** Setzt die Pulsbreite (duty cycle) des entsprechenden PWM-Ausgangs.
			Diese Methode kann aufgerufen werden, wenn der enstprechende PWM-Ausgang aktiv ist.
			\param pin liegt im Bereich (0..3). Als Konstanten sind die Werte kPWM0 bis kPWM3 definiert.
			\param dutyCycle setzt die Pulsbreite des  PWM-Signals. Dieser Parameter ist ein vorzeichenloser
			16-Bit-Wert: ein Wert von 0 entspricht einer Pulsbreite von 0%, ein Wert von 65635 einer Pulsbreite
			von 100%.

			\sa pwmPin, pwmFrequency, set, on, off, setFrequency
		*/
		void setDutyCycle(uint16 dutyCycle);

		/** Setzt die Pulsbreite (duty cycle) des entsprechenden PWM-Ausgangs.
			Diese Methode kann aufgerufen werden, wenn der enstprechende PWM-Ausgang aktiv ist.
			\param pin liegt im Bereich (0..3). Als Konstanten sind die Werte kPWM0 bis kPWM3 definiert.
			\param dutyCycle setzt die Pulsbreite des  PWM-Signals. Dieser Parameter ist eine Gleitkommazahl:
			ein Wert von 0.0 entspricht einer Pulsbreite von 0%, ein Wert von 100.0 einer Pulsbreite
			von 100%.
			\sa pwmPin, pwmFrequency, set, on, off, setFrequency
		*/
		void setDutyCycle(float dutyCycle);
		void setDutyCycle(double dutyCycle);

		uint16	getDutyCycle(void) const;

		/**
			Schaltet den entsprechende PWM-Ausgang an oder aus.
			\param pwm liegt im Bereich (kPWM0..kPWM4),
		*/
		void set(bool on);

		bool get(void);
		
		/**
			Schaltet den entsprechende PWM-Ausgang an.
			\param pwm liegt im Bereich (0..3),
			als Konstanten sind die Werte kPWM0 bis kPWM4 definiert.
		*/
		void on(void);

		/**
		Schaltet den entsprechende PWM-Ausgang aus.
			\param pwm liegt im Bereich (0..3),
			als Konstanten sind die Werte kPWM0 bis kPWM3 definiert.
		*/
		void off(void);
	
	private:
		friend class eDevicePWM;

		pwmPin pin;
	};


class eDevicePWM
	{
	public:
		eDevicePWMChannel&	operator [](pwmPin idx);
		
	private:
		friend class eDevice;

		/** constructor ;-)
		*/
		eDevicePWM(void);
		
		eDevicePWMChannel	channels[kPWMPinCount];
	};


inline void eDevicePWMChannel::setFrequency(pwmFrequency frequency)
	{
	corePWMSetPWMFrequency(pin, frequency);
	}

inline void eDevicePWMChannel::setDutyCycle(uint16 dutyCycle)
	{
	corePWMSetPWMDutyCycle(pin, dutyCycle);
	}

inline void eDevicePWMChannel::setDutyCycle(float dutyCycle)
	{
	corePWMSetPWMDutyCycle(pin, (uint16)(dutyCycle * 655.36));
	}

inline void eDevicePWMChannel::setDutyCycle(double dutyCycle)
	{
	corePWMSetPWMDutyCycle(pin, (uint16)(dutyCycle * 655.36));
	}

inline uint16 eDevicePWMChannel::getDutyCycle(void) const
	{
	return corePWMGetPWMDutyCycle(pin);
	}

inline void eDevicePWMChannel::set(bool on)
	{
	(on?corePWMSetPWMOn(pin):corePWMSetPWMOff(pin));
	}
	
inline bool eDevicePWMChannel::get(void)
	{
	return corePWMGetPWMState(pin);
	}

inline void eDevicePWMChannel::on(void)
	{
	corePWMSetPWMOn(pin);
	}

inline void eDevicePWMChannel::off(void)
	{
	corePWMSetPWMOff(pin);
	}

inline eDevicePWMChannel& eDevicePWM::operator [](pwmPin idx)
	{
	return channels[idx];
	}

#endif

