/*--------------------------------------------------------------------------

eDevice.cc

This file is part of e.Lib

Implementation
Application interface to the eDevice-Board

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $
$Author: steffen $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/ApplicationLayer/PSC/eDevice.cc $

--------------------------------------------------------------------------*/

//#include <avr/pgmspace.h>
#include <stdlib.h>
#include "eDevice.h"


#if E_DEVICE_CAN == 0x0100
static const controllerPort		ledPort[] = {kPortG, kPortG, kPortG};
static const controllerPortBit	ledPortBits[] = {kPortBit0, kPortBit1, kPortBit2};

static const controllerPort		digitalPowerOutPort[] = {kPortA, kPortA, kPortA, kPortA, kPortB, kPortB, kPortB, kPortE};
static const controllerPortBit	digitalPowerOutPortBits[] = {kPortBit0, kPortBit1, kPortBit2, kPortBit3, kPortBit4, kPortBit5, kPortBit6, kPortBit3};
#elif E_BASIC_Revision == 0x0101
static const controllerPort		ledPort[] = {kPortG, kPortG, kPortG};
static const controllerPortBit	ledPortBits[] = {kPortBit0, kPortBit1, kPortBit2};

static const controllerPortBit	digitalPowerOutPortBits[] = {kPortBit0, kPortBit1, kPortBit2, kPortBit3, kPortBit4, kPortBit5, kPortBit6, kPortBit7};
#elif E_BASIC_Revision == 0x0103
static const controllerPort		ledPort[] = {kPortG, kPortG, kPortG};
static const controllerPortBit	ledPortBits[] = {kPortBit0, kPortBit1, kPortBit2};

static const controllerPort		digitalPowerOutPort[] = {kPortA, kPortA, kPortA, kPortA, kPortB, kPortB, kPortB, kPortE};
static const controllerPortBit	digitalPowerOutPortBits[] = {kPortBit0, kPortBit1, kPortBit2, kPortBit3, kPortBit4, kPortBit5, kPortBit6, kPortBit3};
#elif E_BASIC_Revision == 0x0104
static const controllerPort		ledPort[] = {kPortG, kPortG, kPortG};
static const controllerPortBit	ledPortBits[] = {kPortBit0, kPortBit1, kPortBit2};

static const controllerPort		digitalPowerOutPort[] = {kPortA, kPortA, kPortA, kPortB, kPortB, kPortE, kPortE, kPortE};
static const controllerPortBit	digitalPowerOutPortBits[] = {kPortBit0, kPortBit1, kPortBit2, kPortBit5, kPortBit6, kPortBit3, kPortBit4, kPortBit5};
#elif IPECO_PSC_Revision == 0x0103
static const controllerPort		digitalPowerOutPort[] = {kPortA, kPortA, kPortA, kPortA, kPortB, kPortB, kPortB, kPortE};
static const controllerPortBit	digitalPowerOutPortBits[] = {kPortBit0, kPortBit1, kPortBit2, kPortBit3, kPortBit4, kPortBit5, kPortBit6, kPortBit3};
#endif

static bool		eDeviceIsInitialized = false;
/**
	Static member definitions
*/
eDeviceNVM		eDevice::eeprom(kNVM0);
		
#if IPECO_PSC_Revision >= 0x0103
eDeviceNVM		eDevice::fram(kNVM2);
#endif

eDeviceADC		eDevice::adc;
		
eDevicePWM		eDevice::pwm;
		
#if E_BASIC_Revision != 0x0101
eDeviceDigitalIO	eDevice::digitalIO;
#endif
		
#if E_BASIC_Revision == 0x0104
//eDeviceAudio	eDevice::audio;
#endif

#if defined(IPECO_PSC_Revision)
serialStream	eDevice::asc0(kUART0);
#else
#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
serialStream	eDevice::asc0(kUART1);	//!	asc0 defaults to USB, which is connected to UART1
serialStream	eDevice::asc1(kUART0);	//!	asc1 defaults to RS232, which is connected to UART0
#else
serialStream	eDevice::asc0(kUART0);
serialStream	eDevice::asc1(kUART1);
#endif
#endif

eDevice	controller;

/** C++
	Provisorischer Ersatz für die libc++
*/
extern "C"
	{
	void __cxa_pure_virtual(void)
		{
		}
	}

void* operator new(unsigned int size)
	{
	return malloc(size);
	}

void operator delete(void* p)
	{
	free(p);
	}

eDevice::eDevice(void)
	{
	}

void eDevice::initialize(void)
	{
	if (eDeviceIsInitialized)
		return;

	eCoreInitialize();	
#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
	//	Stage 1 done
	clearLED(kRedLED);
#endif	
	// Setup async serial communication
	asc0.open();
#if !defined(IPECO_PSC_Revision)
	asc1.open();
#endif
	
#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
	//	Stage 2 done
	clearLED(kYellowLED);
#endif	

	// Setup I/O
#if E_BASIC_Revision == 0x0104
 	//	audio.initialize();
#endif
	// Setup synchronous serial communication
	//	coreI2CInitialize();
    //i2c.initialize();
	//	coreSPIInitialize();
    //spi.initialize();
#if defined CORE_CAN_SUPPORT && defined __AVR_AT90CAN128__
	//	coreCANInitialize();
    //can.initialize();
#endif	
#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
	//	Stage 2 done
	//	Stage 3 done
	clearLED(kGreenLED);
#endif	
	
	eDeviceIsInitialized = true;
	}

