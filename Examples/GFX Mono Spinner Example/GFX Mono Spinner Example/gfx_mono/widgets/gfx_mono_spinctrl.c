/**
 * \file
 *
 * \brief Spin control widget
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

#include <gfx_mono_spinctrl.h>
#include <stdio.h>

static void gfx_mono_spinctrl_step_down(struct gfx_mono_spinctrl *const me);
static void gfx_mono_spinctrl_step_up(struct gfx_mono_spinctrl *const me);
static void gfx_mono_spinctrl_draw_button(const struct gfx_mono_spinctrl *const me, bool draw, bool indicator);
static void gfx_mono_spinctrl_draw_spin_indicator(const struct gfx_mono_spinctrl *const me, bool draw);
static void gfx_mono_spinctrl_draw_indicator(struct gfx_mono_spinctrl *me, bool draw);

/**
 * \brief Construct a spinner.
 */
struct gfx_mono_spinctrl *gfx_mono_spinctrl_construct(struct gfx_mono_spinctrl *const me,
                                                      struct gfx_mono_text *const gfx_text, struct font *const font)
{
	me->gfx_text = gfx_text;
	me->font     = font;

	return me;
}

/**
 * \brief Initialize a spinner.
 */
void gfx_mono_spinctrl_init(struct gfx_mono_spinctrl *const me, enum gfx_mono_spinctrl_type datatype,
                            PROGMEM_STRING_T title, PROGMEM_STRING_T *data, int16_t lower_limit, int16_t upper_limit,
                            gfx_coord_t y, struct gfx_mono_bitmap *const spin_indicator,
                            struct gfx_mono_bitmap *const indicator)
{
	/* Initialization of spinner parameters */
	me->title            = title;
	me->datatype         = datatype;
	me->lower_limit      = lower_limit;
	me->upper_limit      = upper_limit;
	me->y                = y;
	me->in_focus         = false;
	me->last_saved_value = me->lower_limit;
	me->indicator        = indicator;
	me->spin_indicator   = spin_indicator;

	if (datatype == SPINTYPE_STRING) {
		me->strings.data  = data;
		me->strings.index = lower_limit;
	} else {
		me->integer_data = lower_limit;
	}
}

/**
 * \brief Initialize a spincollection.
 */
void gfx_mono_spinctrl_spincollection_init(struct gfx_mono_spinctrl_spincollection *collection)
{
	collection->active_spinner     = false;
	collection->current_selection  = 0;
	collection->number_of_spinners = 0;
	collection->init               = true;
}

/**
 * \brief Draw spinner at its position.
 */
void gfx_mono_spinctrl_draw(const struct gfx_mono_spinctrl *const me, bool redraw)
{
	uint8_t               string_buf[GFX_MONO_SPINCTRL_INT_SPINNER_WIDTH];
	uint8_t               index;
	uint8_t               offset;
	struct display_mono * display  = me->gfx_text->gfx->display;
	struct gfx_mono_text *gfx_text = me->gfx_text;
	struct gfx_mono *     gfx      = me->gfx_text->gfx;

	if (redraw) {
		/* Clear line */
		gfx_mono_draw_filled_rect(gfx, 0, me->y, display->width, me->font->height, GFX_PIXEL_CLR);
		/* Draw title */
		gfx_mono_text_draw_progmem_string(
		    gfx_text, (uint8_t PROGMEM_PTR_T)me->title, me->indicator->width + 1, me->y, me->font);
	}

	if (me->in_focus) {
		gfx_mono_spinctrl_draw_spin_indicator(me, true);
	} else {
		gfx_mono_spinctrl_draw_spin_indicator(me, false);
	}

	if (me->datatype == SPINTYPE_INTEGER) {
		uint8_t const digit[] = "0123456789";
		int16_t       tmp     = me->integer_data;
		uint8_t *     str     = string_buf;
		int16_t       shift;

		offset = display->width - (me->font->width * GFX_MONO_SPINCTRL_INT_SPINNER_WIDTH);

		if (me->integer_data < 0) {
			*str++ = '-';
			tmp *= -1;
		}

		shift = tmp;
		do {
			++str;
			shift = shift / 10;
		} while (shift);
		*str = 0;
		do {
			*--str = digit[tmp % 10];
			tmp    = tmp / 10;
		} while (tmp);

		/* Delete previous spinner data */
		gfx_mono_draw_filled_rect(
		    gfx, offset, me->y, display->width - offset - me->spin_indicator->width, me->font->height, GFX_PIXEL_CLR);
		/* Draw integer data */
		gfx_mono_text_draw_string(gfx_text, string_buf, offset, me->y, me->font);
	} else if (me->datatype == SPINTYPE_STRING) {
		index  = me->strings.index;
		offset = display->width - (me->font->width * GFX_MONO_SPINCTRL_STRING_SPINNER_WIDTH);

		/* Delete previous spinner data */
		gfx_mono_draw_filled_rect(
		    gfx, offset, me->y, display->width - offset - me->spin_indicator->width, me->font->height, GFX_PIXEL_CLR);

		/* Draw string data */
		gfx_mono_text_draw_progmem_string(
		    gfx_text, (uint8_t PROGMEM_PTR_T)me->strings.data[index], offset, me->y, me->font);
	}
}

