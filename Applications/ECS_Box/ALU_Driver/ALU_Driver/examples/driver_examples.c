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

/* CRC Data in flash */
COMPILER_ALIGNED(4)
static const uint32_t crc_datas[] = {0x00000000,
                                     0x11111111,
                                     0x22222222,
                                     0x33333333,
                                     0x44444444,
                                     0x55555555,
                                     0x66666666,
                                     0x77777777,
                                     0x88888888,
                                     0x99999999};

/**
 * Example of using CRC_CALC to Calculate CRC32 for a buffer.
 */
void CRC_CALC_example(void)
{
	/* The initial value used for the CRC32 calculation usually be 0xFFFFFFFF,
	 * but can be, for example, the result of a previous CRC32 calculation if
	 * generating a common CRC32 of separate memory blocks.
	 */
	uint32_t crc = 0xFFFFFFFF;
	uint32_t crc2;
	uint32_t ind;

	crc_sync_enable(&CRC_CALC);
	crc_sync_crc32(&CRC_CALC, (uint32_t *)crc_datas, 10, &crc);

	/* The read value must be complemented to match standard CRC32
	 * implementations or kept non-inverted if used as starting point for
	 * subsequent CRC32 calculations.
	 */
	crc ^= 0xFFFFFFFF;

	/* Calculate the same data with subsequent CRC32 calculations, the result
	 * should be same as previous way.
	 */
	crc2 = 0xFFFFFFFF;
	for (ind = 0; ind < 10; ind++) {
		crc_sync_crc32(&CRC_CALC, (uint32_t *)&crc_datas[ind], 1, &crc2);
	}
	crc2 ^= 0xFFFFFFFF;

	/* The calculate result should be same. */
	while (crc != crc2)
		;
}

static void button_on_PC00_pressed(void)
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

static void button_on_PB08_pressed(void)
{
}

static void button_on_PB09_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PC00, button_on_PC00_pressed);
	ext_irq_register(PIN_PA04, button_on_PA04_pressed);
	ext_irq_register(PIN_PA05, button_on_PA05_pressed);
	ext_irq_register(PIN_PC22, button_on_PC22_pressed);
	ext_irq_register(PIN_PC23, button_on_PC23_pressed);
	ext_irq_register(PIN_PB08, button_on_PB08_pressed);
	ext_irq_register(PIN_PB09, button_on_PB09_pressed);
}

/**
 * Example of using QSPI_N25Q256 to get N25Q256A status value,
 * and check bit 0 which indicate embedded operation is busy or not.
 */
void QSPI_N25Q256_example(void)
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

	qspi_sync_enable(&QSPI_N25Q256);
	while (status & (1 << 0)) {
		qspi_sync_serial_run_command(&QSPI_N25Q256, &cmd);
	}
	qspi_sync_deinit(&QSPI_N25Q256);
}

/**
 * Example of using DATETIME_CLOCK.
 */
static struct calendar_alarm alarm;

static void alarm_cb(struct calendar_descriptor *const descr)
{
	/* alarm expired */
}

