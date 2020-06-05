/*--------------------------------------------------------------------------

serialStream.cc

This file is part of e.Development

Implementation


Copyright (c) 2006,2007 Steffen Simon.
All rights reserved.
--------------------------------------------------------------------------*/

#include "eDeviceSerialStream.h"

serialStream *ptrClass;

static void tx_cb_USART(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	ptrClass->txDone=true;
}

static void rx_cb_USART(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	ptrClass->rxDone=true;
}

serialStream::serialStream(uint8_t uart)
	:selfUART(uart)
	{
	}

serialStream::serialStream()
	{
		 ptrClass=this;
	}

int32_t	 serialStream::init(void)
	{
	usart_async_register_callback(&USART_2, USART_ASYNC_TXC_CB, tx_cb_USART);
	usart_async_register_callback(&USART_2, USART_ASYNC_RXC_CB, rx_cb_USART);
//	usart_async_register_callback(&USART_2, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&USART_2, &io_usart);
	return	usart_async_enable(&USART_2);
	
	}
int32_t	 serialStream::enable(void){
	return	usart_async_enable(&USART_2);
}



int32_t serialStream::disable(void)
	{
		return usart_async_disable(&USART_2);
	}
bool serialStream::dataAvailable(void)
	{
	return usart_async_is_rx_not_empty(&USART_2)>0;
	}

int32_t serialStream::readData(void* p, int32_t n)
	{
		rxDone=false;
		int32_t r=io_read(io_usart,(uint8_t*)p,(uint16_t)n);
		return  r;
	}

int32_t serialStream::writeData(const void* p, int32_t n)
	{
		txDone=false;
		int32_t	w=io_write(io_usart,(uint8_t*)p,(uint16_t)n);
		return w;
	}

serialStream::~serialStream(){
	usart_async_deinit(&USART_2);
}