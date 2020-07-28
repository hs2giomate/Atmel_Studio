/* 
* ByteBiulder_Class.cpp
*
* Created: 7/24/2020 1:58:01 PM
* Author: GMateusDP
*/


#include "GPIOConnector_Class.h"
#include "CDC_Class.h"


const char HexToAscii[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char HexSymbols[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};
// default constructor
GPIOConnector_Class::GPIOConnector_Class()
{
} //ByteBiulder_Class

// default destructor
GPIOConnector_Class::~GPIOConnector_Class()
{
} //~ByteBiulder_Class

bool	GPIOConnector_Class::Init(void){
	return EXT2.Init();
	
}

uint8_t	GPIOConnector_Class::GetCommand(void){
	serial.read(&command);
}

uint8_t	GPIOConnector_Class::RequestByte(void){
		serial.println("\n\r>");
		//serial.println("\n\rEnter 2 HEX  followed by ENTER\n\r");
		serial.println("To Transmit, enter 2 HEX  followed by ENTER");
		serial.println("Format: Bits 8-5, Bits 4-1, then Enter ");
		serial.println("Example: 67 ENTER");
		serial<<"Type an HEX!"<<NEWLINE;
		byte=ConstructByte();
		return byte;
}

uint8_t	GPIOConnector_Class::ConstructByte(){
		
	do{
		
		done=false;
		chL=(char)serial.read();
		done=CheckHex(chL);
		
		
	}while((!done));
	serial<<"Byte: "<<chL;
	do{
		
		done=false;
		chR=(char)serial.read();
		done=CheckHex(chR);
		
		
	}while((!done));
	serial<<chR<<NEWLINE;
	byte=AtoHex2(chL,chR);

	serial.println(" Press RETURN");
	do{
		chL=(char)serial.read();
		
	}while(chL!=CR);
	serial.println(" Got it!...");
	return	byte;
}
bool	GPIOConnector_Class::IsNextHex(char	ch){
	return (( ch == LF) || (ch == CR) || (ch == SPACE) || (ch==EOF)|| (ch==TAB));
}
bool	GPIOConnector_Class::CheckHex(char ch){
		for (i=0;i<(int)sizeof(HexSymbols);i++)
		{
			if (HexSymbols[i]==ch)
			{
				return true;
			}
		}
		return false;
}
uint8_t GPIOConnector_Class::Atohex(unsigned char ch)
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
uint8_t GPIOConnector_Class::AtoHex2(char upperNibble, char lowerNibble)
{
	
	byte = 0;
	byte = Atohex(upperNibble) << 4;    // get upper hex nibble
	byte |= Atohex(lowerNibble);        // get lower hex nibble and merge
	return byte;

}
uint8_t	GPIOConnector_Class::PutByteOnEXT2(uint8_t by){
	uint8_t mask=0x01;
	for (i = 0; i < 8; i++)
	{
		
		gpio_set_pin_level(EXT2.connector.pinsOutput[i],(by&mask)>0);
		mask=mask<<1;
	}
	return	by;
}
uint8_t	GPIOConnector_Class::ClearByteOnEXT2(void){
	for (i = 0; i < 8; i++)
	{

		gpio_set_pin_level(EXT2.connector.pinsOutput[i],false);
	}
	return	0;
}
uint8_t	GPIOConnector_Class::ReadByteOnEXT2(void){
	uint8_t mask=0x01;
	byte=0;
	bit=false;
	for (i = 0; i < 8; i++)
	{
		bit=gpio_get_pin_level(EXT2.connector.pinsInput[7-i]);
		if (bit)
		{
			byte|=(mask);
		}
		
		mask=mask<<1;
	}
	return	byte;
}


