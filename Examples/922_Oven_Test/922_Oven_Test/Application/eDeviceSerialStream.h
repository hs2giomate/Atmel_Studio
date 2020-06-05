/*--------------------------------------------------------------------------

serialStream.h

This file is part of e.Development

Interface
Serial IO using low level routines provided in asyncSerial

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $

Copyright (c) 2003,2004,2005, 2011 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/

#ifndef SERIAL_STREAM_H
#define SERIAL_STREAM_H

#include "coreAsyncSerial.h"
#include <eDeviceStream.h>

/** \addtogroup RS232
*/
/*@{*/
const int32	kBaud1200 = 1200;
const int32	kBaud2400 = 2400L;
const int32	kBaud9600 = 9600L;
const int32	kBaud19200 = 19200L;
const int32	kBaud57600  = 57600L;
const int32	kBaud115200  = 115200L;


/** serialStream ermöglicht die Ausgabe auf die seriellen Ports des @ref eDevice.
  */
class serialStream :public stream
	{
	public:
		serialStream(uint8 uart);
		/** Konfiguriert den seriellen Port
		 */

		/** Setzt die Einstellungen des seriellen Ports auf die Standardwerte:
			57600 Baud,8 Datenbits, 1 Stopbit, keine Parität
		 */
		bool		open(void);
		bool		open(int32 baud, uint8 bits, uint8 stop, bool parity);
		bool		close(void);

		/** Überprüft, ob ein Zeichen verfügbar ist.
		 */
		virtual uint8	dataAvailable(void);

		virtual int16	readData(void*, int16, tick_t);
		virtual int16	writeData(const void*, int16);
		
	protected:
		uint8	selfUART;
	};
/*@}*/

#endif // SERIAL_STREAM_H
