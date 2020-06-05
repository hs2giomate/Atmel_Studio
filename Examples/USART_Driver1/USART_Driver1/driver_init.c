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

/*! The buffer size for USART */
#define USART_2_BUFFER_SIZE 16

struct usart_async_descriptor USART_2;

static uint8_t USART_2_buffer[USART_2_BUFFER_SIZE];

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void USART_2_CLOCK_init()
{

	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM2);
	_gclk_enable_channel(SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void USART_2_PORT_init()
{

	gpio_set_pin_function(PA12, PINMUX_PA12C_SERCOM2_PAD0);

	gpio_set_pin_function(PA13, PINMUX_PA13C_SERCOM2_PAD1);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void USART_2_init(void)
{
	USART_2_CLOCK_init();
	usart_async_init(&USART_2, SERCOM2, USART_2_buffer, USART_2_BUFFER_SIZE, (void *)NULL);
	USART_2_PORT_init();
}

void system_init(void)
{
	init_mcu();

	USART_2_init();
}
