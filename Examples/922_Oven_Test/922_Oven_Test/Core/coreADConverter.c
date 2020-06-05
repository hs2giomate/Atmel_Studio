/*--------------------------------------------------------------------------

coreADConverter.c

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the AT90CAN128 internal A/D converter

$Author: steffen $
$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/coreADConverter.c $

--------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "coreADConverter.h"
#include "coreSystemTimer.h"

static volatile int8 	analogOversamplingRate;
static volatile int16 	analogNumberOfConversions;
static volatile int32	analogTempResultArray[8];

static volatile analogAutoTriggerSource autoTriggerSource;

static volatile uint8 analogConversionChannel;
static volatile uint8 analogConversionMask;
volatile uint8	analogConversionReadyMask;

volatile int16	analogResultArray[8];

#ifdef E_HAPTIC_RADAR_Revision
//	The following function will be called when analog conversion is done
ISR(ADC_vect)
	{
	static uint8	adlow,adhigh;
	static uint16	adReading;

	adlow = ADCL;	//	read low first!
	adhigh = ADCH;

    adReading = ((adhigh<<8)|adlow);

	//	Discard first reading
	if (analogNumberOfConversions > 0)
		analogTempResultArray[analogConversionChannel] += adReading;
	analogNumberOfConversions++;

	if (analogNumberOfConversions > (1 << (analogOversamplingRate << 1)))
		{
		analogResultArray[analogConversionChannel] = (analogTempResultArray[analogConversionChannel] >> analogOversamplingRate);
        analogTempResultArray[analogConversionChannel] = 0;
		analogNumberOfConversions = 0;

		if (analogConversionMask)
			{
			do
				{
				analogConversionChannel++;
				analogConversionChannel &= 0x07;
				}
			while ((analogConversionMask & (1 << analogConversionChannel)) == 0);

		#if defined (__AVR_AT90CAN128__)
			#ifdef E_DEVICE
			ADMUX = analogConversionChannel|_BV(REFS0);
			#else
			ADMUX = analogConversionChannel;
			#endif
		#else
			ADMUX = analogConversionChannel|_BV(REFS0);
		#endif
			}

	#if defined (__AVR_ATmega128__)
		// Free running mode: start next conversion
		if (autoTriggerSource == kFreeRunningMode)
			{
			//  start next conversion
			ADCSRA |= _BV(ADSC);
			}
	#endif
		}
	}
#else
//	The following function will be called when analog conversion is done
SIGNAL(SIG_ADC)
	{
	static uint8	adlow,adhigh;
	static uint16	adReading;

	adlow = ADCL;	//	read low first!
	adhigh = ADCH;
/*
    adReading = ((adhigh<<8)|adlow) << 5;
	if (adlow & 0x01)
		adReading |= 0x1F;
*/
    adReading = ((adhigh<<8)|adlow);

	//	Discard first reading
	if (analogNumberOfConversions > 0)
		analogTempResultArray[analogConversionChannel] += adReading;
	analogNumberOfConversions++;

	if (analogNumberOfConversions > (1 << (analogOversamplingRate << 1)))
		{
		analogResultArray[analogConversionChannel] = (analogTempResultArray[analogConversionChannel] >> analogOversamplingRate);
        analogTempResultArray[analogConversionChannel] = 0;
		analogNumberOfConversions = 0;

		analogConversionReadyMask |= (1 << analogConversionChannel);
		
		if (analogConversionMask)
			{
			do
				{
				analogConversionChannel++;
				analogConversionChannel &= 0x07;
				}
			while ((analogConversionMask & (1 << analogConversionChannel)) == 0);

		#if defined (__AVR_AT90CAN128__)
			#ifdef E_DEVICE
			ADMUX = analogConversionChannel|_BV(REFS0);
			#else
			ADMUX = analogConversionChannel;
			#endif
		#else
			ADMUX = analogConversionChannel|_BV(REFS0);
		#endif
		#if defined (__AVR_ATmega128__)
			// Free running mode: start next conversion
			if (ADCSRA & _BV(ADFR))
				{
				//  start next conversion
				ADCSRA |= _BV(ADSC);
				}
		#endif
			}

	#if defined (__AVR_ATmega128__)
		// Free running mode: start next conversion
		if (autoTriggerSource == kFreeRunningMode)
			{
			//  start next conversion
			ADCSRA |= _BV(ADSC);
			}
	#endif
		}
	}
#endif

void coreADConverterInitialize(void)
	{
	uint8	i;
	
	ACSR = _BV(ACD);	// Disable anlaog comperator
	//	ADC
	//	- enable
	//	- interrupt enable
	//	- conversion rate 125 KHz
	//	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	// Auto trigger source: no auto trigger
	autoTriggerSource = kNoAutoTrigger;
	
#if defined (__AVR_AT90CAN128__)
	ADCSRB = 0;
	DIDR0 = 0xFF;	// Disable digital input buffer
#endif
	analogConversionMask = 0x00;
	analogNumberOfConversions = kNoOversampling;
	
	for (i=0;i<8; i++)
		{
		analogResultArray[i] = 0;
        analogTempResultArray[i] = 0;
		}
	}

