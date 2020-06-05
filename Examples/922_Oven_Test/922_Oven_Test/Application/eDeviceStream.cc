/*--------------------------------------------------------------------------

eDeviceStream.cc

This file is part of e.Development

Implementation
Serial IO

$Author: steffen $
$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/ApplicationLayer/PSC/eDeviceStream.cc $

--------------------------------------------------------------------------*/

#ifdef AVR
#include <avr/pgmspace.h>
#endif
#include <stdio.h>
#include <math.h>
#include "coreSystemTimer.h"
#include "eDeviceStream.h"

static char timestring[] = "00:00:00.000";
static char shorttimestring[] = "00:00";


stream::stream()
	:outputMode(kModeASCII), timeout(forever)
	{
	}

stream::~stream()
	{
	}

int16 stream::writeAddr(void* v)
	{
	char	tmp[12];

	snprintf(tmp, 12, "0x%.8X", (unsigned int)v);
	return write(tmp);
	}

int16 stream::write(uint8 v)
	{
	char	tmp[6];
	int16	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 6, "%u", (unsigned int)v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 6, "0x%.2X", (unsigned int)v);
			result = write(tmp);
			break;
		
		case kModeBinary:
			result = writeData(&v, sizeof(v));
			break;
		
		default:
			break;
		}

	return result;
	}

int16 stream::write(uint16 v)
	{
	char	tmp[8];
	int16	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 8, "%u", (unsigned int)v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 8, "0x%.4X", (unsigned int)v);
			result = write(tmp);
			break;
		
		case kModeBinary:
			result = writeData(&v, sizeof(v));
			break;
		
		default:
			break;
		}
		
	return result;
	}

int16 stream::write(uint32 v)
	{
	char	tmp[12];
	int16	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 12, "%lu", v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 12, "0x%.8lX", v);
			result = write(tmp);
			break;
		
		case kModeBinary:
			result = writeData(&v, sizeof(v));
			break;
		
		default:
			break;
		}
		
	return result;
	}

int16 stream::write(char v)
	{
	return writeData(&v, 1);
	}

int16 stream::write(int8 v)
	{
	char	tmp[6];
	int16	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 8, "%d", (int)v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 8, "0x%.2X", (int)v);
			result = write(tmp);
			break;
		
		case kModeBinary:
			result = writeData(&v, sizeof(v));
			break;
		
		default:
			break;
		}
		
	return result;
	}

int16 stream::write(int16 v)
	{
	char	tmp[8];
	int16	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 8, "%d", (int)v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 8, "0x%.4X", (int)v);
			result = write(tmp);
			break;
		
		case kModeBinary:
			result = writeData(&v, sizeof(v));
			break;
		
		default:
			break;
		}
		
	return result;
	}

int16 stream::write(int32 v)
	{
	char	tmp[12];
	int16	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 12, "%ld", v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 12, "0x%.8lX", v);
			result = write(tmp);
			break;
		
		case kModeBinary:
			result = writeData(&v, sizeof(v));
			break;
		
		default:
			break;
		}
		
	return result;
	}

int16 stream::write(float v)
	{
	/*
	char	tmp[16];
	int16	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 16, "%g", v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 16, "0x%.8X", (int)v);
			result = write(tmp);
			break;
		
		case kModeBinary:
			result = writeData(&v, sizeof(v));
			break;
		
		default:
			break;
		}
		
	return result;
	*/

	char	tmp[7];
	uint8	i;
	uint32	frac;

	if (v < 0)
		{
		write('-');
		v *= -1.0;
		}
	frac = (uint32)(fmod(v, 1) * 100000.0);
	for (i=5; i>0; i--)
		{
		tmp[i] = (frac%10) + '0';
		frac = frac/10;
		}

	tmp[0] = '.';
	tmp[6] = '\0';

	return write((uint32)v) + write(tmp);
	}

int16 stream::write(const char* v)
	{
	int16	n(0);

	if (outputMode == kModePSTR)
		n = writeP(v);
	else
		{
		outputMode = kModeASCII;	

		while (v && v[n])
			n++;
		if (n)
			writeData((const byte*)v, n);
		}

	return n;
	}

#ifdef AVR
int16 stream::writeP(const char* v)
	{
	int16	n(0);
	char	ch;
	
	outputMode = kModeASCII;	

	ch = pgm_read_byte(v++);
	while (ch)
		{
		writeData(&ch, 1);
		n++;
		ch = pgm_read_byte(v++);
		}

	return n;
	}
#endif

