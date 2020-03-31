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

static void button_on_PC02_pressed(void)
{
}

static void button_on_PC03_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PC02, button_on_PC02_pressed);
	ext_irq_register(PIN_PC03, button_on_PC03_pressed);
}

/**
 * Example of using OLED1_SPI to write "Hello World" using the IO abstraction.
 */
static uint8_t example_OLED1_SPI[12] = "Hello World!";

void OLED1_SPI_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&OLED1_SPI, &io);

	spi_m_sync_enable(&OLED1_SPI);
	io_write(io, example_OLED1_SPI, 12);
}