void coreADConverterRelease(void)
	{
	//	ADC
	//	- disable
	//	- interrupt disable
	ADCSRA = 0;
#if defined (__AVR_AT90CAN128__)
	ADCSRB = 0;
	DIDR0 = 0;		// Enable digital input buffer
#endif
	}

int16 coreADConverterReadSingle(uint8 channel)
	{
	int16	result;
	
	if (!coreADConverterIsAutoTriggerEnabled())
		{
		uint8	adlow,adhigh;

		//	ADCSRA |= ((1<<ADEN)|(1<<ADIE));
	#ifdef __AVR_AT90CAN128__
		#ifdef E_DEVICE
		ADMUX = analogConversionChannel|_BV(REFS0);
		#else
		ADMUX = analogConversionChannel;
		#endif
	#else
		ADMUX = channel|_BV(REFS0);
	#endif
		//	coreSystemTimerWait(2);
		//	Enable ADC
		ADCSRA |= (1<<ADEN);
		//  start conversion
		ADCSRA |= _BV(ADSC);

		// wait for result
		while (ADCSRA & _BV(ADSC))
			sleep_mode();

		adlow = ADCL;	//	read low first!
		adhigh = ADCH;
		ADCSRA &= ~(1<<ADEN);

/*
		analogResultArray[channel] = ((adhigh<<8)|adlow) << 5;
		if (adlow & 0x01)
			analogResultArray[channel] |= 0x1F;
*/				

		analogResultArray[channel] = ((adhigh<<8)|adlow);		
		result = analogResultArray[channel];
		}
	else
		{
		ADCSRA &= ~(1<<ADIE);
		result = analogResultArray[channel];
		ADCSRA |= (1<<ADIE);
		}
			
	return result;
	}

void coreADConverterReadArray(uint8 bits, int16 results[8])
	{
	if (!coreADConverterIsAutoTriggerEnabled())
		{
		int8	i;
		
		for (i=0; i<8; i++)
			{
			if (bits & (1 << i))
				results[i] = coreADConverterReadSingle(i);
			else
				results[i] = 0;
			}
		}
	else
		{
		ADCSRA &= ~(1<<ADIE);
		coreADConverterReadAutoTriggerArray(results);
		ADCSRA |= (1<<ADIE);
		}
	}

void coreADConverterEnableAutoTrigger(analogAutoTriggerSource trigger, uint8 bits, uint8 oversampling)
	{
	if (trigger > kNoAutoTrigger)
		{
		uint8	ch;
	
	#if defined (__AVR_AT90CAN128__)
		ADCSRA &= ~(1 << ADATE);
	    ADCSRB &= 0xF8;
	#endif

		autoTriggerSource = trigger;
		analogConversionMask = bits;
		analogOversamplingRate = oversampling;
		analogNumberOfConversions = 0;
	
		analogConversionChannel = 0;
        analogConversionReadyMask = 0;
        
		for (ch=0; ch<8; ch++)
			{
			if (bits & (1 << ch))
				{
				analogResultArray[ch] = 0;
				analogTempResultArray[ch] = 0;
				}
			}

		for (ch = 0; ch < 8; ch++)
			{
			if (bits & (1 << ch))
				{
				analogConversionChannel = ch;
				break;
				}
			}	
			
	#ifdef __AVR_AT90CAN128__
		#ifdef E_DEVICE
		ADMUX = analogConversionChannel|_BV(REFS0);
		#else
		ADMUX = analogConversionChannel;
		#endif
	#else
		ADMUX = analogConversionChannel|_BV(REFS0);
	#endif
	
	#if defined (__AVR_ATmega128__)
		if (trigger == kFreeRunningMode)
			{
		#ifndef E_HAPTIC_RADAR_Revision			
			ADCSRA |= _BV(ADFR);
		#endif
			//  start first conversion
			ADCSRA |= _BV(ADSC);
			}
	#elif defined (__AVR_AT90CAN128__)
		ADCSRB |= (trigger - 1);
		ADCSRA |= _BV(ADATE);
	#else
		#error "Unknown device"
	#endif
		ADCSRA |= ((1<<ADEN)|(1<<ADIE));
		}
	else
		coreADConverterDisableAutoTrigger();	
	}

void coreADConverterDisableAutoTrigger(void)
	{
	ADCSRA &= ~((1<<ADEN)|(1<<ADIE));
	autoTriggerSource = kNoAutoTrigger;
#if defined (__AVR_ATmega128__)
	ADCSRA &= ~_BV(ADFR);
#elif defined (__AVR_AT90CAN128__)
	ADCSRA &= ~_BV(ADATE);
#else
	#error "Unknown device"
#endif
	}

bool coreADConverterIsAutoTriggerEnabled(void)
	{
	if (autoTriggerSource == kNoAutoTrigger)
		return false;
	else
		return true;
	}

void coreADConverterReadAutoTriggerArray(int16 results[8])
	{
	uint8	ch;
	
	for (ch=0; ch<8; ch++)
		{
		if (analogConversionMask & (1 << ch))
			results[ch] = analogResultArray[ch];
		else
			results[ch] = 0;
		}
	}

uint8 coreADConverterGetResolution(void)
	{
	return 10 + analogOversamplingRate;
	}

int16 coreADConverterGetMaximumValue(void)
	{
	return ((1 << (10 + analogOversamplingRate)) - 1);
	}
