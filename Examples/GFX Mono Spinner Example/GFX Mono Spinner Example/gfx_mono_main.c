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
#ifndef MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN
#define MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN 0
#endif
#ifndef MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN
#define MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN 0
#endif
#ifndef MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN
#define MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN 0
#endif

/*! ug2832hsweg04 display structure */
static struct ug2832hsweg04 MONOCHROME_DISPLAY_0_c12832a1z_desc;
/*! Buffer used by ug2832hsweg04 display */
static uint8_t MONOCHROME_DISPLAY_0_buffer[UG2832HSWEG04_LCD_FRAMEBUFFER_SIZE];

/*! The graphics library */
struct gfx_mono MONOCHROME_GRAPHICS_0_desc;
/*! The graphics text library */
struct gfx_mono_text MONOCHROME_TEXT_0_desc;
/*! The widget library */
struct gfx_mono_spinctrl MONOCHROME_WIDGET_0;

/**
 * \brief Initialize GFX Mono library
 */
void gfx_mono_init(void)
{

	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN, true);
	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN, true);
	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN, true);
	ug2832hsweg04_construct(&MONOCHROME_DISPLAY_0_c12832a1z_desc.parent,
	                        MONOCHROME_DISPLAY_0_buffer,
	                        &OLED1_SPI.io,
	                        MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN,
	                        MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN,
	                        MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN);
	gfx_mono_construct(&MONOCHROME_GRAPHICS_0_desc, &MONOCHROME_DISPLAY_0_c12832a1z_desc.parent);
	gfx_mono_text_construct(&MONOCHROME_TEXT_0_desc, &MONOCHROME_GRAPHICS_0_desc);

	/*! The widget library */
	gfx_mono_spinctrl_construct(&MONOCHROME_WIDGET_0, &MONOCHROME_TEXT_0_desc, &basic_6x7);
}
