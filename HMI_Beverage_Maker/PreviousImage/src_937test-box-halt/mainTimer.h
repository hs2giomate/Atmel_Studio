
/** \file		mainTimer.h
 *
 *  \brief		main timer which is used for general timing
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 



/* Global Variables */
extern uint8_t gc10msTick;
extern uint8_t gc100msTick;
extern uint8_t gc1sTick;
extern uint8_t gc250msTick;

extern uint32_t gdw_msTimeCount;
extern uint32_t gdwSecondsAlive;
extern uint32_t gdw_100msTimeCount;


/** \brief Initialise the main timer which is used for general timing
 *
 *  \return  Nothing
 */
void mainTimerInit(void);

/** \brief Manage general timing events.	This is called from the main loop and manages slower timing that can be handled outside of the interrupt.
 *
 *  \return  Nothing
 */
void mainTimerService(void);

/** \brief Formats the current date and time into the given buffer
 *
 *  \return  Nothing
 */
void GetDateAndTime(char* pBuf, uint8_t cMaxLength);

/** \brief print the current date and time to stdout
 *
 *  \return  Nothing
 */
void printfDateAndTime(void);


/*** END of FILE ***/