/*
 * SPI_Asyn_Class.cpp
 *
 * Created: 2/5/2020 12:17:26 PM
 * Author : GMateusDP
 */ 


#include "sam.h"
#include <atmel_start.h>

#include "SPI_Asyn_Class.h"
static uint8_t example_SPI_0[] = "Hello World!";

static void complete_cb_SPI_1(const struct spi_m_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

int main(void)
{
		atmel_start_init();
		SPI_Asyn_Class	spi0(0);
		spi0.init();
    /* Replace with your application code */
    while (1) 
    {
		struct io_descriptor *io;
		spi_m_async_get_io_descriptor(&SPI_0, &io);

		spi_m_async_register_callback(&SPI_0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)complete_cb_SPI_1);
		spi_m_async_enable(&SPI_0);
		io_write(io, example_SPI_0, 12);
		delay_ms(500);
    }
}
