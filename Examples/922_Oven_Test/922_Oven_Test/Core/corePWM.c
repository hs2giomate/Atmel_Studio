/*--------------------------------------------------------------------------

corePWM.cc

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the ATMega128 PWM controller

$Author: steffen $
$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/corePWM.c $

--------------------------------------------------------------------------*/
#include <avr/io.h>
#include "corePorts.h"
#include "corePWM.h"
#include "coreSystemTimer.h"

static	uint8	pwmFlags[kPWMPinCount];
static	uint8	pwmState[kPWMPinCount];
static	uint16	pwmDutyCycle[kPWMPinCount];
	
static const uint8	pwm0deviderMap[14] = {0, 0, 7, 128 + 7, 6, 5, 4, 3, 128 + 3, 2, 128 + 2, 0, 1, 128 + 1};
static const uint8	pwm1deviderMap[14] = {48 + 5, 2, 48 + 4, 128 + 48 + 4,
									1, 128 + 48 + 3, 128 + 32 + 3,
									48 + 2, 128 + 48 + 2, 128 + 32 + 2,
									48 + 1, 128 + 48 + 1, 128 + 32 + 1, 128 + 16 + 1};

typedef enum
	{
	kCorePWM0 = 0,	//!< PWM 1	OC0/OC0A
	kCorePWM1,		//!< PWM 2  OC0B
	kCorePWM2,		//!< PWM 3  OC0C
	
	kCorePWM3,		//!< PWM 4  OC1A
	kCorePWM4,		//!< PWM 5  OC1B
	kCorePWM5,		//!< PWM 6  OC1C

	kCorePWM6,		//!< PWM 7  OC2/OC2A
	kCorePWM7,		//!< PWM 8  OC2B
	kCorePWM8,		//!< PWM 9  OC2C

	kCorePWM9,		//!< PWM 10  OC3A
	kCorePWM10,		//!< PWM 11  OC3B
	kCorePWM11,		//!< PWM 12  OC3C
	} corePWM;

#if defined (E_BASIC_Revision)
	#if E_BASIC_Revision == 0x0101
		#define CORE_CONFIGURE_PWM_OC3A
		#define CORE_CONFIGURE_PWM_OC3B
		#define CORE_CONFIGURE_PWM_OC1B
		#define CORE_CONFIGURE_PWM_OC1A
		#define CORE_CONFIGURE_PWM_OC0
		static const uint8	pwmLookup[] = {kCorePWM9, kCorePWM10, kCorePWM4, kCorePWM3, kCorePWM0};
	#elif E_BASIC_Revision == 0x0103
		#define CORE_CONFIGURE_PWM_OC0
		#define CORE_CONFIGURE_PWM_OC1A
		#define CORE_CONFIGURE_PWM_OC1B
		#define CORE_CONFIGURE_PWM_OC3A
		static const uint8	pwmLookup[] = {kCorePWM0, kCorePWM3, kCorePWM4, kCorePWM9};
	#elif E_BASIC_Revision == 0x0104
		#define CORE_CONFIGURE_PWM_OC1A
		#define CORE_CONFIGURE_PWM_OC1B
		#define CORE_CONFIGURE_PWM_OC3A
		#define CORE_CONFIGURE_PWM_OC3B
		#define CORE_CONFIGURE_PWM_OC3C
		#define CORE_CONFIGURE_PWM_OC0
		static const uint8	pwmLookup[] = {kCorePWM3, kCorePWM4, kCorePWM9, kCorePWM10, kCorePWM11, kCorePWM0};
	#else
		#error "Unknown e.Device basic"
	#endif
#elif defined (E_HAPTIC_RADAR_Revision)
		#define CORE_CONFIGURE_PWM_OC1A
		#define CORE_CONFIGURE_PWM_OC1B
		#define CORE_CONFIGURE_PWM_OC1C
		#define CORE_CONFIGURE_PWM_OC3A
		#define CORE_CONFIGURE_PWM_OC3B
		#define CORE_CONFIGURE_PWM_OC3C
		static const uint8	pwmLookup[] = {kCorePWM3, kCorePWM4, kCorePWM5, kCorePWM9, kCorePWM10, kCorePWM11};
