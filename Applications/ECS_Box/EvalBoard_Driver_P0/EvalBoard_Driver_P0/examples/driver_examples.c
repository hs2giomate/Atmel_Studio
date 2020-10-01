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

static void button_on_PA04_pressed(void)
{
}

static void button_on_PA05_pressed(void)
{
}

static void button_on_PC22_pressed(void)
{
}

static void button_on_PC23_pressed(void)
{
}

static void button_on_PA27_pressed(void)
{
}

static void button_on_PA14_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PD00, button_on_PD00_pressed);
	ext_irq_register(PIN_PA04, button_on_PA04_pressed);
	ext_irq_register(PIN_PA05, button_on_PA05_pressed);
	ext_irq_register(PIN_PC22, button_on_PC22_pressed);
	ext_irq_register(PIN_PC23, button_on_PC23_pressed);
	ext_irq_register(PIN_PA27, button_on_PA27_pressed);
	ext_irq_register(PIN_PA14, button_on_PA14_pressed);
}

/**
 * Example of using QSPI_MEMORY to get N25Q256A status value,
 * and check bit 0 which indicate embedded operation is busy or not.
 */
void QSPI_MEMORY_example(void)
{
	uint8_t              status = 0xFF;
	struct _qspi_command cmd    = {
        .inst_frame.bits.inst_en  = 1,
        .inst_frame.bits.data_en  = 1,
        .inst_frame.bits.tfr_type = QSPI_READ_ACCESS,
        .instruction              = 0x05,
        .buf_len                  = 1,
        .rx_buf                   = &status,
    };

	qspi_sync_enable(&QSPI_MEMORY);
	while (status & (1 << 0)) {
		qspi_sync_serial_run_command(&QSPI_MEMORY, &cmd);
	}
	qspi_sync_deinit(&QSPI_MEMORY);
}

/**
 * Example of using SPI_ARINC to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_ARINC[12] = "Hello World!";

void SPI_ARINC_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_ARINC, &io);

	spi_m_sync_enable(&SPI_ARINC);
	io_write(io, example_SPI_ARINC, 12);
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

void I2C_SHARED_example(void)
{
	struct io_descriptor *I2C_SHARED_io;

	i2c_m_sync_get_io_descriptor(&I2C_SHARED, &I2C_SHARED_io);
	i2c_m_sync_enable(&I2C_SHARED);
	i2c_m_sync_set_slaveaddr(&I2C_SHARED, 0x12, I2C_M_SEVEN);
	io_write(I2C_SHARED_io, (uint8_t *)"Hello World!", 12);
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

void I2C_FV_example(void)
{
	struct io_descriptor *I2C_FV_io;

	i2c_m_sync_get_io_descriptor(&I2C_FV, &I2C_FV_io);
	i2c_m_sync_enable(&I2C_FV);
	i2c_m_sync_set_slaveaddr(&I2C_FV, 0x12, I2C_M_SEVEN);
	io_write(I2C_FV_io, (uint8_t *)"Hello World!", 12);
}

void I2C_FANS_example(void)
{
	struct io_descriptor *I2C_FANS_io;

	i2c_m_sync_get_io_descriptor(&I2C_FANS, &I2C_FANS_io);
	i2c_m_sync_enable(&I2C_FANS);
	i2c_m_sync_set_slaveaddr(&I2C_FANS, 0x12, I2C_M_SEVEN);
	io_write(I2C_FANS_io, (uint8_t *)"Hello World!", 12);
}

void CAN_CCU_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_CCU_rx_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

/**
 * Example of using CAN_CCU to Encrypt/Decrypt datas.
 */
void CAN_CCU_example(void)
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
	can_async_register_callback(&CAN_CCU, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_CCU_tx_callback);
	can_async_enable(&CAN_CCU);

	/**
	 * CAN_CCU_tx_callback callback should be invoked after call
	 * can_async_write, and remote device should recieve message with ID=0x45A
	 */
	can_async_write(&CAN_CCU, &msg);

	msg.id  = 0x100000A5;
	msg.fmt = CAN_FMT_EXTID;
	/**
	 * remote device should recieve message with ID=0x100000A5
	 */
	can_async_write(&CAN_CCU, &msg);

	/**
	 * CAN_CCU_rx_callback callback should be invoked after call
	 * can_async_set_filter and remote device send CAN Message with the same
	 * content as the filter.
	 */
	can_async_register_callback(&CAN_CCU, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_CCU_rx_callback);
	filter.id   = 0x469;
	filter.mask = 0;
	can_async_set_filter(&CAN_CCU, 0, CAN_FMT_STDID, &filter);

	filter.id   = 0x10000096;
	filter.mask = 0;
	can_async_set_filter(&CAN_CCU, 1, CAN_FMT_EXTID, &filter);
}
