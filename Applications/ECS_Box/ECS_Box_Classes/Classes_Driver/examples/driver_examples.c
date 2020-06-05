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

static struct timer_task TIMER_RTC_task1, TIMER_RTC_task2;
/**
 * Example of using TIMER_RTC.
 */
static void TIMER_RTC_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_RTC_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_RTC_example(void)
{
	TIMER_RTC_task1.interval = 100;
	TIMER_RTC_task1.cb       = TIMER_RTC_task1_cb;
	TIMER_RTC_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_RTC_task2.interval = 200;
	TIMER_RTC_task2.cb       = TIMER_RTC_task2_cb;
	TIMER_RTC_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_RTC, &TIMER_RTC_task1);
	timer_add_task(&TIMER_RTC, &TIMER_RTC_task2);
	timer_start(&TIMER_RTC);
}

/**
 * Example of using SPI_HOLT to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_HOLT[12] = "Hello World!";

void SPI_HOLT_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_HOLT, &io);

	spi_m_sync_enable(&SPI_HOLT);
	io_write(io, example_SPI_HOLT, 12);
}

/**
 * Example of using SPI_TEMP to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_TEMP[12] = "Hello World!";

void SPI_TEMP_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_TEMP, &io);

	spi_m_sync_enable(&SPI_TEMP);
	io_write(io, example_SPI_TEMP, 12);
}

void I2C_EEPROM_example(void)
{
	struct io_descriptor *I2C_EEPROM_io;

	i2c_m_sync_get_io_descriptor(&I2C_EEPROM, &I2C_EEPROM_io);
	i2c_m_sync_enable(&I2C_EEPROM);
	i2c_m_sync_set_slaveaddr(&I2C_EEPROM, 0x12, I2C_M_SEVEN);
	io_write(I2C_EEPROM_io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using SPI_MEMORIES to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_MEMORIES[12] = "Hello World!";

void SPI_MEMORIES_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_MEMORIES, &io);

	spi_m_sync_enable(&SPI_MEMORIES);
	io_write(io, example_SPI_MEMORIES, 12);
}

static struct timer_task TIMEOUT_task1, TIMEOUT_task2;

/**
 * Example of using TIMEOUT.
 */
static void TIMEOUT_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMEOUT_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMEOUT_example(void)
{
	TIMEOUT_task1.interval = 100;
	TIMEOUT_task1.cb       = TIMEOUT_task1_cb;
	TIMEOUT_task1.mode     = TIMER_TASK_REPEAT;
	TIMEOUT_task2.interval = 200;
	TIMEOUT_task2.cb       = TIMEOUT_task2_cb;
	TIMEOUT_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMEOUT, &TIMEOUT_task1);
	timer_add_task(&TIMEOUT, &TIMEOUT_task2);
	timer_start(&TIMEOUT);
}

void CAN_Compressor_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_Compressor_rx_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

/**
 * Example of using CAN_Compressor to Encrypt/Decrypt datas.
 */
void CAN_Compressor_example(void)
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
	can_async_register_callback(&CAN_Compressor, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_Compressor_tx_callback);
	can_async_enable(&CAN_Compressor);

	/**
	 * CAN_Compressor_tx_callback callback should be invoked after call
	 * can_async_write, and remote device should recieve message with ID=0x45A
	 */
	can_async_write(&CAN_Compressor, &msg);

	msg.id  = 0x100000A5;
	msg.fmt = CAN_FMT_EXTID;
	/**
	 * remote device should recieve message with ID=0x100000A5
	 */
	can_async_write(&CAN_Compressor, &msg);

	/**
	 * CAN_Compressor_rx_callback callback should be invoked after call
	 * can_async_set_filter and remote device send CAN Message with the same
	 * content as the filter.
	 */
	can_async_register_callback(&CAN_Compressor, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_Compressor_rx_callback);
	filter.id   = 0x469;
	filter.mask = 0;
	can_async_set_filter(&CAN_Compressor, 0, CAN_FMT_STDID, &filter);

	filter.id   = 0x10000096;
	filter.mask = 0;
	can_async_set_filter(&CAN_Compressor, 1, CAN_FMT_EXTID, &filter);
}
