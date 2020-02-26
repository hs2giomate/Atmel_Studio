/* 
* Usart_Asyn_Class.cpp
*
* Created: 2/12/2020 10:21:01 AM
* Author: GMateusDP
*/


#include "Usart_Asyn_Class.h"
#include "driver_init.h"
Usart_Asyn_Class* ptrUsartClass;
static void tx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
		ptrUsartClass->setUsartTxReady();
}
static void rx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
	ptrUsartClass->setUsartRxReady();
}
// default constructor
Usart_Asyn_Class::Usart_Asyn_Class()
{
	portNumber=0;
	USART0=USART_0;
	ptrUsartClass=this;
} //Usart_Asyn_Class
Usart_Asyn_Class::Usart_Asyn_Class(uint8_t p)
{
	portNumber=p;
	USART0=USART_0;
	ptrUsartClass=this;
} //Usart_Asyn_Class

// default destructor
Usart_Asyn_Class::~Usart_Asyn_Class()
{
	usart_async_deinit(&USART0);
} //~Usart_Asyn_Class

void Usart_Asyn_Class::init(){
	USART_0_CLOCK_init();
	usart_async_init(&USART0, SERCOM2, USART_0_buffer, USART_0_BUFFER_SIZE, (void *)NULL);
	USART_0_PORT_init();
	pUsartClass=this;
	ptrUsartClass=pUsartClass;
	setUsartRxReady();
	setUsartTxReady();
	isUsartReady();
	
}

int32_t Usart_Asyn_Class::enable(){
	usart_async_register_callback(&USART0, USART_ASYNC_TXC_CB, tx_cb_USART_0);
	usart_async_register_callback(&USART0, USART_ASYNC_RXC_CB, rx_cb_USART_0);
	/*usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&USART0, &usartio);
	usart_async_enable(&USART0);
	setUsartRxReady();
	setUsartTxReady();
	
	return 0;
}

int32_t  Usart_Asyn_Class::write(const uint8_t * p, int32_t n){
	int32_t w;
		while(!txReady);
		
	txReady=false;
	w= io_write(usartio,p,(uint16_t)n);

	 return w;
	
}
int32_t  Usart_Asyn_Class::read(uint8_t * p, int32_t n){
	int32_t r;
		while(!rxReady);
		
	rxReady=false;
	r=io_read(usartio,p,(uint16_t)n);

	return  r;
}
void Usart_Asyn_Class::disable(void){
	usart_async_disable(&USART0);
	
}
void  Usart_Asyn_Class::setUsartTxReady(void)
{
	txReady=true;

}

void  Usart_Asyn_Class::setUsartRxReady(void)
{
	rxReady=true;

}


bool Usart_Asyn_Class::isUsartReady(void){
	//spi_m_async_register_callback(&SPI0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)(&Usart_Asyn_Class::complete_cb_SPI_0));
	if ((txReady==true)&&(rxReady==true))
	{
		return true;
	} 
	else
	{
		return false;
	}

	
}
