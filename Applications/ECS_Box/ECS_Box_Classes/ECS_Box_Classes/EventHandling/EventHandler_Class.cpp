/* 
* EventHandler_Class.cpp
*
* Created: 7/3/2020 4:28:51 PM
* Author: GMateusDP
*/


#include "EventHandler_Class.h"
#include "Timer_Class.h"


const int16		maxEventCount(EVENT_QUEUE_BUFFER/sizeof(event));
static event	eventStorage[EVENT_QUEUE_BUFFER];
static event*	eventQueue(NULL);
const int16		sizeOfEventQueue(sizeof(eventStorage)/sizeof(event));
#ifndef PREEMPTION_SUPPORTED
static volatile event*	mainThreadEventQueue((event*)NULL);
#endif
#ifndef CHECK_EVENT_PERIOD
#define CHECK_EVENT_PERIOD 50
#endif
const tick_t	forever = 0x7fffffff;
EventHandler_Class	*ptrEventHandlerClass;

static void	EventTimerTask(const struct timer_task *const timer_task){
	ptrEventHandlerClass->eventTimeout=true;
}


// default constructor
EventHandler_Class::EventHandler_Class()
{
	ptrEventHandlerClass=this;
} //EventHandler_Class

// default destructor
EventHandler_Class::~EventHandler_Class()
{
	ptrEventHandlerClass=this;
} //~EventHandler_Class

void EventHandler_Class::Init(void)
{
	int16	i;
	eventTimeout=false;
	eventsInUse=0;
	eventsInUsePeak=0;
	eventCollisions=0;
	eventEvalCollisions=0;
	eventQueueUnderflow=0;
	#if __DEBUG__  > 4

	usb << "initializeEvents\r";

	#endif
	eventQueue = eventStorage;
	for (i=1; i<sizeOfEventQueue; i++)
	{
		eventStorage[i].next = (event*)NULL;
		eventStorage[i-1].next = &eventStorage[i];
	}

	#if __DEBUG__  > 4

	usb << "initializeEvents: eventQueue == " << (void*)eventQueue << newline;
	usb << "initializeEvents: sizeOfEventQueue == " << sizeOfEventQueue << newline;
	usb << "initializeEvents: sizeof(event) == " << (int32)sizeof(event) << newline;
	usb << "initializeEvents: sizeof(eventStorage) == " << (int32)sizeof(eventStorage) << newline;

	for (i=0; i<sizeOfEventQueue; i++)
	{
		usb<< "initializeEvents: event #" << i << " == " << (void*)&eventStorage[i] << ", next == " << (void*)eventStorage[i].next << newline;
	}

	#endif
}

bool	EventHandler_Class::WaitForEvent(event& e, uint16 eventClass, uint16 eventType, tick_t timeout)
{
	bool	result(false);
	event*	queue;
	event*	queuePrev(NULL);
	
	
	e.eventClass = eventClass;
	e.eventType = eventType;

	tick_t	timeoutTime;
	
	if (timeout != forever)
	timeoutTime = (eventTimer.get_ticks() + timeout);
	else
	timeoutTime = forever;

	while (!result && (eventTimer.get_ticks() < timeoutTime))
	{
		if (eventHandler)
		{
			eventHandler();
		}

		queue = (event*)mainThreadEventQueue;

		while (queue)
		{
			#if __DEBUG__  > 4
			
			usb << "waitForEvent:\r";
			usb << "     queue:            " << (void*)queue << newline;
			usb << "     eventClass:       " << eventClass << newline;
			usb << "     eventType:        " << eventType << newline;
			usb << "     queue->class:     " << (*queue).eventClass << newline;
			usb << "     queue->type:      " << (*queue).eventType << newline;
			usb << "     queue->timestamp: " << (*queue).timestamp << newline;
		
			#endif
			if (
			((e.eventClass == kAnyEventClass) ||
			(queue->eventClass == e.eventClass))
			&&
			((e.eventType == kAnyEventType) ||
			(queue->eventType == e.eventType))
			)
			{
				if (queuePrev)
				{
					queuePrev->next = queue->next;
				}
				else
				{
					mainThreadEventQueue = queue->next;
				}
				
				e = *queue;

				queue->next = eventQueue;
				eventQueue = queue;
				
				eventsInUse--;

				queue = NULL;
				result = true;
			}
			else
			{
				queuePrev = queue;
				queue = queue->next;
			}
		}

		if (result)
		{
			#if __DEBUG__  > 4

			usb << "waitForEvent:\r";
			usb << "     got event!" << newline;
			usb << "     eventClass:       " << e.eventClass << newline;
			usb << "     eventType:        " << e.eventType << newline;
		
			#endif
		}
		else
		{
			tick_t	timeRemaining(timeoutTime - eventTimer.get_ticks());
			eventTimeout=false;
			if (timeRemaining > CHECK_EVENT_PERIOD){
				eventTimer.add_oneShot_task((FUNC_PTR)EventTimerTask,CHECK_EVENT_PERIOD);
			}else if (timeRemaining > 0){
				eventTimer.add_oneShot_task((FUNC_PTR)EventTimerTask,timeoutTime - eventTimer.get_ticks());
			}
			eventTimer.start();
			while (!eventTimeout);
		
		}
	}

	return result;
}
void EventHandler_Class::SendEvent(event& e, contextID receiver)
{
	SendEventSelf(e);
}

