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

struct calendar_descriptor CALENDAR_0;

void CALENDAR_0_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
}

void CALENDAR_0_init(void)
{
	CALENDAR_0_CLOCK_init();
	calendar_init(&CALENDAR_0, RTC);
}

void system_init(void)
{
	init_mcu();

	CALENDAR_0_init();
}