/**
 * \brief Add spinner to spincollection
 */
void gfx_mono_spinctrl_spincollection_add_spinner(struct gfx_mono_spinctrl *               me,
                                                  struct gfx_mono_spinctrl_spincollection *spinners)
{
	uint8_t                   i;
	struct gfx_mono_spinctrl *lastspinner;

	/* Do not add more spinner elements than maximum number of spinners */
	if (spinners->number_of_spinners >= ((me->gfx_text->gfx->display->height / me->font->line_spacing) - 1)) {
		return;
	}

	/* Place new spinner below previous spinners on screen */
	me->y = (me->font->height + 1) * spinners->number_of_spinners;

	/* Add pointer to the spinner in spincollection if empty */
	if (spinners->number_of_spinners == 0) {
		spinners->collection = me;
	} else {
		lastspinner = spinners->collection;
		for (i = 1; i < spinners->number_of_spinners; i++) {
			lastspinner = lastspinner->next;
		}
		/* Link the new spinner to the current last spinner in the
		 * collection */
		lastspinner->next = me;
		/* Link the current last spinner as previous spinner for new
		 * spinner */
		me->prev = lastspinner;
	}

	/* Set added spinner as last spinner in collection */
	spinners->collection_last = me;
	/* Update number of spinners in collection */
	spinners->number_of_spinners++;
}

/**
 * \brief Show spincollection
 */
void gfx_mono_spinctrl_spincollection_show(struct gfx_mono_spinctrl_spincollection *spinners)
{
	uint8_t                   i;
	struct gfx_mono_spinctrl *iterator = spinners->collection;

	/* Make sure there are spinners in the collection */
	if (spinners->number_of_spinners == 0) {
		return;
	}

	/* Draw spinners on screen */
	for (i = 0; i < spinners->number_of_spinners; i++) {
		struct display_mono *display = iterator->gfx_text->gfx->display;
		struct gfx_mono *    gfx     = iterator->gfx_text->gfx;

		gfx_mono_draw_filled_rect(gfx, 0, 0, display->width, display->height, GFX_PIXEL_CLR);
		gfx_mono_spinctrl_draw(iterator, true);
		iterator = iterator->next;
	}
	/* Draw OK button at bottom of screen */
	gfx_mono_spinctrl_draw_button(spinners->collection, true, false);
	/* Draw indicator arrow in front of first spinner */
	gfx_mono_spinctrl_draw_indicator(spinners->collection, true);
}

/**
 * \brief Update single spinner depending on input.
 */
