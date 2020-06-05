/*--------------------------------------------------------------------------

eDeviceStream.h

This file is part of e.Development

Interface
Serial IO

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $

--------------------------------------------------------------------------*/
#ifndef STREAM_H
#define STREAM_H

#include "coreTypes.h"
/** \addtogroup RS232
*/
/*@{*/

#define	newline	'\r'

/**
stream ist die Basisklasse für serielle Ein-/Ausgabe.
Abgeleitet davon sind und @ref serialStream.

stream bietet zwei Arten von Methoden: Ein "klassisches"
 Interface mittels read(...) und write(...) und ein
an die C++ Standardbibliothek angelehntes Interface mit den überladenen Operatoren
<< und >>.
 read(...) und write(...) geben die Anzahl jeweils gelesener  bzw.
geschriebener Zeichen zurück.
*/
typedef enum
	{
	kModeASCII,
#ifdef AVR
	kModePSTR,
#endif
	kModeHex,
	kModeBinary
	} streamMode;

class stream
	{
	public:
		stream();
		virtual	~stream();

		bool	lock(void);
		void	unlock(void);

		/** Überprüft, ob ein Zeichen verfügbar ist.
		 */
		virtual uint8 dataAvailable(void) = 0;

		/** Gibt die Zeit an, die beim Lesen bzw. Schreiben gewartet wird, bis die Operation beendet ist.
			Dies ist besonders beim Lesen hilfreich,da nicht unendlich gewartet wird, wenn kein Zeichen verfügbar ist. 
		 */
		void setTimeout(tick_t);
		/**
			Gibt Stunde h, Minute m und Sekunde s (und ggfs, wenn msec >= 0),
			als String im Format hh::mm:ss[.msec] aus.
		*/
		const char* time(int8 h, int8 m, int8 s, int16 msecs = -1);
		/**
			Gibt einen tick_t als String im Format hh::mm:ss aus.
		*/
		const char* time(tick_t time, bool msecs = false);
		/**
			Gibt die aktuelle Zeit als String im Format hh::mm:ss aus.
		*/
		const char* now(bool msecs = false);
		/**
			Gibt die aktuelle Zeit in ticks (Millisekunden) aus.
		*/
		const char* ticks(void);
		
		/**
			Set the output mode.
		*/
		stream&	operator<<(streamMode);
		/**
			Gibt einen vorzeichenlosen 8 Bit-Wert aus.
		*/
		stream&	operator<<(uint8);
		/**
			Gibt einen vorzeichenlosen 16 Bit-Wert aus.
		*/
		stream&	operator<<(uint16);
		/**
			Gibt einen vorzeichenlosen 32 Bit-Wert aus.
		*/
		stream&	operator<<(uint32);
		/**
			Gibt einen pointer in hex aus.
		*/
		stream&	operator<<(void*);

		/**
			Gibt ein Zeichen aus.
		*/
		stream&	operator<<(char);
		/**
			Gibt einen vorzeichenbehafteten 8 Bit-Wert aus.
		*/
		stream&	operator<<(int8);
		/**
			Gibt einen vorzeichenbehafteten 16 Bit-Wert aus.
		*/
		stream&	operator<<(int16);
		/**
			Gibt einen vorzeichenbehafteten 32 Bit-Wert aus.
		*/
		stream&	operator<<(int32);

		/**
			Gibt eine Gleitkommazahl aus.
		*/
		stream&	operator<<(float);
		/**
			Gibt einen C-String aus.
		*/
		stream&	operator<<(const char*);

		
		/**
			Liest einen vorzeichenlosen 8 Bit-Wert ein.
		*/
		stream&	operator>>(uint8&);
		/**
			Liest einen vorzeichenlosen 16 Bit-Wert ein.
		*/
		stream&	operator>>(uint16&);
		/**
			Liest einen vorzeichenlosen 32 Bit-Wert ein.
		*/
		stream&	operator>>(uint32&);

		/**
			Liest ein Zeichen ein.
		*/
		stream&	operator>>(char&);
		/**
			Liest einen vorzeichenbehafteten 8 Bit-Wert ein.
		*/
		stream&	operator>>(int8&);
		/**
			Liest einen vorzeichenbehafteten 16 Bit-Wert ein.
		*/
		stream&	operator>>(int16&);
		/**
			Liest einen vorzeichenbehafteten 32 Bit-Wert ein.
		*/
		stream&	operator>>(int32&);

		/**
			Liest eine Zeichenfolge (ein Token) ein. Dabei werden alle Zeichen mit einem ASCII-Code <= 32 am
			Anfang übersprungen und dann solange gelesen, bis das erste Zeichen mit einem ASCII-Code <= 32 empfangen
			wird. Siehe dazu auch @ref read(char*, int16 maxCharacters).
			Achtung: da bei diesem Aufruf keine maximale Anzahl zu lesender Zeichen angegeben werden kann, ist diese
			Methode prinzipiell unsicherer als @ref read(char*, int16 maxCharacters). Hier kann es vorkommen, das mehr Zeichen
			eingelesen werden, als Speicherplatz zur Verfügung gestellt wurde (buffer overflow!).
		*/
		stream&	operator>>(char*);

		/**
			Gibt einen vorzeichenlosen 8 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden: Die Zahl "129" würde also 3 zurückgeben.
		*/
		int16	write(uint8);
		/**
			Gibt einen vorzeichenlosen 16 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int16	write(uint16);
		/**
			Gibt einen vorzeichenlosen 32 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int16	write(uint32);
		
		/**
			Gibt ein Zeichen aus.
			\return  Anzahl Zeichen, die geschrieben wurden, in diesem fall immer 1.
		*/
		int16	write(char);
		/**
			Gibt einen vorzeichenbehafteten 8 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int16	write(int8);
		/**
			Gibt einen vorzeichenbehafteten 16 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int16	write(int16);
		/**
			Gibt einen vorzeichenbehafteten 32 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int16	write(int32);

		/**
			Gibt eine Gleitkommazahl aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int16	write(float);

		int16	writeAddr(void*);
		
		/**
			Gibt einen nullterminierten C-String aus.
			Beispiel: write("Hallo, Welt!");
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int16	write(const char*);
	#ifdef AVR
		int16	writeP(const char* v);
	#endif
		/**
			Liest einen vorzeichenlosen 8 Bit-Wert ein.
			\return  Anzahl Zeichen, die gelesen wurden.
		*/
		int16	read(uint8&);
		/**
			Liest einen vorzeichenlosen 16 Bit-Wert ein.
			\return  Anzahl Zeichen, die gelesen wurden.
		*/
		int16	read(uint16&);
		/**
			Liest einen vorzeichenlosen 32 Bit-Wert ein.
			\return  Anzahl Zeichen, die gelesen wurden.
		*/
		int16	read(uint32&);

		/**
			Liest ein Zeichen ein.
			Rückgabewert ist 1.
		*/
		int16	read(char&);
		/**
			Liest einen vorzeichenbehafteten 16 8-Wert ein.
			Rückgabewert ist die Anzahl Zeichen, die gelesen wurden.
		*/
		int16	read(int8&);
		/**
			Liest einen vorzeichenbehafteten 16 Bit-Wert ein.
			Rückgabewert ist die Anzahl Zeichen, die gelesen wurden.
		*/
		int16	read(int16&);
		/**
			Liest einen vorzeichenbehafteten 32 Bit-Wert ein.
			Rückgabewert ist die Anzahl Zeichen, die gelesen wurden.
		*/
		int16	read(int32&);

		/**
			Liest eine Zeichenfolge (ein Token) ein. Dabei werden alle Zeichen mit einem ASCII-Code <= 32 am
			Anfang übersprungen und dann solange gelesen, bis das erste Zeichen mit einem ASCII-Code <= 32 empfangen
			wird oder bereits maxCharacters gespeichert wurden. Zeichen mit einem ASCII-Code <= 32 sind nicht druckbar, das Zeichen mit dem Code 32 ist das Leerzeichen (Space).
			Rückgabewert ist die Anzahl Zeichen, die gespeichert wurden.
			So würde die Eingabe "        Hallo " nur das Token "Hallo" speichern, die Rückgabe wäre in diesem Fall
			ein Wert von 5.
		*/
		int16	read(char*, int16 maxCharacters);

		virtual int16	readData(void*, int16, tick_t) = 0;
		virtual int16	writeData(const void*, int16) = 0;
		
		/**
			Eine vorzeichenbehaftete Zahl in einen String wandeln.
			\return Anzahl Zeichen des Strings (wenn v den Wert -1234 hat, gibt
			die Funktion den Wert 5 zurück)
		*/
		static uint8 signed2string(char* s, int32 v);

		/**
			Eine nicht vorzeichenbehaftete Zahl in einen String wandeln.
			\return Anzahl Zeichen des Strings (wenn v den Wert 1234 hat, gibt
			die Funktion den Wert 4 zurück)
		*/
		static uint8 unsigned2string(char* s, uint32 v);

		/**
			Einen String in eine vorzeichenbehaftete Zahl wandeln.
		*/
		static void str2signed(const char* s, int32& v);

		/**
			Einen String in eine nicht vorzeichenbehaftete Zahl wandeln.
		*/
		static void str2unsigned(const char* s, uint32& v);

		/**
			Einen tick_t in einen String wandeln.
			Das Format ist "hh:mm:ss" bzw. "hh:mm:ss.mmmm", je nachdem ob msecs "true" ist.
			
		*/
		static const char* time2string(tick_t t, uint8 msecs);

		/**
			Eine Zeit mit Stunden h, Minuten m, Sekunden s und ggfs. Millisekunden msecs in
			einen String wandeln. Das Format ist "hh:mm:ss" bzw. "hh:mm:ss.mmmm", je nachdem
			ob  msecs ungleich -1 ist.			
		*/
		static const char* time2string(int8 h, int8 m, int8 s, int16 msecs);

		/**
			Eine Zeit mit Sekunden s in einen String wandeln. Das Format ist "mm:ss".			
		*/
		static const char* time2stringShort(uint16 s);
	
	protected:
		streamMode	outputMode;
		tick_t		timeout;
	};
