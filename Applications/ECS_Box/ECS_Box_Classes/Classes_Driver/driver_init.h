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

#include <hal_evsys.h>

#include <hal_timer.h>
#include <hal_spi_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>

#include "hal_usb_device.h"
#include <hal_can_async.h>

extern struct timer_descriptor      TIMER_RTC;
extern struct spi_m_sync_descriptor SPI_HOLT;
extern struct spi_m_sync_descriptor SPI_TEMP;

extern struct i2c_m_sync_desc       I2C_EEPROM;
extern struct spi_m_sync_descriptor SPI_MEMORIES;
extern struct timer_descriptor      TIMEOUT;

extern struct can_async_descriptor CAN_Compressor;

void SPI_HOLT_PORT_init(void);
void SPI_HOLT_CLOCK_init(void);
void SPI_HOLT_init(void);

void SPI_TEMP_PORT_init(void);
void SPI_TEMP_CLOCK_init(void);
void SPI_TEMP_init(void);

void I2C_EEPROM_CLOCK_init(void);
void I2C_EEPROM_init(void);
void I2C_EEPROM_PORT_init(void);

void SPI_MEMORIES_PORT_init(void);
void SPI_MEMORIES_CLOCK_init(void);
void SPI_MEMORIES_init(void);

void CDCUSB_CLOCK_init(void);
void CDCUSB_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
