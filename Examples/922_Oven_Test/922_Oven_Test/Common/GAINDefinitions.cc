/*--------------------------------------------------------------------------

GAINDefinitions.cc

Implementation

Common GAIN enum and type definitions

$Date: 2018-04-05 14:33:17 +0200 (Do, 05. Apr 2018) $
$Revision: 7912 $
$Author: steffen $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/Common/GAINDefinitions.cc $

--------------------------------------------------------------------------*/
#include "GAINDefinitions.h"

#ifdef __DEBUG__
#ifdef AVR
const char gainStateDescription[][18] PROGMEM =
#else
const char* gainStateDescription[] =
#endif
	{
	"Reset",
	"Initialize",
	"Resume",
	"StandbyOFF",
	"PrepareStandbyON",
	"StandbyON",
	"StandbyReady",
	"Delayed",
	"Interrupted",
	"ON",
	"StoppedProcess",
	"Cleanup",
	"Finished",
	"KeepHot",
	"SelfProtect",
	"Error",
	"Maintenance",
	"HMIUpdate",
	"FirmwareUpdate",
	};
#endif

