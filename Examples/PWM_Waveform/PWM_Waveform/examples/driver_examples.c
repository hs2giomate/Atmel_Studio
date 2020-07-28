/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static void button_on_PB31_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PB31, button_on_PB31_pressed);
}

/**
 * Example of using EDBG_UART to write "Hello World" using the IO abstraction.
 */
void EDBG_UART_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&EDBG_UART, &io);
	usart_sync_enable(&EDBG_UART);

	io_write(io, (uint8_t *)"Hello World!", 12);
}