int16_t gfx_mono_spinctrl_process_key(struct gfx_mono_spinctrl *const me, uint8_t keycode)
{
	switch (keycode) {
	case GFX_MONO_SPINCTRL_KEYCODE_DOWN:
		if (me->in_focus) {
			gfx_mono_spinctrl_step_down(me);
			/* Update spinner on display */
			gfx_mono_spinctrl_draw(me, false);
		}

		/* Nothing selected yet */
		return GFX_MONO_SPINCTRL_EVENT_IDLE;

	case GFX_MONO_SPINCTRL_KEYCODE_UP:
		if (me->in_focus) {
			gfx_mono_spinctrl_step_up(me);
			/* Update spinner on display */
			gfx_mono_spinctrl_draw(me, false);
		}

		/* Nothing selected yet */
		return GFX_MONO_SPINCTRL_EVENT_IDLE;

	case GFX_MONO_SPINCTRL_KEYCODE_ENTER:
		if (me->in_focus) {
			if (me->datatype == SPINTYPE_INTEGER) {
				me->in_focus = false;
				gfx_mono_spinctrl_draw(me, false);
				/* Store saved value in case of aborting spinner
				 * later */
				me->last_saved_value = me->integer_data;
				/* Got what we want. Return selection. */
				return me->integer_data;
			} else if (me->datatype == SPINTYPE_STRING) {
				me->in_focus = false;
				gfx_mono_spinctrl_draw(me, false);
				/* Store saved value in case of aborting spinner
				 * later */
				me->last_saved_value = me->strings.index;
				/* Got what we want. Return selection. */
				return me->strings.index;
			}
		} else {
			/* Spinner selected */
			me->in_focus = true;
			gfx_mono_spinctrl_draw(me, false);
			return GFX_MONO_SPINCTRL_EVENT_IDLE;
		}

	case GFX_MONO_SPINCTRL_KEYCODE_BACK:
		/* User pressed "back" key, */
		me->in_focus = false;
		/* Spinner choice aborted, show last saved value instead */
		if (me->datatype == SPINTYPE_INTEGER) {
			me->integer_data = me->last_saved_value;
		} else if (me->datatype == SPINTYPE_STRING) {
			me->strings.index = me->last_saved_value;
		}

		gfx_mono_spinctrl_draw(me, false);
		return GFX_MONO_SPINCTRL_EVENT_BACK;

	default:
		/* Unknown key event */
		return GFX_MONO_SPINCTRL_EVENT_IDLE;
	}
}

/**
 * \brief Update spincollection on screen depending on input
 */
