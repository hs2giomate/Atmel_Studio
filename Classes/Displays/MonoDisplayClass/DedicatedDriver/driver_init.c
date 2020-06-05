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

struct spi_m_sync_descriptor SPI_INSTANCE;

void SPI_INSTANCE_PORT_init(void)
{

	gpio_set_pin_level(PC04,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PC04, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PC04, PINMUX_PC04C_SERCOM6_PAD0);

	gpio_set_pin_level(PC05,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PC05, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PC05, PINMUX_PC05C_SERCOM6_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PC07, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PC07,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PC07, PINMUX_PC07C_SERCOM6_PAD3);
}

void SPI_INSTANCE_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_CORE, CONF_GCLK_SERCOM6_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_SLOW, CONF_GCLK_SERCOM6_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM6_bit(MCLK);
}

void SPI_INSTANCE_init(void)
{
	SPI_INSTANCE_CLOCK_init();
	spi_m_sync_init(&SPI_INSTANCE, SERCOM6);
	SPI_INSTANCE_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PC01

	gpio_set_pin_level(DC_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(DC_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DC_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC14

	gpio_set_pin_level(CS_PIN_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(CS_PIN_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(CS_PIN_0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC31

	gpio_set_pin_level(RES_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(RES_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RES_PIN, GPIO_PIN_FUNCTION_OFF);

	SPI_INSTANCE_init();

	delay_driver_init();
}
