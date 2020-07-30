/* 
* EventHandler_Class.h
*
* Created: 7/3/2020 4:28:51 PM
* Author: GMateusDP
*/


#ifndef __EVENTHANDLER_CLASS_H__
#define __EVENTHANDLER_CLASS_H__
#include "eDeviceEvent.h"

#define		EVENT_QUEUE_BUFFER	256
#define		EVENT_QUEUE_MAX		32
extern const int16	sizeOfEventQueue; 
extern const int16	maxEventCount;
typedef void (*WaitEventCallbackHandler)(void);
class EventHandler_Class
{
//variables
public:
	volatile	bool		eventTimeout;
	WaitEventCallbackHandler	eventHandler;
protected:
private:
	int32	eventsInUse;
	int32	eventsInUsePeak;
	int32	eventCollisions;
	int32	eventEvalCollisions;
	int32	eventQueueUnderflow;

//functions
public:
	EventHandler_Class();
	~EventHandler_Class();
	void Init();
	bool WaitForEvent(event& e, EventClass eventClass, EventType eventType, tick_t timeout= forever);
	bool WaitForEvent(event& e, uint16 eventClass, uint16 eventType, tick_t timeout= forever);
	void SendEvent(event& e, contextID receiver);
	void SendEventSelf(const event& e);
	void SendEvent(EventClass eventClass, EventType eventType,const eventData& data, contextID receiver);
	void SendEventSelf(EventClass eventClass, EventType eventType, const eventData& data = 0);
	void SendErrorSelf(uint16_t, const eventData& data = 0 );
	uint32_t CheckEvent(void);
protected:
private:
	EventHandler_Class( const EventHandler_Class &c );
	EventHandler_Class& operator=( const EventHandler_Class &c );

}; //EventHandler_Class
extern	EventHandler_Class	listener;
#endif //__EVENTHANDLER_CLASS_H__