#elif defined (E_DEVICE_CAN)
	#if E_DEVICE_CAN == 0x0100
		#define CORE_CONFIGURE_PWM_OC2
		#define CORE_CONFIGURE_PWM_OC1A
		#define CORE_CONFIGURE_PWM_OC1B
		#define CORE_CONFIGURE_PWM_OC3A
		static const uint8	pwmLookup[] = {kCorePWM6, kCorePWM3, kCorePWM4, kCorePWM9};
	#else
		#error "Unknown e.Device CAN"
	#endif
#elif defined (E_PANEL_Revision)
	#if E_PANEL_Revision == 0x0100
		#define CORE_CONFIGURE_PWM_OC1A
		#define CORE_CONFIGURE_PWM_OC1B
		#define CORE_CONFIGURE_PWM_OC3A
		static const uint8	pwmLookup[] = {kCorePWM3, kCorePWM4, kCorePWM9};
	#else
		#error "Unknown e.Device Panel"
	#endif
#elif defined (IPECO_PSC_Revision)
	#if IPECO_PSC_Revision < 0x0103
		#define CORE_CONFIGURE_PWM_OC2
		static const uint8	pwmLookup[] = {kCorePWM6};
	#elif IPECO_PSC_Revision == 0x0103
		#define CORE_CONFIGURE_PWM_OC1C
		#define CORE_CONFIGURE_PWM_OC2
		static const uint8	pwmLookup[] = {kCorePWM5, kCorePWM6};
	#else
		#error "Unknown Ipeco PSC"
	#endif
#elif defined (IPECO_Panel_Revision)
	#if IPECO_Panel_Revision == 0x0103
		#define CORE_CONFIGURE_PWM_OC1A
		#define CORE_CONFIGURE_PWM_OC2A
		#define CORE_CONFIGURE_PWM_OC2B
		static const uint8	pwmLookup[] = {kCorePWM3, kCorePWM7, kCorePWM6};
	#else
		#error "Unknown Ipeco Panel"
	#endif
#else
	#error "Unknown device"
#endif

//	Extra PWM (nur Rev 1.3!)
//	_____________		_____________
//	| o o o o o |		| o o o o o |
//	| o x o o o |		| o x o o o |
//	-----   -----		-----   -----
//	PWM #5				PWM #4
#if defined (__AVR_ATmega128__) && defined (CORE_CONFIGURE_PWM_OC2) && defined (CORE_CONFIGURE_PWM_OC1C)
	#error ATmega128: only OC1C or OC2 can be configured
#endif

#if defined (__AVR_AT90CAN128__) && defined (CORE_CONFIGURE_PWM_OC0) && defined (CORE_CONFIGURE_PWM_OC1C)
	#error AT90CAN128: only OC1C or OC0 can be configured
#endif


void corePWMInitialize(void)
	{
	uint8	i;

#if defined (CORE_CONFIGURE_PWM_OC0)||(CORE_CONFIGURE_PWM_OC0A)||defined (CORE_CONFIGURE_PWM_OC0B)||defined (CORE_CONFIGURE_PWM_OC0C)
#if defined (__AVR_ATmega128__)
	//	Timer #0 initialisieren (OC0):
	TCCR0 = 0;					// Clear TCCR0
	TCNT0 = 0;					// Clear TCNT0	
#elif defined (__AVR_AT90CAN128__)
	//	Timer #0 initialisieren (OC0A):
	TCCR0A = 0;					// Clear TCCR0
	TCNT0 = 0;					// Clear TCNT0	
#elif defined (__AVR_ATmega644P__)
	//	Timer #0 initialisieren (OC0A/OC0B):
	TCCR0A = 0;					// Clear TCCR0
	TCNT0 = 0;					// Clear TCNT0	
#endif
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC0)
#if defined (__AVR_ATmega128__)
	//	PWM #0 initialisieren:
	//	OCR0 löschen
	OCR0 = 0;
	//	PWM #0 auf Pin PINB4/OC0 als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit4, kPortBit4, kPortBitNone);
