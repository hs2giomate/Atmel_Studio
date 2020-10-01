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

#include <hal_ext_irq.h>

#include <hal_qspi_sync.h>

#include <hal_spi_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_i2c_m_sync.h>

#include <hal_i2c_m_sync.h>
#include <hal_can_async.h>

extern struct qspi_sync_descriptor  QSPI_MEMORY;
extern struct spi_m_sync_descriptor SPI_ARINC;
extern struct spi_m_sync_descriptor SPI_MEMORIES;

extern struct i2c_m_sync_desc       I2C_SHARED;
extern struct spi_m_sync_descriptor SPI_TEMP;

extern struct i2c_m_sync_desc I2C_FV;

extern struct i2c_m_sync_desc      I2C_FANS;
extern struct can_async_descriptor CAN_CCU;

void QSPI_MEMORY_PORT_init(void);
void QSPI_MEMORY_CLOCK_init(void);
void QSPI_MEMORY_init(void);

void SPI_ARINC_PORT_init(void);
void SPI_ARINC_CLOCK_init(void);
void SPI_ARINC_init(void);

void SPI_MEMORIES_PORT_init(void);
void SPI_MEMORIES_CLOCK_init(void);
void SPI_MEMORIES_init(void);

void I2C_SHARED_CLOCK_init(void);
void I2C_SHARED_init(void);
void I2C_SHARED_PORT_init(void);

void SPI_TEMP_PORT_init(void);
void SPI_TEMP_CLOCK_init(void);
void SPI_TEMP_init(void);

void I2C_FV_CLOCK_init(void);
void I2C_FV_init(void);
void I2C_FV_PORT_init(void);

void I2C_FANS_CLOCK_init(void);
void I2C_FANS_init(void);
void I2C_FANS_PORT_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
