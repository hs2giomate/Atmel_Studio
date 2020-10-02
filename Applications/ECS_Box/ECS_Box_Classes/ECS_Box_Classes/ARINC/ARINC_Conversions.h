/* 
* ARINC_Conversions.h
*
* Created: 4/15/2020 9:40:34 AM
* Author: GMateusDP
*/


#ifndef __ARINC_CONVERSIONS_H__
#define __ARINC_CONVERSIONS_H__
//#include "sam.h"
#include "stdint.h"
#include "math.h"


class ARINC_Conversions
{
//variables
public:
protected:
private:
	uint8_t i,j,k;
	uint32_t result;
	//ARINC429Struct	datagram;
//functions
public:
	ARINC_Conversions();
	~ARINC_Conversions();
	uint8_t		Octal2Hex(uint32_t);
	uint32_t	Hex2Octal(uint8_t);
	char		AtoHex2(char upperNibble, char lowerNibble);
	uint8_t		atohex(unsigned char ch);
	virtual		uint8_t	FlipByte(uint8_t);
	virtual		uint8_t	Label2Byte(uint32_t);
	uint32_t	GetData(uint32_t);
	void		PrepareSingleTXBuffer(uint8_t* buff, uint8_t* src);
	uint8_t		GetIndexTXLabelarray(uint8_t l,uint8_t* arr);
	uint32_t	FourBytesArray2Uint32(uint8_t* arr);
	//virtual	uint32_t	ReadBufferLabel(uint8_t)=0;
protected:
private:
	ARINC_Conversions( const ARINC_Conversions &c );
	ARINC_Conversions& operator=( const ARINC_Conversions &c );
	

}; //ARINC_Conversions

#endif //__ARINC_CONVERSIONS_H__
