/* 
* Protocol_Class.h
*
* Created: 6/19/2020 5:54:35 PM
* Author: GMateusDP
*/


#ifndef __PROTOCOL_CLASS_H__
#define __PROTOCOL_CLASS_H__

#include <stdint.h>
#include "ARINC_Conversions.h"
enum LineStatus
{
	NA1=0,
	LINE1,
	LINE2,
	NA2,
};
typedef struct  
{
struct{
	 uint32_t	label:8;
	 uint32_t	SDI:2;
	 uint32_t	dataField:19;
	 uint32_t	ssm:2;
	 uint32_t	P:1;
}bit;
uint32_t	rawWord;
}Datagram;



class Protocol_Class: private virtual ARINC_Conversions
{
	//Variables
public:	

private:
		Datagram	datagram;
		uint32_t value;
		uint8_t label;
		int	counter;
		uint8_t	testByte;
	
	
	

//functions
public:
	virtual ~Protocol_Class();

	virtual  uint32_t	ReadBufferLabel(uint8_t)=0;
	//virtual uint8_t		Label2Byte(uint32_t)=0;
	uint8_t   GetActiveLine(void);
	uint32_t GetDataField(uint32_t label);
	uint32_t MakeLabel374(uint32_t d,float v);
	uint32_t MakeEvenParity(uint32_t data);
	uint32_t MakeLabel270(uint32_t d);
	uint32_t MakeTXLabel275(uint8_t d);
protected:
	//virtual uint8_t		Label2Byte(uint32_t)=0;
//	friend	class	ARINC_Conversions;
private:

	uint8_t WavingValue(void);

}; //Protocol_Class

#endif //__PROTOCOL_CLASS_H__
