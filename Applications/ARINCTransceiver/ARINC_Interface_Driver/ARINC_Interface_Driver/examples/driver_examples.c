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

static struct timer_task TIMER_0_task1, TIMER_0_task2;
/**
 * Example of using TIMER_0.
 */
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_0_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_0_example(void)
{
	TIMER_0_task1.interval = 100;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 200;
	TIMER_0_task2.cb       = TIMER_0_task2_cb;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
}

/**
 * Example of using SPI_LTC2983 to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_LTC2983[12] = "Hello World!";

void SPI_LTC2983_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_LTC2983, &io);

	spi_m_sync_enable(&SPI_LTC2983);
	io_write(io, example_SPI_LTC2983, 12);
}

static uint8_t I2C_1_example_str[12] = "Hello World!";

void I2C_1_tx_complete(struct i2c_m_async_desc *const i2c)
{
}

void I2C_1_example(void)
{
	struct io_descriptor *I2C_1_io;

	i2c_m_async_get_io_descriptor(&I2C_1, &I2C_1_io);
	i2c_m_async_enable(&I2C_1);
	i2c_m_async_register_callback(&I2C_1, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_1_tx_complete);
	i2c_m_async_set_slaveaddr(&I2C_1, 0x12, I2C_M_SEVEN);

	io_write(I2C_1_io, I2C_1_example_str, 12);
}

static uint8_t I2C_EEPROM_example_str[12] = "Hello World!";

void I2C_EEPROM_tx_complete(struct i2c_m_async_desc *const i2c)
{
}

void I2C_EEPROM_example(void)
{
	struct io_descriptor *I2C_EEPROM_io;

	i2c_m_async_get_io_descriptor(&I2C_EEPROM, &I2C_EEPROM_io);
	i2c_m_async_enable(&I2C_EEPROM);
	i2c_m_async_register_callback(&I2C_EEPROM, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_EEPROM_tx_complete);
	i2c_m_async_set_slaveaddr(&I2C_EEPROM, 0x12, I2C_M_SEVEN);

	io_write(I2C_EEPROM_io, I2C_EEPROM_example_str, 12);
}

/**
 * Example of using SPI_Holt to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_Holt[12] = "Hello World!";

void SPI_Holt_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_Holt, &io);

	spi_m_sync_enable(&SPI_Holt);
	io_write(io, example_SPI_Holt, 12);
}

void delay_example(void)
{
	delay_ms(5000);
}

void CAN_0_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_0_rx_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

/**
 * Example of using CAN_0 to Encrypt/Decrypt datas.
 */
void CAN_0_example(void)
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
	can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_tx_callback);
	can_async_enable(&CAN_0);

	/**
	 * CAN_0_tx_callback callback should be invoked after call
	 * can_async_write, and remote device should recieve message with ID=0x45A
	 */
	can_async_write(&CAN_0, &msg);

	msg.id  = 0x100000A5;
	msg.fmt = CAN_FMT_EXTID;
	/**
	 * remote device should recieve message with ID=0x100000A5
	 */
	can_async_write(&CAN_0, &msg);

	/**
	 * CAN_0_rx_callback callback should be invoked after call
	 * can_async_set_filter and remote device send CAN Message with the same
	 * content as the filter.
	 */
	can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_rx_callback);
	filter.id   = 0x469;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 0, CAN_FMT_STDID, &filter);

	filter.id   = 0x10000096;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 1, CAN_FMT_EXTID, &filter);
}
