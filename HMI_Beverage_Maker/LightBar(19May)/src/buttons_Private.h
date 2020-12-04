/** \file		buttons_Private.h
 *
 *  \brief		buttons manager
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef BUTTONS_PRIVATE_H
#define BUTTONS_PRIVATE_H
#include "compiler.h"
#ifdef __cplusplus
extern "C"{
	#endif
/*
  Button layout, looking at the HMI from the front,
				Left			Centre			Right
				On/Off, Brew/Confirm, HotWater/Cancel
				On/Off,		Btn1,				Btn2
				SW1802,		SW1803,			SW1804

Matrix :
							Col0			Col1
							SW-COL1		SW-COL2
Row0 (SW-ROW1)			SW1803		SW1804
*/

#define VOID_BUTTON_NUM_HMI				0xFF		/* Invalid button #, does not exist on this variant */
#define HIGHEST_VALID_COLUMN_NUM			1
#define COLUMN_COUNT				(HIGHEST_VALID_COLUMN_NUM +1)
#define HIGHEST_VALID_ROW_NUM				0
#define ROW_COUNT					(HIGHEST_VALID_ROW_NUM +1)
#define MATRIX_BUTTON_COUNT					2

static const int BUTTON_DEBOUNCE_INTERVAL_MS = 20;	/* ms */
static const int BTN_STATE_PUBLIC_INTERVAL_MIN_MS = 100;	/* ms */


typedef struct ButtonState
{
	uint32_t m_dw_TimeOfLastChange_ms;
	bool m_bUnconfirmedState_Active:1;	/* Used while confirming a state after a change */
	bool m_bActive_Confirmed:1;	/* state reported to the host */
} ButtonState_t;

/** \brief .
 *
 *  \return  Nothing
 */
void ManageButtonScan(void);


#ifdef __cplusplus
}
#endif

#endif /* BUTTONS_PRIVATE_H */
/*** END of FILE ***/