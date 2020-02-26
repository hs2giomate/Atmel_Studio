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
	struct usbd_descriptors *usb_desc;
	//uint32_t USBbuffer[];
	struct usbdf_driver        cdcdfacm;
	
protected:
private:
	uint8_t ctrl_buffer[64];
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
		int32_t  read(uint8_t * ,uint32_t );
	
		bool	isUSBEnabled(void);
protected:
private:
	USB_CDC_Class( const USB_CDC_Class &c );
	USB_CDC_Class& operator=( const USB_CDC_Class &c );

}; //USB_CDC_Class

#endif //__USB_CDC_CLASS_H__
