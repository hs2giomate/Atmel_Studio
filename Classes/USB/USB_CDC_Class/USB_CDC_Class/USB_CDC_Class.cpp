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

static struct usbd_descriptors desc[]
= {{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)}
#if CONF_USBD_HS_SP
,
{single_desc_bytes_hs, single_desc_bytes_hs + sizeof(single_desc_bytes_hs)}
#endif
};
static uint32_t usbd_cdc_buffer[CDCD_ECHO_BUF_SIZ / 4];
// default constructor
static uint8_t usb_ctrl_buffer[64];


USB_CDC_Class* ptrUSBClass;


/**
 * \brief Callback invoked when bulk OUT data received
 */
static bool cb_bulk_read(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{

	ptrUSBClass->setLastChar();
	ptrUSBClass->setRxDone();

	return false;
}

/**
 * \brief Callback invoked when bulk IN data received
 */
static bool cb_bulk_write(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	/* Echo data. */

	
	ptrUSBClass->setTxDone();

	

	/* No error. */
	return false;
}

static bool cb_state_c(usb_cdc_control_signal_t state)
{
	if (state.rs232.DTR) {
		/* Callbacks must be registered after endpoint allocation */
		cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)cb_bulk_read);
		cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)cb_bulk_write);

		ptrUSBClass->setTxDone();

	}

	/* No error. */
	return false;
}

// default constructor
USB_CDC_Class::USB_CDC_Class()
{
	descUSB=desc;
	ptr_cdc_buffer=usbd_cdc_buffer;
	ptrUSBClass=this;
	pUSBClass=this;



} //USB_CDC_Class
USB_CDC_Class::USB_CDC_Class(uint8_t p)
{
	portNumber=p;
	//usb_desc=&single_desc;
	//USBbuffer=usbd_cdc_buffer;

} //USB_CDC_Class

// default destructor


void USB_CDC_Class::init(){
	usbdc_init(usb_ctrl_buffer);

	/* usbdc_register_funcion inside */
	cdcdf_acm_init();

	usbdc_start(desc);
	usbdc_attach();

	cdcdf_acm_register_callback(CDCDF_ACM_CB_STATE_C, (FUNC_PTR)cb_state_c);
	cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)cb_bulk_read);
	cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)cb_bulk_write);
	txDone=true;
	rxDone=true;
	while (!isUSBEnabled()){};
	
}

int32_t USB_CDC_Class::enable(){
	cdcdf_acm_init();
	return 0;
}

int32_t  USB_CDC_Class::write( uint8_t *p, uint32_t n){
	int32_t w;

	while(!isUSBEnabled());
	while(!txDone);
	txDone=false;
	w=cdcdf_acm_write(p,n);
	
	return w;
	
}
int32_t  USB_CDC_Class::write_buffer(  uint32_t n){
	int32_t w;

	while(!isUSBEnabled());
	//while(!txDone);
	txDone=false;
	w=cdcdf_acm_write((uint8_t *)ptr_cdc_buffer, n);
	
	return w;
	
}
int32_t  USB_CDC_Class::read(uint8_t * p, uint32_t n,bool wait){
	int32_t r;
	r=read(p,n);
	if (wait)
	{
		while(!rxDone);
	}
	return  r;
}
int32_t  USB_CDC_Class::read(uint8_t * p, uint32_t n){
	int32_t r;
	char_buffer=p;
	while(!isUSBEnabled());
	//while(!rxDone);
	rxDone=false;
	r=cdcdf_acm_read(p, n);
	

	return  r;
}
int32_t  USB_CDC_Class::read_char(uint32_t count){
	
	return  read(&byte, count);
}
int32_t  USB_CDC_Class::read_buffer(uint32_t count){
	
	return  read((uint8_t *)ptr_cdc_buffer, count);
}
void USB_CDC_Class::disable(void){
cdcdf_acm_deinit();
	
}
bool USB_CDC_Class::isUSBEnabled(void){
	

	return cdcdf_acm_is_enabled();
	
}
bool  USB_CDC_Class::setTxDone(void){
	txDone=true;
	txDone=true;
	return txDone;
}
bool  USB_CDC_Class::setRxDone(void){
	rxDone=true;
	rxDone=true;
	return rxDone;
}
char USB_CDC_Class::getLastChar(){
	
	return (char)byte;
	
}
uint8_t USB_CDC_Class::setLastChar(){
	byte=*char_buffer;

	return byte;
	
}


// default destructor
USB_CDC_Class::~USB_CDC_Class()
{
	cdcdf_acm_deinit();
} //~USB_CDC_Class
void USB_CDC_Class::xprint(const char *message){

	uint32_t s;
	char *p=(char *)message;
	s=strlen(p);
	uint8_t	chain[256];

	
	for (uint8_t i=0;i<s;i++){
		chain[i]=*p;
		p++;
	}

	//uint8_t	pArray[s]=*string;
	uint8_t *ptr=(uint8_t*)chain;
	
	write(ptr,s);
}
void USB_CDC_Class::xprint(char *message){

	uint32_t s;
	char *p=(char *)message;
	s=strlen(p);
	uint8_t	chain[256];

	
	for (uint8_t i=0;i<s;i++){
		chain[i]=*p;
		p++;
	}

	//uint8_t	pArray[s]=*string;
	uint8_t *ptr=(uint8_t*)chain;
	
	write(ptr,s);
}