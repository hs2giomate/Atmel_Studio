/*--------------------------------------------------------------------------

$Date: $
$Revision: $

THIS IS A AUTOMATIC GENERATED FILE!
MODIFICATIONS WILL BE OVERWRITTEN

--------------------------------------------------------------------------*/
#include "version.h"


const uint8_t versionMajor = 1;
const uint8_t versionMinor = 0;
const uint8_t versionBugfix = 0;
const uint8_t developmentStage = 1;
const uint16_t release = 1;
const uint16_t build =4;
const uint16_t revision = 1;


const char* versionString = "1.0d1b01";
const char* revisionString = "Revision: 1";
#if BOOTABLE >1
	const char* targetString = "Release";	
#else
	const char* targetString = "Debug";	
#endif
const char* DeveloperName = "GMateusDP";
const char* ComputerName = "1260HS2";

const char* getVersionAsString(void)
	{
	return versionString;
	}

const char* getRevisionAsString(void)
	{
	return revisionString;
	}

const char* getTargetAsString(void)
	{
	return targetString;
	}

const char* getBuildDateAsString(void)
	{
	return __DATE__;
	}

const char* getBuildTimeAsString(void)
	{
	return __TIME__;
	}
	
const char* getDeveloperNameAsString(void)
	{
		return DeveloperName;
	}
	
const char* getBuildMachineAsString(void)
	{
		return ComputerName;
	}