int16_t gfx_mono_spinctrl_spincollection_process_key(struct gfx_mono_spinctrl_spincollection *spinners, uint8_t keycode,
                                                     int16_t results[])
{
	uint8_t                   i;
	struct gfx_mono_spinctrl *iterator;

	/* Make sure there are spinners in the collection, if not, cancel */
	if (spinners->number_of_spinners == 0) {
		return GFX_MONO_SPINCTRL_EVENT_BACK;
	}

	/* Store initial values in results array first time function is run */
	if (spinners->init) {
		iterator = spinners->collection;
		for (i = 0; i < spinners->number_of_spinners; i++) {
			if (iterator->datatype == SPINTYPE_INTEGER) {
				results[i] = iterator->integer_data;
			} else {
				results[i] = iterator->strings.index;
			}

			iterator = iterator->next;
		}
		spinners->init = false;
	}

	/* Find current spinner selection */
	iterator = spinners->collection;
	if (spinners->current_selection != GFX_MONO_SPINCTRL_BUTTON) {
		for (i = 0; i < spinners->current_selection; i++) {
			iterator = iterator->next;
		}
	}

	if (spinners->active_spinner) {
		/* Process chosen spinner */
		spinners->selection = gfx_mono_spinctrl_process_key(iterator, keycode);
		if (spinners->selection == GFX_MONO_SPINCTRL_EVENT_BACK) {
			/* User has exited spinner without saving the result */
			spinners->active_spinner = false;
		} else if (spinners->selection != GFX_MONO_SPINCTRL_EVENT_IDLE) {
			/* Value selected, store in array */
			results[spinners->current_selection] = spinners->selection;
			/* Step out of spinner and into spincollection */
			spinners->active_spinner = false;
		}

		return GFX_MONO_SPINCTRL_EVENT_IDLE;
	} else {
		switch (keycode) {
		case GFX_MONO_SPINCTRL_KEYCODE_DOWN:
			if (spinners->current_selection == GFX_MONO_SPINCTRL_BUTTON) {
				spinners->current_selection = 0;
				/* Delete indicator arrow in front of button */
				gfx_mono_spinctrl_draw_button(iterator, true, false);
				/* Draw indicator arrow in front of first
				 * spinner */
				gfx_mono_spinctrl_draw_indicator(iterator, true);
			} else if (spinners->current_selection < spinners->number_of_spinners - 1) {
				/* Delete indicator arrow */
				gfx_mono_spinctrl_draw_indicator(iterator, false);
				spinners->current_selection++;
				/* Draw indicator arrow in front of new spinner */
				gfx_mono_spinctrl_draw_indicator(iterator->next, true);
			} else {
				/* Delete indicator arrow */
				gfx_mono_spinctrl_draw_indicator(iterator, false);
				spinners->current_selection = GFX_MONO_SPINCTRL_BUTTON;
				/* Draw indicator arrow in front of button */
				gfx_mono_spinctrl_draw_button(iterator, true, true);
			}

			return GFX_MONO_SPINCTRL_EVENT_IDLE;

		case GFX_MONO_SPINCTRL_KEYCODE_UP:
			if (spinners->current_selection == GFX_MONO_SPINCTRL_BUTTON) {
				/* Delete indicator arrow in front of button */
				gfx_mono_spinctrl_draw_button(iterator, true, false);
				spinners->current_selection = spinners->number_of_spinners - 1;
				/* Draw indicator arrow in front of new spinner */
				gfx_mono_spinctrl_draw_indicator(spinners->collection_last, true);
			} else if (spinners->current_selection > 0) {
				/* Delete indicator arrow */
				gfx_mono_spinctrl_draw_indicator(iterator, false);
				spinners->current_selection--;
				/* Draw indicator arrow in front of new spinner */
				gfx_mono_spinctrl_draw_indicator(iterator->prev, true);
			} else {
				/* Delete indicator arrow */
				gfx_mono_spinctrl_draw_indicator(iterator, false);
				spinners->current_selection = GFX_MONO_SPINCTRL_BUTTON;
				/* Draw indicator arrow in front of button */
				gfx_mono_spinctrl_draw_button(iterator, true, true);
			}

			return GFX_MONO_SPINCTRL_EVENT_IDLE;

		case GFX_MONO_SPINCTRL_KEYCODE_ENTER:
			if (spinners->current_selection == GFX_MONO_SPINCTRL_BUTTON) {
				/* Finished with all selections, return */
				return GFX_MONO_SPINCTRL_EVENT_FINISH;
			} else {
				/* Spinner selected, send next keycode directly
				 * to spinner */
				gfx_mono_spinctrl_process_key(iterator, keycode);
				spinners->active_spinner = true;
				return GFX_MONO_SPINCTRL_EVENT_IDLE;
			}

		case GFX_MONO_SPINCTRL_KEYCODE_BACK:
			/* User pressed "back" key, */
			return GFX_MONO_SPINCTRL_EVENT_BACK;

		default:
			/* Unknown key event */
			return GFX_MONO_SPINCTRL_EVENT_IDLE;
		}
	}
}

/**
 * \brief Draw or delete indicator arrow in front of spinner
 *
 * \param[in] me   The pointer to spinctrl instance
 * \param[in] draw True on draw, false on delete
 */
static void gfx_mono_spinctrl_draw_indicator(struct gfx_mono_spinctrl *me, bool draw)
{
	if (draw) {
		gfx_mono_put_bitmap(me->gfx_text->gfx, me->indicator, 0, me->y);
	} else {
		gfx_mono_draw_filled_rect(
		    me->gfx_text->gfx, 0, me->y, me->indicator->width, me->indicator->height, GFX_PIXEL_CLR);
	}
}

/**
 * \brief Draw or delete indicator in front of spinner data
 *
 * \param[in] me   The pointer to spinctrl instance
 * \param[in] draw True on draw, false on delete
 */
static void gfx_mono_spinctrl_draw_spin_indicator(const struct gfx_mono_spinctrl *const me, bool draw)
{
	if (draw) {
		gfx_mono_put_bitmap(me->gfx_text->gfx,
		                    me->spin_indicator,
		                    me->gfx_text->gfx->display->width - me->spin_indicator->width,
		                    me->y);
	} else {
		gfx_mono_draw_filled_rect(me->gfx_text->gfx,
		                          me->gfx_text->gfx->display->width - me->spin_indicator->width,
		                          me->y,
		                          me->spin_indicator->width,
		                          me->spin_indicator->height,
		                          GFX_PIXEL_CLR);
	}
}

