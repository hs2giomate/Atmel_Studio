/*--------------------------------------------------------------------------

coreCAN.cc

This file is part of e.Core

Copyright (c) 2006,2007, 2009, 2010, 2011, 2012, 2013 Steffen Simon.
All rights reserved.

Implementation
low level interface to the AT90CAN128 CAN controller

$Author: steffen $
$Date: 2018-03-01 13:31:50 +0100 (Do, 01. Mrz 2018) $
$Revision: 7770 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Core/PSC/coreCAN.c $

--------------------------------------------------------------------------*/
#if defined(IPECO_PSC_Revision)||defined(E_DEVICE_CAN)

#include <avr/io.h>
#include <avr/interrupt.h>

#include "coreCAN.h"
#include "coreController.h"
#include "coreSystemTimer.h"
#if __DEBUG__ > 0		
#include "coreAsyncSerial.h"
#endif

/*----- Identifier definition -----*/
#define CAN_SET_STD_ID_10_4(identifier) (((*((uint8 *)(&identifier)+1))<<5)+((*(uint8 *)(&identifier))>>3))
#define CAN_SET_STD_ID_3_0(identifier)  ((*(uint8 *)(&identifier))<<5)
#define coreCANSetStandardIdentifier(identifier) {\
        CANIDT1 = CAN_SET_STD_ID_10_4 (identifier);\
        CANIDT2 = CAN_SET_STD_ID_3_0  (identifier);}
#define Can_set_std_remote_id(identifier) {\
        CANIDT1 = CAN_SET_STD_ID_10_4 (identifier);\
        CANIDT2 = CAN_SET_STD_ID_3_0  (identifier);\
        CANIDT4 = (1<<RTRTAG);}

#define CAN_SET_EXT_ID_28_21(identifier) (((*((uint8 *)(&identifier)+3))<<3)+((*((uint8 *)(&identifier)+2))>>5))
#define CAN_SET_EXT_ID_20_13(identifier) (((*((uint8 *)(&identifier)+2))<<3)+((*((uint8 *)(&identifier)+1))>>5))
#define CAN_SET_EXT_ID_12_5(identifier)  (((*((uint8 *)(&identifier)+1))<<3)+((* (uint8 *)(&identifier)   )>>5))
#define CAN_SET_EXT_ID_4_0(identifier)    ((* (uint8 *)(&identifier))<<3)

#define coreCANSetExtendedIdentifier(identifier) {\
        CANIDT1 = CAN_SET_EXT_ID_28_21 (identifier);\
        CANIDT2 = CAN_SET_EXT_ID_20_13 (identifier);\
        CANIDT3 = CAN_SET_EXT_ID_12_5  (identifier);\
        CANIDT4 = CAN_SET_EXT_ID_4_0   (identifier);\
        CANCDMOB |= (1<<IDE); }

#define Can_set_ext_remote_id(identifier) {\
        CANIDT1 = CAN_SET_EXT_ID_28_21 (identifier);\
        CANIDT2 = CAN_SET_EXT_ID_20_13 (identifier);\
        CANIDT3 = CAN_SET_EXT_ID_12_5  (identifier);\
        CANIDT4 = CAN_SET_EXT_ID_4_0   (identifier);\
        CANCDMOB |= (1<<IDE); \
        CANIDT4 |= (1<<RTRTAG);}



#define coreCANGetExtendedIdentifier(identifier){     \
        *((uint8 *)(&identifier)+3) = CANIDT1>>3;   \
        *((uint8 *)(&identifier)+2) = (CANIDT2>>3)+(CANIDT1<<5);   \
        *((uint8 *)(&identifier)+1) = (CANIDT3>>3)+(CANIDT2<<5);   \
        *((uint8 *)(&identifier))   = (CANIDT4>>3)+(CANIDT3<<5);}


#define coreCANGetStandardIdentifier(identifier){  \
        *((uint8 *)(&identifier)+1) = CANIDT1>>5; \
        *((uint8 *)(&identifier)  ) = (CANIDT2>>5)+(CANIDT1<<3); \
}

// global variables
#define	numberOfMObs		15
#define	numberOfReceiveMObs	14

#define NOMOB	0xFF

