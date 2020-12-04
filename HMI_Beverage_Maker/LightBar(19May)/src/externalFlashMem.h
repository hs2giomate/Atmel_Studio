/** \file		externalFlashMem.h
 *
 *  \brief		perform write-read-verify test for HALT
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef EXTERNALFLASHMEM_H
#define EXTERNALFLASHMEM_H

	#ifdef __cplusplus
	extern "C"{
		#endif

/** \brief Manages the test writes to the external SPI connected flash memory during HALT.
 *
 *  \return  Nothing
 */
void esfHALTtestManage(void);

/** \brief Restarts the test to the external SPI connected flash memory when the test-box restarts the test.
 *
 *  \return  Nothing
 */
void esfTestRestart(void);

/** \brief Initialises in preparation for the test writes to the external SPI connected flash memory during HALT.
 *
 *  \return  Nothing
 */
void esfInit(void);

#ifdef __cplusplus
}
#endif

#endif /* EXTERNALFLASHMEM_H */
/*** END of FILE ***/