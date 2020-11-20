/** \file		manualResetDetect.h
 *
 *  \brief		Detect the manual reset condition
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef MANUALRESETDETECT_H
#define MANUALRESETDETECT_H
#ifdef __cplusplus
extern "C"{
	#endif
/** \brief Initialise the manual Reset Detect.
 *
 *  \return  Nothing
 */
void manualResetDetectInit(void);

/** \brief Check for manual Reset condition.
 *
 *  \return  true if the condition is detected
 */
bool manageManualResetDetect(void);


#ifdef __cplusplus
}
#endif

#endif /* MANUALRESETDETECT_H */
/*** END of FILE ***/