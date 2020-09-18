/*--------------------------------------------------------------------------
Copyright (c)  2019 hs2-engineering. All rights reserved.
eCore.c



Implementation
Library initialization code


$Date: 2016-09-28 14:13:45 +0200 (Mi, 28 Sep 2016) $
Author: Steffen Simon / Giovanny Mateus

--------------------------------------------------------------------------*/


#include "eLib.h"

//............................................................................
extern "C" void *malloc(size_t)
	{
    return (void *)0;
	}
//............................................................................
extern "C" void free(void *)
	{
	}

extern "C" void *_sbrk (ptrdiff_t)
	{
    return (void *)0;
	}

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

extern "C" void eLibInitialize(void)
	{
	}

