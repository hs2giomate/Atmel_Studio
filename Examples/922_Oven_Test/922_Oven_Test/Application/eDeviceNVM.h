/*--------------------------------------------------------------------------

coreController.h

This file is part of e.Lib

Interface
Application interface eDevice N(on) V(olatile) M(emory)

$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $

Copyright (c) 2006, 2011 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef E_DEVICE_NVM_H
#define E_DEVICE_NVM_H

#include "coreNVM.h"

class eDeviceNVM
	{
	public:
		/*@{*/
		/** Liest aus dem NVM
			\param adr Offset im NVM
			\param p Zeiger auf Empfangsdatenpuffer
			\param n Anzahl zu lesender Bytes
			\return Anzahl gelesener Byte
		*/
		int32	read(int32 adr, void* p, int32 n);
		/** Schreibt ins NVM
			\param adr Offset im NVM
			\param p Zeiger auf die zu übertragenden Daten
			\param n Anzahl zu sendender Bytes
			\return Anzahl geschriebener Byte
		*/
		int32	write(int32 adr, const void* p, int32 n);
	
	private:
		friend class eDevice;

		/** Constructor ;-)
		*/
		eDeviceNVM(uint8 v);
		
		uint8 	nvm;
	};


inline int32 eDeviceNVM::read(int32 adr, void* p, int32 n)
	{
	return coreNVMRead(nvm, adr, p, n);
	}

inline int32 eDeviceNVM::write(int32 adr, const void* p, int32 n)
	{
	return coreNVMWrite(nvm, adr, p, n);
	}

#endif

