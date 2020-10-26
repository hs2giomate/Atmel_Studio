/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_crc_sync.h>
#include <hal_ext_irq.h>

#include <hal_evsys.h>

#include <hal_qspi_sync.h>

#include <hal_calendar.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_i2c_m_async.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_timer.h>
#include <hpl_tcc.h>
#include <hal_timer.h>
#include <hpl_tcc.h>

#include "hal_usb_device.h"

#include <hal_wdt.h>
#include <hal_can_async.h>

extern struct crc_sync_descriptor CRC_CALC;

extern struct qspi_sync_descriptor QSPI_N25Q256;

extern struct calendar_descriptor DATETIME_CLOCK;

extern struct i2c_m_sync_desc       I2C_SHARED;
extern struct spi_m_sync_descriptor SPI_TEMP;

extern struct i2c_m_sync_desc       I2C_EXPANDER;
extern struct spi_m_sync_descriptor SPI_HI3593;

extern struct i2c_m_async_desc I2C_EEPROM;
extern struct timer_descriptor TIMER_USB;
extern struct timer_descriptor TIMER_ARINC;
extern struct timer_descriptor TIMER_MAINTENANCE;
extern struct timer_descriptor TIMER_INTERFACE;
extern struct timer_descriptor TIMER_EVENT;

extern struct pwm_descriptor LIVE_PULSE;

extern struct timer_descriptor TIMER_HVAC;

extern struct timer_descriptor TIMER_TEMPERATURES;

extern struct wdt_descriptor       WATCHDOG;
extern struct can_async_descriptor CAN_CCU;

void QSPI_N25Q256_PORT_init(void);
void QSPI_N25Q256_CLOCK_init(void);
void QSPI_N25Q256_init(void);

void DATETIME_CLOCK_CLOCK_init(void);
void DATETIME_CLOCK_init(void);

void I2C_SHARED_CLOCK_init(void);
void I2C_SHARED_init(void);
void I2C_SHARED_PORT_init(void);

void SPI_TEMP_PORT_init(void);
void SPI_TEMP_CLOCK_init(void);
void SPI_TEMP_init(void);

void I2C_EXPANDER_CLOCK_init(void);
void I2C_EXPANDER_init(void);
void I2C_EXPANDER_PORT_init(void);

void SPI_HI3593_PORT_init(void);
void SPI_HI3593_CLOCK_init(void);
void SPI_HI3593_init(void);

void I2C_EEPROM_PORT_init(void);
void I2C_EEPROM_CLOCK_init(void);
void I2C_EEPROM_init(void);

void LIVE_PULSE_PORT_init(void);
void LIVE_PULSE_CLOCK_init(void);
void LIVE_PULSE_init(void);

void TIMER_HVAC_CLOCK_init(void);
void TIMER_HVAC_init(void);

void TIMER_TEMPERATURES_CLOCK_init(void);
void TIMER_TEMPERATURES_init(void);

void CDCUSB_CLOCK_init(void);
void CDCUSB_init(void);

void WATCHDOG_CLOCK_init(void);
void WATCHDOG_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