#else
	#error "OC0 not supported on this device"
#endif
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC0A)
#if defined (__AVR_AT90CAN128__)
	//	PWM #0 initialisieren:
	//	OCR0A löschen
	OCR0A = 0;
	//	PWM #0 auf Pin PINB7/OCR0A als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit7, kPortBit7, kPortBitNone);
#elif defined (__AVR_ATmega644P__)
	//	PWM #0 initialisieren:
	//	OCR0A löschen
	OCR0A = 0;
	//	PWM #0 auf Pin PINB4/OCR0A als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit3, kPortBit3, kPortBitNone);
#else
	#error "OC0A not supported on this device"
#endif
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC0B)
#if defined (__AVR_ATmega644P__)
	//	PWM #1 initialisieren:
	//	OCR0B löschen
	OCR0B = 0;
	//	PWM #0 auf Pin PINB4/OCR0B als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit4, kPortBit4, kPortBitNone);
#else
	#error "OC0B not supported on this device"
#endif
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC0C)
	//	PWM #2 initialisieren:
	#error "OC0B not supported on this device"
#endif

#if defined (CORE_CONFIGURE_PWM_OC1A)||defined (CORE_CONFIGURE_PWM_OC1B)||defined (CORE_CONFIGURE_PWM_OC1C)
	//	PWM #3, PWM #4, PWM #5 konfigurieren
	//	Timer #1 initialisieren (OC1A/OC1B/OC1C):
	TCCR1A = 0;					// Clear TCCR1A
	TCCR1B = 0;					// Clear TCCR1B
	TCNT1H = 0;					// Clear TCNT1
	TCNT1L = 0;					// Clear TCNT1
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC1A)
	//	PWM #3 initialisieren:
	//	OCR1A löschen
	OCR1A = 0;
#if defined (__AVR_ATmega128__) || defined (__AVR_AT90CAN128__)
	//	PWM #3 auf Pin PINB5/OC1A als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit5, kPortBit5, kPortBitNone);
#elif defined (__AVR_ATmega644P__)
	//	PWM #3 auf Pin PIND5/OC1A als Ausgang konfigurieren
	corePortsConfigurePort(kPortD, kPortBit5, kPortBit5, kPortBitNone);
#else
	#error "OC1A not supported on this device"
#endif
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC1B)
	//	PWM #4 initialisieren:
	//	OCR1B löschen
	OCR1B = 0;
#if defined (__AVR_ATmega128__) || defined (__AVR_AT90CAN128__)
	//	PWM #4 auf Pin PINB6/OC1B als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit6, kPortBit6, kPortBitNone);
#elif defined (__AVR_ATmega644P__)
	//	PWM #4 auf Pin PIND4/OC1B als Ausgang konfigurieren
	corePortsConfigurePort(kPortD, kPortBit4, kPortBit4, kPortBitNone);
#else
	#error "OC1B not supported on this device"
#endif
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC1C)
	//	PWM #5 initialisieren:
	//	OCR1C löschen
	OCR1C = 0;
#if defined (__AVR_ATmega128__) || defined (__AVR_AT90CAN128__)
	//	PWM #5 auf Pin PINB7/OC1C als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit7, kPortBit7, kPortBitNone);
#else
	#error "OC1C not supported on this device"
#endif
#endif

#if defined (CORE_CONFIGURE_PWM_OC2) || defined(CORE_CONFIGURE_PWM_OC2A) || defined(CORE_CONFIGURE_PWM_OC2B) || defined(CORE_CONFIGURE_PWM_OC2C)
#if defined (__AVR_ATmega128__)
	//	Timer #2 initialisieren (OC2):
	TCCR2 = 0;					// Clear TCCR2
	TCNT2 = 0;					// Clear TCNT2	
#elif defined (__AVR_AT90CAN128__) || defined (__AVR_ATmega644P__)
	//	Timer #2 initialisieren (OC2A/OC2B):
	TCCR2A = 0;					// Clear TCCR2A
#if defined (__AVR_ATmega644P__)
	TCCR2B = 0;					// Clear TCCR2B
#endif
	TCNT2 = 0;					// Clear TCNT2	
