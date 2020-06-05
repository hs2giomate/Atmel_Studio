/*--------------------------------------------------------------------------

eDevice.h

This file is part of e.Lib

Interface
Application interface to the eDevice-Board

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $

--------------------------------------------------------------------------*/
#ifndef E_DEVICE_H
#define E_DEVICE_H

#include "eCore.h"
#include "eDeviceADC.h"
#include "eDevicePWM.h"
#include "eDeviceNVM.h"
#include "eDeviceSerialStream.h"

#if E_BASIC_Revision != 0x0101
	#include "eDeviceDigitalIO.h"
#endif
#if E_BASIC_Revision == 0x0104
//	#include "eDeviceAudio.h"
#endif

#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
/**
	* \defgroup LEDOut Ansteuerung der LEDs
*/
/*@{*/
/** Konstanten für die Onboard-LED's: Rot, Gelb, Grün
*/
typedef enum
	{
	kGreenLED = 0,	//!< Grüne LED
	kYellowLED = 1,	//!< Gelbe LED
	kRedLED = 2		//!< Rote LED
	} ledPin;
/** Bit-Konstanten für die Onboard-LED's: Rot, Gelb, Grün
	\sa setLedBits()
*/
typedef enum
	{
	kBitGreenLED = (1 << kGreenLED),	//!< Grüne LED
	kBitYellowLED = (1 << kYellowLED),	//!< Gelbe LED
	kBitRedLED = (1 << kRedLED)			//!< Rote LED
	} ledBitValues;
/*@}*/
#endif

/**
 * \defgroup RS232 Serielle Schnittstellen
*/
/**
	*\defgroup EEPROM Zugriff auf EEPROM
*/

/** eDevice bietet Methoden zum Zugriff auf den Microcontroller
*/
class eDevice
	{
	public:
		/** Konstruktor.
		*/
		eDevice(void);
		
		/** Initialisiert den Controller.
		*/
		static void	reset(void);

		/** Enable the watchdog timer.
		*/
		static void	enableWatchdog(void);

		/** Disable the watchdog timer.
		*/
		static void	disableWatchdog(void);

		/** Resets the watchdog timer.
		*/
		static void	serviceWatchdog(void);

		/** Setzt den Controller in den Uploadmodus.
		Nach Aufruf dieser Methode kann das Board mit der eIDE programmiert werden.
		*/
		static void	update(void);

		/** Gibt die Zeit in Millisekunden seit der letzten Initialisierung an.
		Initialisiert wird das Board entweder durch Anlegen einer Spannung (USB oder
		über Erweiterungsport) oder durch Aufruf der Methode \ref reset().
		\sa reset(), eDevice()
		*/
		static tick_t	ticks(void);

		/** Waits blocking for t Millisekunden.
		*/
		static void		wait(tick_t t);

		/** Waits blocking for t Millisekunden.
			Calls handler, if not NULL, every callbackIntervall milliseconds
		*/
		static void		wait(tick_t t, tick_t callbackIntervall, timerWaitCallbackHandler handler);

		/** \addtogroup I2C
		*/
		/*@{*/
		/** Liest vom I2C-Bus
			\param adr Adresse des GerÃ¤ts
			\param p Zeiger auf die zu übertragenden Daten
			\param n Anzahl zu lesender Bytes
			\param timeout Zeit in Millisekunden bis zum Abbruch des Lesevorgangs, wenn
			bis dahin nicht genug Bytes gelesen werden konnten.
		*/
		static int16	i2cRead(uint8 adr, void* p, int16 n, tick_t timeout = forever);
		/** Schreibt auf den I2C-Bus
			\param adr Adresse des GerÃ¤ts
			\param p Zeiger auf die zu übertragenden Daten
			\param n Anzahl zu sendender Bytes
			\param timeout Zeit in Millisekunden bis zum Abbruch des Schreibvorgangs, wenn
			bis dahin nicht genug Bytes gelesen werden konnten.
		*/
		static int16	i2cWrite(uint8 adr, const void* p, int16 n, tick_t timeout = forever);
		/*@}*/
		/** \addtogroup PWMOut
		*/
		/*@{*/
		/*@}*/

	#if defined(E_BASIC_Revision)|| defined(E_DEVICE_CAN)|| defined(IPECO_PSC_Revision)
		/** \addtogroup DigitalIO
		*/
		/*@{*/
		/** Setzt alle digitalen AusgÃ¤nge mit einem Befehl. Jedes Bit im Parameter pins
			entspricht genau einem Ausgang.
			\param bits liegt im Bereich 0 (binÃ¤r 00000000, alle Pins aus)
			bis 255 (binÃ¤r 11111111, alle Pins an).
			GezÃ¤hlt wird von Pin 0  (niedrigstwertiges Bit, LSB)
			bis Pin 7 (hÃ¶chstwertiges Bit, MSB). Pin 0 ist binÃ¤r 00000001 oder dezimal 1, Pin 7 ist binÃ¤r 10000000 oder dezimal 128.
		*/
		static void	setDigitalPowerBits(uint8 bits);

		/** Setzt den angegebenen Pin an oder aus,
			je nach Wert des Parameters on.
			\param pin liegt im Bereich (0..7).
			\param on == true: Pin an
			\param on == false: Pin aus
		*/
		static void	switchDigitalPowerPin(controllerPortPin pin, bool on);

		/** Schaltet den entsprechenden Pin an.
			\param pin liegt im Bereich kPortPin0...kPortPin7.
		*/
		static void	setDigitalPowerPin(controllerPortPin pin);

		/** Schaltet den entsprechenden Pin aus. \param pin liegt im Bereich kPortPin0...kPortPin7.
		*/
		static void	clearDigitalPowerPin(controllerPortPin pin);
		
		/** Schaltet den entsprechenden Pin um. \param pin liegt im Bereich kPortPin0...kPortPin7.
		*/
		static void	toggleDigitalPowerPin(controllerPortPin pin);


		static void	switchDigitalPowerPin(controllerPort port, controllerPortPin pin, bool on);

		static bool	getPin(controllerPort port, controllerPortPin pin);

		static void	switchPin(controllerPort port, controllerPortPin pin, bool on);

		static void	setPin(controllerPort port, controllerPortPin pin);

		static void	clearPin(controllerPort port, controllerPortPin pin);
		
		static void	togglePin(controllerPort port, controllerPortPin pin);
		/*@}*/
	#endif
	#if defined(E_DEVICE_CAN)||defined(E_BASIC_Revision)
		/**
			\addtogroup LEDOut
		*/
		/*@{*/
		/** Setzt alle LEDs mit einem Befehl. Die drei niedrigstwertigen Bits im Parameter bits
			entsprechen genau einer LED.
		*/
		static void	setLEDBits(uint8 bits);

		/**
			Schaltet die entsprechende LED an (on == true)  oder aus (on == false),
			je nach Wert der Variablen on.
			\param led liegt im Bereich (0..2).
			\param on == true: Pin an
			\param on == false: Pin aus

			LED's sind verbunden mit PortG, Pin 0 - 2
		*/
		static void	switchLED(ledPin led, bool on);

		/**
			Schaltet die entsprechende LED an. <led> liegt im Bereich (0..2)
		*/
		static void	setLED(ledPin led);

		/**
			Schaltet die entsprechende LED aus. <led> liegt im Bereich (0..2)
		*/
		static void	clearLED(ledPin led);

		/**
			Schaltet die entsprechende LED um. <led> liegt im Bereich (0..2)
		*/
		static void	toggleLED(ledPin led);
		/*@}*/
	#endif
		
		/** \addtogroup NVM
		*/
		static eDeviceNVM	eeprom;
		
	#if IPECO_PSC_Revision >= 0x0103
		static eDeviceNVM	fram;
	#endif
		/*@}*/
		
		static eDeviceADC	adc;
		
		static eDevicePWM	pwm;
		
	#if E_BASIC_Revision != 0x0101
		static eDeviceDigitalIO	digitalIO;
	#endif
		
	#if E_BASIC_Revision == 0x0104
		//	static eDeviceAudio	audio;
	#endif
		/** \addtogroup RS232
		*/
		/*@{*/
		/** Serielle Schnittstelle #1: RS232.
		*/
		static serialStream	asc0;
	#if !defined(IPECO_PSC_Revision)
		/** Serielle Schnittstelle #2: RS232 über USB.
		*/
		static serialStream	asc1;
	#endif
		/*@}*/
	
	//private:
		/** Initialize controller subsystem.
		*/
		void	initialize(void);
	};

