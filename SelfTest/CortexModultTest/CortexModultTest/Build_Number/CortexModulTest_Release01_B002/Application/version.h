/*--------------------------------------------------------------------------

    version.h

    Interface
    Versions/Revisionsinformationen

THIS IS A AUTOMATIC GENERATED FILE!
MODIFICATIONS WILL BE OVERWRITTEN

--------------------------------------------------------------------------*/
#ifndef VERSION_H
#define VERSION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t versionMajor;
extern const uint8_t versionMinor;
extern const uint8_t versionBugfix;
extern const uint8_t developmentStage;
extern const uint16_t release;
extern const uint16_t build;
extern const uint16_t revision;


extern const char* versionString;
extern const char* revisionString;
extern const char* targetString;

const char* getRevisionAsString(void);
const char* getVersionAsString(void);
const char* getTargetAsString(void);
const char* getBuildDateAsString(void);
const char* getBuildTimeAsString(void);
const char* getDeveloperNameAsString(void);
const char* getBuildMachineAsString(void);

#ifdef __cplusplus
	}
#endif

#endif // VERSION_H