#endif
#endif

#if defined (CORE_CONFIGURE_PWM_OC2)
#if defined (__AVR_ATmega128__)
	//	OCR2 löschen
	OCR2 = 0;
	//	PWM #6 auf Pin PINB7/OC2 als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit7, kPortBit7, kPortBitNone);
#elif defined (__AVR_AT90CAN128__)
	OCR2A = 0;
	//	PWM #6 auf Pin PINB4/OCR2A als Ausgang konfigurieren
	corePortsConfigurePort(kPortB, kPortBit4, kPortBit4, kPortBitNone);
#else
	#error "OC2 not supported on this device"
#endif
#endif

#if defined (CORE_CONFIGURE_PWM_OC2A)
	//	PWM #6 initialisieren:
	//	OCR2A löschen
	OCR2A = 0;
#if defined (__AVR_ATmega644P__)
	//	PWM #6 auf Pin PIND7/OCR2A als Ausgang konfigurieren
	corePortsConfigurePort(kPortD, kPortBit7, kPortBit7, kPortBitNone);
#else
	#error "OC2A not supported on this device"
#endif
#endif

#if defined (CORE_CONFIGURE_PWM_OC2B)
#if defined (__AVR_ATmega644P__)
	//	PWM #7 initialisieren:
	//	OCR2B löschen
	OCR2B = 0;
	//	PWM #7 auf Pin PIND6/OCR2B: als Ausgang konfigurieren
	corePortsConfigurePort(kPortD, kPortBit6, kPortBit6, kPortBitNone);
#else
	#error "OC2B not supported on this device"
#endif
#endif
	
#if defined (CORE_CONFIGURE_PWM_OC2C)
	//	PWM #8 initialisieren:
	#error "OC2C not supported on this device"
#endif

#if defined (CORE_CONFIGURE_PWM_OC3A)||defined (CORE_CONFIGURE_PWM_O3B)||defined (CORE_CONFIGURE_PWM_OC3C)
#if defined (__AVR_ATmega128__) || defined (__AVR_AT90CAN128__)
	//	PWM #9, PWM #10, PWM #11 konfigurieren
	//	Timer #3 initialisieren (OC3A/OC3B/OC3C):
	TCCR3A = 0;				// Clear TCCR3A
	TCCR3B = 0;				// Clear TCCR3B
	TCNT3H = 0;				// Clear TCNT3
	TCNT3L = 0;				// Clear TCNT3
#else
	#error "OC3 not supported on this device"
#endif
#endif

#if defined (CORE_CONFIGURE_PWM_OC3A)
	//	PWM #9 initialisieren:
#if defined (__AVR_ATmega128__) || defined (__AVR_AT90CAN128__)
	//	OCR3A löschen
	OCR3A = 0;
	//	PWM #9 auf Pin PINE3/OCR3A: als Ausgang konfigurieren
	corePortsConfigurePort(kPortE, kPortBit3, kPortBit3, kPortBitNone);
#else
	#error "OC3A not supported on this device"
#endif
#endif

#if defined (CORE_CONFIGURE_PWM_OC3B)
	//	PWM #10 initialisieren:
#if defined (__AVR_ATmega128__) || defined (__AVR_AT90CAN128__)
	//	OCR3B löschen
	OCR3B = 0;
	//	PWM #10 auf Pin PINE4/OCR3B: als Ausgang konfigurieren
	corePortsConfigurePort(kPortE, kPortBit4, kPortBit4, kPortBitNone);
#else
	#error "OC3B not supported on this device"
#endif
#endif

#if defined (CORE_CONFIGURE_PWM_OC3C)
	//	PWM #11 initialisieren:
#if defined (__AVR_ATmega128__) || defined (__AVR_AT90CAN128__)
	//	OCR3C löschen
	OCR3C = 0;
	//	PWM #11 auf Pin PINE5/OCR3C: als Ausgang konfigurieren
	corePortsConfigurePort(kPortE, kPortBit5, kPortBit5, kPortBitNone);
#else
	#error "OC3C not supported on this device"