void EventHandler_Class::SendEventSelf(const event& e)
{
	#if __DEBUG__  > 4

	usb << "send event:\r";
	usb << "     class:   " << (uint16)e.eventClass << newline;
	usb << "     type:    " << (uint16)e.eventType << newline;
	
	#endif
	event*	theEvent(eventQueue);
	
	if (theEvent)
	{
		eventQueue = eventQueue->next;
		theEvent->next = NULL;
		eventsInUse++;

		event*	threadQueue((event*)mainThreadEventQueue);
		if (threadQueue == NULL)
		mainThreadEventQueue = theEvent;
		else
		{
			while (threadQueue->next)
			threadQueue = threadQueue->next;
			threadQueue->next = theEvent;
		}

		#if __DEBUG__  > 0
		if (eventsInUse > eventsInUsePeak)
		{
			eventsInUsePeak = eventsInUse;
			usb << "eventsInUsePeak => " << eventsInUsePeak << newline;
			
		}
		#endif

		(*theEvent).eventClass = e.eventClass;
		(*theEvent).eventType = e.eventType;
		(*theEvent).data = e.data;
		(*theEvent).timestamp = eventTimer.get_ticks();
		#if __DEBUG__  > 4
	
		usb << "     mainThreadEventQueue: " << (void*)mainThreadEventQueue << newline;
		usb << "     threadQueue:          " << (void*)threadQueue << newline;
		usb << "     &theEvent:            " << (void*)theEvent << newline;
		usb << "     timestamp:            " << (*theEvent).timestamp << newline;
		
		#endif
	}
	else
	{
		#if __DEBUG__  > 0
		
		usb << "send event failed!" << newline;
		usb << "     class:   " << (uint16)e.eventClass << newline;
		usb << "     type:    " << (uint16)e.eventType << newline;
		
		#endif
		eventQueueUnderflow++;
	}
}

void EventHandler_Class::SendEvent(uint16 eventClass, uint16 eventType, const eventData& data, contextID receiver)
{
	event		theEvent;

	theEvent.eventClass = eventClass;
	theEvent.eventType = eventType;
	theEvent.data = data;
	
	SendEventSelf(theEvent);
}

void EventHandler_Class::SendEventSelf(uint16 eventClass, uint16 eventType, const eventData& data)
{
	event		theEvent;

	theEvent.eventClass = eventClass;
	theEvent.eventType = eventType;
	theEvent.data = data;
	
	SendEventSelf(theEvent);
}

void EventHandler_Class::SendErrorSelf(uint16_t et,const eventData& data)
{
	event		theEvent;
	theEvent.eventClass = kErrorEventClass;
	theEvent.eventType =(uint16_t)et ;
	theEvent.data = data;
	SendEventSelf(theEvent);
}
uint32_t EventHandler_Class::CheckEvent(void)
{
	
	float	temperatureSMPST = 0.0;	//stores temp value for fan off function
	float 	temperatureExhaust = 0.0;
	float	temperatureMagnetron = 0.0;

	static bool i2cAlreadyInitializedAliveBeacon = false;	//!< Part of alive beacon. In case of I2C connection lost initialization/reset is done only once until connection is established again
	static tick_t	timeStamp;
	static tick_t   lasttimedoorstatecheck = 0;

	
	#if __DEBUG__ > 4
	usb<< "checkEvent" << newline;
	#endif
	event	e;

	

	//Alive beacon function: i2cAlive is reset by a successful coreI2Cread to true and checked here.
	//If coreI2Cread was unsuccessful I2C is reseted



	return	(uint32_t)eventsInUse;
	
}


EventHandler_Class	listener;