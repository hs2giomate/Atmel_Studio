/* 
* ARINC_Conversions.h
*
* Created: 4/15/2020 9:40:34 AM
* Author: GMateusDP
*/


#ifndef __ARINC_CONVERSIONS_H__
#define __ARINC_CONVERSIONS_H__
#include "sam.h"
#include "math.h"
/*
enum
{
	label103=0,
	label211,
	labe301,
};

uint32_t Labels [3] = {
	//   [0]     [1]     [2]
	270,   271,   370,
};

struct ARINC429Struct
{
	uint8_t		label;
	uint32_t	databits;
	uint8_t		SSM;
	uint8_t		P;
	
};
typedef	struct ARINC429Struct ARINC429Struct;
*/
class ARINC_Conversions
{
//variables
public:
protected:
private:
	//ARINC429Struct	datagram;
//functions
public:
	ARINC_Conversions();
	~ARINC_Conversions();
	uint8_t octal2Hex(uint32_t);
	uint32_t Hex2Octal(uint8_t);
	char	atohex2(char upperNibble, char lowerNibble);
	uint8_t atohex(unsigned char ch);
	friend	class	ARINC_Interface;
protected:
private:
	ARINC_Conversions( const ARINC_Conversions &c );
	ARINC_Conversions& operator=( const ARINC_Conversions &c );
	

}; //ARINC_Conversions

#endif //__ARINC_CONVERSIONS_H__
