/*--------------------------------------------------------------------------
Copyright (c)  2019 hs2-engineering. All rights reserved.
stream.h

This file is part of e.Development

Interface
Serial IO

$Date: 2017-06-07 11:15:35 +0200 (Mi, 07 Jun 2017) $
Author: Steffen Simon 

--------------------------------------------------------------------------*/
#ifndef STREAM_H
#define STREAM_H

/** \addtogroup RS232
*/
/*@{*/
#include <stdint.h>

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
		//virtual bool dataAvailable(void) = 0;

		/** Gibt die Zeit an, die beim Lesen gewartet wird, bis die Operation beendet ist.
		 */
		//void setTimeout(time_t);
		/**
			Gibt Stunde h, Minute m und Sekunde s (und ggfs, wenn msec >= 0),
			als String im Format hh::mm:ss[.msec] aus.
		*/
		//const char* time(int8_t h, int8_t m, int8_t s, int32_t msecs = -1);
		/**
			Gibt einen time_t als String im Format hh::mm:ss aus.
		*/
		//const char* time(time_t time, bool msecs = false);
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
		stream&	operator<<(uint8_t);
		/**
			Gibt einen vorzeichenlosen 16 Bit-Wert aus.
		*/
		stream&	operator<<(uint16_t);
		/**
			Gibt einen vorzeichenlosen 32 Bit-Wert aus.
		*/
		stream&	operator<<(uint32_t);
		/**
			Gibt einen vorzeichenlosen int-Wert aus.
		*/
		stream&	operator<<(unsigned int);
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
		stream&	operator<<(int8_t);
		/**
			Gibt einen vorzeichenbehafteten 16 Bit-Wert aus.
		*/
		stream&	operator<<(int16_t);
		/**
			Gibt einen vorzeichenbehafteten 32 Bit-Wert aus.
		*/
		stream&	operator<<(int32_t);
		/**
			Gibt einen vorzeichenbehafteten int-Wert aus.
		*/
		stream&	operator<<(int);

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
		stream&	operator>>(uint8_t&);
		/**
			Liest einen vorzeichenlosen 16 Bit-Wert ein.
		*/
		stream&	operator>>(uint16_t&);
		/**
			Liest einen vorzeichenlosen 32 Bit-Wert ein.
		*/
		stream&	operator>>(uint32_t&);

		/**
			Liest ein Zeichen ein.
		*/
		stream&	operator>>(char&);
		/**
			Liest einen vorzeichenbehafteten 8 Bit-Wert ein.
		*/
		stream&	operator>>(int8_t&);
		/**
			Liest einen vorzeichenbehafteten 16 Bit-Wert ein.
		*/
		stream&	operator>>(int16_t&);
		/**
			Liest einen vorzeichenbehafteten 32 Bit-Wert ein.
		*/
		stream&	operator>>(int32_t&);

		/**
			Liest eine Zeichenfolge (ein Token) ein. Dabei werden alle Zeichen mit einem ASCII-Code <= 32 am
			Anfang übersprungen und dann solange gelesen, bis das erste Zeichen mit einem ASCII-Code <= 32 empfangen
			wird. Siehe dazu auch @ref read(char*, int16_t maxCharacters).
			Achtung: da bei diesem Aufruf keine maximale Anzahl zu lesender Zeichen angegeben werden kann, ist diese
			Methode prinzipiell unsicherer als @ref read(char*, int16_t maxCharacters). Hier kann es vorkommen, das mehr Zeichen
			eingelesen werden, als Speicherplatz zur Verfügung gestellt wurde (buffer overflow!).
		*/
		stream&	operator>>(char*);

		/**
			Gibt einen vorzeichenlosen 8 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden: Die Zahl "129" würde also 3 zurückgeben.
		*/
		int32_t	write(uint8_t);
		/**
			Gibt einen vorzeichenlosen 16 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int32_t	write(uint16_t);
		/**
			Gibt einen vorzeichenlosen 32 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int32_t	write(uint32_t);
		
		/**
			Gibt ein Zeichen aus.
			\return  Anzahl Zeichen, die geschrieben wurden, in diesem fall immer 1.
		*/
		int32_t	write(char);
		/**
			Gibt einen vorzeichenbehafteten 8 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int32_t	write(int8_t);
		/**
			Gibt einen vorzeichenbehafteten 16 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int32_t	write(int16_t);
		/**
			Gibt einen vorzeichenbehafteten 32 Bit-Wert aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int32_t	write(int32_t);

		/**
			Gibt eine Gleitkommazahl aus.
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int32_t	write(float);

		int32_t	writeAddr(void*);
		
		int32_t	writeDataAsHexString(const uint8_t* s, int16_t n, bool writeSpace);
		/**
			Gibt einen nullterminierten C-String aus.
			Beispiel: write("Hallo, Welt!");
			\return  Anzahl Zeichen, die geschrieben wurden.
		*/
		int32_t	write(const char*);

		/**
			Liest einen vorzeichenlosen 8 Bit-Wert ein.
			\return  Anzahl Zeichen, die gelesen wurden.
		*/
		int32_t	read(uint8_t&);
		/**
			Liest einen vorzeichenlosen 16 Bit-Wert ein.
			\return  Anzahl Zeichen, die gelesen wurden.
		*/
		int32_t	read(uint16_t&);
		/**
			Liest einen vorzeichenlosen 32 Bit-Wert ein.
			\return  Anzahl Zeichen, die gelesen wurden.
		*/
		int32_t	read(uint32_t&);

		/**
			Liest ein Zeichen ein.
			Rückgabewert ist 1.
		*/
		int32_t	read(char&);
		/**
			Liest einen vorzeichenbehafteten 16 8-Wert ein.
			Rückgabewert ist die Anzahl Zeichen, die gelesen wurden.
		*/
		int32_t	read(int8_t&);
		/**
			Liest einen vorzeichenbehafteten 16 Bit-Wert ein.
			Rückgabewert ist die Anzahl Zeichen, die gelesen wurden.
		*/
		int32_t	read(int16_t&);
		/**
			Liest einen vorzeichenbehafteten 32 Bit-Wert ein.
			Rückgabewert ist die Anzahl Zeichen, die gelesen wurden.
		*/
		int32_t	read(int32_t&);

		/**
			Liest eine Zeichenfolge (ein Token) ein. Dabei werden alle Zeichen mit einem ASCII-Code <= 32 am
			Anfang übersprungen und dann solange gelesen, bis das erste Zeichen mit einem ASCII-Code <= 32 empfangen
			wird oder bereits maxCharacters gespeichert wurden. Zeichen mit einem ASCII-Code <= 32 sind nicht druckbar, das Zeichen mit dem Code 32 ist das Leerzeichen (Space).
			Rückgabewert ist die Anzahl Zeichen, die gespeichert wurden.
			So würde die Eingabe "        Hallo " nur das Token "Hallo" speichern, die Rückgabe wäre in diesem Fall
			ein Wert von 5.
		*/
		int32_t	read(char*, int32_t maxCharacters);

		virtual int32_t	readData(void*, int32_t) = 0;
		virtual int32_t	writeData(const void*, int32_t) = 0;
		virtual	bool dataAvailable(void)= 0;
		/**
			Eine vorzeichenbehaftete Zahl in einen String wandeln.
			\return Anzahl Zeichen des Strings (wenn v den Wert -1234 hat, gibt
			die Funktion den Wert 5 zurück)
		*/
		static uint8_t signed2string(char* s, int32_t v);

		/**
			Eine nicht vorzeichenbehaftete Zahl in einen String wandeln.
			\return Anzahl Zeichen des Strings (wenn v den Wert 1234 hat, gibt
			die Funktion den Wert 4 zurück)
		*/
		static uint8_t unsigned2string(char* s, uint32_t v);

		/**
			Einen String in eine vorzeichenbehaftete Zahl wandeln.
		*/
		static void str2signed(const char* s, int32_t& v);

		/**
			Einen String in eine nicht vorzeichenbehaftete Zahl wandeln.
		*/
		static void str2unsigned(const char* s, uint32_t& v);

		/**
			Einen time_t in einen String wandeln.
			Das Format ist "hh:mm:ss" bzw. "hh:mm:ss.mmmm", je nachdem ob msecs "true" ist.
			
		*/
	//	static const char* time2string(time_t t, uint8_t msecs);

		/**
			Eine Zeit mit Stunden h, Minuten m, Sekunden s und ggfs. Millisekunden msecs in
			einen String wandeln. Das Format ist "hh:mm:ss" bzw. "hh:mm:ss.mmmm", je nachdem
			ob  msecs ungleich -1 ist.			
		*/
		static const char* time2string(int8_t h, int8_t m, int8_t s, int32_t msecs);

		/**
			Eine Zeit mit Sekunden s in einen String wandeln. Das Format ist "mm:ss".			
		*/
		static const char* time2stringShort(uint32_t s);

	protected:
		streamMode	outputMode;
	
	};
/*@}*/

/*
inline const char* stream::time(int8_t h, int8_t m, int8_t s, int16_t msecs)
	{
	return time2string(h, m, s, msecs);
	}

inline const char* stream::time(time_t time, bool msec)
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


inline stream& stream::operator<<(streamMode m)
	{
	outputMode = m;
	return *this;
	}

inline stream& stream::operator<<(uint8_t v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(uint16_t v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(uint32_t v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(unsigned int v)
	{
	write((uint32_t)v);
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

inline stream& stream::operator<<(int8_t v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(int16_t v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(int32_t v)
	{
	write(v);
	return *this;
	}

inline stream& stream::operator<<(int v)
	{
	write((int32_t)v);
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


inline stream& stream::operator>>(uint8_t& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(uint16_t& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(uint32_t& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(char& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(int8_t& v)
	{
	read(v);
	return *this;
	}


inline stream& stream::operator>>(int16_t& v)
	{
	read(v);
	return *this;
	}

inline stream& stream::operator>>(int32_t& v)
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
