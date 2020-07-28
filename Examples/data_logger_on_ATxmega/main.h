
#ifndef MAIN_H
#define MAIN_H

// <<< Use Configuration Wizard in Context Menu >>>
// <s> Configurable String
// <i> This string will be sent to console
// <id> conf_string
#ifndef CONF_STRING
#define CONF_STRING "test"
#endif
// <<< end of configuration section >>>

#ifdef __GNUC__
#include "avr/sleep.h"
#elif __ICCAVR__
#include "ioavr.h"
#include "inavr.h"
#endif

#endif