int16 stream::read(uint8& v)
	{
	char	tmp[4];
	int16	n = read(tmp,4);
	uint32	w;

	str2unsigned(tmp, w);
	v = (byte)w;
	return n;
	}

int16 stream::read(uint16& v)
	{
	char	tmp[8];
	int16	n = read(tmp,8);
	uint32	w;

	str2unsigned(tmp, w);
	v = (word)w;
	return n;
	}

int16 stream::read(uint32& v)
	{
	char	tmp[12];
	int16	n = read(tmp,12);

	str2unsigned(tmp, v);
	return n;
	}

int16 stream::read(char& v)
	{
	return readData(&v, 1, timeout);
	}


int16 stream::read(int8& v)
	{
	char	tmp[8];
	int16	n = read(tmp, 8);
	int32	w;

	str2signed(tmp, w);
	v = (int16)w;
	return n;
	}

int16 stream::read(int16& v)
	{
	char	tmp[8];
	int16	n = read(tmp, 8);
	int32	w;

	str2signed(tmp, w);
	v = (int16)w;
	return n;
	}

int16 stream::read(int32& v)
	{
	char	tmp[12];
	int16	n = read(tmp, 12);

	str2signed(tmp, v);
	return n;
	}

int16 stream::read(char* v, int16 maxCharacters)
	{
	int16	n=0;
	
	if (!v)
		return 0;

	do
		readData(v, 1, timeout);
	while (*v<=' ');

	do
		readData(++v, 1, timeout);
	while ((*v > ' ') && (maxCharacters-- > 0));

	*v = '\0';
	return n;
	}

/*
uint8 stream::signed2string(char* s, int32 v)
	{
	uint8	i=0;
	uint8	j=0;
	char	tmp[12];
	
	if (v<0)
		{
		*s++ = '-';
		v = v * -1;
		}
	
	while (v >= 10)
		{
		tmp[i++] = '0' + (v%10);
		v = v/10;
		}
	
	tmp[i++] = '0' + v;
	while (i>0)
		s[j++] = tmp[--i];
	s[j] = 0;
	
	return j;
	}

uint8 stream::unsigned2string(char* s, uint32 v)
	{
	uint8	i=0;
	uint8	j=0;
	char	tmp[12];

	while (v >= 10)
		{
		tmp[i++] = '0' + (v%10);
		v = v/10;
		}

	tmp[i++] = '0' + v;
	while (i>0)
		s[j++] = tmp[--i];
	s[j] = 0;
	
	return j;
	}
*/

void stream::str2signed(const char* s, int32& v)
	{
	unsigned long	w;

	if (*s == '-')
		{
		str2unsigned(s+1, w);
		v = (int32)w;
		v = v * -1;
		}
	else
		{
		str2unsigned(s, w);
		v = (int32)w;
		}
	}

void stream::str2unsigned(const char* s, uint32& v)
	{
	v = 0;

	while (*s)
		{
		v = v*10;
		v += (uint32)(*s++ - '0');
		}
	}

const char* stream::time2string(tick_t t, uint8 msecs)
	{
	tick_t	seconds = t/(tick_t)1000;
	int16	minutes = seconds/60;
	int16	hours = minutes/60;
 	int16	millisecs;
     			
	
	seconds = (seconds%60);
	minutes = (minutes%60);
	
	if (msecs)
		millisecs = t%1000;
	else
		millisecs = -1;
	
	return time2string(hours, minutes, seconds, millisecs);
	}


const char* stream::time2string(int8 h, int8 m, int8 s, int16 msecs)
	{
	if (h>99)
		h = h%100;

	timestring[7] = '0' + (char)(s % 10);
	timestring[6] = '0' + (char)(s / 10);
	timestring[4] = '0' + (char)(m % 10);
	timestring[3] = '0' + (char)(m / 10);
	timestring[1] = '0' + (char)(h % 10);
	timestring[0] = '0' + (char)(h / 10);
	
	if (msecs >= 0)
		{
		timestring[8] = '.';
		timestring[11] = '0' + (char)(msecs % 10);
		timestring[10] = '0' + (char)((msecs / 10) % 10);
		timestring[9] = '0' + (char)(msecs / 100);
		}
	else
		timestring[8] = 0;
		
	return timestring;
	}


const char* stream::time2stringShort(uint16 s)
	{
	int16	m = s/60;
     			
	s = (s%60);
	shorttimestring[4] = '0' + (char)(s % 10);
	shorttimestring[3] = '0' + (char)(s / 10);
	shorttimestring[1] = '0' + (char)(m % 10);
	shorttimestring[0] = '0' + (char)(m / 10);
		
	return shorttimestring;
	}
