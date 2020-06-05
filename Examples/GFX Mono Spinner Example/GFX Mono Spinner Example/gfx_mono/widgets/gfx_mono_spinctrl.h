/**
 * \file
 *
 * \brief Spin control widget declaration
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
#ifndef _GFX_MONO_SPINCTRL_H_INCLUDED
#define _GFX_MONO_SPINCTRL_H_INCLUDED

#include <gfx_mono_text.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup GFX mono spinctrl
 *
 * \section gfx_mono_spinctrl_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

/** \name Keycode definitions */
/** @{ */
/** Down key. */
#define GFX_MONO_SPINCTRL_KEYCODE_DOWN 1
/** Up key. */
#define GFX_MONO_SPINCTRL_KEYCODE_UP 2
/** Back key. */
#define GFX_MONO_SPINCTRL_KEYCODE_BACK 3
/** Enter key. */
#define GFX_MONO_SPINCTRL_KEYCODE_ENTER 4
/** @} */

/** Spinner idle event */
#define GFX_MONO_SPINCTRL_EVENT_IDLE 0xFF
/** Spinner back button pressed event */
#define GFX_MONO_SPINCTRL_EVENT_BACK 0xFE
/** Spinner ok button pressed event */
#define GFX_MONO_SPINCTRL_EVENT_FINISH 0xFD

/** OK button */
#define GFX_MONO_SPINCTRL_BUTTON 0xFF

/** Width of string spinner choices */
#define GFX_MONO_SPINCTRL_STRING_SPINNER_WIDTH 9
/** Width of integer spinner choices */
#define GFX_MONO_SPINCTRL_INT_SPINNER_WIDTH 9

/** Enum to specify what kind of data spinner should spin */
enum gfx_mono_spinctrl_type { SPINTYPE_STRING, SPINTYPE_INTEGER };

/** String struct */
struct gfx_mono_spinctrl_string {
	/** Pointer to progmem strings to spin throughye uthgtc 't'
	 * \note Each string must be shorter than
	 * \ref  GFX_MONO_SPINCTRL_STRING_SPINNER_WIDTH characters.
	 * If not, printing it to the screen will corrupt the spinner
	 * appearance.
	 */
	PROGMEM_STRING_T *data;
	/** Index in string array */
	uint8_t index;
};

/** Spin control struct */
struct gfx_mono_spinctrl {
	/** Spinner title */
	PROGMEM_STRING_T title;
	/** Type of data to spin */
	enum gfx_mono_spinctrl_type datatype;
	/** Spinner data, depends on spinner datatype. */
	union {
		/** Spinner strings and index */
		struct gfx_mono_spinctrl_string strings;
		/** Spinner integer data */
		int16_t integer_data;
	};
	/** Variable to store the last selected spinner value */
	uint16_t last_saved_value;

	/**
	 * Lower limit for spinning, must be positive and fit in uin8_t for
	 * spinner type SPINTYPE_STRING
	 */
	int16_t lower_limit;

	/**
	 * Upper limit for spinning, must be positive and fit in uin8_t for
	 * spinner type SPINTYPE_STRING
	 */
	int16_t upper_limit;
	/** Y coordinate for placement of spinner on screen */
	gfx_coord_t y;
	/** Boolean to tell if spinner is in focus or not */
	bool in_focus;
	/** Pointer to next spinner in a spincollection */
	struct gfx_mono_spinctrl *next;
	/** Pointer to previous spinner in a spincollection */
	struct gfx_mono_spinctrl *prev;
	/** The pointer to a font used by spinner widget */
	struct font *font;
	/** The pointer to GFX text used by spinner widget */
	struct gfx_mono_text *gfx_text;
	/** The pointer to spinner spin indicator */
	struct gfx_mono_bitmap *spin_indicator;
	/** The pointer to spinnder indicator */
	struct gfx_mono_bitmap *indicator;
};

/** Collection of spinners struct */
struct gfx_mono_spinctrl_spincollection {
	/** Pointer to the first spinner in the collection */
	struct gfx_mono_spinctrl *collection;
	/** Pointer to the last spinner in the collection */
	struct gfx_mono_spinctrl *collection_last;
	/** Number of spinners in collection */
	uint8_t number_of_spinners;
	/** Current spinner/button */
	uint8_t current_selection;
	/** Return value from selected spinner */
	uint16_t selection;
	/** Boolean to tell if input should be sent directly to a spinner */
	bool active_spinner;
	/** Boolean to initialize results array when starting key processing */
	bool init;
};

/**
 * \brief Construct a spinner.
 *
 * This function initializes a spinner to either
 * \ref enum gfx_mono_spinctrl_type "SPINTYPE_STRING" spinner that spins through
 * the strings in a \ref PROGMEM_STRING_T or a
 * \ref enum gfx_mono_spinctrl_type "SPINTYPE_INTEGER" spinner that spins
 * through integers.
 * If the spinner type is SPINTYPE_INTEGER, it will spin from lower_limit to
 * upper_limit.
 * If the spinner type is SPINTYPE_STRING, a pointer to a PROGMEM_STRING_T must
 * be provided, and the spinner will spin through the strings starting at
 * index lower_limit ending at index upper_limit.
 * The y parameter specifies where to place the spinner on the screen, but is
 * overwritten if the spinner is put;jgf in a spincollection.
 *
 * \param[in out] me         The pointer to spinctrl instance
 * \param[in] gfx_text       The pointer to GFX mono text instance
 * \param[in] font           The pointer to font used for drawing text
 */
