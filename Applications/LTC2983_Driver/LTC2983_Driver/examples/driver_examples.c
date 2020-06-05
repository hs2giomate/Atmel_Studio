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

/**
 * Example of using SPI_0 to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_SPI_0[12] = "Hello World!";

static void complete_cb_SPI_0(const struct spi_m_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void SPI_0_example(void)
{
	struct io_descriptor *io;
	spi_m_async_get_io_descriptor(&SPI_0, &io);

	spi_m_async_register_callback(&SPI_0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)complete_cb_SPI_0);
	spi_m_async_enable(&SPI_0);
	io_write(io, example_SPI_0, 12);
}

static uint8_t I2C_0_example_str[12] = "Hello World!";

void I2C_0_tx_complete(struct i2c_m_async_desc *const i2c)
{
}

void I2C_0_example(void)
{
	struct io_descriptor *I2C_0_io;

	i2c_m_async_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_async_enable(&I2C_0);
	i2c_m_async_register_callback(&I2C_0, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_0_tx_complete);
	i2c_m_async_set_slaveaddr(&I2C_0, 0x12, I2C_M_SEVEN);

	io_write(I2C_0_io, I2C_0_example_str, 12);
}