void DATETIME_CLOCK_example(void)
{
	struct calendar_date date;
	struct calendar_time time;

	calendar_enable(&DATETIME_CLOCK);

	date.year  = 2000;
	date.month = 12;
	date.day   = 31;

	time.hour = 12;
	time.min  = 59;
	time.sec  = 59;

	calendar_set_date(&DATETIME_CLOCK, &date);
	calendar_set_time(&DATETIME_CLOCK, &time);

	alarm.cal_alarm.datetime.time.sec = 4;
	alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm.cal_alarm.mode              = REPEAT;

	calendar_set_alarm(&DATETIME_CLOCK, &alarm, alarm_cb);
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

static uint8_t I2C_EXPANDER_example_str[12] = "Hello World!";

void I2C_EXPANDER_tx_complete(struct i2c_m_async_desc *const i2c)
{
}

void I2C_EXPANDER_example(void)
{
	struct io_descriptor *I2C_EXPANDER_io;

	i2c_m_async_get_io_descriptor(&I2C_EXPANDER, &I2C_EXPANDER_io);
	i2c_m_async_enable(&I2C_EXPANDER);
	i2c_m_async_register_callback(&I2C_EXPANDER, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_EXPANDER_tx_complete);
	i2c_m_async_set_slaveaddr(&I2C_EXPANDER, 0x12, I2C_M_SEVEN);

	io_write(I2C_EXPANDER_io, I2C_EXPANDER_example_str, 12);
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

static struct timer_task TIMER_USB_task1, TIMER_USB_task2;

/**
 * Example of using TIMER_USB.
 */
static void TIMER_USB_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_USB_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_USB_example(void)
{
	TIMER_USB_task1.interval = 100;
	TIMER_USB_task1.cb       = TIMER_USB_task1_cb;
	TIMER_USB_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_USB_task2.interval = 200;
	TIMER_USB_task2.cb       = TIMER_USB_task2_cb;
	TIMER_USB_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_USB, &TIMER_USB_task1);
	timer_add_task(&TIMER_USB, &TIMER_USB_task2);
	timer_start(&TIMER_USB);
}

/**
 * Example of using LIVE_PULSE.
 */
void LIVE_PULSE_example(void)
{
	pwm_set_parameters(&LIVE_PULSE, 10000, 5000);
	pwm_enable(&LIVE_PULSE);
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

static struct timer_task TIMER_MAINTENANCE_task1, TIMER_MAINTENANCE_task2;

/**
 * Example of using TIMER_MAINTENANCE.
 */
static void TIMER_MAINTENANCE_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_MAINTENANCE_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_MAINTENANCE_example(void)
{
	TIMER_MAINTENANCE_task1.interval = 100;
	TIMER_MAINTENANCE_task1.cb       = TIMER_MAINTENANCE_task1_cb;
	TIMER_MAINTENANCE_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_MAINTENANCE_task2.interval = 200;
	TIMER_MAINTENANCE_task2.cb       = TIMER_MAINTENANCE_task2_cb;
	TIMER_MAINTENANCE_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_MAINTENANCE, &TIMER_MAINTENANCE_task1);
	timer_add_task(&TIMER_MAINTENANCE, &TIMER_MAINTENANCE_task2);
	timer_start(&TIMER_MAINTENANCE);
}

static struct timer_task TIMER_INTERFACE_task1, TIMER_INTERFACE_task2;

/**
 * Example of using TIMER_INTERFACE.
 */
static void TIMER_INTERFACE_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_INTERFACE_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_INTERFACE_example(void)
{
	TIMER_INTERFACE_task1.interval = 100;
	TIMER_INTERFACE_task1.cb       = TIMER_INTERFACE_task1_cb;
	TIMER_INTERFACE_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_INTERFACE_task2.interval = 200;
	TIMER_INTERFACE_task2.cb       = TIMER_INTERFACE_task2_cb;
	TIMER_INTERFACE_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_INTERFACE, &TIMER_INTERFACE_task1);
	timer_add_task(&TIMER_INTERFACE, &TIMER_INTERFACE_task2);
	timer_start(&TIMER_INTERFACE);
}

static struct timer_task TIMER_EVENT_task1, TIMER_EVENT_task2;

/**
 * Example of using TIMER_EVENT.
 */
static void TIMER_EVENT_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_EVENT_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_EVENT_example(void)
{
	TIMER_EVENT_task1.interval = 100;
	TIMER_EVENT_task1.cb       = TIMER_EVENT_task1_cb;
	TIMER_EVENT_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_EVENT_task2.interval = 200;
	TIMER_EVENT_task2.cb       = TIMER_EVENT_task2_cb;
	TIMER_EVENT_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_EVENT, &TIMER_EVENT_task1);
	timer_add_task(&TIMER_EVENT, &TIMER_EVENT_task2);
	timer_start(&TIMER_EVENT);
}

/**
 * Example of using TIMER_HVAC.
 */
struct timer_task TIMER_HVAC_task1, TIMER_HVAC_task2;

static void TIMER_HVAC_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_HVAC_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_HVAC_example(void)
{
	TIMER_HVAC_task1.interval = 100;
	TIMER_HVAC_task1.cb       = TIMER_HVAC_task1_cb;
	TIMER_HVAC_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_HVAC_task2.interval = 200;
	TIMER_HVAC_task2.cb       = TIMER_HVAC_task2_cb;
	TIMER_HVAC_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_HVAC, &TIMER_HVAC_task1);
	timer_add_task(&TIMER_HVAC, &TIMER_HVAC_task2);
	timer_start(&TIMER_HVAC);
}

/**
 * Example of using TIMER_TEMPERATURES.
 */
struct timer_task TIMER_TEMPERATURES_task1, TIMER_TEMPERATURES_task2;

static void TIMER_TEMPERATURES_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_TEMPERATURES_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_TEMPERATURES_example(void)
{
	TIMER_TEMPERATURES_task1.interval = 100;
	TIMER_TEMPERATURES_task1.cb       = TIMER_TEMPERATURES_task1_cb;
	TIMER_TEMPERATURES_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_TEMPERATURES_task2.interval = 200;
	TIMER_TEMPERATURES_task2.cb       = TIMER_TEMPERATURES_task2_cb;
	TIMER_TEMPERATURES_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_TEMPERATURES, &TIMER_TEMPERATURES_task1);
	timer_add_task(&TIMER_TEMPERATURES, &TIMER_TEMPERATURES_task2);
	timer_start(&TIMER_TEMPERATURES);
}

/**
 * Example of using WATCHDOG.
 */
void WATCHDOG_example(void)
{
	uint32_t clk_rate;
	uint16_t timeout_period;

	clk_rate       = 1000;
	timeout_period = 4096;
	wdt_set_timeout_period(&WATCHDOG, clk_rate, timeout_period);
	wdt_enable(&WATCHDOG);
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
