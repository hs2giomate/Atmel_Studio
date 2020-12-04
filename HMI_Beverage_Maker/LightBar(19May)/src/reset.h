/** \file		reset.h
 *
 *  \brief		Read the micro-controller register that indicates the reason for the last reset
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef RESET_H
#define RESET_H

/** \brief Initialise the reset object, read the reason for the reset.
 *
 *  \return  Nothing
 */
void rstInit(void);

/** \brief .
 *
 *  \return  the value read from RST.STATUS
 */
uint8_t rstGetLastReasonForReset(void);




#endif /* RESET_H */
/*** END of FILE ***/