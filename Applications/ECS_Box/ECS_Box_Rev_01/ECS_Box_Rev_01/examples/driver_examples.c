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

static void button_on_PD00_pressed(void)
{
}

static void button_on_PD01_pressed(void)
{
}

static void button_on_PC18_pressed(void)
{
}

static void button_on_PB03_pressed(void)
{
}

static void button_on_PB04_pressed(void)
{
}

static void button_on_PB05_pressed(void)
{
}

static void button_on_PB06_pressed(void)
{
}

static void button_on_PB07_pressed(void)
{
}

static void button_on_PB08_pressed(void)
{
}

static void button_on_PB09_pressed(void)
{
}

static void button_on_PB10_pressed(void)
{
}

static void button_on_PB28_pressed(void)
{
}

static void button_on_PB29_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PD00, button_on_PD00_pressed);
	ext_irq_register(PIN_PD01, button_on_PD01_pressed);
	ext_irq_register(PIN_PC18, button_on_PC18_pressed);
	ext_irq_register(PIN_PB03, button_on_PB03_pressed);
	ext_irq_register(PIN_PB04, button_on_PB04_pressed);
	ext_irq_register(PIN_PB05, button_on_PB05_pressed);
	ext_irq_register(PIN_PB06, button_on_PB06_pressed);
	ext_irq_register(PIN_PB07, button_on_PB07_pressed);
	ext_irq_register(PIN_PB08, button_on_PB08_pressed);
	ext_irq_register(PIN_PB09, button_on_PB09_pressed);
	ext_irq_register(PIN_PB10, button_on_PB10_pressed);
	ext_irq_register(PIN_PB28, button_on_PB28_pressed);
	ext_irq_register(PIN_PB29, button_on_PB29_pressed);
}

/**
 * Example of using Flash_Mem_SPI to write "Hello World" using the IO abstraction.
 */
static uint8_t example_Flash_Mem_SPI[12] = "Hello World!";

void Flash_Mem_SPI_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&Flash_Mem_SPI, &io);

	spi_m_sync_enable(&Flash_Mem_SPI);
	io_write(io, example_Flash_Mem_SPI, 12);
}

void Fvx_I2C_example(void)
{
	struct io_descriptor *Fvx_I2C_io;

	i2c_m_sync_get_io_descriptor(&Fvx_I2C, &Fvx_I2C_io);
	i2c_m_sync_enable(&Fvx_I2C);
	i2c_m_sync_set_slaveaddr(&Fvx_I2C, 0x12, I2C_M_SEVEN);
	io_write(Fvx_I2C_io, (uint8_t *)"Hello World!", 12);
}

void FANs_I2C_example(void)
{
	struct io_descriptor *FANs_I2C_io;

	i2c_m_sync_get_io_descriptor(&FANs_I2C, &FANs_I2C_io);
	i2c_m_sync_enable(&FANs_I2C);
	i2c_m_sync_set_slaveaddr(&FANs_I2C, 0x12, I2C_M_SEVEN);
	io_write(FANs_I2C_io, (uint8_t *)"Hello World!", 12);
}

void Shared_I2C_example(void)
{
	struct io_descriptor *Shared_I2C_io;

	i2c_m_sync_get_io_descriptor(&Shared_I2C, &Shared_I2C_io);
	i2c_m_sync_enable(&Shared_I2C);
	i2c_m_sync_set_slaveaddr(&Shared_I2C, 0x12, I2C_M_SEVEN);
	io_write(Shared_I2C_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using SPI_Temp to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_Temp[12] = "Hello World!";

void SPI_Temp_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_Temp, &io);

	spi_m_sync_enable(&SPI_Temp);
	io_write(io, example_SPI_Temp, 12);
}

/**
 * Example of using SPI_AMMC to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_AMMC[12] = "Hello World!";

void SPI_AMMC_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_AMMC, &io);

	spi_m_sync_enable(&SPI_AMMC);
	io_write(io, example_SPI_AMMC, 12);
}

void CCu_CAN_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CCu_CAN_rx_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

/**
 * Example of using CCu_CAN to Encrypt/Decrypt datas.
 */
void CCu_CAN_example(void)
{
	struct can_message msg;
	struct can_filter  filter;
	uint8_t            send_data[4];
	send_data[0] = 0x00;
	send_data[1] = 0x01;
	send_data[2] = 0x02;
	send_data[3] = 0x03;

	msg.id   = 0x45A;
	msg.type = CAN_TYPE_DATA;
	msg.data = send_data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_STDID;
	can_async_register_callback(&CCu_CAN, CAN_ASYNC_TX_CB, (FUNC_PTR)CCu_CAN_tx_callback);
	can_async_enable(&CCu_CAN);

	/**
	 * CCu_CAN_tx_callback callback should be invoked after call
	 * can_async_write, and remote device should recieve message with ID=0x45A
	 */
	can_async_write(&CCu_CAN, &msg);

	msg.id  = 0x100000A5;
	msg.fmt = CAN_FMT_EXTID;
	/**
	 * remote device should recieve message with ID=0x100000A5
	 */
	can_async_write(&CCu_CAN, &msg);

	/**
	 * CCu_CAN_rx_callback callback should be invoked after call
	 * can_async_set_filter and remote device send CAN Message with the same
	 * content as the filter.
	 */
	can_async_register_callback(&CCu_CAN, CAN_ASYNC_RX_CB, (FUNC_PTR)CCu_CAN_rx_callback);
	filter.id   = 0x469;
	filter.mask = 0;
	can_async_set_filter(&CCu_CAN, 0, CAN_FMT_STDID, &filter);

	filter.id   = 0x10000096;
	filter.mask = 0;
	can_async_set_filter(&CCu_CAN, 1, CAN_FMT_EXTID, &filter);
}
