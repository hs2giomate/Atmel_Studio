/** \file		rs485test.h
 *
 *  \brief		test the RS485 hardware with a simple loop-back connection
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.
 *
 *  \date		12 September 2019
 *  \version	1
 */

#ifndef RS485_TEST_H
#define RS485_TEST_H

#ifdef __cplusplus
extern "C"{
	#endif
/** \brief Manage a loop-back test on Rs485.
 *
 *  \return  Nothing
 */
void rs485testManager(void);

/** \brief Enable the rs485 loop-back test.
 *
 *  \return  Nothing
 */
void rs485testEnable(bool bEnable);

#ifdef __cplusplus
}
#endif

#endif
/*** END of FILE ***/
