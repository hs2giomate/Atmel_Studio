/* 
* CDC_Class.cpp
*
* Created: 6/4/2020 3:05:36 PM
* Author: GMateusDP
*/


#include "CDC_Class.h"
#if CONF_USBD_HS_SP
static uint8_t cdc_desc_bytes[] = {
	/* Device descriptors and Configuration descriptors list. */
CDCD_ACM_HS_DESCES_LS_FS};
static uint8_t cdc_desc_bytes_hs[] = {
	/* Device descriptors and Configuration descriptors list. */
CDCD_ACM_HS_DESCES_HS};
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ_HS
#else
static uint8_t cdc_desc_bytes[] = {
	/* Device descriptors and Configuration descriptors list. */
CDCD_ACM_DESCES_LS_FS};
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ
#endif

static struct usbd_descriptors cdc_desc[]
= {{cdc_desc_bytes, cdc_desc_bytes + sizeof(cdc_desc_bytes)}
#if CONF_USBD_HS_SP
,
{cdc_desc_bytes_hs, cdc_desc_bytes_hs + sizeof(cdc_desc_bytes_hs)}
#endif
};

/** Buffers to receive and echo the communication bytes. */
static uint32_t cdc_buffer[CDCD_ECHO_BUF_SIZ / 4];

/** Ctrl endpoint buffer */
static uint8_t cdc_ctrl_buffer[64];

CDC_Class	*ptrCDCClass;


/**
 * \brief Callback invoked when bulk OUT data received
 */
static bool cb_bulk_read(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{

	
	ptrCDCClass->rxReady=true;

	return false;
}

/**
 * \brief Callback invoked when bulk IN data received
 */
static bool cb_bulk_write(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	/* Echo data. */

	
	ptrCDCClass->txReady=true;

	

	/* No error. */
	return false;
}

static bool cb_state_c(usb_cdc_control_signal_t state)
{
	if (state.rs232.DTR) {
		/* Callbacks must be registered after endpoint allocation */
		cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)cb_bulk_read);
		cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)cb_bulk_write);

		ptrCDCClass->txReady=true;

	}

	/* No error. */
	return false;
}

// default constructor
CDC_Class::CDC_Class()
{
	_serialPeek=-1;
} //CDC_Class

// default destructor
CDC_Class::~CDC_Class()
{
} //~CDC_Class

void CDC_Class::init(void){
	usbdc_init(cdc_ctrl_buffer);

	/* usbdc_register_funcion inside */
	cdcdf_acm_init();

	usbdc_start(cdc_desc);
	usbdc_attach();
	
	cdcdf_acm_register_callback(CDCDF_ACM_CB_STATE_C, (FUNC_PTR)cb_state_c);
	cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)cb_bulk_read);
	cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)cb_bulk_write);
	txReady=true;
	rxReady=true;
	while (!is_enabled());
}

bool CDC_Class::is_enabled(void){
	

	return cdcdf_acm_is_enabled();
	
}

int CDC_Class::peek(void)
{
	if (_serialPeek != -1)
	return _serialPeek;
	_serialPeek = read();
	return _serialPeek;
}

int CDC_Class::read(void)
{
	uint8_t	r;
	rxReady=false;
	cdcdf_acm_read(&r, 1);
	while(!rxReady);
	return (int)r;
}

void	CDC_Class::flush(void){
	cdcdf_acm_write((uint8_t *)cdc_buffer, sizeof(cdc_buffer));
}

void	CDC_Class::clear(void){
	cdcdf_acm_stop_xfer();
}
size_t CDC_Class::write(uint8_t c) {
	return write(&c, 1);
}
size_t CDC_Class::write(const uint8_t *buffer, size_t size)
{
	uint32_t r = (uint32_t)cdcdf_acm_write((uint8_t *)buffer,(uint32_t)size);

	
		return r;

}

int	CDC_Class::available(void){
	if (rxReady && txReady)
	{
		return 1;
	}else
	{
		
	return 0;
	}
	
}
