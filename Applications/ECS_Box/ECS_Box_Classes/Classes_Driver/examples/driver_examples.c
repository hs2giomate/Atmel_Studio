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

static void button_on_PC22_pressed(void)
{
}

static void button_on_PC23_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PC22, button_on_PC22_pressed);
	ext_irq_register(PIN_PC23, button_on_PC23_pressed);
}

static struct timer_task Live_Pulse_task1, Live_Pulse_task2;
/**
 * Example of using Live_Pulse.
 */
static void Live_Pulse_task1_cb(const struct timer_task *const timer_task)
{
}

static void Live_Pulse_task2_cb(const struct timer_task *const timer_task)
{
}

void Live_Pulse_example(void)
{
	Live_Pulse_task1.interval = 100;
	Live_Pulse_task1.cb       = Live_Pulse_task1_cb;
	Live_Pulse_task1.mode     = TIMER_TASK_REPEAT;
	Live_Pulse_task2.interval = 200;
	Live_Pulse_task2.cb       = Live_Pulse_task2_cb;
	Live_Pulse_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&Live_Pulse, &Live_Pulse_task1);
	timer_add_task(&Live_Pulse, &Live_Pulse_task2);
	timer_start(&Live_Pulse);
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

/**
 * Example of using SPI_TEMP to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_SPI_TEMP[12] = "Hello World!";

static void complete_cb_SPI_TEMP(const struct spi_m_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void SPI_TEMP_example(void)
{
	struct io_descriptor *io;
	spi_m_async_get_io_descriptor(&SPI_TEMP, &io);

	spi_m_async_register_callback(&SPI_TEMP, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)complete_cb_SPI_TEMP);
	spi_m_async_enable(&SPI_TEMP);
	io_write(io, example_SPI_TEMP, 12);
}

/**
 * Example of using SPI_HI3593 to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_HI3593[12] = "Hello World!";

void SPI_HI3593_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_HI3593, &io);

	spi_m_sync_enable(&SPI_HI3593);
	io_write(io, example_SPI_HI3593, 12);
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

static struct timer_task Event_Timer_task1, Event_Timer_task2;

/**
 * Example of using Event_Timer.
 */
static void Event_Timer_task1_cb(const struct timer_task *const timer_task)
{
}

static void Event_Timer_task2_cb(const struct timer_task *const timer_task)
{
}

void Event_Timer_example(void)
{
	Event_Timer_task1.interval = 100;
	Event_Timer_task1.cb       = Event_Timer_task1_cb;
	Event_Timer_task1.mode     = TIMER_TASK_REPEAT;
	Event_Timer_task2.interval = 200;
	Event_Timer_task2.cb       = Event_Timer_task2_cb;
	Event_Timer_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&Event_Timer, &Event_Timer_task1);
	timer_add_task(&Event_Timer, &Event_Timer_task2);
	timer_start(&Event_Timer);
}

static struct timer_task TIMER_ARINC_task1, TIMER_ARINC_task2;

/**
 * Example of using TIMER_ARINC.
 */
static void TIMER_ARINC_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_ARINC_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_ARINC_example(void)
{
	TIMER_ARINC_task1.interval = 100;
	TIMER_ARINC_task1.cb       = TIMER_ARINC_task1_cb;
	TIMER_ARINC_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_ARINC_task2.interval = 200;
	TIMER_ARINC_task2.cb       = TIMER_ARINC_task2_cb;
	TIMER_ARINC_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_ARINC, &TIMER_ARINC_task1);
	timer_add_task(&TIMER_ARINC, &TIMER_ARINC_task2);
	timer_start(&TIMER_ARINC);
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
