/*--------------------------------------------------------------------------

eDeviceEvent.h

This file is part of e.Lib

Interface
hi-level interface for events

$Date: 2018-05-30 18:12:58 +0200 (Mi, 30 Mai 2018) $
$Revision: 2298 $
$HeadURL: https://svn.s2embedded.at/customers/hs2-engineering/trunk/Phoenix/Firmware/Libraries/Cortex/e.Lib/eDeviceEvent.h $

Copyright (c) 2006,2007,2008,2009,2010 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#ifndef E_DEVICE_EVENT_H
#define E_DEVICE_EVENT_H

#include "coreTypes.h"


typedef uint8	contextID;




extern const int16	sizeOfEventQueue;
extern const tick_t	forever;

enum EventClass
	{
	kHVACEventClass = 0x200,
	kALUEventClass=0x300,
	kInterfaceEventClass=0x400,
	kErrorEventClass = 0x8000,
	kCommunicationEventClass = 0x9000,
	kAnyEventClass = 0xFFFF
	};
enum AluEventEnum
{
		kALUNotEvent=0x100,
		kALUSwitchkEvent,
		kALUEventCheckCommunication,
		kALUEventRunHAVAC,
		kALUEventSimpleStart,
		kALUEventCheckPheripherals,
		kALUEventSimpleStop,
		kALUEventSimpleResume,
		kALUNumberEvents
};

enum EventType
	{
	kTimeoutEvent = 0x0001,
	kALUControllerEvent,
	kHVACEventDoPendingTasks,
	k_AC_CMD_ENABLE_Event,
	k_AC_CMD_DISABLE_Event,
	k_AC_AUTO_Event,
	k_AC_VENT_Event,
	k_AC_BACKUP_Event,
	k_AC_OFF_Event,
	kSerialDataAvailableEvent,
	kUARTDataAvailableEvent,
	kUART0DataAvailableEvent,
	kUART1DataAvailableEvent,
	kUART2DataAvailableEvent,
	kUART3DataAvailableEvent,
	kCANMessageAvailableEvent,
	kCAN0MessageAvailableEvent,
	kCAN1MessageAvailableEvent,
	kI2CDataAvailableEvent,
	kInterfaceAvailable,
	kBootingRequest,
	kAnyEventType = 0xFFFF
	};

union eventData
	{
	eventData(int);
	eventData(int32);
	eventData(int16, int16);
	eventData(uint32);
	eventData(float);
	eventData(uint16, uint16);
	eventData(uint8, uint8, uint8, uint8);
	
	uint32		data;
	float		floatData;
	uint16		int16Data[2];
	uint16		wordData[2];
	uint8		byteData[4];
	int32		integerData;
	};

typedef union eventData	eventData;

inline eventData::eventData(int v)
	{
	integerData = v;
	}

inline eventData::eventData(int32 v)
	{
	integerData = v;
	}

inline eventData::eventData(int16 v1, int16 v2)
	{
	int16Data[0] = v1;
	int16Data[1] = v2;
	};

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


class event
	{
	public:
		event();
		/*event(EventClass, EventType);*/
		event(EventClass, EventType, const eventData& = 0);
		

		EventClass		eventClass;
		EventType		eventType;
		tick_t		timestamp;
		eventData	data;
		event*		next;

	private:
		
		
	};

inline event::event()
	:eventClass(kAnyEventClass), eventType(kAnyEventType), timestamp(0), data((uint32)0), next(NULL)
	{
		
	}

inline event::event(EventClass c, EventType t, const eventData& d)
	:eventClass(c), eventType(t), timestamp(0), data(d), next(NULL)
	{
	}
// inline event::event(EventClass c, EventType t)
// :eventClass(c), eventType(t), timestamp(0), data((uint32)0), next(NULL)
// {
// }

#endif // E_DEVICE_EVENT_H

