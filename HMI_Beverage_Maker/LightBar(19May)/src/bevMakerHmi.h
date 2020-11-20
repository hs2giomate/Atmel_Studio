/** \file		bevMakerHmi.c
 *
 *  \brief		include file for all project files
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#ifndef BEV_MAKER_HMI_H
#define BEV_MAKER_HMI_H
#ifdef __cplusplus
extern "C"{
	#endif

/* clarify configuration of a high-impedance input with a new define */
#define PORT_OPC_INPUT_HIGH_IMPEDANCE		PORT_OPC_TOTEM_gc

/* CPU frequency is 8 MHz */
#ifndef F_CPU
/*! \brief Define default CPU frequency, if this is not already defined. */
#define F_CPU 8000000UL
#endif

#include <avr/io.h>

#include "compiler.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Uncomment only one of the following, to define the target LED controller IC */
#define LED_DRIVER_PCA9955B
//#define LED_DRIVER_TLC59116


/*! \brief This macro will protect the following code from interrupts. */
#define AVR_ENTER_CRITICAL_REGION( ) uint8_t volatile saved_sreg = SREG; cli();

/*! \brief This macro must always be used in conjunction with AVR_ENTER_CRITICAL_REGION
 *        so the interrupts are enabled again.
 */
#define AVR_LEAVE_CRITICAL_REGION( ) SREG = saved_sreg;

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

static inline void EnableInterrupts(void)	{sei();}
static inline void DisableInterrupts(void)	{cli();}
#ifdef __cplusplus
}
#endif

#endif	/* BEV_MAKER_HMI_H */



/*** END of FILE ***/
