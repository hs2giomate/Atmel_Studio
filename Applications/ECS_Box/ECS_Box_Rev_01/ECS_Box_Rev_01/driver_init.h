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

#include <hal_spi_m_sync.h>

#include <hal_i2c_m_sync.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_i2c_m_sync.h>

#include <hal_i2c_m_async.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>

#include "hal_usb_device.h"
#include <hal_can_async.h>

extern struct spi_m_sync_descriptor Flash_Mem_SPI;

extern struct i2c_m_sync_desc Fvx_I2C;

extern struct i2c_m_sync_desc       Heater_I2C;
extern struct spi_m_sync_descriptor SPI_Alc;

extern struct i2c_m_sync_desc Misc_Inc_I2C;

extern struct i2c_m_async_desc I2C_CCu;

extern struct i2c_m_sync_desc       FANs_I2C;
extern struct spi_m_sync_descriptor SPI_Spare;

extern struct can_async_descriptor CCu_CAN;

void Flash_Mem_SPI_PORT_init(void);
void Flash_Mem_SPI_CLOCK_init(void);
void Flash_Mem_SPI_init(void);

void Fvx_I2C_CLOCK_init(void);
void Fvx_I2C_init(void);
void Fvx_I2C_PORT_init(void);

void Heater_I2C_CLOCK_init(void);
void Heater_I2C_init(void);
void Heater_I2C_PORT_init(void);

void SPI_Alc_PORT_init(void);
void SPI_Alc_CLOCK_init(void);
void SPI_Alc_init(void);

void Misc_Inc_I2C_CLOCK_init(void);
void Misc_Inc_I2C_init(void);
void Misc_Inc_I2C_PORT_init(void);

void I2C_CCu_PORT_init(void);
void I2C_CCu_CLOCK_init(void);
void I2C_CCu_init(void);

void FANs_I2C_CLOCK_init(void);
void FANs_I2C_init(void);
void FANs_I2C_PORT_init(void);

void SPI_Spare_PORT_init(void);
void SPI_Spare_CLOCK_init(void);
void SPI_Spare_init(void);

void MaintUSB_CLOCK_init(void);
void MaintUSB_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
