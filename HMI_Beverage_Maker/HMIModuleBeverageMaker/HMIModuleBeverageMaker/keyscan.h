
/** \file		keyscan.h
 *
 *  \brief		scan for test-box keyboard activity
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


enum keyCode_enum
{
	gcEscapeKey = 27,
	gcSpaceKey = 32,
};


/* Function prototypes */

/** \brief change the test-box keyboard handler to enable keystroke detection
 *
 *  \return  Nothing
 */
void changemode(int);

/** \brief return the key pressed on the test-box keybard, if any
 *
 *  \return  Nothing
 */
int  kbhit(void);


/*** END of FILE ***/