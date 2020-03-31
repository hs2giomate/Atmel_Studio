/**
 * \file
 *
 * \brief Simple menu system implementation
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
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
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

#include "gfx_mono.h"
#include "gfx_mono_menu.h"

static void menu_draw(struct gfx_mono_menu *const me, bool redraw);

/**
 *\brief Construct the menu instance. Clear screen and draw menu.
 */
struct gfx_mono_menu *gfx_mono_menu_construct(struct gfx_mono_menu *const me, struct gfx_mono_text *const gfx_text,
                                              struct font *const font)
{
	me->gfx_text = gfx_text;
	me->font     = font;

	return me;
}

/**
 *\brief Draw the given menu
 */
void gfx_mono_menu_show(struct gfx_mono_menu *const me, PROGMEM_STRING_T title, PROGMEM_STRING_T *strings,
                        const uint8_t amount, const uint8_t current, struct gfx_mono_bitmap *const indicator)
{
	me->title             = title;
	me->strings           = strings;
	me->num_elements      = amount;
	me->current_selection = current;
	me->indicator         = indicator;

	/* Clear screen */
	gfx_mono_draw_filled_rect(
	    me->gfx_text->gfx, 0, 0, me->gfx_text->gfx->display->width, me->gfx_text->gfx->display->height, GFX_PIXEL_CLR);

	/* Draw the menu title on the top of the screen */
	gfx_mono_text_draw_progmem_string(me->gfx_text, (uint8_t PROGMEM_PTR_T)me->title, 0, 0, me->font);

	/* Draw menu options below */
	menu_draw(me, true);
}

/**
 * \brief Update menu depending on input.
 */
uint8_t gfx_mono_menu_process_key(struct gfx_mono_menu *me, uint8_t keycode)
{
	switch (keycode) {
	case GFX_MONO_MENU_KEYCODE_DOWN:
		if (me->current_selection == me->num_elements - 1) {
			me->current_selection = 0;
		} else {
			me->current_selection++;
		}

		/* Update menu on display */
		menu_draw(me, false);
		/* Nothing selected yet */
		return GFX_MONO_MENU_EVENT_IDLE;

	case GFX_MONO_MENU_KEYCODE_UP:
		if (me->current_selection) {
			me->current_selection--;
		} else {
			me->current_selection = me->num_elements - 1;
		}

		/* Update menu on display */
		menu_draw(me, false);
		/* Nothing selected yet */
		return GFX_MONO_MENU_EVENT_IDLE;

	case GFX_MONO_MENU_KEYCODE_ENTER:
		/* Got what we want. Return selection. */
		return me->current_selection;

	case GFX_MONO_MENU_KEYCODE_BACK:
		/* User pressed "back" key, inform user */
		return GFX_MONO_MENU_EVENT_EXIT;

	default:
		/* Unknown key event */
		return GFX_MONO_MENU_EVENT_IDLE;
	}
}

/**
 * \internal Draw menu strings and an icon by the current selection.
 *
 * \param[in] menu     a menu struct with menu settings
 * \param[in] redraw   clear screen before drawing menu
 */
static void menu_draw(struct gfx_mono_menu *const me, bool redraw)
{
	static bool          redraw_state;
	struct display_mono *display = me->gfx_text->gfx->display;
	struct font *        font    = me->font;
	uint8_t              i;
	uint8_t              line             = 1;
	uint8_t              elems_per_screen = display->height / font->line_spacing - 1;
	uint8_t              menu_page        = me->current_selection / elems_per_screen;

	if (me->current_page != menu_page || redraw == true) {
		/* clear screen if we have changed the page or menu and prepare
		 * redraw */
		gfx_mono_draw_filled_rect(me->gfx_text->gfx,
		                          0,
		                          font->line_spacing,
		                          display->width,
		                          display->height - font->line_spacing,
		                          GFX_PIXEL_CLR);
		redraw_state = true;
	}

	me->current_page = menu_page;

	/* Clear old indicator icon */
	gfx_mono_draw_filled_rect(me->gfx_text->gfx,
	                          0,
	                          font->line_spacing,
	                          me->indicator->width,
	                          display->height - font->line_spacing,
	                          GFX_PIXEL_CLR);

	/* Put indicator icon on current selection */
	gfx_mono_put_bitmap(
	    me->gfx_text->gfx, me->indicator, 0, font->line_spacing * ((me->current_selection % elems_per_screen) + 1));

	/* Print visible options if page or menu has changed */
	if (redraw_state == true) {
		for (i = menu_page * elems_per_screen;
		     i < menu_page * elems_per_screen + elems_per_screen && i < me->num_elements;
		     i++) {
			gfx_mono_text_draw_progmem_string(me->gfx_text,
			                                  (uint8_t PROGMEM_PTR_T)me->strings[i],
			                                  me->indicator->width + 1,
			                                  line * font->line_spacing,
			                                  font);
			line++;
		}
		redraw_state = false;
	}
}
