/**
 * \file
 *
 * \brief Simple menu system declaration
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
#ifndef _GFX_MONO_MENU_H_INCLUDED
#define _GFX_MONO_MENU_H_INCLUDED

#include <utils.h>
#include <display_mono.h>
#include <gfx_mono_text.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup GFX mono menu
 *
 * \section gfx_mono_menu_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

/** \name Keycode definitions */
/** @{ */
/** Down key. */
#define GFX_MONO_MENU_KEYCODE_DOWN 1
/** Up key. */
#define GFX_MONO_MENU_KEYCODE_UP 2
/** Back key. */
#define GFX_MONO_MENU_KEYCODE_BACK 3
/** Enter key. */
#define GFX_MONO_MENU_KEYCODE_ENTER 4
/** @} */

/** \name Menu events definitions */
/** @{ */
/** Idle. Nothing to report. */
#define GFX_MONO_MENU_EVENT_IDLE 0xFF
/** Exit. User has pressed the back button. */
#define GFX_MONO_MENU_EVENT_EXIT 0xFE
/** @} */

/** Menu struct */
struct gfx_mono_menu {
	/** The pointer to the string with menu title */
	PROGMEM_STRING_T title;
	/** The array of strings with menu elements */
	PROGMEM_STRING_T *strings;
	/** The amount of menu elements */
	uint8_t num_elements;
	/** Currently selected menu element */
	uint8_t current_selection;
	/** Current menu page */
	uint8_t current_page;
	/** The pointer to GFX text used by menu widget */
	struct gfx_mono_text *gfx_text;
	/** The pointer to a font used by menu widget */
	struct font *font;
	/** The pointer to the menu indicator */
	struct gfx_mono_bitmap *indicator;
};

/**
 *\brief Construct the menu instance. Clear screen and draw menu.
 *
 * \param[in out] me    The pointer to menu instance
 * \param[in] gfx_text  The pointer to GFX mono text instance
 * \param[in] font      The pointer to font used for drawing text
 *
 * \return The pointer to constructed menu
 */
struct gfx_mono_menu *gfx_mono_menu_construct(struct gfx_mono_menu *const me, struct gfx_mono_text *const gfx_text,
                                              struct font *const font);

/**
 *\brief Draw the given menu
 *
 * \param[in] me The pointer to menu instance
 * \param[in] title     The pointer to a string with menu title
 * \param[in] strings   The pointer an array of menu strings
 * \param[in] amount    The amount menu elements
 * \param[in] current   Currently selected menu element
 * \param[in] indicator The pointer to menu indicator
 */
void gfx_mono_menu_show(struct gfx_mono_menu *const me, PROGMEM_STRING_T title, PROGMEM_STRING_T *strings,
                        const uint8_t amount, const uint8_t current, struct gfx_mono_bitmap *const indicator);

/**
 * \brief Update menu depending on input.
 *
 * \param[in] me       The pointer to menu instance
 * \param[in] keycode  Keycode to process
 *
 * \retval selected menu option or status code
 */
uint8_t gfx_mono_menu_process_key(struct gfx_mono_menu *me, uint8_t keycode);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif /* _GFX_MONO_MENU_H_INCLUDED */
