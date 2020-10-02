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

uint8_t ARINC_Conversions::Octal2Hex(uint32_t label){
	uint8_t octals[3], result;
	result=0;
	for (uint8_t i = 0; i < sizeof(octals); i++)
	{
		octals[i]=int(label/(pow(10,i)))<<(sizeof(octals)-i);
		result|=octals[i];
	}
	return result;
	
}

uint8_t ARINC_Conversions::Label2Byte(uint32_t label){
	uint8_t octals[3], result;
	result=0;
	for (uint8_t i = 0; i < sizeof(octals); i++){
		
		octals[2-i]=(uint8_t)(label/(pow(10,2-i)));
		label-=octals[2-i]*pow(10,2-i);
		result|=octals[2-i]<<(6-3*i);
	}
	result=FlipByte(result);
	return result;
	
}
uint32_t ARINC_Conversions::Hex2Octal(uint8_t hex){
	uint8_t octals[3];
	uint32_t result;
	result=0;
	for (uint8_t i = 0; i < sizeof(octals); i++)
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
char ARINC_Conversions::AtoHex2(char upperNibble, char lowerNibble)
{
	char hex2;
	hex2 = 0;
	hex2 = atohex(upperNibble) << 4;    // get upper hex nibble
	hex2 |= atohex(lowerNibble);        // get lower hex nibble and merge
	return hex2;

}


uint8_t ARINC_Conversions::FlipByte(uint8_t byte){
	uint8_t rev=0;
	for (int i=0;i<8;i++)
	{
		rev=rev<<1;
		rev|=(byte>>i)&(0x01);
	}
	return rev;
}
uint8_t ARINC_Conversions::GetIndexTXLabelarray(uint8_t l, uint8_t* arr){
	for (i = 0; i < 32; i++)
	{
		if (arr[i]==l)
		{
			return i;
		}
		
	}
	return 32;
}

void ARINC_Conversions::PrepareSingleTXBuffer(uint8_t* buff, uint8_t* src){

	for (i = 0; i < 4; i++)
	{
		buff[3-i]=src[i];
	
		
	}


}
uint32_t	ARINC_Conversions::FourBytesArray2Uint32(uint8_t* arr){

		result=0;
		for (i = 0; i < 4; i++)
		{
			result|=((uint32_t)arr[i])<<(3-i);

		}
		return result;
}