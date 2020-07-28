/*
 * Code generated from START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <port.h>

/**
 * \brief Set LED0 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void LED0_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(0, pull_mode);
}

/**
 * \brief Set LED0 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void LED0_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(0, dir);
}

/**
 * \brief Set LED0 input/sense configuration
 *
 * Enable/disable LED0 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void LED0_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(0, isc);
}

/**
 * \brief Set LED0 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on LED0 is inverted
 *                     false = I/O on LED0 is not inverted
 */
static inline void LED0_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(0, inverted);
}

/**
 * \brief Set LED0 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void LED0_set_level(const bool level)
{
	PORTC_set_pin_level(0, level);
}

/**
 * \brief Toggle output level on LED0
 *
 * Toggle the pin level
 */
static inline void LED0_toggle_level()
{
	PORTC_toggle_pin_level(0);
}

/**
 * \brief Get level on LED0
 *
 * Reads the level on a pin
 */
static inline bool LED0_get_level()
{
	return PORTC_get_pin_level(0);
}

/**
 * \brief Set LED1 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void LED1_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(1, pull_mode);
}

/**
 * \brief Set LED1 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void LED1_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(1, dir);
}

/**
 * \brief Set LED1 input/sense configuration
 *
 * Enable/disable LED1 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void LED1_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(1, isc);
}

/**
 * \brief Set LED1 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on LED1 is inverted
 *                     false = I/O on LED1 is not inverted
 */
static inline void LED1_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(1, inverted);
}

/**
 * \brief Set LED1 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void LED1_set_level(const bool level)
{
	PORTC_set_pin_level(1, level);
}

/**
 * \brief Toggle output level on LED1
 *
 * Toggle the pin level
 */
static inline void LED1_toggle_level()
{
	PORTC_toggle_pin_level(1);
}

/**
 * \brief Get level on LED1
 *
 * Reads the level on a pin
 */
static inline bool LED1_get_level()
{
	return PORTC_get_pin_level(1);
}

/**
 * \brief Set SW0 pull mode
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] pull_mode Pin pull mode
 */
static inline void SW0_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(5, pull_mode);
}

/**
 * \brief Set SW0 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void SW0_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(5, dir);
}

/**
 * \brief Set SW0 input/sense configuration
 *
 * Enable/disable SW0 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_INTDISABLE_gc    = Iterrupt disabled but input buffer enabled
 *                PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void SW0_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(5, isc);
}

/**
 * \brief Set SW0 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on SW0 is inverted
 *                     false = I/O on SW0 is not inverted
 */
static inline void SW0_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(5, inverted);
}

/**
 * \brief Set SW0 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void SW0_set_level(const bool level)
{
	PORTC_set_pin_level(5, level);
}

/**
 * \brief Toggle output level on SW0
 *
 * Toggle the pin level
 */
static inline void SW0_toggle_level()
{
	PORTC_toggle_pin_level(5);
}

/**
 * \brief Get level on SW0
 *
 * Reads the level on a pin
 */
static inline bool SW0_get_level()
{
	return PORTC_get_pin_level(5);
}

#endif /* ATMEL_START_PINS_H_INCLUDED */
