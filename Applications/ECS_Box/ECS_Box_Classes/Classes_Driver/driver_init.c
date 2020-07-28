/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

#include <hpl_rtc_base.h>

struct timer_descriptor      Live_Pulse;
struct spi_m_sync_descriptor SPI_MEMORIES;
struct spi_m_sync_descriptor SPI_HI3593;
struct timer_descriptor      TIMEOUT;
struct timer_descriptor      Event_Timer;
struct timer_descriptor      TIMER_ARINC;
struct can_async_descriptor  CAN_Compressor;

struct spi_m_async_descriptor SPI_TEMP;

struct i2c_m_async_desc I2C_EEPROM;

void EXTERNAL_IRQ_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBAMASK_EIC_bit(MCLK);

	// Set pin direction to input
	gpio_set_pin_direction(R2Int, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(R2Int,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(R2Int, PINMUX_PC22A_EIC_EXTINT6);

	// Set pin direction to input
	gpio_set_pin_direction(R1int, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(R1int,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(R1int, PINMUX_PC23A_EIC_EXTINT7);

	ext_irq_init();
}

void EVENT_SYSTEM_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EVSYS_GCLK_ID_0, CONF_GCLK_EVSYS_CHANNEL_0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_EVSYS_bit(MCLK);

	event_system_init();
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void Live_Pulse_init(void)
{
	hri_mclk_set_APBAMASK_RTC_bit(MCLK);
	timer_init(&Live_Pulse, RTC, _rtc_get_timer());
}

void SPI_MEMORIES_PORT_init(void)
{

	gpio_set_pin_level(PA00,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA00, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA00, PINMUX_PA00D_SERCOM1_PAD0);

	gpio_set_pin_level(PA01,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PA01, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA01, PINMUX_PA01D_SERCOM1_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PA18, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA18,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA18, PINMUX_PA18C_SERCOM1_PAD2);
}

void SPI_MEMORIES_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBAMASK_SERCOM1_bit(MCLK);
}

void SPI_MEMORIES_init(void)
{
	SPI_MEMORIES_CLOCK_init();
	spi_m_sync_init(&SPI_MEMORIES, SERCOM1);
	SPI_MEMORIES_PORT_init();
}

void SPI_TEMP_PORT_init(void)
{

	gpio_set_pin_level(PB27,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB27, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB27, PINMUX_PB27D_SERCOM4_PAD0);

	gpio_set_pin_level(PB26,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PB26, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PB26, PINMUX_PB26D_SERCOM4_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PB28, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PB28,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PB28, PINMUX_PB28D_SERCOM4_PAD2);
}

void SPI_TEMP_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_SLOW, CONF_GCLK_SERCOM4_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM4_bit(MCLK);
}

void SPI_TEMP_init(void)
{
	SPI_TEMP_CLOCK_init();
	spi_m_async_init(&SPI_TEMP, SERCOM4);
	SPI_TEMP_PORT_init();
}

void SPI_HI3593_PORT_init(void)
{

	gpio_set_pin_level(PC04,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PC04, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PC04, PINMUX_PC04C_SERCOM6_PAD0);

	gpio_set_pin_level(PC05,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PC05, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PC05, PINMUX_PC05C_SERCOM6_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PC07, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PC07,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PC07, PINMUX_PC07C_SERCOM6_PAD3);
}

void SPI_HI3593_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_CORE, CONF_GCLK_SERCOM6_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_SLOW, CONF_GCLK_SERCOM6_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM6_bit(MCLK);
}

void SPI_HI3593_init(void)
{
	SPI_HI3593_CLOCK_init();
	spi_m_sync_init(&SPI_HI3593, SERCOM6);
	SPI_HI3593_PORT_init();
}

void I2C_EEPROM_PORT_init(void)
{

	gpio_set_pin_pull_mode(PD08,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PD08, PINMUX_PD08C_SERCOM7_PAD0);

	gpio_set_pin_pull_mode(PD09,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PD09, PINMUX_PD09C_SERCOM7_PAD1);
}

void I2C_EEPROM_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_CORE, CONF_GCLK_SERCOM7_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_SLOW, CONF_GCLK_SERCOM7_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM7_bit(MCLK);
}

