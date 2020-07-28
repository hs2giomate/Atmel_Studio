/* 
* ByteBiulder_Class.h
*
* Created: 7/24/2020 1:58:02 PM
* Author: GMateusDP
*/


#ifndef __BYTEBIULDER_CLASS_H__
#define __BYTEBIULDER_CLASS_H__
#include "PinCodding_SAMD21Xplained.h"
#define SPACE	0x20
#define TAB 9
#define CR 0x0D            // carrage return
#define LF 0x0A            // line feed
class GPIOConnector_Class
{
//variables
public:
protected:
private:
	char command,chR,chL;
	uint8_t	i,j,k,byte;
	bool done,bit;

//functions
public:
	GPIOConnector_Class();
	~GPIOConnector_Class();
	uint8_t	RequestByte(void);
	bool	Init(void);
	uint8_t	ReadByteOnEXT2(void);
	uint8_t	PutByteOnEXT2(uint8_t);
	uint8_t	ClearByteOnEXT2(void);
protected:
private:
	GPIOConnector_Class( const GPIOConnector_Class &c );
	GPIOConnector_Class& operator=( const GPIOConnector_Class &c );
	uint8_t	GetCommand(void);
	uint8_t	ConstructByte(void);
	uint8_t Atohex(unsigned char ch);
	uint8_t AtoHex2(char upperNibble, char lowerNibble);
	bool	CheckHex(char ch);
	bool	IsNextHex(char	ch);

}; //ByteBiulder_Class

#endif //__BYTEBIULDER_CLASS_H__