struct gfx_mono_spinctrl *gfx_mono_spinctrl_construct(struct gfx_mono_spinctrl *const me,
                                                      struct gfx_mono_text *const gfx_text, struct font *const font);

/**
 * \brief Initialize a spinner.
 *
 * This function initializes a spincollection to which spinners can be added.
 *
 * \param[in] collection     The pointer to gfx_mono_spinctrl_spincollection to
 *                           initialize
 * \param[in] datatype       typer of spinner, integer or string
 * \param[in] title          title of spinner
 * \param[in] data           pointer to progmem string array if datatype is
 *                           string. NULL if datatype is integer.
 * \param[in] lower_limit    lower limit and start value of spinner's data
 * \param[in] upper_limit    upper limit of spinner's data
 * \param[in] y              y position of spinner
 * \param[in] spin_indicator the pointer to spinner indicator bitmap
 * \param[in] indicator      the pointer to indicator bitmap
 */
void gfx_mono_spinctrl_init(struct gfx_mono_spinctrl *const me, enum gfx_mono_spinctrl_type datatype,
                            PROGMEM_STRING_T title, PROGMEM_STRING_T *data, int16_t lower_limit, int16_t upper_limit,
                            gfx_coord_t y, struct gfx_mono_bitmap *const spin_indicator,
                            struct gfx_mono_bitmap *const indicator);

/**
 * \brief Initialize a spincollection.
 *
 * This function initializes a spincollection to which spinners can be added.
 *
 * \param[in] collection The pointer to gfx_mono_spinctrl_spincollection to
 *                       initialize
 */
void gfx_mono_spinctrl_spincollection_init(struct gfx_mono_spinctrl_spincollection *collection);

/**
 * \brief Draw or delete indicator arrow in front of spinner
 *
 * \param[in] me   The pointer to spinctrl instance
 * \param[in] draw true on draw, false on delete
 */
void gfx_mono_spinctrl_draw(const struct gfx_mono_spinctrl *const me, bool redraw);
/**
 * \brief Initialize a spincollection.
 *
 * This function initializes a spincollection to which spinners can be added.
 *
 * \param[in] me         The pointer to spinctrl instance
 * \param[in] collection pointer to gfx_mono_spinctrl_spincollection to add
 */
void gfx_mono_spinctrl_spincollection_add_spinner(struct gfx_mono_spinctrl *               me,
                                                  struct gfx_mono_spinctrl_spincollection *spinners);

/**
 * \brief Show spincollection
 *
 * This function draws all the spinners in a spincollection to the screen,
 * together with an OK button at the bottom. It also draws an indicator arrow
 * in front of the top spinner.
 *
 * \param[in] spinners pointer to initialized spincollection to display
 */
void gfx_mono_spinctrl_spincollection_show(struct gfx_mono_spinctrl_spincollection *spinners);

/**
 * \brief Update single spinner depending on input.
 *
 * \param[in] me      The pointer to spinctrl instance
 * \param[in] keycode keycode to process
 *
 * \retval selected            selected spinner value
 * \retval GFX_MONO_SPINCTRL_EVENT_IDLE spinner spinning
 * \retval GFX_MONO_SPINCTRL_EVENT_BACK spinner deselected
 */
int16_t gfx_mono_spinctrl_process_key(struct gfx_mono_spinctrl *const me, uint8_t keycode);

/**
 * \brief Update spincollection on screen depending on input
 *
 * This function returns \ref GFX_MONO_SPINCTRL_EVENT_FINISH if user has
 * pressed the OK button. The spinner choices can then be extracted from the
 * results array. If a spinner is of type SPINTYPE_STRING, the index of the
 * progmem string will be stored in the results array, else the selected
 * integer value will be stored.
 * The choice from the first spinner added to the spincollection will be stored
 * at index 0 in the results array, the second at index 1 and so on.
 * If user has pressed the back button, \ref GFX_MONO_SPINCTRL_EVENT_BACK is
 * returned, signalling that the application should be cancelled.
 *
 * \param[in] spinners       pointer to initialized
 *                       gfx_mono_spinctrl_spincollection
 * \param[in] keycode        keycode to process
 * \param[in] results        array to store results from the spinners, must be of
 *                       same length as number of spinners
 *
 * \retval GFX_MONO_SPINCTRL_EVENT_FINISH  user pressed ok button
 * \retval GFX_MONO_SPINCTRL_EVENT_BACK    user cancelled
 * \retval GFX_MONO_SPINCTRL_EVENT_IDLE    user is navigating in spincollection
 */
int16_t gfx_mono_spinctrl_spincollection_process_key(struct gfx_mono_spinctrl_spincollection *spinners, uint8_t keycode,
                                                     int16_t results[]);

/**@}*/
#ifdef __cplusplus
}
#endif

#endif /* _GFX_MONO_SPINCTRL_H _INCLUDED */