void I2C_EEPROM_init(void)
{
	I2C_EEPROM_CLOCK_init();
	i2c_m_async_init(&I2C_EEPROM, SERCOM7);
	I2C_EEPROM_PORT_init();
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMEOUT_init(void)
{
	hri_mclk_set_APBAMASK_TC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&TIMEOUT, TC0, _tc_get_timer());
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void Event_Timer_init(void)
{
	hri_mclk_set_APBAMASK_TC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC1_GCLK_ID, CONF_GCLK_TC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&Event_Timer, TC1, _tc_get_timer());
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_ARINC_init(void)
{
	hri_mclk_set_APBBMASK_TC2_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC2_GCLK_ID, CONF_GCLK_TC2_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	timer_init(&TIMER_ARINC, TC2, _tc_get_timer());
}

void CDCUSB_PORT_init(void)
{

	gpio_set_pin_direction(PA24,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA24,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA24,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA24,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA24H_USB_DM"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA24H_USB_DM);

	gpio_set_pin_direction(PA25,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA25,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA25,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA25,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <PINMUX_PA25H_USB_DP"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      // <GPIO_PIN_FUNCTION_E"> E
	                      // <GPIO_PIN_FUNCTION_F"> F
	                      // <GPIO_PIN_FUNCTION_G"> G
	                      // <GPIO_PIN_FUNCTION_H"> H
	                      // <GPIO_PIN_FUNCTION_I"> I
	                      // <GPIO_PIN_FUNCTION_J"> J
	                      // <GPIO_PIN_FUNCTION_K"> K
	                      // <GPIO_PIN_FUNCTION_L"> L
	                      // <GPIO_PIN_FUNCTION_M"> M
	                      // <GPIO_PIN_FUNCTION_N"> N
	                      PINMUX_PA25H_USB_DP);
}

/* The USB module requires a GCLK_USB of 48 MHz ~ 0.25% clock
 * for low speed and full speed operation. */
#if (CONF_GCLK_USB_FREQUENCY > (48000000 + 48000000 / 400)) || (CONF_GCLK_USB_FREQUENCY < (48000000 - 48000000 / 400))
#warning USB clock should be 48MHz ~ 0.25% clock, check your configuration!
#endif

void CDCUSB_CLOCK_init(void)
{

	hri_gclk_write_PCHCTRL_reg(GCLK, USB_GCLK_ID, CONF_GCLK_USB_SRC | GCLK_PCHCTRL_CHEN);
	hri_mclk_set_AHBMASK_USB_bit(MCLK);
	hri_mclk_set_APBBMASK_USB_bit(MCLK);
}

void CDCUSB_init(void)
{
	CDCUSB_CLOCK_init();
	usb_d_init();
	CDCUSB_PORT_init();
}

void CAN_Compressor_PORT_init(void)
{

	gpio_set_pin_function(PB13, PINMUX_PB13H_CAN1_RX);

	gpio_set_pin_function(PB12, PINMUX_PB12H_CAN1_TX);
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CAN_Compressor_init(void)
{
	hri_mclk_set_AHBMASK_CAN1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, CAN1_GCLK_ID, CONF_GCLK_CAN1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	can_async_init(&CAN_Compressor, CAN1);
	CAN_Compressor_PORT_init();
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA12

	gpio_set_pin_level(Ethernet_Led,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(Ethernet_Led, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(Ethernet_Led, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB07

	// Set pin direction to input
	gpio_set_pin_direction(INT_LTC2983, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(INT_LTC2983,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(INT_LTC2983, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB29

	gpio_set_pin_level(CS_SPI_LTC,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(CS_SPI_LTC, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(CS_SPI_LTC, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB31

	gpio_set_pin_level(SW0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SW0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SW0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC14

	gpio_set_pin_level(CS_Holt,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(CS_Holt, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(CS_Holt, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC18

	gpio_set_pin_level(LED0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(LED0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED0, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_0_init();

	EVENT_SYSTEM_0_init();

	Live_Pulse_init();

	SPI_MEMORIES_init();

	SPI_TEMP_init();

	SPI_HI3593_init();

	I2C_EEPROM_init();

	TIMEOUT_init();
	Event_Timer_init();
	TIMER_ARINC_init();
	CDCUSB_init();
	CAN_Compressor_init();
}
