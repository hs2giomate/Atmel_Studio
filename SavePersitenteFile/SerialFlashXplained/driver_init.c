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

void SPI_0_PORT_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(PB16, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PB16,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PB16, PINMUX_PB16C_SERCOM5_PAD0);

	gpio_set_pin_level(PB22,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB22, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB22, PINMUX_PB22D_SERCOM5_PAD2);

	gpio_set_pin_level(PB23,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB23, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB23, PINMUX_PB23D_SERCOM5_PAD3);
}

void SPI_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM5);
	_gclk_enable_channel(SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM5);
	SPI_0_PORT_init();
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA13

	gpio_set_pin_level(SPI_CS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_CS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB30

	gpio_set_pin_level(USER_LED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(USER_LED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(USER_LED, GPIO_PIN_FUNCTION_OFF);

	SPI_0_init();
}
