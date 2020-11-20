/** \file		lowPower.h
 *
 *  \brief		Enable / Disable power regulators
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef LOWPOWER_H
#define LOWPOWER_H

#ifdef __cplusplus
extern "C"{
	#endif
/** \brief Initialise the low power (standby) mode driver .
 *
 *  \return  Nothing
 */
void lowPowerInit(void);

/** \brief Enable/Disable the regulator circuits.
 *
 *  \return  Nothing
 */
void lowPowerTestEnable(bool bEnable);


#ifdef __cplusplus
}
#endif


#endif /* LOWPOWER_H */
/*** END of FILE ***/