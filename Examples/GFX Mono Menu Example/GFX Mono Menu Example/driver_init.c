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

struct spi_m_sync_descriptor OLED1_SPI;

void EXTERNAL_IRQ_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBAMASK_EIC_bit(MCLK);

	// Set pin direction to input
	gpio_set_pin_direction(OLED1_BUTTON2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(OLED1_BUTTON2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(OLED1_BUTTON2, PINMUX_PC02A_EIC_EXTINT2);

	// Set pin direction to input
	gpio_set_pin_direction(OLED1_BUTTON3, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(OLED1_BUTTON3,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(OLED1_BUTTON3, PINMUX_PC03A_EIC_EXTINT3);

	ext_irq_init();
}

void OLED1_SPI_PORT_init(void)
{

	gpio_set_pin_level(OLED1_SPI_MOSI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(OLED1_SPI_MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(OLED1_SPI_MOSI, PINMUX_PC04C_SERCOM6_PAD0);

	gpio_set_pin_level(OLED1_SPI_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(OLED1_SPI_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(OLED1_SPI_SCK, PINMUX_PC05C_SERCOM6_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(OLED1_SPI_MISO_NC, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(OLED1_SPI_MISO_NC,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(OLED1_SPI_MISO_NC, PINMUX_PC07C_SERCOM6_PAD3);
}

void OLED1_SPI_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_CORE, CONF_GCLK_SERCOM6_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_SLOW, CONF_GCLK_SERCOM6_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM6_bit(MCLK);
}

void OLED1_SPI_init(void)
{
	OLED1_SPI_CLOCK_init();
	spi_m_sync_init(&OLED1_SPI, SERCOM6);
	OLED1_SPI_PORT_init();
}

void system_init(void)
{
	init_mcu();

	// GPIO on PC01

	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC14

	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC30

	// Set pin direction to input
	gpio_set_pin_direction(OLED1_BUTTON1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(OLED1_BUTTON1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(OLED1_BUTTON1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC31

	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PD10

	gpio_set_pin_level(OLED1_LED1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(OLED1_LED1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(OLED1_LED1, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_0_init();

	OLED1_SPI_init();
}
