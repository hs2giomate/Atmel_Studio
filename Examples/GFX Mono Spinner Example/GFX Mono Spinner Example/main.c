/**
 * \file
 *
 * \brief Application implement
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "gfx_mono_main.h"
#include <hal_delay.h>

PROGMEM_DECLARE(uint8_t const, spinnertitle[]) = "LED State:";

PROGMEM_DECLARE(uint8_t const, spinner_choice1[]) = "on";
PROGMEM_DECLARE(uint8_t const, spinner_choice2[]) = "off";

PROGMEM_STRING_T spinner_choicestrings[] = {
    spinner_choice1,
    spinner_choice2,
};

static PROGMEM_DECLARE(uint8_t, spin_indicator_data[]) = {0x18, 0x3C, 0x7E, 0xFF};

static PROGMEM_DECLARE(uint8_t, indicator_data[]) = {0xFF, 0x7E, 0x3C, 0x18};

static struct gfx_mono_bitmap default_spin_indicator
    = {.width = 4, .height = 8, .type = GFX_MONO_BITMAP_PROGMEM, .data.progmem = spin_indicator_data};

static struct gfx_mono_bitmap default_indicator
    = {.width = 4, .height = 8, .type = GFX_MONO_BITMAP_PROGMEM, .data.progmem = indicator_data};

static void BUTTON2_pressed(void)
{
	gfx_mono_spinctrl_process_key(&MONOCHROME_WIDGET_0, GFX_MONO_SPINCTRL_KEYCODE_ENTER);

	delay_ms(500);
	ext_irq_register(OLED1_BUTTON2, NULL);
	ext_irq_register(OLED1_BUTTON2, BUTTON2_pressed);
}

static void BUTTON3_pressed(void)
{
	gfx_mono_spinctrl_process_key(&MONOCHROME_WIDGET_0, GFX_MONO_SPINCTRL_KEYCODE_UP);

	delay_ms(500);
	ext_irq_register(OLED1_BUTTON3, NULL);
	ext_irq_register(OLED1_BUTTON3, BUTTON3_pressed);

	if (MONOCHROME_WIDGET_0.in_focus) {
		gpio_toggle_pin_level(OLED1_LED1);
	}
}

int main(void)
{
	atmel_start_init();

	spi_m_sync_enable(&OLED1_SPI);

	gfx_mono_init();
	gfx_mono_spinctrl_init(&MONOCHROME_WIDGET_0,
	                       SPINTYPE_STRING,
	                       spinnertitle,
	                       spinner_choicestrings,
	                       0,
	                       1,
	                       0,
	                       &default_spin_indicator,
	                       &default_indicator);
	gfx_mono_spinctrl_draw(&MONOCHROME_WIDGET_0, true);

	gpio_set_pin_pull_mode(OLED1_BUTTON2, GPIO_PULL_UP);
	gpio_set_pin_pull_mode(OLED1_BUTTON3, GPIO_PULL_UP);
	ext_irq_register(OLED1_BUTTON2, BUTTON2_pressed);
	ext_irq_register(OLED1_BUTTON3, BUTTON3_pressed);
}
