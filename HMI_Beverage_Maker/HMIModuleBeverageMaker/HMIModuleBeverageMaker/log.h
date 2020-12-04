
/** \file		log.h
 *
 *  \brief		log HALT results to a file
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 



/* Function prototypes */


/** \brief Open the data log file
 *
 *  \return  Nothing
 */
bool logOpen(void);

/** \brief Close the data log file
 *
 *  \return  Nothing
 */
void logClose(void);

/** \brief Append an entry to the data log file
 *
 *  \return  Nothing
 */
bool logAppendHaltEntry(bool isRead, bool success, uint32_t leftButtonPressInterval, uint32_t centreButtonPressInterval, uint32_t rightButtonPressInterval);

/** \brief Pause the data log file
 *
 *  \return  Nothing
 */
void logPause(uint32_t i2cTrasmissionFailCount);

/** \brief Resume the data log file, after it was previously paused
 *
 *  \return  Nothing
 */
void logResume(void);

/** \brief Ready the log file at the end of a HALT sequence
 *
 *  \return  Nothing
 */
void logFinish(uint32_t i2cTrasmissionFailCount);


/*** END of FILE ***/