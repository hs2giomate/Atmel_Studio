
/** \file		hmiManualReset.h
 *
 *  \brief		monitor the HMI Manual Reset signal 
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


extern uint8_t gHmiManualResetState;


/** \brief Initialise the HMI Manual Reset detect module
 *
 *  \return  Nothing
 */
void hmrInit(void);

/** \brief Handles state changes of the HMI Manual Reset Signal
 *
 *  \return  Nothing
 */
void hmrReadHmiManualResetSignal(void);






/*** END of FILE ***/