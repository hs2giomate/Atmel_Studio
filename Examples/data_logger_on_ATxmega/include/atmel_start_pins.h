/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <port.h>

/**
 * \brief Set PA0 output & pull configuration
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] output_pull_mode Pin output & pull mode
 */
static inline void PA0_set_output_pull_mode(const enum port_output_pull_mode output_pull_mode)
{
	PORTA_set_pin_output_pull_mode(0, output_pull_mode);
}

/**
 * \brief Set PA0 data direction
 *
 * Select if the pin data direction is input, output or disabled.
 * If disabled state is not possible, this function throws an assert.
 *
 * \param[in] direction PORT_DIR_IN  = Data direction in
 *                      PORT_DIR_OUT = Data direction out
 *                      PORT_DIR_OFF = Disables the pin
 *                      (low power state)
 */
static inline void PA0_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(0, dir);
}

/**
 * \brief Set PA0 input/sense configuration
 *
 * Enable/disable PA0 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void PA0_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(0, isc);
}

/**
 * \brief Set PA0 inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on PA0 is inverted
 *                     false = I/O on PA0 is not inverted
 */
static inline void PA0_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(0, inverted);
}

/**
 * \brief Set PA0 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void PA0_set_level(const bool level)
{
	PORTA_set_pin_level(0, level);
}

/**
 * \brief Toggle output level on PA0
 *
 * Toggle the pin level
 */
static inline void PA0_toggle_level()
{
	PORTA_toggle_pin_level(0);
}

/**
 * \brief Get level on PA0
 *
 * Reads the level on a pin
 */
static inline bool PA0_get_level()
{
	return PORTA_get_pin_level(0);
}

/**
 * \brief Set PA0 interrupt level
 *
 * Sets interrupt level for port
 *
 * \param[in] level Value to write to the port register
 */
static inline void PA0_int_level(const uint8_t level)
{
	PORTA_set_int_level(level);
}

/**
 * \brief Set PA0 interrupt vector 0 mask
 *
 * Sets interrupt mask for port vector 0
 *
 * \param[in] value Value to write to the port register
 */
static inline void PA0_int0_mask(const uint8_t value)
{
	PORTA_write_int0_mask(value);
}

/**
 * \brief Set PA0 interrupt vector 1 mask
 *
 * Sets interrupt mask for port vector 1
 *
 * \param[in] value Value to write to the port register
 */
static inline void PA0_int1_mask(const uint8_t value)
{
	PORTA_write_int1_mask(value);
}

/**
 * \brief Set LED0 output & pull configuration
 *
 * Configure pin to pull up, down or disable pull mode, supported pull
 * modes are defined by device used
 *
 * \param[in] output_pull_mode Pin output & pull mode
 */
static inline void LED0_set_output_pull_mode(const enum port_output_pull_mode output_pull_mode)
{
	PORTQ_set_pin_output_pull_mode(3, output_pull_mode);
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
	PORTQ_set_pin_dir(3, dir);
}

/**
 * \brief Set LED0 input/sense configuration
 *
 * Enable/disable LED0 digital input buffer and pin change interrupt,
 * select pin interrupt edge/level sensing mode
 *
 * \param[in] isc PORT_ISC_BOTHEDGES_gc     = Sense Both Edges
 *                PORT_ISC_RISING_gc        = Sense Rising Edge
 *                PORT_ISC_FALLING_gc       = Sense Falling Edge
 *                PORT_ISC_INPUT_DISABLE_gc = Digital Input Buffer disabled
 *                PORT_ISC_LEVEL_gc         = Sense low Level
 */
static inline void LED0_set_isc(const PORT_ISC_t isc)
{
	PORTQ_pin_set_isc(3, isc);
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
	PORTQ_pin_set_inverted(3, inverted);
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
	PORTQ_set_pin_level(3, level);
}

/**
 * \brief Toggle output level on LED0
 *
 * Toggle the pin level
 */
static inline void LED0_toggle_level()
{
	PORTQ_toggle_pin_level(3);
}

/**
 * \brief Get level on LED0
 *
 * Reads the level on a pin
 */
static inline bool LED0_get_level()
{
	return PORTQ_get_pin_level(3);
}

/**
 * \brief Set LED0 interrupt level
 *
 * Sets interrupt level for port
 *
 * \param[in] level Value to write to the port register
 */
static inline void LED0_int_level(const uint8_t level)
{
	PORTQ_set_int_level(level);
}

/**
 * \brief Set LED0 interrupt vector 0 mask
 *
 * Sets interrupt mask for port vector 0
 *
 * \param[in] value Value to write to the port register
 */
static inline void LED0_int0_mask(const uint8_t value)
{
	PORTQ_write_int0_mask(value);
}

/**
 * \brief Set LED0 interrupt vector 1 mask
 *
 * Sets interrupt mask for port vector 1
 *
 * \param[in] value Value to write to the port register
 */
static inline void LED0_int1_mask(const uint8_t value)
{
	PORTQ_write_int1_mask(value);
}

#endif /* ATMEL_START_PINS_H_INCLUDED */