static volatile uint16	canResetCounter;
static volatile int8	canActualReceiveFilterID;
static volatile	int8	canDataPending;
static volatile int8	canDataReceived;
static volatile uint16	canDataOverflow;
static volatile int8	canDataReceivePointer;
static volatile int8	canDataReadPointer;
static volatile uint8	canReceiveMOb[numberOfReceiveMObs];

void coreCANClearMessageObject(uint8 mob);

// returns the mob which has the message
// suchen des MOb in dem die Nachricht ist
uint8 coreGANGetMessageObjectID(uint32 bits)
	{
	uint8 mob;

	if (bits == 0)
		return NOMOB;

	for( mob=0; (bits & 0x01) == 0; bits >>= 1, ++mob)
		;

	//  security check
	if (mob >= numberOfMObs)
		return NOMOB;
    else
		return mob;
	}

// CAN interrupt service handler
SIGNAL(CANIT_vect)
	{
	uint8	canpage = CANPAGE;		//  actual CANPAGE
	uint8	mob;
	uint8	readmodwrite;

	// check in which MOb the INT was initiated
	mob = coreGANGetMessageObjectID(CANSIT2 | (CANSIT1 << 8));
	if (mob == NOMOB)
		return;

	// select concerned MOb page
	CANPAGE = (mob << 4);	

	// Senden war erfolgreich
	if (CANSTMOB & (1<<TXOK))
		{
	#ifdef E_DEVICE
		PORTG ^= (1 << 0);
	#endif		
		readmodwrite = CANSTMOB;	 	
		readmodwrite &= ~(1<<TXOK);	//  reset INT reason
		CANSTMOB = readmodwrite;			
		CANCDMOB = 0;//(1<<CONMOB0);  //  stay tuned!
		canDataPending--;
		}
	// Nachricht erfolgreich empfangen
	else if (CANSTMOB & (1<<RXOK))
		{
	#ifdef E_DEVICE
		PORTG ^= (1 << 1);
	#endif		
		canReceiveMOb[canDataReceivePointer++] = mob;
		if (canDataReceivePointer == numberOfReceiveMObs)
			canDataReceivePointer = 0;
			
		canDataReceived++;

		if (canDataReceived > numberOfReceiveMObs)
			{
		#ifdef E_DEVICE
			PORTG &= ~(1 << 2);
		#endif
			canDataReceived = numberOfReceiveMObs;		
			canDataOverflow++;
			}
		#ifdef E_DEVICE
		else
			PORTG |= (1 << 2);
		#endif

		CANCDMOB &= ~(1<<CONMOB1);	//  Reset MOb
		readmodwrite = CANSTMOB;
		readmodwrite &= ~(1<<RXOK);	//  reset interrupt 	
		CANSTMOB = readmodwrite;
		}

	CANPAGE = canpage;  //  restore CANPAGE
	}

// access routines
int8 coreCANInitialize(void)
	{
	uint8	i;
	
	// switch CAN controller to reset mode
	CANGCON |= (1 << SWRES);
	//	Setup bit rate 125 kbit
	CANBT1 = 0x0E;
	CANBT2 = 0x0C;
	CANBT3 = 0x37;
	
	//	Clear Message Object buffers
	for (i = 0; i < numberOfMObs; i++)
  		coreCANClearMessageObject(i);

	//	Prepare receive MObs
	for (i = 0; i < numberOfReceiveMObs; i++)
		{
		CANPAGE  = (i << 4);
		CANCDMOB = (1 << CONMOB1);
		}

	canDataPending = 0;
	canDataReceived = 0;
	canDataOverflow = 0;
	canDataReceivePointer = 0;
	canDataReadPointer = 0;
	canActualReceiveFilterID = 0;

/*
	coreCANResetMessageFilters(kCAN0);
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
*/
/*
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
	coreCANAddMessageFilter(kCAN0, 0x0BDA0400UL, 0x0FFFFF00UL);	//	STA_MGCU
*/
/*
	coreCANAddMessageFilter(kCAN0, 0x13DEF600UL, 0xFFFFFF00UL);
	coreCANAddMessageFilter(kCAN0, 0x13DBF600UL, 0xFFFFFF00UL);
	coreCANAddMessageFilter(kCAN0, 0x0BDA3C00UL, 0xFFFFFF00UL);
	coreCANAddMessageFilter(kCAN0, 0x0BDA4400UL, 0xFFFFFF00UL);//	DIAG_CMD
*/	
	//	Clear interrupt flags
	CANGIT = 0;
	//	Enable CAN transmit- and receive-interrupt
	//	CANGIE = (1 << ENRX) | (1 << ENTX);
	CANGIE = (1 << ENIT) | (1 << ENRX) | (1 << ENTX);
	//	Enable all MOb interrupts
	CANIE1 = 0x7F;
	CANIE2 = 0xFF;
	//	Enable CAN
	CANGCON |= (1<<ENASTB);

	return 0xFF;
	}

