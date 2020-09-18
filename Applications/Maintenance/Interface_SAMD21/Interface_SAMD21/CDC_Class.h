/* 
* CDC_Class.h
*
* Created: 6/4/2020 3:05:36 PM
* Author: GMateusDP
*/


#ifndef __CDC_CLASS_H__
#define __CDC_CLASS_H__
#define		USBSERIALCLASSVERSION	0x02
#define		USBSERIALCLASSSUBVERSION	0x00
#include "usb_start.h"
#include "Stream.h"
#include "SerialTerminal_Class.h"

class CDC_Class: public Stream,public SerialTerminal_Class
{
//variables
public:
	volatile	bool	txReady;
	volatile	bool	rxReady;
	volatile	int _serialPeek;
	uint8_t cdc_buffer[64];

	
protected:
private:
	usbd_descriptors *usb_desc;

	

//functions
public:
	CDC_Class();
	CDC_Class(usbd_descriptors *);
	~CDC_Class();
	void Init(void);
	 bool IsEnabled(void);
	virtual bool available(void) ;
	virtual int peek(void);
	virtual int read(void);
	virtual int32_t readData(void* p, int32_t size);
	int32_t readDataAsyn(void* p, int32_t size);
	virtual void flush(void);
	virtual void clear(void);
	virtual size_t write(uint8_t) ;
	virtual int32_t writeData(const void *buffer, int32_t size);
	virtual size_t write(const uint8_t *buffer, size_t size);
	int32_t read(void* p, int32_t size);
	void	read(char *);
	using Print::write; // pull in write(str) from Print
	operator bool();

protected:
private:
	CDC_Class( const CDC_Class &c );
	CDC_Class& operator=( const CDC_Class &c );
	

}; //CDC_Class

extern	CDC_Class	usb;
#endif //__CDC_CLASS_H__
