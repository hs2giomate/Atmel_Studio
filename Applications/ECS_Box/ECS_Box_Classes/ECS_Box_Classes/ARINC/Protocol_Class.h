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



class Protocol_Class: public virtual ARINC_Conversions
{
//functions
public:
	virtual ~Protocol_Class();

	virtual  uint32_t	ReadBufferLabel(uint8_t)=0;
	//virtual uint8_t		Label2Byte(uint32_t)=0;
	uint8_t   GetActiveLine(void);
	uint32_t GetDataField(uint32_t label);
protected:
	//virtual uint8_t		Label2Byte(uint32_t)=0;
//	friend	class	ARINC_Conversions;
private:
	Datagram	datagram;

}; //Protocol_Class

#endif //__PROTOCOL_CLASS_H__
