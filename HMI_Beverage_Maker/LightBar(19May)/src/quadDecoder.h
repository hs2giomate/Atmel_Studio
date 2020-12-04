/** \file		quadDecoder.h
 *
 *  \brief		Quadrature Decoder test
 *
 *  \author		Tim Ingersoll
 *
 *  \copyright	Copyright (c) 2019 TT Electronics
 *					All rights reserved.  
 * 
 *  \date		12 September 2019
 *  \version	1
 */ 


#ifndef QUADDECODER_H
#define QUADDECODER_H
#ifdef __cplusplus
extern "C"{
	#endif

/** \brief Initialise the quadrature decoder.
 *
 *  \return  Nothing
 */
void quadInit(void);

/** \brief Enable the quadrature decoder test.
 *
 *  \return  Nothing
 */
void quadTestEnable(bool bEnable);

/** \brief Manage the quadrature decoder test.
 *
 *  \return  Nothing
 */
void quadManager(void);


#ifdef __cplusplus
}
#endif

#endif /* QUADDECODER_H */
/*** END of FILE ***/