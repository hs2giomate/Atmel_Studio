/* 
* CDC_Class.h
*
* Created: 6/4/2020 3:05:36 PM
* Author: GMateusDP
*/


#ifndef __CDC_CLASS_H__
#define __CDC_CLASS_H__
#include "driver_init.h"
#include "usb_start.h"
#include "Stream.h"

class CDC_Class: public Stream
{
//variables
public:
	volatile	bool	txReady;
	volatile	bool	rxReady;
	volatile	int _serialPeek;
protected:
private:
	usbd_descriptors *usb_desc;

//functions
public:
	CDC_Class();
	CDC_Class(usbd_descriptors *);
	~CDC_Class();
	void init(void);
	 bool is_enabled(void);
	virtual int available(void);
	virtual int peek(void);
	virtual int read(void);
	virtual void flush(void);
	virtual void clear(void);
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buffer, size_t size);
	using Print::write; // pull in write(str) from Print

protected:
private:
	CDC_Class( const CDC_Class &c );
	CDC_Class& operator=( const CDC_Class &c );

}; //CDC_Class

//extern	CDC_Class	usb;
#endif //__CDC_CLASS_H__
