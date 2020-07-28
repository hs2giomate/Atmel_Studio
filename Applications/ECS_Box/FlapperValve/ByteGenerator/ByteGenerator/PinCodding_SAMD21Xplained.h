/* 
* PinCodding_SAMD21Xplained.h
*
* Created: 7/27/2020 10:11:37 AM
* Author: GMateusDP
*/


#ifndef __PINCODDING_SAMD21XPLAINED_H__
#define __PINCODDING_SAMD21XPLAINED_H__
#include "compiler.h"
#include "driver_init.h"
typedef struct Connector
{
	uint8_t	pinsInput[8];
	uint8_t	pinsOutput[8];
};


class PinCodding_SAMD21Xplained
{
//variables
public:
	 Connector	connector;
protected:
private:
	uint8_t	i,j,k;

//functions
public:
	PinCodding_SAMD21Xplained();
	PinCodding_SAMD21Xplained(Connector& );
	~PinCodding_SAMD21Xplained();
	bool	Init(void);
protected:
private:
	PinCodding_SAMD21Xplained( const PinCodding_SAMD21Xplained &c );
	PinCodding_SAMD21Xplained& operator=( const PinCodding_SAMD21Xplained &c );

}; //PinCodding_SAMD21Xplained
extern	PinCodding_SAMD21Xplained	EXT2;
#endif //__PINCODDING_SAMD21XPLAINED_H__