/** Liest vom I2C-Bus
	\param adr Adresse des Geräts
	\param p Zeiger auf die zu übertragenden Daten
	\param n Anzahl zu lesender Bytes
	\param timeout Zeit in Millisekunden bis zum Abbruch des Lesevorgangs, wenn
	bis dahin nicht genug Bytes gelesen werden konnten.
*/
int16 eDevice::i2cRead(uint8 adr, void* p, int16 n, tick_t timeout)
	{
	return coreI2CRead(kI2C0, adr, p, n, timeout);
	}

/** Schreibt auf den I2C-Bus
	\param adr Adresse des Geräts
	\param p Zeiger auf die zu übertragenden Daten
	\param n Anzahl zu sendender Bytes
	\param timeout Zeit in Millisekunden bis zum Abbruch des Schreibvorgangs, wenn
	bis dahin nicht genug Bytes gelesen werden konnten.
*/
int16 eDevice::i2cWrite(uint8 adr, const void* p, int16 n, tick_t timeout)
	{
	return coreI2CWrite(kI2C0, adr, p, n, timeout);
	}

#if defined(E_BASIC_Revision)|| defined(E_DEVICE_CAN)
void eDevice::setDigitalPowerBits(uint8 bits)
	{
	uint8	i;
	
	for (i=0; i<8; i++)
		{
	#if E_BASIC_Revision == 0x0101
		if (bits & (1 << i))
			corePortsSetPortBit(kPortA, (controllerPortPin)i);
		else
			corePortsClearPortBit(kPortA, (controllerPortPin)i);
	#else
		if (bits & (1 << i))
			corePortsSetPortBit(digitalPowerOutPort[i], digitalPowerOutPortBits[i]);
		else
			corePortsClearPortBit(digitalPowerOutPort[i], digitalPowerOutPortBits[i]);
	#endif
		}
	}

void eDevice::switchDigitalPowerPin(controllerPortPin pin, bool on)
	{
#if E_BASIC_Revision == 0x0101
	if (on)
		corePortsSetPortBit(kPortA, pin);
	else
		corePortsClearPortBit(kPortA, pin);
#else
	if (on)
		corePortsSetPortBit(digitalPowerOutPort[pin], digitalPowerOutPortBits[pin]);
	else
		corePortsClearPortBit(digitalPowerOutPort[pin], digitalPowerOutPortBits[pin]);
#endif
	}

void eDevice::setDigitalPowerPin(controllerPortPin pin)
	{
#if E_BASIC_Revision == 0x0101
	corePortsSetPortBit(kPortA, pin);
#else
	corePortsSetPortBit(digitalPowerOutPort[pin], digitalPowerOutPortBits[pin]);
#endif
	}

void eDevice::clearDigitalPowerPin(controllerPortPin pin)
	{
#if E_BASIC_Revision == 0x0101
	corePortsClearPortBit(kPortA, pin);
#else
	corePortsClearPortBit(digitalPowerOutPort[pin], digitalPowerOutPortBits[pin]);
#endif
	}
		
void eDevice::toggleDigitalPowerPin(controllerPortPin pin)
	{
#if E_BASIC_Revision == 0x0101
	corePortsTogglePortBit(kPortA, pin);
#else
	corePortsTogglePortBit(digitalPowerOutPort[pin], digitalPowerOutPortBits[pin]);
#endif
	}
#endif	//	E_BASIC_Revision

#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
/** Setzt alle LEDs mit einem Befehl. Die drei niedrigstwertigen Bits im Parameter bits
	entsprechen genau einer LED.
*/
void eDevice::setLEDBits(uint8 bits)
	{
	corePortsSetPortMasked(kPortG, 0x07, ~bits);
	}

/**
Schaltet die entsprechende LED an (on == true)  oder aus (on == false),
je nach Wert der Variablen on. <led> liegt im Bereich (0..3)

LED's sind verbunden mit PortG, Pin 0 - 2
*/
void eDevice::switchLED(ledPin led, bool on)
	{
	if (on)
		corePortsClearPortBit(kPortG, ledPortBits[led]);
	else	
		corePortsSetPortBit(kPortG, ledPortBits[led]);
	}

/**
Schaltet die entsprechende LED an. <led> liegt im Bereich (0..2)
*/
void eDevice::setLED(ledPin led)
	{
	corePortsClearPortBit(kPortG, ledPortBits[led]);
	}

/**
Schaltet die entsprechende LED aus. <led> liegt im Bereich (0..2)
*/
void eDevice::clearLED(ledPin led)
	{
	corePortsSetPortBit(kPortG, ledPortBits[led]);
	}

/**
Schaltet die entsprechende LED um. <led> liegt im Bereich (0..2)
*/
void eDevice::toggleLED(ledPin led)
	{
	corePortsTogglePortBit(kPortG, ledPortBits[led]);
	}
#endif

