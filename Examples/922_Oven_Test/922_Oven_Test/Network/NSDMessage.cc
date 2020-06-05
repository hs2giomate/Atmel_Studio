/*--------------------------------------------------------------------------

NSDMessage.cc

Implementation
NSD Message Object

Autor: Steffen Simon

$Author: steffen $
$Date: 2016-12-01 19:08:07 +0100 (Do, 01. Dez 2016) $
$Revision: 7202 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Network/NSDMessage.cc $

--------------------------------------------------------------------------*/
#ifdef NSD_SUPPORT

#include "NSDMessage.h"
#if __DEBUG__ > 0
#include "eDevice.h"
#define debugStream	controller.asc0
#endif

NSDMessage::NSDMessage(void)
	{
	message.timestamp = 0;
	message.identifier = 0;
	message.isPending = false;
	message.dataLengthCode = 8;
		
	for (uint8 i = 0; i<sizeof(message.data); i++)
		message.data[i] = 0;
	}

void NSDMessage::setMessageCode(uint8 v)
	{
	coreNSD_setMessageCode(&(message.identifier), v);
	}

void NSDMessage::setDeviceID(uint8 v)
	{
	coreNSD_setDeviceID(&(message.identifier), v);
	}

void NSDMessage::setRCI(uint8 v)
	{
	coreNSD_setRCI(&(message.identifier), v);
	}


void NSDMessage::addDIAG_WarningBit(NSDWarningBit v)
	{
	uint8	byte(v/8);
	uint8	bit =(v%8);
							
	setDIAGMessageDirtyBit(true);
	setWarningPendingBit(true);
	message.data[byte] |= (1 << (7 - bit));
#if __DEBUG__ > 3
	debugStream.lock();
	debugStream << "addDIAG_WarningBit: bit #" << (uint8)v << ", data == " << kModeHex << message.data[byte] << newline;
	debugStream.unlock();
#endif
	}

void NSDMessage::addDIAG_MalfunctionBit(NSDMalfunctionBit v)
	{
	uint8	byte(v/8);
	uint8	bit =(v%8);
							
	setDIAGMessageDirtyBit(true);
	setMalfunctionPendingBit(true);
	message.data[byte] |= (1 << (7 - bit));
#if __DEBUG__ > 3
	debugStream.lock();
	debugStream << "addDIAG_MalfunctionBit: bit #" << (uint8)v << ", data == " << kModeHex << message.data[byte] << newline;
	debugStream.unlock();
#endif
	}

void NSDMessage::addDIAG_FailureBit(NSDFailureBit v)
	{
	uint8	byte(v/8);
	uint8	bit =(v%8);
							
	setDIAGMessageDirtyBit(true);
	setFailurePendingBit(true);
	message.data[byte] |= (1 << (7 - bit));
#if __DEBUG__ > 3
	debugStream.lock();
	debugStream << "addDIAG_FailureBit: bit #" << (uint8)v << ", data == " << kModeHex << message.data[byte] << newline;
	debugStream.unlock();
#endif
	}

bool NSDMessage::removeDIAG_WarningBit(NSDWarningBit v)
	{
	uint8	byte(v/8);
	uint8	bit =(v%8);
	uint8	mask(0x80 >> bit);
	bool	warningPending(false);
	bool	messageDirty(getDIAGMessageDirtyBit());
		
	messageDirty |= ((message.data[byte] & mask) == mask);
	message.data[v/8] &= ~mask;
	
	for (int i=2; i<8; i++)
		warningPending |= (message.data[i] != 0);
			
	setDIAGMessageDirtyBit(messageDirty);
	setWarningPendingBit(warningPending);
	
#if __DEBUG__ > 3
	debugStream.lock();
	debugStream << "removeDIAG_WarningBit: bit #" << (uint8)v << ", warningPending == " << (warningPending?"true":"false") << ", messageDirty == " << (messageDirty?"true":"false") << newline;
	debugStream.unlock();
#endif
	return warningPending;
	}

bool NSDMessage::removeDIAG_MalfunctionBit(NSDMalfunctionBit v)
	{
	uint8	byte(v/8);
	uint8	bit =(v%8);
	uint8	mask(0x80 >> bit);
	bool	malfunctionPending(false);
	bool	messageDirty(getDIAGMessageDirtyBit());
		
	messageDirty |= ((message.data[byte] & mask) == mask);
	message.data[v/8] &= ~mask;
	
	for (int i=2; i<8; i++)
		malfunctionPending |= (message.data[i] != 0);
	
	setDIAGMessageDirtyBit(messageDirty);
	setMalfunctionPendingBit(malfunctionPending);
	
	return malfunctionPending;
	}

bool NSDMessage::removeDIAG_FailureBit(NSDFailureBit v)
	{
	uint8	byte(v/8);
	uint8	bit =(v%8);
	uint8	mask(0x80 >> bit);
	bool	failurePending(false);
	bool	messageDirty(getDIAGMessageDirtyBit());
		
	messageDirty |= ((message.data[byte] & mask) == mask);
	message.data[v/8] &= ~mask;
	
	for (int i=2; i<8; i++)
		failurePending |= (message.data[i] != 0);
			
	setDIAGMessageDirtyBit(messageDirty);
	setFailurePendingBit(failurePending);
			
	return failurePending;
	}

void NSDMessage::clearDIAG(void)
	{
	uint8	i;
	
	for (i=0; i<8; i++)
		message.data[i] = 0;
	}

#endif	//	NSD_SUPPORT

