/*
 * CppTestDriver.cpp
 *
 * Created: 2/12/2020 9:41:08 AM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "driver_init.h"
#include "utils.h"

static uint8_t example_SPI_0[] = "Hello World!";
bool ready=true;

static void tx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
	ready=true;
	/* Transfer completed */
}
int main(void)
{
	/* Replace with your application code */
	atmel_start_init();
	while (1)
	{
		struct io_descriptor *io;

		usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, tx_cb_USART_0);
	/*usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&USART_0, &io);
	usart_async_enable(&USART_0);
	while(!ready);
	ready=false;
		io_write(io, example_SPI_0, 12);
		delay_ms(100);
	}
}