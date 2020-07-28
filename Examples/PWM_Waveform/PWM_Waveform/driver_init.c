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

struct usart_sync_descriptor EDBG_UART;

void EXTERNAL_IRQ_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBAMASK_EIC_bit(MCLK);

	// Set pin direction to input
	gpio_set_pin_direction(SW0, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SW0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SW0, PINMUX_PB31A_EIC_EXTINT15);

	ext_irq_init();
}

void EDBG_UART_PORT_init(void)
{

	gpio_set_pin_function(PB25, PINMUX_PB25D_SERCOM2_PAD0);

	gpio_set_pin_function(PB24, PINMUX_PB24D_SERCOM2_PAD1);
}

void EDBG_UART_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

void EDBG_UART_init(void)
{
	EDBG_UART_CLOCK_init();
	usart_sync_init(&EDBG_UART, SERCOM2, (void *)NULL);
	EDBG_UART_PORT_init();
}

void PWM_0_PORT_init(void)
{

	gpio_set_pin_function(PC10, PINMUX_PC10F_TCC0_WO0);

	gpio_set_pin_function(PC11, PINMUX_PC11F_TCC0_WO1);

	gpio_set_pin_function(PC12, PINMUX_PC12F_TCC0_WO2);

	gpio_set_pin_function(PA23, PINMUX_PA23G_TCC0_WO3);

	gpio_set_pin_function(PA16, PINMUX_PA16G_TCC0_WO4);

	gpio_set_pin_function(PA17, PINMUX_PA17G_TCC0_WO5);

	gpio_set_pin_function(PA18, PINMUX_PA18G_TCC0_WO6);
}

void PWM_0_CLOCK_init(void)
{
	hri_mclk_set_APBBMASK_TCC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TCC0_GCLK_ID, CONF_GCLK_TCC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void system_init(void)
{
	init_mcu();

	EXTERNAL_IRQ_0_init();

	EDBG_UART_init();

	PWM_0_CLOCK_init();

	PWM_0_PORT_init();

	PWM_0_init();
}
