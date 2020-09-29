/* 
* SerialTerminal_Class.h
*
* Created: 7/23/2020 6:43:18 PM
* Author: GMateusDP
*/


#ifndef __SERIALTERMINAL_CLASS_H__
#define __SERIALTERMINAL_CLASS_H__
#define		SERIALTERMINALCLASSVERSION	0x01
#define		SERIALTERMINALCLASSSUBVERSION	0x00
#include "compiler.h"
#define USB_TIMEOUT	10

class SerialTerminal_Class
{
//variables
public:
	volatile bool	connected, plugged,timeout;

protected:
private:
	bool	vbusDetected;

//functions
public:
	SerialTerminal_Class();
	~SerialTerminal_Class();
	void	OnInit(void);
	bool	CheckTerminal(void);
	bool	CheckTerminal(uint32_t size);
	void	OnPlugged(void);
	bool	VBUSDetected(void);
protected:
private:
	SerialTerminal_Class( const SerialTerminal_Class &c );
	SerialTerminal_Class& operator=( const SerialTerminal_Class &c );

}; //SerialTerminal_Class

#endif //__SERIALTERMINAL_CLASS_H__
