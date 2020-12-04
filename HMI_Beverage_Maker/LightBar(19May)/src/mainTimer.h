/** \file		mainTimer.h
 *
 *  \brief		Support time intervals as required by the rest of the code
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 

#ifndef MAIN_TIMER_H
#define MAIN_TIMER_H

#ifdef __cplusplus
extern "C"{
	#endif

/* Global Variables */
extern uint8_t gc10msTick;
extern uint8_t gc100msTick;
extern uint8_t gc1sTick;
extern uint8_t gc250msTick;

extern uint32_t gdw_msTimeCount;
extern uint32_t gdwSecondsAlive;


/** Initialise the main timer which is used for general timing
	\return Nothing
*/   
/** \brief .
 *
 *  \return  Nothing
 */
void mainTimerInit(void);

/** \brief Manage general timing events. Called once from the main loop and manages slower timing that can be handled outside of the interrupt.
 *
 *  \return  Nothing
 */
void mainTimerService(void);
#ifdef __cplusplus
}
#endif
#endif

/*** END of FILE ***/