#endif
#endif
	//	Initialize PWMs
	for (i=kPWM0; i<kPWMPinCount; i++)
		{
    	corePWMSetPWMFrequency((pwmPin)i, kPWM900Hz);
    	pwmDutyCycle[i] = 0;
    	pwmState[i] = 0;
    	}
	}

void corePWMSetPWMFrequency(pwmPin pin, pwmFrequency frequency)
	{
	if (pin < kPWMPinCount)
		{
		bool	pinState;
		
		pinState = (pwmState[pin] != 0);
				
		if (frequency > kPWM57600Hz)
			frequency = kPWM57600Hz;

		if (pinState)
			corePWMSetPWMOff(pin);
			
		pwmFlags[pin] = (pwm1deviderMap[frequency] >> 4);
		uint8	pwmShift = (9 - (pwmFlags[pin] & 0x03));

		switch (pwmLookup[pin])
			{
		#if defined (CORE_CONFIGURE_PWM_OC0)
			case kCorePWM0:				
				//	set PWM (output compare) duty
				//	this PWM output is generated on OC0 pin
				//	Resolution is 8-Bit
				pwmFlags[pin] = (pwm0deviderMap[frequency] >> 4);
		       	//	Fast PWM (single slope) mode ?
				if (pwmFlags[pin] & (1 << 3))
					TCCR0 |= _BV(WGM01);
				else
					TCCR0 &= ~_BV(WGM01);

				TCCR0 = (TCCR0 & ~0x07)|_BV(WGM00)|(pwm0deviderMap[frequency] & 0x07);
				break;
		#endif
			
		#if defined (CORE_CONFIGURE_PWM_OC0A)
			case kCorePWM0:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC0B)
			case kCorePWM1:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC0C)
			case kCorePWM2:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC0A)||defined (CORE_CONFIGURE_PWM_OC0B)||defined (CORE_CONFIGURE_PWM_OC0C)
				//	set PWM (output compare) duty
				//	this PWM output is generated on OC0 pin
				//	Resolution is 8-Bit
				pwmFlags[pin] = (pwm0deviderMap[frequency] >> 4);
		       	//	Fast PWM (single slope) mode ?
				if (pwmFlags[pin] & (1 << 3))
					TCCR0A |= _BV(WGM01);
				else
					TCCR0A &= ~_BV(WGM01);
					
				TCCR0A = (TCCR0A & ~0x07)|_BV(WGM00)|(pwm0deviderMap[frequency] & 0x07);
				break;
		#endif
			
		#if defined (CORE_CONFIGURE_PWM_OC1A)
			case kCorePWM3:
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC1B)
			case kCorePWM4:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC1C)
			case kCorePWM5:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC1A)||defined (CORE_CONFIGURE_PWM_OC1B)||defined (CORE_CONFIGURE_PWM_OC1C)
				if (pwmShift <= 8)
					{
					TCCR1B &= ~0x10;
					}
				else
					{
					ICR1 = 0xFFFF;
					TCCR1B |= 0x10;
					}
       			TCCR1A = (TCCR1A & ~0x03)|(pwmFlags[pin] & 0x03);
				TCCR1B = (TCCR1B & ~0x0F)|(pwmFlags[pin] & 0x08) | (pwm1deviderMap[frequency] & 0x07);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC2)
			case kCorePWM6:	
			#if defined (__AVR_AT90CAN128__)
				//	set PWM (output compare) duty
				//	this PWM output is generated on OC2 pin
				//	Resolution is 8-Bit
				pwmFlags[pin] = (pwm0deviderMap[frequency] >> 4);		       	
		       	//	Fast PWM (single slope) mode ?
				if (pwmFlags[pin] & (1 << 3))
					TCCR2A |= _BV(WGM21);
				else
					TCCR2A &= ~_BV(WGM21);

				TCCR2A = (TCCR2A & ~0x07)|_BV(WGM20)|(pwm0deviderMap[frequency] & 0x07);
			#else
				//	set PWM (output compare) duty
				//	this PWM output is generated on OC2 pin
				//	Resolution is 8-Bit
				pwmFlags[pin] = (pwm0deviderMap[frequency] >> 4);		       	
		       	//	Fast PWM (single slope) mode ?
				if (pwmFlags[pin] & (1 << 3))
					TCCR2 |= _BV(WGM21);
				else
					TCCR2 &= ~_BV(WGM21);

				TCCR2 = (TCCR2 & ~0x07)|_BV(WGM20)|(pwm0deviderMap[frequency] & 0x07);
			#endif
				break;
		#endif
	
		#if defined (CORE_CONFIGURE_PWM_OC2A)
			case kCorePWM6:
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC2B)
			case kCorePWM7:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC2A)||defined (CORE_CONFIGURE_PWM_OC2B)||defined (CORE_CONFIGURE_PWM_OC2C)
				if (pwmShift <= 8)
					{
					TCCR2B &= ~0x10;
					}
				else
					{
					TCCR2B |= 0x10;
					}
			
	       		TCCR2A = (TCCR2A & ~0x03)|(pwmFlags[pin] & 0x03);
				TCCR2B = (TCCR2B & ~0x0F)|(pwmFlags[pin] & 0x08) | (pwm1deviderMap[frequency] & 0x07);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3A)
			case kCorePWM9:
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC3B)
			case kCorePWM10:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC3C)
			case kCorePWM11:				
		#endif
		#if defined (CORE_CONFIGURE_PWM_OC3A)||defined (CORE_CONFIGURE_PWM_OC3B)||defined (CORE_CONFIGURE_PWM_OC3C)
				if (pwmShift <= 8)
					{
					TCCR3B &= ~0x10;
					}
				else
					{
					ICR3 = 0xFFFF;
					TCCR3B |= 0x10;
					}
			
				TCCR3A = (TCCR3A & ~0x03)|(pwmFlags[pin] & 0x03);
				TCCR3B = (TCCR3B & ~0x0F)|(pwmFlags[pin] & 0x08) | (pwm1deviderMap[frequency] & 0x07);
				break;
		#endif
		
			default:
				break;
			}			

		if (pinState)
			{
			corePWMSetPWMDutyCycle(pin, pwmDutyCycle[pin]);
			corePWMSetPWMOn(pin);
			}			
		}
	}


