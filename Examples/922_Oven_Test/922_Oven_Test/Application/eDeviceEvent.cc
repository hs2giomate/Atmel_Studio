/*--------------------------------------------------------------------------

eDeviceEvent.cc

This file is part of e.Lib

Implementation
low-level interface for events

$Date: 2017-09-14 16:43:44 +0200 (Do, 14. Sep 2017) $
$Revision: 7554 $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/ApplicationLayer/PSC/eDeviceEvent.cc $

Copyright (c) 2003,2004,2005 Steffen Simon.
Copyright (c) 2008 Steffen Simon.
All rights reserved.

--------------------------------------------------------------------------*/
#include "coreSystemTimer.h"
#include "eDeviceEvent.h"

#ifdef __DEBUG__
#include "eDevice.h"
#endif

//#define EVENT_QUEUE_ON_HEAP

#ifdef EVENT_QUEUE_ON_HEAP
	extern unsigned char __heap_start;
	static event*	eventStorage = (event*)&__heap_start;
	const int16		sizeOfEventQueue(16);
#else
	#ifdef __DEBUG__
	static event	eventStorage[16];
	#else
	static event	eventStorage[16];
	#endif
	const int16		sizeOfEventQueue(sizeof(eventStorage)/sizeof(event));
#endif

static event*	eventQueue(NULL);
#ifndef PREEMPTION_SUPPORTED
static volatile event*	mainThreadEventQueue((event*)NULL);
#endif

int8	eventsInUse(0);
int8	eventsInUsePeak(0);
int16	eventCollisions(0);
int16	eventEvalCollisions(0);
int16	eventQueueUnderflow(0);

void (*WaitEventCallbackHandler)(void);

void initializeEvents(void)
	{
	int16	i;
	
#if __DEBUG__  > 4
	controller.asc0.lock();
	controller.asc0 << "initializeEvents\r";
	controller.asc0.unlock();
#endif
	eventQueue = eventStorage;
	eventStorage[0].next = (event*)NULL;

	for (i=1; i<sizeOfEventQueue; i++)
		{
		eventStorage[i].next = (event*)NULL;
		eventStorage[i-1].next = &eventStorage[i];
		}

#if __DEBUG__  > 4
	controller.asc0.lock();
	controller.asc0 << "initializeEvents: eventQueue == " << (void*)eventQueue << newline;
	controller.asc0 << "initializeEvents: sizeOfEventQueue == " << sizeOfEventQueue << newline;
	controller.asc0 << "initializeEvents: sizeof(event) == " << (int32)sizeof(event) << newline;
	controller.asc0 << "initializeEvents: sizeof(eventStorage) == " << (int32)sizeof(eventStorage) << newline;

	for (i=0; i<sizeOfEventQueue; i++)
		{
		controller.asc0 << "initializeEvents: event #" << i << " == " << (void*)&eventStorage[i] << ", next == " << (void*)eventStorage[i].next << newline;
		}
	controller.asc0.unlock();
#endif
	}

bool waitForEvent(event& e, uint16 eventClass, uint16 eventType, tick_t timeout)
	{
	bool	result(false);
	event*	queue;
	event*	queuePrev(NULL);
			
		
	e.eventClass = eventClass;
	e.eventType = eventType;

	tick_t	timeoutTime;
	
	if (timeout != forever)
		timeoutTime = (coreSystemTimerTicks() + timeout);
	else
		timeoutTime = forever;	

	do
		{
		if (WaitEventCallbackHandler)
			{
			WaitEventCallbackHandler();
			}

    	queue = (event*)mainThreadEventQueue;

	#if __DEBUG__  > 4
		controller.asc0.lock();
		controller.asc0 << "waitForEvent:\r";
		controller.asc0 << "     eventClass:       " << eventClass << newline;
		controller.asc0 << "     eventType:        " << eventType << newline;
		controller.asc0.unlock();
	#endif
		while (queue)
			{
		#if __DEBUG__  > 4
			controller.asc0.lock();
			controller.asc0 << "     queue:            " << (void*)queue << newline;
			controller.asc0 << "     queue->class:     " << (*queue).eventClass << newline;
			controller.asc0 << "     queue->type:      " << (*queue).eventType << newline;
			controller.asc0.unlock();
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
			controller.asc0.lock();
			controller.asc0 << "waitForEvent:\r";
			controller.asc0 << "     got event!" << newline;
			controller.asc0 << "     eventClass:       " << e.eventClass << newline;
			controller.asc0 << "     eventType:        " << e.eventType << newline;
			controller.asc0.unlock();
		#endif
			}
		else
			{
			tick_t	timeRemaining(timeoutTime - coreSystemTimerTicks());
			
			if (timeRemaining > 50)
				coreSystemTimerWait(50);
			else if (timeRemaining > 0)
				coreSystemTimerWait(timeoutTime - coreSystemTimerTicks());
			}
		}
	while (!result && (coreSystemTimerTicks() < timeoutTime));

	return result;
	}

void sendEventSelf(const event& e)
	{
#if __DEBUG__  >4
	controller.asc0.lock();
	controller.asc0 << "send event:\r";
	controller.asc0 << "     class:   " << (uint16)e.eventClass << newline;
	controller.asc0 << "     type:    " << (uint16)e.eventType << newline;
	controller.asc0.unlock();
#endif
	event*	theEvent(eventQueue);
	
	if (theEvent && (eventsInUse < sizeOfEventQueue))
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
			controller.asc0.lock();
			controller.asc0 << "eventsInUsePeak => " << eventsInUsePeak << newline;
			controller.asc0.unlock();
			}
	#endif
		(*theEvent).eventClass = e.eventClass;
		(*theEvent).eventType = e.eventType;
		(*theEvent).data = e.data;
	#if __DEBUG__  > 4
		controller.asc0.lock();
		controller.asc0 << "     mainThreadEventQueue: " << (void*)mainThreadEventQueue << newline;
		controller.asc0 << "     threadQueue:          " << (void*)threadQueue << newline;
		controller.asc0 << "     &theEvent:            " << (void*)theEvent << newline;
		controller.asc0.unlock();
	#endif
		}
	else
		{
	#if __DEBUG__  > 0
		controller.asc0.lock();
		controller.asc0 << "send event failed!" << newline;
		controller.asc0 << "     class:   " << (uint16)e.eventClass << newline;
		controller.asc0 << "     type:    " << (uint16)e.eventType << newline;
		controller.asc0.unlock();
	#endif
		eventQueueUnderflow++;
		}
	}

void sendEventSelf(uint16 eventClass, uint16 eventType, const eventData& data)
	{
	event		theEvent;

	theEvent.eventClass = eventClass;
	theEvent.eventType = eventType;
	theEvent.data = data;
	
	sendEventSelf(theEvent);
	}

