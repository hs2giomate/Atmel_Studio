/* 
* ARINC_Interface.h
*
* Created: 3/18/2020 12:01:24 PM
* Author: GMateusDP
*/


#ifndef __ARINC_INTERFACE_H__
#define __ARINC_INTERFACE_H__
#include "USB_CDC_Class.h"
#include "SPI_Asyn_Class.h"
#define SELFTEST_ON 1
#define SELFTEST_OFF 0
#define g_RXBuffSize 4  
#define TRANSMIT_PARTY_ON  1
#define TRANSMIT_PARTY_OFF 0
#define PARITYODD 0
#define PARITYEVEN 1
#define TMODE 1
#define TFLIP 1
#define RFLIP 0x80

class ARINC_Interface
{
//variables
public:
	USB_CDC_Class	usb0;
	SPI_Asyn_Class	spi0(0);
	Holt_3593_Class	HI3893;
	unsigned char MessageCount;
	bool	BITRATE0;
	bool	PARITY;

protected:
private:

//functions
public:
	ARINC_Interface();
	~ARINC_Interface();
	
	void Init(void);
	void SayHello(void);
	void TransmitReceiveWithLabels_Mode(const uint8_t);
	void xprint(char *string);
	void	CE_Low_Holt(void);
	void	CE_High_Holt(void);
	
protected:
private:
	ARINC_Interface( const ARINC_Interface &c );
	ARINC_Interface& operator=( const ARINC_Interface &c );

}; //ARINC_Interface

#endif //__ARINC_INTERFACE_H__