/**
 * \brief Draw OK button at bottom of screen
 *
 * This function draws an OK button at the bottom of the screen. It will
 * also draw an indicator arrow in front of the button if the indicator bool
 * is true. If the draw bool is false, the OK button will be deleted, and if
 * the indicator bool is false, the indicator will be deleted.
 *
 * \param[in] me        The pointer to spinctrl instance
 * \param[in] draw      True on draw, false on delete
 * \param[in] indicator True on draw indicator, false on delete
 */
static void gfx_mono_spinctrl_draw_button(const struct gfx_mono_spinctrl *const me, bool draw, bool indicator)
{
	uint8_t               width;
	uint8_t               height;
	uint8_t               offset;
	uint8_t               string_buf[3];
	struct display_mono * display  = me->gfx_text->gfx->display;
	struct gfx_mono *     gfx      = me->gfx_text->gfx;
	struct font *         font     = me->font;
	struct gfx_mono_text *gfx_text = me->gfx_text;
	const uint8_t         max_elem = (display->height / font->line_spacing) - 1;

	/* Clear bottom line */
	gfx_mono_draw_filled_rect(gfx, 0, (font->height + 1) * max_elem, display->width, font->height, GFX_PIXEL_CLR);

	string_buf[0] = 'O';
	string_buf[1] = 'K';
	string_buf[2] = 0;
	gfx_mono_text_get_string_bounding_box(gfx_text, string_buf, font, &width, &height);
	offset = (display->width - width) / 2;

	if (draw) {
		/* Draw OK button in the middle of the last line */
		gfx_mono_text_draw_string(gfx_text, string_buf, offset, (font->height + 1) * max_elem, font);
		if (indicator) {
			/* Draw indicator arrow in front of button */
			gfx_mono_put_bitmap(gfx, me->indicator, offset - me->indicator->width, (font->height + 1) * max_elem);
		} else {
			/* Delete indicator */
			gfx_mono_draw_filled_rect(gfx,
			                          offset - me->indicator->width,
			                          (font->height + 1) * max_elem,
			                          me->indicator->width,
			                          me->indicator->height,
			                          GFX_PIXEL_CLR);
		}
	} else {
		/* Delete OK button */
		gfx_mono_draw_filled_rect(
		    gfx, offset - me->indicator->width, (font->height + 1) * max_elem, 20, font->height, GFX_PIXEL_CLR);
	}
}

/**
 * \brief Step up spinner data
 *
 * This function steps up the data, making sure it does not go
 * beyond the upper limit. Wraps around if it does.
 *
 * \param[in] me The pointer to spinctrl instance
 */
static void gfx_mono_spinctrl_step_up(struct gfx_mono_spinctrl *const me)
{
	/* Check if spinner type is integer or string, increment integer data or
	 * move to next string index.
	 */
	if (me->datatype == SPINTYPE_INTEGER) {
		if (me->integer_data < me->upper_limit) {
			me->integer_data++;
		} else {
			me->integer_data = me->lower_limit;
		}
	} else if (me->datatype == SPINTYPE_STRING) {
		if (me->strings.index < me->upper_limit) {
			me->strings.index++;
		} else {
			(me->strings.index) = me->lower_limit;
		}
	}
}

/**
 * \brief Step down spinner data
 *
 * * This function steps down the data, making sure it does not go
 * below the lower limit. Wraps around if it does.
 *
 * \param[in] me   The pointer to spinctrl instance
 */
static void gfx_mono_spinctrl_step_down(struct gfx_mono_spinctrl *const me)
{
	/* Check if spinner type is integer or string, increment integer data,
	 * move to next string index.
	 */
	if (me->datatype == SPINTYPE_INTEGER) {
		if (me->integer_data > me->lower_limit) {
			me->integer_data--;
		} else {
			me->integer_data = me->upper_limit;
		}
	} else if (me->datatype == SPINTYPE_STRING) {
		if (me->strings.index > (me->lower_limit)) {
			me->strings.index--;
		} else {
			me->strings.index = me->upper_limit;
		}
	}
}