/*@}*/

/*
inline const char* stream::time(int8 h, int8 m, int8 s, int16 msecs)
	{
	return time2string(h, m, s, msecs);
	}

inline const char* stream::time(tick_t time, bool msec)
	{
	return time2string(time, msec);
	}

inline const char* stream::now(bool msecs)
	{
	return time2string(timeServices::hours, timeServices::minutes,
					timeServices::seconds, msecs?timeServices::milliseconds:-1);
	}
*/
inline bool stream::lock(void)
	{
	return true;
	}

inline void stream::unlock(void)
	{
	}

inline void stream::setTimeout(tick_t t)
	{
	timeout = t;
	}

inline stream& stream::operator<<(streamMode m)
	{
	outputMode = m;
	return *this;
	}

inline stream& stream::operator<<(uint8 v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(uint16 v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(uint32 v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(void* v)
	{
	writeAddr(v);
	return *this;
	}

inline stream& stream::operator<<(char v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(int8 v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(int16 v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(int32 v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(float v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(const char* v)
	{
	write(v);
	return *this;
	}


inline stream& stream::operator>>(uint8& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(uint16& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(uint32& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(char& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(int8& v)
	{
	read(v);
	return *this;
	}


inline stream& stream::operator>>(int16& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(int32& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(char* v)
	{
	read(v, 0x7FFF);
	return *this;
	}

#endif // STREAM_H
