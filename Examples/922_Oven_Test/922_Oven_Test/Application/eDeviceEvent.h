/*--------------------------------------------------------------------------

eDeviceEvent.h

This file is part of e.Lib

Interface
hi-level interface for events

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/ApplicationLayer/PSC/eDeviceEvent.h $

Copyright (c) 2006,2007,2008,2009,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef E_DEVICE_EVENT_H
#define E_DEVICE_EVENT_H

#include "coreTypes.h"
#include "coreSystemTimer.h"

typedef uint8	contextID;

extern int8		eventsInUse;
extern int8		eventsInUsePeak;
extern int16	eventCollisions;
extern int16	eventEvalCollisions;
extern int16	eventQueueUnderflow;
extern const int16	sizeOfEventQueue;

enum
	{
	kErrorEventClass = 0x8000,
	kCommunicationEventClass = 0x9000,
	kAnyEventClass = 0xFFFF
	};

enum
	{
	kTimeoutEvent = 0x0001,
	kSerialDataAvailableEvent,
	kUARTDataAvailableEvent,
	kUART0DataAvailableEvent,
	kUART1DataAvailableEvent,
	kUART2DataAvailableEvent,
	kUART3DataAvailableEvent,
	kCANMessageAvailableEvent,
	kCAN0MessageAvailableEvent,
	kCAN1MessageAvailableEvent,
	kAnyEventType = 0xFFFF
	};

union eventData
	{
	eventData(int);
	eventData(uint32);
	eventData(float);
	eventData(uint16, uint16);
	eventData(uint8, uint8, uint8, uint8);
	
	uint32		data;
	int32		integerData;
	float		floatData;
	uint16		wordData[2];
	uint8		byteData[4];
	};

typedef union eventData	eventData;

inline eventData::eventData(int v)
	{
	data = v;
	}

inline eventData::eventData(uint32 v)
	{
	data = v;
	}

inline eventData::eventData(float v)
	{
	floatData = v;
	}

inline eventData::eventData(uint16 v1, uint16 v2)
	{
	wordData[0] = v1;
	wordData[1] = v2;
	};

/** Ereignisdaten-Overlay: 4mal Byte ohne Vorzeichen */
inline eventData::eventData(uint8 v1, uint8 v2, uint8 v3, uint8 v4)
	{
	byteData[0] = v1;
	byteData[1] = v2;
	byteData[2] = v3;
	byteData[3] = v4;
	};

void initializeEvents(void);

class event
	{
	public:
		event();
		event(uint16, uint16, const eventData& = 0);

		uint16		eventClass;
		uint16		eventType;
		eventData	data;
		event*		next;
	};

inline event::event()
	:eventClass(kAnyEventClass), eventType(kAnyEventType), data((uint32)0), next(NULL)
	{
	}

inline event::event(uint16 c, uint16 t, const eventData& d)
	:eventClass(c), eventType(t), data(d), next(NULL)
	{
	}

bool waitForEvent(event&, uint16 = kAnyEventClass, uint16 = kAnyEventType, tick_t = forever);

void sendEventSelf(const event& e);
void sendEventSelf(uint16 eventClass, uint16 eventType, const eventData& data);

extern void (*WaitEventCallbackHandler)(void);

#endif // E_DEVICE_EVENT_H