tick_t ticks(void);

inline void	eDevice::reset(void)
	{
	coreControllerReset();
	}

inline void	eDevice::enableWatchdog(void)
	{
	coreControllerEnableWatchdog();
	}

inline void	eDevice::disableWatchdog(void)
	{
	coreControllerDisableWatchdog();
	}

inline void	eDevice::serviceWatchdog(void)
	{
	coreControllerServiceWatchdog();
	}

inline void	eDevice::update(void)
	{
	coreControllerEnterBootloader();
	}

#if defined(E_BASIC_Revision)|| defined(E_DEVICE_CAN)|| defined(IPECO_PSC_Revision)
inline bool eDevice::getPin(controllerPort port, controllerPortPin pin)
	{
	return corePortsGetPortPin(port, pin);
	}

inline void eDevice::switchPin(controllerPort port, controllerPortPin pin, bool on)
 	{
 	if (on)
		corePortsSetPortPin(port, pin);
	else
		corePortsClearPortPin(port, pin);
	}

inline void eDevice::setPin(controllerPort port, controllerPortPin pin)
 	{
	corePortsSetPortPin(port, pin);
	}

inline void eDevice::clearPin(controllerPort port, controllerPortPin pin)
	{
	corePortsClearPortPin(port, pin);
	}
		
inline void eDevice::togglePin(controllerPort port, controllerPortPin pin)
	{
	corePortsTogglePortPin(port, pin);
	}
#endif

inline tick_t ticks(void)
	{
	return coreSystemTimerTicks();
	}

inline tick_t eDevice::ticks(void)
	{
	return coreSystemTimerTicks();
	}

inline void eDevice::wait(tick_t t)
	{
	coreSystemTimerWait(t);
	}

inline void eDevice::wait(tick_t t, tick_t callbackIntervall, timerWaitCallbackHandler handler)
	{
	if (handler)
		coreSystemTimerWaitWithCallback(t, callbackIntervall, handler);
	else
		coreSystemTimerWait(t);
	}

extern	eDevice	controller;

#endif

