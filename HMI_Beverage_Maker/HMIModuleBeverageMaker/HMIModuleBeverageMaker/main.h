
/** \file		main.h
 *
 *  \brief		main 
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

/** \brief Handler for a fatal error condition, such as not being able to create the data log file
 *
 *  \return  Nothing
 */
void OnFatalError(void);

/** \brief Indicates if a fatal error condition has already been flagged
 *
 *  \return  true if a fatal error condition has already been flagged
 */
bool isFatalError(void);

/** \brief Indicates if the HALT sequence should continue
 *
 *  \return  true if the HALT sequence should continue
 */
bool IsRun(void);

/** \brief Enable / Disable the HALT sequence
 *
 *  \return  Nothing
 */
void setRun(bool bRun);


/*** END of FILE ***/