int8 coreCANOpen(uint8 can, uint32 bitrate)
	{
	return 0;
	}

void coreCANClearMessageObject(uint8 mob)
	{
	uint8	i;
	
	CANPAGE  = (mob << 4);
	CANSTMOB = 0;
	CANCDMOB = 0;

	//	Clear CAN identifier
	CANIDT4  = 0;
	CANIDT3  = 0;
	CANIDT2  = 0;
	CANIDT1  = 0;

	//	Clear CAN identifier mask
	CANIDM4  = 0;
	CANIDM3  = 0;
	CANIDM2  = 0;
	CANIDM1  = 0;

	for (i = 0; i < 8; i++)
		CANMSG = 0;
	}

int8 coreCANClose(uint8 can)
	{
	//	Disable CAN
	CANGCON &= ~(1<<ENASTB);
	//	Disable all MOb interrupts
	CANIE1 = 0x00;
	CANIE2 = 0x00;
	//	Disable all interrupts
	CANGIE = 0x00;

	return 0;
	}

/**
	Set default message buffer filtering
	Default is no filtering
*/
bool coreCANResetMessageFilters(uint8 can)
	{
	bool	result = true;
	
	canActualReceiveFilterID = 0;
	
	for (uint8 i = 0; i < numberOfReceiveMObs; i++)
		{
		result &= coreCANAddMessageFilter(can, 0xFFFFFFFD, 0xFFFFFFFD);
		}

	canActualReceiveFilterID = 0;
	
	return result;
	}

/**
	Set message buffer filtering on MOb id
*/
bool coreCANAddMessageFilter(uint8 can, uint32 identifier, uint32 mask)
	{
	bool	result;
	
	result = (canActualReceiveFilterID < numberOfReceiveMObs);
	
	if (result)
		{
		//  set page
		CANPAGE = (canActualReceiveFilterID << 4);

		// set extended identifier
		CANIDT4 = (uint8_t)  identifier << 3;
		CANIDT3 = 			 identifier >> 5;
		CANIDT2 =            identifier >> 13;
		CANIDT1 =            identifier >> 21;
		
		//  set mask
		CANIDM4 = ((uint8_t) mask << 3) | (1 << IDEMSK);
		CANIDM3 = 			 mask >> 5;
		CANIDM2 =            mask >> 13;
		CANIDM1 =            mask >> 21;
		
		CANCDMOB |= (1 << IDE);
		
		canActualReceiveFilterID++;
		}
	
	return result;
	}

void coreCANReset(uint8 can)
	{
	coreCANInitialize();
/*
	// switch CAN controller to reset mode
	CANGCON |= (1 << SWRES);
*/
	canResetCounter++;
/*
	//	Wait until CAN is ready
	while (!(CANGSTA & (1 << ENFG)))
		coreControllerIdle();
*/
	}

/**
	Get CAN reset counter
*/
uint16 coreCANGetResetCounter(uint8 can)
	{
	return canResetCounter;
	}


/*
	Get CAN bus state ON/off
*/
bool coreCANGetBusOffState(uint8 can)
	{
	return (CANGSTA & (1 << BOFF)) != 0;
/*
	bool	result;	
	result = ((CANTEC == 0xFF)||(CANREC == 0xFF));
	return result;
*/
	}

/*
	Reset CAN bus state ON/off
*/
void coreCANResetBusOffState(uint8 can)
	{
	coreCANReset(can);
	}
	
/**
	Get CAN transmit error counter
*/
uint8 coreCANGetTEC(uint8 can)
	{
	return CANTEC;
	}

/**
	Get CAN receive error counter
*/
uint8 coreCANGetREC(uint8 can)
	{
	return CANREC;
	}

/**
	Get CAN bus state
*/
bool coreCANGetBusState(uint8 can)
	{
	return (CANGSTA & (1 << BOFF)) == 0;
	}