uint16 corePWMGetPWMDutyCycle(pwmPin pin)
	{
	uint16	result = 0;

	if (pin < kPWMPinCount)
		{
		result = pwmDutyCycle[pin];
		}
	
	return result;
	}		

void corePWMSetPWMDutyCycle(pwmPin pin, uint16 dutyCycle)
	{
	if (pin < kPWMPinCount)
		{		
		uint8	pwmShift = (9 - (pwmFlags[pin] & 0x03));
		uint16	pwmDuty;
	
     	pwmDutyCycle[pin] = dutyCycle;

		if (pwmShift <= 8)
			{
			pwmDuty = (dutyCycle >> pwmShift);
			}
		else
			{
			pwmDuty = dutyCycle;
			}

		switch (pwmLookup[pin])
			{
		#if defined (CORE_CONFIGURE_PWM_OC0)
			case kCorePWM0:
				OCR0 = (dutyCycle >> 8);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC0A)
			case kCorePWM0:
				OCR0A = (dutyCycle >> 8);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC0B)
			case kCorePWM1:
				OCR0B = (dutyCycle >> 8);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC1A)
			case kCorePWM3:
				OCR1A = pwmDuty;
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC1B)
			case kCorePWM4:
				OCR1B = pwmDuty;
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC1C)
			case kCorePWM5:
				OCR1C = pwmDuty;
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC2)
			case kCorePWM6:
			#if defined (__AVR_AT90CAN128__)
				OCR2A = (dutyCycle >> 8);
			#else
				OCR2 = (dutyCycle >> 8);
			#endif
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC2A)
			case kCorePWM6:
				OCR2A = (dutyCycle >> 8);
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC2B)
			case kCorePWM7:
				OCR2B = (dutyCycle >> 8);
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC3A)
			case kCorePWM9:
				OCR3A = pwmDuty;
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3B)
			case kCorePWM10:
				OCR3B = pwmDuty;
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3C)
			case kCorePWM11:
				OCR3C = pwmDuty;
				break;
		#endif
				
			default:
				break;
			}
		}
	}

