/*
 * 922_Oven_Test.cpp
 *
 * Created: 5/7/2020 3:52:41 PM
 * Author : GMateusDP
 */ 

#include <avr/io.h>

#include "eCore.h"
#include "GAINClass.h"
#include "eDeviceEvent.h"
#ifdef __DEBUG__
#ifdef AVR
#include <avr/pgmspace.h>
#endif
#include "version.h"
#include "eDevice.h"
#endif

/***************************************************************************/
/*  main                                                                   */
/***************************************************************************/


#warning "Remove me!"
#ifdef BOHNER_ICB_Revision
#define MGS_ZST_Revision 0x0500
#endif

#ifdef MGS_ZST_Revision
int main (void)
{
	initializeEvents();

	#if __DEBUG__
	controller.asc0 << "_______________________________________________________" << newline;
	controller.asc0 << "Startup sequence finished" << newline;

	if (coreControllerWatchdogResetOccured())
	controller.asc0 << "*** Watchdog fired ***" << newline;

	if (coreControllerBrownOutDetected())
	controller.asc0 << "*** brown out detected ***" << newline;

	controller.asc0 << getVersionAsString() << newline;
	controller.asc0 << getRevisionAsString() << newline;
	controller.asc0 << getTargetAsString() << newline;
	controller.asc0 << getBuildDateAsString() << ", " << getBuildTimeAsString() << newline << newline;
	controller.asc0 << "_______________________________________________________" << newline << newline << newline;
	#endif
	
	GAIN.run();
	
	return(0);
}
#else
int main (void)
{
	controller.initialize();
	initializeEvents();
	#ifdef __DEBUG__
	controller.asc0 << kModePSTR << PSTR("_______________________________________________________") << newline;
	controller.asc0 << kModePSTR << PSTR("Startup sequence finished") << newline;

	controller.asc0 << getVersionAsString() << newline;
	controller.asc0 << getRevisionAsString() << newline;
	controller.asc0 << getTargetAsString() << newline;
	controller.asc0 << getBuildDateAsString() << ", " << getBuildTimeAsString() << newline << newline;
	controller.asc0 << kModePSTR << PSTR("_______________________________________________________") << newline << newline << newline;
	#endif

	GAIN.run();
	
	return(0);
}
#endif