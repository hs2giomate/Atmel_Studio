/* 
* USB_CDC_Class.cpp
*
* Created: 2/24/2020 1:16:25 PM
* Author: GMateusDP
*/


#include "USB_CDC_Class.h"

#if CONF_USBD_HS_SP
static uint8_t single_desc_bytes[] = {
	/* Device descriptors and Configuration descriptors list. */
CDCD_ACM_HS_DESCES_LS_FS};
static uint8_t single_desc_bytes_hs[] = {
	/* Device descriptors and Configuration descriptors list. */
CDCD_ACM_HS_DESCES_HS};
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ_HS
#else
static uint8_t single_desc_bytes[] = {
	/* Device descriptors and Configuration descriptors list. */
CDCD_ACM_DESCES_LS_FS};
#define CDCD_ECHO_BUF_SIZ CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ
#endif

static struct usbd_descriptors single_desc[]
= {{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)}
#if CONF_USBD_HS_SP
,
{single_desc_bytes_hs, single_desc_bytes_hs + sizeof(single_desc_bytes_hs)}
#endif
};
static uint32_t usbd_cdc_buffer[CDCD_ECHO_BUF_SIZ / 4];
// default constructor




USB_CDC_Class* ptrUSBClass;

// default constructor
USB_CDC_Class::USB_CDC_Class()
{
	//portNumber=0;
	//usb_desc=&single_desc;
	//USBbuffer=usbd_cdc_buffer;

} //USB_CDC_Class
USB_CDC_Class::USB_CDC_Class(uint8_t p)
{
	portNumber=p;
	//usb_desc=&single_desc;
	//USBbuffer=usbd_cdc_buffer;

} //USB_CDC_Class

// default destructor


void USB_CDC_Class::init(){
	usbdc_init(ctrl_buffer);

	/* usbdc_register_funcion inside */
	cdcdf_acm_init();

	usbdc_start(single_desc);
	usbdc_attach();

	isUSBEnabled();
	
}

int32_t USB_CDC_Class::enable(){
	
	return 0;
}

int32_t  USB_CDC_Class::write( uint8_t *p, uint32_t n){
	int32_t w;
	uint8_t *ptr;
	ptr=p;
	while(!isUSBEnabled());
	w=cdcdf_acm_write(p,n);

	 return w;
	
}
int32_t  USB_CDC_Class::read(uint8_t * p, uint32_t n){
	int32_t r;
	while(!isUSBEnabled());
	r=cdcdf_acm_read(p, n);

	return  r;
}
void USB_CDC_Class::disable(void){
cdcdf_acm_deinit();
	
}



bool USB_CDC_Class::isUSBEnabled(void){
	

		return cdcdf_acm_is_enabled();


	
}
// default destructor
USB_CDC_Class::~USB_CDC_Class()
{
	cdcdf_acm_deinit();
} //~USB_CDC_Class