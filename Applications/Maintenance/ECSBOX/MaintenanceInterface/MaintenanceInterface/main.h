/*
 * main.h
 *
 * Created: 7/20/2020 1:47:35 PM
 *  Author: GMateusDP
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "FlashMemoryClass.h"
#include "FRAM_Memory_Class.h"
#include "CDC_Class.h"
#include "TimerSerial_Class.h"
#include "Maintenance_Tool.h"
#include "Parameters.h"
#include "Maintenance_Tool.h"
#include "Timer_Class.h"
#include "MemoryFlash_Class.h"
#include "TemperatureSensors_Class.h"
#include "EvaporatorAndCondesatorFans_Class.h"

#include "Dual_Flapper_Valve_Controller.h"
#include "HEATERS_HANDLER.h"
#include "Scavenge_Fan.h"
#include "Compresor_Controller.h"
#include "Event_Logger_Class.h"

void	FramMemoryTest(void);
void	FlashMemoryTest(void);
#endif /* MAIN_H_ */