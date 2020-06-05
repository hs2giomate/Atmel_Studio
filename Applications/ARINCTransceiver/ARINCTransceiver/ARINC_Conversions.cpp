/* 
* ARINC_Conversions.cpp
*
* Created: 4/15/2020 9:40:33 AM
* Author: GMateusDP
*/


#include "ARINC_Conversions.h"


// default constructor
ARINC_Conversions::ARINC_Conversions()
{
} //ARINC_Conversions

// default destructor
ARINC_Conversions::~ARINC_Conversions()
{
} //~ARINC_Conversions

uint8_t ARINC_Conversions::octal2Hex(uint32_t label){
	uint8_t octals[3], result;
	result=0;
	for (int i = 0; i < sizeof(octals); i++)
	{
		octals[i]=int(label/(pow(10,i)))<<(sizeof(octals)-i);
		result|=octals[i];
	}
	return result;
	
}

uint32_t ARINC_Conversions::Hex2Octal(uint8_t hex){
	uint8_t octals[3];
	uint32_t result;
	result=0;
	for (int i = 0; i < sizeof(octals); i++)
	{		
		octals[i]=(uint8_t)((0x07<<sizeof(octals)*i)&hex);
		octals[i]=octals[i]>>sizeof(octals)*i;
		result+=octals[i]*pow(10,i);
	}
	return result;
}

uint8_t ARINC_Conversions::atohex(unsigned char ch)
{
	uint8_t hex;
	
	if((ch>='0') && (ch<='9'))
	hex = ch - '0';
	else if ((ch>='A') && (ch<='F'))
	hex = ch - 'A' + 10;
	else if ((ch>='a') && (ch<='f'))
	hex = ch - 'a' +10;
	else hex = 0;
	
	return hex;

}
char ARINC_Conversions::atohex2(char upperNibble, char lowerNibble)
{
	char hex2;
	hex2 = 0;
	hex2 = atohex(upperNibble) << 4;    // get upper hex nibble
	hex2 |= atohex(lowerNibble);        // get lower hex nibble and merge
	return hex2;

}