uint8 coreCANDataAvailable(uint8 can)
	{
	return canDataReceived;
	}

uint16 coreCANDataOverflow(uint8 can)
	{
	return canDataOverflow;
	}
/*
void coreCANSetBitrate(uint8 spr)
{
	outb(SPCR, (inb(SPCR) & ((1<<SPR0)|(1<<SPR1))) | (spr&((1<<SPR0)|(1<<SPR1)))));
}
*/

/**
	Read CAN message
*/
bool coreCANReadMessage(uint8 can, CANMessage* message, tick_t timeout)
	{
	tick_t		timeoutTime = timeout;
	bool		timeoutOccured = false;
	
	if (timeoutTime != forever)
		timeoutTime += coreSystemTimerTicks();

	switch (can)
		{
		case kCAN0:
			//	If no data available: Wait until message is in CAN MOb
			while(!canDataReceived && !timeoutOccured)
				{
				coreControllerIdle();
				timeoutOccured = (coreSystemTimerTicks() > timeoutTime);
				}
			// Copy data
			if (canDataReceived)
				{
				uint8	mob = canReceiveMOb[canDataReadPointer++];
				int8	i;
	
				if (canDataReadPointer == numberOfReceiveMObs)
					canDataReadPointer = 0;

				//	Select Message Object Page
				CANPAGE  = (mob << 4);
				//	Get Extended Identifier
				coreCANGetExtendedIdentifier((*message).identifier);
				//	Get DLC
                (*message).dataLengthCode = CANCDMOB & 0x0F;
				//	Get data
				for (i=0; i<(*message).dataLengthCode; i++)	
					(*message).data[i] = CANMSG;
				//	Clear unused buffer
				for (; i<8; i++)	
					(*message).data[i] = 0;

				CANGIE &= ~(1 << ENRX);
				canDataReceived--;
				CANGIE |= (1 << ENRX);
				CANCDMOB = (1<<CONMOB1);  //  Enable MOb
				}
			break;
		
		default:
			break;
		}
			
 	return !timeoutOccured;
	}

/**
	Write CAN message
*/
bool coreCANWriteMessage(uint8 can, CANMessage* message)
	{
	bool	result = true;
	uint8	i;	

		
#if __DEBUG__  > 3
	coreAsyncSerialWriteString(kUARTDebug, "coreCANWriteMessage...\r");
#endif
	while (canDataPending)
		{
		coreControllerIdle();
		if (CANGIT & (1 << AERG))
			{
			CANGIT |= (1 << AERG);
			canDataPending = 0;
		#if __DEBUG__  > 3
			coreAsyncSerialWriteString(kUARTDebug, "coreCANWriteMessage: ACK error!\r");
		#endif
			result = false;
			}
		
		if (CANGSTA & (1 << BOFF))
			{
			canDataPending = 0;
		#if __DEBUG__  > 3
			coreAsyncSerialWriteString(kUARTDebug, "coreCANWriteMessage: bus off!\r");
		#endif
			result = false;
			}
		
		if (CANGSTA & (1 << ERRP))
			{
			canDataPending = 0;
		#if __DEBUG__ > 3
			coreAsyncSerialWriteString(kUARTDebug, "coreCANWriteMessage error passive!\r");
		#endif
			result = false;
			}
		}
	
	if (!result)
		{
	#if __DEBUG__  > 3
		coreAsyncSerialWriteString(kUARTDebug, "coreCANWriteMessage: resetting CAN controller\r");
	#endif
		//coreCANInitialize();
		}
	else
		{		
		CANPAGE = (numberOfReceiveMObs << 4);
		//  set ID
		coreCANSetExtendedIdentifier((*message).identifier);

		//  write data to MOb
		for (i=0; i<(*message).dataLengthCode; i++)							
			CANMSG = (*message).data[i];

		//  set IDE bit, length = 8
		CANCDMOB = ((1<<IDE) | (*message).dataLengthCode);		//ide = 1: extendet, ide = 0: standard	
	
		canDataPending++;
		//  send message
		CANCDMOB |= (1<<CONMOB0);
		}
	
#if __DEBUG__ > 3
	coreAsyncSerialWriteString(kUARTDebug, "...done\r");
#endif
	return true;
	}

#endif	// CORE_CAN
