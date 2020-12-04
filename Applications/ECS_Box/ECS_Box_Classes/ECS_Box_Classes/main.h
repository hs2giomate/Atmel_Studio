/*
 * main.h
 *
 * Created: 6/9/2020 10:26:57 AM
 *  Author: GMateusDP
 */ 


#ifndef MAIN_H_
#define MAIN_H_
//#include "sam.h"
#include <atmel_start.h>
#include "CDC_Class.h"
//#include "AT24MAC_Class.h"
#include "LTC2983_Class.h"
#include "Timer_Class.h"
#include "N25Q256_Class.h"
#include "Calendar_Class.h"
//#include "MCP23017_Class.h"

void usb_test(void);
uint8_t EEPROM_Test(void);
void LTC2983_test(void);

uint8_t	StartLivePulse(void);
void	QSPIDriverTest(void);
void	DateTimeTest(void);
void	IO_ExpanderTest(void);
#endif /* MAIN_H_ */