void corePWMSetPWMOn(pwmPin pin)
	{
#if __DEBUG__ > 5
	char	number[] = "x\r";
	
	coreAsyncSerialWriteString(kUART0, "corePWMSetPWMOn #");
	number[0] = pin + '0';
	coreAsyncSerialWriteString(kUART0, number);
#endif	
	if (pin < kPWMPinCount)
		{
		pwmState[pin] = 0xFF;
				
		switch (pwmLookup[pin])
			{
		#if defined (CORE_CONFIGURE_PWM_OC0)
			case kCorePWM0:
				// turn on timer0 PWM output
				TCCR0 |= _BV(COM01);
				TCCR0 &= ~_BV(COM00);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC0A)
			case kCorePWM0:
				// turn on timer0 PWM output
				TCCR0A |= _BV(COM0A1);
				TCCR0A &= ~_BV(COM0A0);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC0B)
			case kCorePWM1:
				// turn on timer0 PWM output
				TCCR0A |= _BV(COM0B1);
				TCCR0A &= ~_BV(COM0B0);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC1A)
			case kCorePWM3:
				// turn on channel A (OC1A) PWM output
				TCCR1A |= _BV(COM1A1);
				TCCR1A &= ~_BV(COM1A0);
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC1B)
			case kCorePWM4:
				// turn on channel B (OC1B) PWM output
				TCCR1A |= _BV(COMB1);
				TCCR1A &= ~_BV(COMB0);
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC1C)
			case kCorePWM5:
				// turn on channel C (OC1C) PWM output
				TCCR1A |= _BV(COMC1);
				TCCR1A &= ~_BV(COMC0);
				break;
 		#endif

		#if defined (CORE_CONFIGURE_PWM_OC2)
			case kCorePWM6:
				// turn on timer2 PWM output
			#if defined (__AVR_AT90CAN128__)
				TCCR2A |= _BV(COM2A1);
				TCCR2A &= ~_BV(COM2A0);
			#else
				TCCR2 |= _BV(COM21);
				TCCR2 &= ~_BV(COM20);
			#endif
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC2A)
			case kCorePWM6:
				// turn on timer 2/channel A (OC2A) PWM output
				TCCR2A |= _BV(COM2A1);
				TCCR2A &= ~_BV(COM2A0);
				break;
		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC2B)
			case kCorePWM7:
				// turn on timer 2/channel B (OC2B) PWM output
				TCCR2A |= _BV(COM2B1);
				TCCR2A &= ~_BV(COM2B0);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3A)
			case kCorePWM9:
				// turn on channel A (OC3A) PWM output
				TCCR3A |= _BV(COMA1);
				TCCR3A &= ~_BV(COMA0);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3B)
			case kCorePWM10:
				// turn on channel B (OC3B) PWM output
				TCCR3A |= _BV(COMB1);
				TCCR3A &= ~_BV(COMB0);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3C)
			case kCorePWM11:
				// turn on channel C (OC3C) PWM output
				TCCR3A |= _BV(COMC1);
				TCCR3A &= ~_BV(COMC0);
				break;
		#endif

			default:
				break;
			}
		}
	}

