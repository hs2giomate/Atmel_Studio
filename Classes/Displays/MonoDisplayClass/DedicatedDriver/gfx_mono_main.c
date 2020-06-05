/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include <hal_gpio.h>
#include "gfx_mono_main.h"
#include "atmel_start_pins.h"

/* Pins to use for Chip Select, Reset and Data/Command signals */
#ifndef CONTROLLER_INSTANCE_0_CS_PIN
#define CONTROLLER_INSTANCE_0_CS_PIN 0
#endif
#ifndef CONTROLLER_INSTANCE_0_RES_PIN
#define CONTROLLER_INSTANCE_0_RES_PIN 0
#endif
#ifndef CONTROLLER_INSTANCE_0_DC_PIN
#define CONTROLLER_INSTANCE_0_DC_PIN 0
#endif

/*! ug2832hsweg04 display structure */
static struct ug2832hsweg04 DISPLAY_INSTANCE_0_c12832a1z_desc;
/*! Buffer used by ug2832hsweg04 display */
static uint8_t DISPLAY_INSTANCE_0_buffer[UG2832HSWEG04_LCD_FRAMEBUFFER_SIZE];

/*! The graphics library */
struct gfx_mono PRIMITIVES_INSTANCE_desc;
/*! The graphics text library */
struct gfx_mono_text MONOCHROME_TEXT_0_desc;

/**
 * \brief Initialize GFX Mono library
 */
void gfx_mono_init(void)
{

	gpio_set_pin_direction(CONTROLLER_INSTANCE_0_CS_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(CONTROLLER_INSTANCE_0_RES_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(CONTROLLER_INSTANCE_0_DC_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(CONTROLLER_INSTANCE_0_CS_PIN, true);
	gpio_set_pin_level(CONTROLLER_INSTANCE_0_RES_PIN, true);
	gpio_set_pin_level(CONTROLLER_INSTANCE_0_DC_PIN, true);
	ug2832hsweg04_construct(&DISPLAY_INSTANCE_0_c12832a1z_desc.parent,
	                        DISPLAY_INSTANCE_0_buffer,
	                        &SPI_INSTANCE.io,
	                        CONTROLLER_INSTANCE_0_CS_PIN,
	                        CONTROLLER_INSTANCE_0_RES_PIN,
	                        CONTROLLER_INSTANCE_0_DC_PIN);
	gfx_mono_construct(&PRIMITIVES_INSTANCE_desc, &DISPLAY_INSTANCE_0_c12832a1z_desc.parent);
	gfx_mono_text_construct(&MONOCHROME_TEXT_0_desc, &PRIMITIVES_INSTANCE_desc);
}
