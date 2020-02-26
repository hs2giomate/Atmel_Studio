/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

struct spi_m_sync_descriptor SPI_0;

struct flash_descriptor FLASH_INSTANCE;

void FLASH_INSTANCE_CLOCK_init(void)
{

	_pm_enable_bus_clock(PM_BUS_APBB, NVMCTRL);
}

void FLASH_INSTANCE_init(void)
{
	FLASH_INSTANCE_CLOCK_init();
	flash_init(&FLASH_INSTANCE, NVMCTRL);
}

void SPI_0_PORT_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(PA09, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA09,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA09, PINMUX_PA09C_SERCOM0_PAD1);

	gpio_set_pin_level(PA10,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA10, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA10, PINMUX_PA10C_SERCOM0_PAD2);

	gpio_set_pin_level(PA11,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA11, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA11, PINMUX_PA11C_SERCOM0_PAD3);
}

void SPI_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM0);
	SPI_0_PORT_init();
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA27

	gpio_set_pin_level(FLASH_CS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(FLASH_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(FLASH_CS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA28

	gpio_set_pin_level(FRAM_CS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(FRAM_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(FRAM_CS, GPIO_PIN_FUNCTION_OFF);

	FLASH_INSTANCE_init();

	SPI_0_init();
}