void corePWMSetPWMOff(pwmPin pin)
	{
#if __DEBUG__ > 5
	char	number[] = "x\r";
	
	coreAsyncSerialWriteString(kUART0, "corePWMSetPWMOff #");
	number[0] = pin + '0';
	coreAsyncSerialWriteString(kUART0, number);
#endif	
	if (pin < kPWMPinCount)
		{		
		pwmState[pin] = 0x00;
				
		switch (pwmLookup[pin])
			{
		#if defined (CORE_CONFIGURE_PWM_OC0)
			case kCorePWM0:
				// turn off timer0 PWM output
				TCCR0 &= ~(_BV(COM00)|_BV(COM01));
				corePortsClearPortBit(kPortB, kPortBit4);
				break;
 		#endif

		#if defined (CORE_CONFIGURE_PWM_OC0A)
			case kCorePWM0:
				// turn off timer0 channelA PWM output
				TCCR0A &= ~(_BV(COM0A0)|_BV(COM0A1));
			#if defined (__AVR_AT90CAN128__)
				corePortsClearPortBit(kPortB, kPortBit7);
			#elif defined (__AVR_ATmega644P__)
				corePortsClearPortBit(kPortB, kPortBit3);
			#endif
				break;
 		#endif

		#if defined (CORE_CONFIGURE_PWM_OC0B)
			case kCorePWM1:
				// turn off timer0 channelA PWM output
				TCCR0A &= ~(_BV(COM0B0)|_BV(COM0B1));
			#if defined (__AVR_ATmega644P__)
				corePortsClearPortBit(kPortB, kPortBit4);
			#endif
				break;
 		#endif

		#if defined (CORE_CONFIGURE_PWM_OC1A)
			case kCorePWM3:
				// turn off channel A (OC1A) PWM output
				TCCR1A &= ~(_BV(COM1A0)|_BV(COM1A1));
				corePortsClearPortBit(kPortB, kPortBit5);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC1B)
			case kCorePWM4:
				// turn off channel B (OC1B) PWM output
				TCCR1A &= ~(_BV(COM1B0)|_BV(COM1B1));
				corePortsClearPortBit(kPortB, kPortBit6);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC1C)
			case kCorePWM5:
				// turn off channel C (OC1C) PWM output
				TCCR1A &= ~(_BV(COM1C0)|_BV(COM1C1));
				corePortsClearPortBit(kPortB, kPortBit7);
				break;
 		#endif

		#if defined (CORE_CONFIGURE_PWM_OC2)
			case kCorePWM6:
				// turn off timer 2 PWM output
			#if defined (__AVR_ATmega128__)
				TCCR2 &= ~(_BV(COM20)|_BV(COM21));
				corePortsClearPortBit(kPortB, kPortBit7);
			#elif defined (__AVR_AT90CAN128__)
				TCCR2A &= ~(_BV(COM2A1)|_BV(COM2A0));
				corePortsClearPortBit(kPortB, kPortBit4);
			#endif
				break;
 		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC2A)
			case kCorePWM6:
				// turn off timer 1/channel A (OC2A) PWM output
				TCCR2A &= ~(_BV(COM2A1)|_BV(COM2A0));
			#if defined (__AVR_AT90CAN128__)
				corePortsClearPortBit(kPortB, kPortBit7);
			#elif defined (__AVR_ATmega644P__)
				corePortsClearPortBit(kPortD, kPortBit7);
			#endif
				break;
 		#endif
				
		#if defined (CORE_CONFIGURE_PWM_OC2B)
			case kCorePWM7:
				// turn off timer 2/channel B (OC2B) PWM output
				TCCR2A &= ~(_BV(COM2B1)|_BV(COM2B0));
			#if defined (__AVR_ATmega644P__)
				corePortsClearPortBit(kPortD, kPortBit6);
			#endif
				break;
 		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3A)
			case kCorePWM9:
				// turn off channel A (OC3A) PWM output
				TCCR3A &= ~(_BV(COMA0)|_BV(COMA1));
				corePortsClearPortBit(kPortE, kPortBit3);
				break;
		#endif

		#if defined (CORE_CONFIGURE_PWM_OC3B)
			case kCorePWM10:
				// turn off channel B (OC3B) PWM output
				TCCR3A &= ~(_BV(COMB0)|_BV(COMB1));
				corePortsClearPortBit(kPortE, kPortBit4);
				break;
		#endif

 		#if defined (CORE_CONFIGURE_PWM_OC3C)
			case kCorePWM11:
				// turn off channel C (OC3C) PWM output
				TCCR3A &= ~(_BV(COMC0)|_BV(COMC1));
				corePortsClearPortBit(kPortE, kPortBit5);
				break;
		#endif

			default:
				break;
			}
		}
	}

bool corePWMGetPWMState(pwmPin pin)
	{
	bool	result = false;
	
	if (pin < kPWMPinCount)
		{
		if (pwmState[pin])
			result = true;
		}
	
	return result;
	}

