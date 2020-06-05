/**
 * \file
 *
 * \brief BPmono 10z14 font declaration
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
#ifndef _GFX_MONO_FONT_BPMONO_10X14_H_INCLUDED
#define _GFX_MONO_FONT_BPMONO_10X14_H_INCLUDED

#include <compiler.h>
#include <utils.h>
#include <gfx_mono_font.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup GFX mono fonts
 *
 * @{
 */

/* "BPmono" font (http://www.backpacker.gr) at size 10x14 */

/** Width of each glyph, including spacer column. */
#define FONT_BPMONO_10x14_WIDTH 10
/** Height of each glyph, excluding spacer line. */
#define FONT_BPMONO_10x14_HEIGHT 14
/** Line height. */
#define FONT_BPMONO_10x14_LINESPACING 8
/** First character defined. */
#define FONT_BPMONO_10x14_FIRSTCHAR ((uint8_t)' ')
/** Last character defined. */
#define FONT_BPMONO_10x14_LASTCHAR ((uint8_t)'}')
/** Font definition */
extern struct font bpmono_10x14;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _GFX_MONO_FONT_BPMONO_10X14_H_INCLUDED */
