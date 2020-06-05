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

#include <hal_i2c_m_async.h>

#include <hal_i2c_m_async.h>
#include <hal_spi_m_sync.h>

#include <hal_delay.h>

#include "hal_usb_device.h"
#include <hal_can_async.h>

extern struct timer_descriptor      TIMER_0;
extern struct spi_m_sync_descriptor SPI_LTC2983;

extern struct i2c_m_async_desc I2C_1;

extern struct i2c_m_async_desc      I2C_EEPROM;
extern struct spi_m_sync_descriptor SPI_Holt;

extern struct can_async_descriptor CAN_0;

void SPI_LTC2983_PORT_init(void);
void SPI_LTC2983_CLOCK_init(void);
void SPI_LTC2983_init(void);

void I2C_1_PORT_init(void);
void I2C_1_CLOCK_init(void);
void I2C_1_init(void);

void I2C_EEPROM_PORT_init(void);
void I2C_EEPROM_CLOCK_init(void);
void I2C_EEPROM_init(void);

void SPI_Holt_PORT_init(void);
void SPI_Holt_CLOCK_init(void);
void SPI_Holt_init(void);

void delay_driver_init(void);

void USB_DEVICE_INSTANCE_CLOCK_init(void);
void USB_DEVICE_INSTANCE_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
