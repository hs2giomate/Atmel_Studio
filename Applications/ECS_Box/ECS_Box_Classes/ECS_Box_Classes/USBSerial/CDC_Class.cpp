/* 
* CDC_Class.cpp
*
* Created: 6/4/2020 3:05:36 PM
* Author: GMateusDP
*/


#include "CDC_Class.h"
#include "Calendar_Class.h"
#include	"Timer_Class.h"

CDC_Class	*ptrCDCClass;

static void	USBTimeoutTask(const struct timer_task *const timer_task){
	ptrCDCClass->timeout=true;
	ptrCDCClass->terminal=false;
}

/**
 * \brief Callback invoked when bulk OUT data received
 */
volatile bool cb_bulk_read(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	
	ptrCDCClass->rxReady=true;
	ptrCDCClass->terminal=true;
	ptrCDCClass->plugged=true;


	return false;
}

/**
 * \brief Callback invoked when bulk IN data received
 */
volatile bool cb_bulk_write(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	/* Echo data. */
	
	ptrCDCClass->txReady=true;
	ptrCDCClass->terminal=true;
	ptrCDCClass->plugged=true;

	

	/* No error. */
	return false;
}

volatile bool cb_state_c(usb_cdc_control_signal_t state)
{
	if (state.rs232.DTR) {
		/* Callbacks must be registered after endpoint allocation */
		cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)cb_bulk_read);
		cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)cb_bulk_write);

		ptrCDCClass->txReady=true;
		ptrCDCClass->rxReady=true;
		ptrCDCClass->plugged=true;
		ptrCDCClass->terminal=true;
		
	}

	/* No error. */
	return false;
}

// default constructor
CDC_Class::CDC_Class()
{
	_serialPeek=-1;
	ptrCDCClass=this;
} //CDC_Class

// default constructor
CDC_Class::CDC_Class(usbd_descriptors *usb_d)
{
	_serialPeek=-1;
	ptrCDCClass=this;
	usb_desc=usb_d;
} //CDC_Class

// default destructor
CDC_Class::~CDC_Class()
{
} //~CDC_Class

void CDC_Class::init(void){
	
	timeout=false;
	terminal=true;
	connectionTimer.add_oneShot_task((FUNC_PTR)USBTimeoutTask,USB_TIMEOUT);
	while (!IsEnabled()){
		if (timeout)
		{
			plugged=false;
			return;
		}
		else
		{
		}
	}
	connectionTimer.stop();
	
	cdcdf_acm_register_callback(CDCDF_ACM_CB_STATE_C, (FUNC_PTR)cb_state_c);


}

bool CDC_Class::IsEnabled(void){

	return cdcdf_acm_is_enabled();
	
}

int CDC_Class::peek(void)
{
	uint8_t	r;
	if (_serialPeek != -1)
	return _serialPeek;
	_serialPeek = readData(&r,1);
	_serialPeek=(int)r;
	return _serialPeek;
}

int32_t CDC_Class::readData(void* p, int32_t size)
{
	uint8_t	r;
	rxReady=false;
	r=cdcdf_acm_read((uint8_t *)p, size);
	while(!rxReady);
	return r;
}
int CDC_Class::read(void)
{
	uint8_t	p;
	rxReady=false;
	cdcdf_acm_read(&p, 1);
	while(!rxReady);
	return (int)p;
}
void CDC_Class::read(char *c)
{
	cdcdf_acm_read((uint8_t *)c, 1);
}
int32_t CDC_Class::read(void* p, int32_t size)
{
	uint8_t	r;
	rxReady=false;
	r=cdcdf_acm_read((uint8_t *)p, size);
	while(!rxReady);
	return r;
}
void	CDC_Class::flush(void){
	cdcdf_acm_write((uint8_t *)cdc_buffer, sizeof(cdc_buffer));
}

void	CDC_Class::clear(void){
	cdcdf_acm_stop_xfer();
}
size_t CDC_Class::write(uint8_t c) {
		while(!txReady);
	
		return writeData(&c, 1);
}
int32_t CDC_Class::writeData(const void *buffer, int32_t size)
{
	
	txReady=false;
	if ((CheckTerminal())&&terminal)
	{
		uint32_t r = (uint32_t)cdcdf_acm_write((uint8_t *)buffer,(uint32_t)size);
		while((!txReady)&&(terminal)){
			
			};
			connectionTimer.stop();
		return r;
	} 
	else
	{
		return 0;
	}
	

}
size_t CDC_Class::write(const uint8_t *buffer, size_t size)
{
	txReady=false;
	if (CheckTerminal()&&terminal)
	{
		uint32_t r = (uint32_t)cdcdf_acm_write((uint8_t *)buffer,(uint32_t)size);
		while((!txReady)&&(terminal)){
			
		};
		connectionTimer.stop();
		return r;
	}
	else
	{
		return 0;
	}
	
	
	
}
int32_t CDC_Class::writeData2(const void *buffer, int32_t size)
{
		txReady=false;
		
		if (CheckTerminal()&&terminal)
		{
			uint32_t r = (uint32_t)cdcdf_acm_write((uint8_t *)buffer,(uint32_t)size);
			while((!txReady)&&(terminal)){
				
			};
			connectionTimer.stop();
			return r;
		}
		else
		{
			return 0;
		}

}

bool	CDC_Class::available(void){
	if (rxReady && txReady)
	{
		return true;
	}else
	{
		
	return false;
	}
	
}
bool	CDC_Class::CheckTerminal(void){
	if (terminal)
	{
		if (plugged||IsEnabled())
		{
			terminal=connectionTimer.start()==0;
		}
		else
		{
			terminal=false;
		}
	} 
	else
	{
		return	terminal;	
	}
	
	return	terminal;	
}
CDC_Class::operator bool()
{
	// this is here to avoid spurious opening after upload
	
	delay_us(10);
	bool result = false;

	if (IsEnabled())
	{
		result = true;
	}

	delay_us(10);
	return result;
}

CDC_Class	usb;
