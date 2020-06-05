/*--------------------------------------------------------------------------
Copyright (c)  2019 hs2-engineering. All rights reserved.
serialStream.h


Interface
Serial IO using low level routines provided in asyncSerial

$Date: 2016-09-28 14:13:45 +0200 (Mi, 28 Sep 2016) $
Author: Steffen Simon / Giovanny Mateus

--------------------------------------------------------------------------*/

#ifndef SERIAL_STREAM_H
#define SERIAL_STREAM_H


#include "eDeviceStream.h"
#include "driver_init.h"
/** \addtogroup RS232
*/
/*@{*/
const int32_t	kBaud1200 = 1200;
const int32_t	kBaud2400 = 2400L;
const int32_t	kBaud9600 = 9600L;
const int32_t	kBaud19200 = 19200L;
const int32_t	kBaud38400  = 38400L;
const int32_t	kBaud57600  = 57600L;
const int32_t	kBaud115200  = 115200L;
const int32_t	kBaud230400  = 230400L;


/** serialStream ermöglicht die Ausgabe auf die seriellen Ports des @ref eDeviceController.
  */
class serialStream :public stream
	{
		//variables
	public:
		volatile bool txDone;
		volatile bool rxDone;
		
	protected:
	uint8_t	selfUART;
	
	private:
	io_descriptor *io_usart;
	//functions
	public:	
		serialStream();
		serialStream(uint8_t uart);
		~serialStream();
		
		int32_t		init(void);
		int32_t		enable(void);
		int32_t		disable(void);

		/** Überprüft, ob ein Zeichen verfügbar ist.
		 */
		virtual bool dataAvailable(void);

		virtual int32_t	readData( void*, int32_t);
		virtual int32_t	writeData(const void*,int32_t);
	protected:
		
		private:
	serialStream( const serialStream &c );
	serialStream& operator=( const serialStream &c );
	};
/*@}*/

#endif // SERIAL_STREAM_H
