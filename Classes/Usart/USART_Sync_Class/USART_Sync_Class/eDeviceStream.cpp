/*--------------------------------------------------------------------------
Copyright (c)  2019 hs2-engineering. All rights reserved.
stream.cc


Implementation
Serial IO


$Date: 2016-11-12 17:02:47 +0100 (Sa, 12 Nov 2016) $
Author: Steffen Simon / Giovanny Mateus
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include "eDeviceStream.h"

static char timestring[] = "00:00:00.000";
static char shorttimestring[] = "00:00";


stream::stream()
	:outputMode(kModeASCII)
	{
	}

stream::~stream()
	{
	}

int32_t stream::writeAddr(void* v)
	{
	char	tmp[12];

	snprintf(tmp, 12, "0x%.8X", (unsigned int)v);
	return write(tmp);
	}

int32_t stream::write(uint8_t v)
	{
	char	tmp[6];
	int32_t	result(0);
	
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

int32_t stream::write(uint16_t v)
	{
	char	tmp[8];
	int32_t	result(0);
	
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

int32_t stream::write(uint32_t v)
	{
	char	tmp[12];
	int32_t	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 12, "%u", (unsigned int)v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 12, "0x%.8X", (unsigned int)v);
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

int32_t stream::write(char v)
	{
	char	tmp[8];
	int32_t	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			result = writeData(&v, 1);
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

int32_t stream::write(int8_t v)
	{
	char	tmp[8];
	int32_t	result(0);
	
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

int32_t stream::write(int16_t v)
	{
	char	tmp[8];
	int32_t	result(0);
	
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

int32_t stream::write(int32_t v)
	{
	char	tmp[12];
	int32_t	result(0);
	
	switch (outputMode)
		{
		case kModeASCII:
			snprintf(tmp, 12, "%d", (int)v);
			result = write(tmp);
			break;

		case kModeHex:
			snprintf(tmp, 12, "0x%.8X", (int)v);
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

int32_t stream::write(float v)
	{
	/*
	char	tmp[16];
	int32_t	result(0);
	
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
	uint8_t	i;
	uint32_t	frac;

	if (v < 0)
		{
		write('-');
		v *= -1.0;
		}
	frac = (uint32_t)(fmod(v, 1) * 100000.0);
	for (i=5; i>0; i--)
		{
		tmp[i] = (frac%10) + '0';
		frac = frac/10;
		}

	tmp[0] = '.';
	tmp[6] = '\0';

	return write((uint32_t)v) + write(tmp);
	}

int32_t stream::write(const char* v)
	{
	int32_t	n(0);

	outputMode = kModeASCII;	

	while (v && v[n])
		n++;
	if (n)
		writeData((const uint8_t*)v, n);

	return n;
	}


int32_t stream::read(uint8_t& v)
	{
	char	tmp[4];
	int32_t	n = read(tmp,4);
	uint32_t	w;

	str2unsigned(tmp, w);
	v = (uint8_t)w;
	return n;
	}

int32_t stream::read(uint16_t& v)
	{
	char	tmp[8];
	int32_t	n = read(tmp,8);
	uint32_t	w;

	str2unsigned(tmp, w);
	v = (uint16_t)w;
	return n;
	}

int32_t stream::read(uint32_t& v)
	{
	char	tmp[12];
	int32_t	n = read(tmp,12);

	str2unsigned(tmp, v);
	return n;
	}

int32_t stream::read(char& v)
	{
	return readData((uint8_t*)&v, 1);
	}


int32_t stream::read(int8_t& v)
	{
	char	tmp[8];
	int32_t	n = read(tmp, 8);
	int32_t	w;

	str2signed(tmp, w);
	v = (int32_t)w;
	return n;
	}

int32_t stream::read(int16_t& v)
	{
	char	tmp[8];
	int32_t	n = read(tmp, 8);
	int32_t	w;

	str2signed(tmp, w);
	v = (int32_t)w;
	return n;
	}

int32_t stream::read(int32_t& v)
	{
	char	tmp[12];
	int32_t	n = read(tmp, 12);

	str2signed(tmp, v);
	return n;
	}

int32_t stream::read(char* v, int32_t maxCharacters)
	{
	int32_t	n=0;
	
	if (!v)
		return n;

	while (dataAvailable())
		{
		char	ch;
		readData(&ch, 1);
		if (ch > ' ')
			{
			*v++ = ch;
			n = 1;
			break;
			}
		}

	while (dataAvailable())
		{
		char	ch;
		readData(&ch, 1);
		if (ch > ' ')
			{
			*v++ = ch;
			n++;
			}
		else
			break;
		}
		
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

void stream::str2signed(const char* s, int32_t& v)
	{
	unsigned long	w;

	if (*s == '-')
		{
		str2unsigned(s+1, w);
		v = (int32_t)w;
		v = v * -1;
		}
	else
		{
		str2unsigned(s, w);
		v = (int32_t)w;
		}
	}

void stream::str2unsigned(const char* s, uint32_t& v)
	{
	v = 0;

	while (*s)
		{
		v = v*10;
		v += (uint32_t)(*s++ - '0');
		}
	}




const char* stream::time2string(int8_t h, int8_t m, int8_t s, int32_t msecs)
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

const char* stream::time2stringShort(uint32_t s)
	{
	int32_t	m = s/60;
     			
	s = (s%60);
	shorttimestring[4] = '0' + (char)(s % 10);
	shorttimestring[3] = '0' + (char)(s / 10);
	shorttimestring[1] = '0' + (char)(m % 10);
	shorttimestring[0] = '0' + (char)(m / 10);
		
	return shorttimestring;
	}

