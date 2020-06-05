/* 
* USB_CDC_Class.h
*
* Created: 2/24/2020 1:16:25 PM
* Author: GMateusDP
*/


#ifndef __USB_CDC_CLASS_H__
#define __USB_CDC_CLASS_H__
#include "usb_start.h"


class USB_CDC_Class
{
//variables
public:

	struct io_descriptor *usartio;
	 usbd_descriptors *descUSB;
	uint32_t *ptr_cdc_buffer;
	uint8_t *	char_buffer;
	mutable  uint8_t		byte;
	struct usbdf_driver        cdcdfacm;
	mutable bool txDone;
	mutable bool rxDone;

protected:
private:
	uint8_t buffer[64];
	uint8_t portNumber;
	USB_CDC_Class* pUSBClass;
	
//functions
public:
	USB_CDC_Class();
	USB_CDC_Class(uint8_t p);
	~USB_CDC_Class();
	void init(void);
	int32_t enable(void);
	void disable(void);
	int32_t write( uint8_t *p  , uint32_t n);
	int32_t write_buffer( uint32_t n);
	int32_t  read(uint8_t * ,uint32_t );
	int32_t  read(uint8_t * ,uint32_t,bool);
	int32_t read_char(uint32_t count);
	int32_t read_buffer(uint32_t count);
	bool	isUSBEnabled(void);
	 bool setTxDone(void);
	 bool setRxDone(void);
	 char getLastChar(void);
	 uint8_t	setLastChar(void);
	 void	xprint(const char*);
	 void	xprint(char*);
	//friend bool cb_bulk_read(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count);
	//friend bool cb_bulk_write(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count);
protected:
private:
	USB_CDC_Class( const USB_CDC_Class &c );
	USB_CDC_Class& operator=( const USB_CDC_Class &c );

}; //USB_CDC_Class

#endif //__USB_CDC_CLASS_H__
