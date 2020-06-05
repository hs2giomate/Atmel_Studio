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

struct spi_m_sync_descriptor Flash_Mem_SPI;
struct spi_m_sync_descriptor SPI_Alc;
struct spi_m_sync_descriptor SPI_Spare;
struct can_async_descriptor  CCu_CAN;

struct i2c_m_sync_desc Fvx_I2C;

struct i2c_m_sync_desc Heater_I2C;

struct i2c_m_sync_desc Misc_Inc_I2C;

struct i2c_m_async_desc I2C_CCu;

struct i2c_m_sync_desc FANs_I2C;

void EXTERNAL_IRQ_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBAMASK_EIC_bit(MCLK);

	// Set pin direction to input
	gpio_set_pin_direction(iAclHeater1StatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclHeater1StatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclHeater1StatusChanged, PINMUX_PB03A_EIC_EXTINT3);

	// Set pin direction to input
	gpio_set_pin_direction(niAlcFansStatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(niAlcFansStatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(niAlcFansStatusChanged, PINMUX_PB04A_EIC_EXTINT4);

	// Set pin direction to input
	gpio_set_pin_direction(iAclCcuStatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclCcuStatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclCcuStatusChanged, PINMUX_PB05A_EIC_EXTINT5);

	// Set pin direction to input
	gpio_set_pin_direction(niAlcMiscInStatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(niAlcMiscInStatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(niAlcMiscInStatusChanged, PINMUX_PB06A_EIC_EXTINT6);

	// Set pin direction to input
	gpio_set_pin_direction(iAclHeater2StatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclHeater2StatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclHeater2StatusChanged, PINMUX_PB07A_EIC_EXTINT7);

	// Set pin direction to input
	gpio_set_pin_direction(iAclFv1StatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclFv1StatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclFv1StatusChanged, PINMUX_PB08A_EIC_EXTINT8);

	// Set pin direction to input
	gpio_set_pin_direction(iAclFv2StatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclFv2StatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclFv2StatusChanged, PINMUX_PB09A_EIC_EXTINT9);

	// Set pin direction to input
	gpio_set_pin_direction(nAlcReset, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(nAlcReset,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(nAlcReset, PINMUX_PB10A_EIC_EXTINT10);

	// Set pin direction to input
	gpio_set_pin_direction(iAmmcInt1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAmmcInt1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAmmcInt1, PINMUX_PB28A_EIC_EXTINT14);

	// Set pin direction to input
	gpio_set_pin_direction(iAmmcInt2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAmmcInt2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAmmcInt2, PINMUX_PB29A_EIC_EXTINT15);

	ext_irq_init();
}

void Flash_Mem_SPI_PORT_init(void)
{

	gpio_set_pin_level(FlashMem_MOSI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(FlashMem_MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(FlashMem_MOSI, PINMUX_PA04D_SERCOM0_PAD0);

	gpio_set_pin_level(FlashMem_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(FlashMem_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(FlashMem_SCK, PINMUX_PA05D_SERCOM0_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(FlashMem_MISO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(FlashMem_MISO,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(FlashMem_MISO, PINMUX_PA06D_SERCOM0_PAD2);
}

void Flash_Mem_SPI_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBAMASK_SERCOM0_bit(MCLK);
}

void Flash_Mem_SPI_init(void)
{
	Flash_Mem_SPI_CLOCK_init();
	spi_m_sync_init(&Flash_Mem_SPI, SERCOM0);
	Flash_Mem_SPI_PORT_init();
}

void Fvx_I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(iAlcFvxI2cSda,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFvxI2cSda, PINMUX_PA16C_SERCOM1_PAD0);

	gpio_set_pin_pull_mode(iAlcFvxI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFvxI2cScl, PINMUX_PA17C_SERCOM1_PAD1);
}

void Fvx_I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBAMASK_SERCOM1_bit(MCLK);
}

void Fvx_I2C_init(void)
{
	Fvx_I2C_CLOCK_init();
	i2c_m_sync_init(&Fvx_I2C, SERCOM1);
	Fvx_I2C_PORT_init();
}

void Heater_I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(iAclHeaterI2cSda,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclHeaterI2cSda, PINMUX_PA09D_SERCOM2_PAD0);

	gpio_set_pin_pull_mode(iAclHeaterI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclHeaterI2cScl, PINMUX_PA08D_SERCOM2_PAD1);
}

void Heater_I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

void Heater_I2C_init(void)
{
	Heater_I2C_CLOCK_init();
	i2c_m_sync_init(&Heater_I2C, SERCOM2);
	Heater_I2C_PORT_init();
}

void SPI_Alc_PORT_init(void)
{

	gpio_set_pin_level(iAlcSpiMosi,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiMosi, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiMosi, PINMUX_PC23D_SERCOM3_PAD0);

	gpio_set_pin_level(iAlcSpiSck,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiSck, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiSck, PINMUX_PC22D_SERCOM3_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(iAlcSpiMiso, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAlcSpiMiso,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcSpiMiso, PINMUX_PA18D_SERCOM3_PAD2);
}

void SPI_Alc_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_SLOW, CONF_GCLK_SERCOM3_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM3_bit(MCLK);
}

void SPI_Alc_init(void)
{
	SPI_Alc_CLOCK_init();
	spi_m_sync_init(&SPI_Alc, SERCOM3);
	SPI_Alc_PORT_init();
}

void Misc_Inc_I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(iAlcFMiscIncI2cSda,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFMiscIncI2cSda, PINMUX_PA13D_SERCOM4_PAD0);

	gpio_set_pin_pull_mode(iAlcMiscIncI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcMiscIncI2cScl, PINMUX_PA12D_SERCOM4_PAD1);
}

void Misc_Inc_I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_SLOW, CONF_GCLK_SERCOM4_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM4_bit(MCLK);
}

void Misc_Inc_I2C_init(void)
{
	Misc_Inc_I2C_CLOCK_init();
	i2c_m_sync_init(&Misc_Inc_I2C, SERCOM4);
	Misc_Inc_I2C_PORT_init();
}

void I2C_CCu_PORT_init(void)
{

	gpio_set_pin_pull_mode(iAclCcuI2cSda,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclCcuI2cSda, PINMUX_PA23D_SERCOM5_PAD0);

	gpio_set_pin_pull_mode(iAclCcuI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclCcuI2cScl, PINMUX_PA22D_SERCOM5_PAD1);
}

void I2C_CCu_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_SLOW, CONF_GCLK_SERCOM5_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM5_bit(MCLK);
}

void I2C_CCu_init(void)
{
	I2C_CCu_CLOCK_init();
	i2c_m_async_init(&I2C_CCu, SERCOM5);
	I2C_CCu_PORT_init();
}

void FANs_I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(iAlcFansI2cSda,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFansI2cSda, PINMUX_PD09D_SERCOM6_PAD0);

	gpio_set_pin_pull_mode(iAlcFansI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFansI2cScl, PINMUX_PD08D_SERCOM6_PAD1);
}

void FANs_I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_CORE, CONF_GCLK_SERCOM6_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_SLOW, CONF_GCLK_SERCOM6_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM6_bit(MCLK);
}

void FANs_I2C_init(void)
{
	FANs_I2C_CLOCK_init();
	i2c_m_sync_init(&FANs_I2C, SERCOM6);
	FANs_I2C_PORT_init();
}

void SPI_Spare_PORT_init(void)
{

	gpio_set_pin_level(SPI_Spare_MOSI,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_Spare_MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_Spare_MOSI, PINMUX_PC12C_SERCOM7_PAD0);

	gpio_set_pin_level(SPI_Spare_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_Spare_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_Spare_SCK, PINMUX_PC13C_SERCOM7_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(SPI_Spare_MISO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SPI_Spare_MISO,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SPI_Spare_MISO, PINMUX_PD10C_SERCOM7_PAD2);
}

void SPI_Spare_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_CORE, CONF_GCLK_SERCOM7_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_SLOW, CONF_GCLK_SERCOM7_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM7_bit(MCLK);
}

void SPI_Spare_init(void)
{
	SPI_Spare_CLOCK_init();
	spi_m_sync_init(&SPI_Spare, SERCOM7);
	SPI_Spare_PORT_init();
}

void MaintUSB_PORT_init(void)
{

	gpio_set_pin_direction(iMaintUsbDm,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(iMaintUsbDm,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(iMaintUsbDm,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iMaintUsbDm,
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

	gpio_set_pin_direction(iMaintUsbDp,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(iMaintUsbDp,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(iMaintUsbDp,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iMaintUsbDp,
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

void MaintUSB_CLOCK_init(void)
{

	hri_gclk_write_PCHCTRL_reg(GCLK, USB_GCLK_ID, CONF_GCLK_USB_SRC | GCLK_PCHCTRL_CHEN);
	hri_mclk_set_AHBMASK_USB_bit(MCLK);
	hri_mclk_set_APBBMASK_USB_bit(MCLK);
}

void MaintUSB_init(void)
{
	MaintUSB_CLOCK_init();
	usb_d_init();
	MaintUSB_PORT_init();
}

void CCu_CAN_PORT_init(void)
{

	gpio_set_pin_function(iAlcCcuCanRxd, PINMUX_PB13H_CAN1_RX);

	gpio_set_pin_function(iAlcCcuCanTxd, PINMUX_PB12H_CAN1_TX);
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CCu_CAN_init(void)
{
	hri_mclk_set_AHBMASK_CAN1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, CAN1_GCLK_ID, CONF_GCLK_CAN1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	can_async_init(&CCu_CAN, CAN1);
	CCu_CAN_PORT_init();
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA00

	// Set pin direction to input
	gpio_set_pin_direction(XIN32, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(XIN32,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(XIN32, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA01

	gpio_set_pin_level(XOUT32,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(XOUT32, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(XOUT32, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB22

	// Set pin direction to input
	gpio_set_pin_direction(XIN1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(XIN1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(XIN1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB23

	gpio_set_pin_level(XOUT,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(XOUT, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(XOUT, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB27

	gpio_set_pin_level(niAlcSPIFramCs,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcSPIFramCs, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcSPIFramCs, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC00

	// Set pin direction to input
	gpio_set_pin_direction(iMaintUsbVbus, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iMaintUsbVbus,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iMaintUsbVbus, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC02

	gpio_set_pin_level(iUcNoFailure,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iUcNoFailure, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iUcNoFailure, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC04

	gpio_set_pin_level(iAlcCcuCanStdby,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcCcuCanStdby, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcCcuCanStdby, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC11

	gpio_set_pin_level(SPI_Spare_CS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_Spare_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_Spare_CS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC19

	// Set pin direction to input
	gpio_set_pin_direction(iMaintUsbId, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iMaintUsbId,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iMaintUsbId, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC24

	gpio_set_pin_level(niAmmSPIFlashMemCS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAmmSPIFlashMemCS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAmmSPIFlashMemCS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC25

	gpio_set_pin_level(niAmmcSpiSs,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(niAmmcSpiSs, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAmmcSpiSs, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC26

	gpio_set_pin_level(niAlcTempSensSpiSs2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcTempSensSpiSs2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcTempSensSpiSs2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC27

	gpio_set_pin_level(niAlcTempSensSpiSs1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcTempSensSpiSs1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcTempSensSpiSs1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC28

	gpio_set_pin_level(niAlcTempSensSpiSs0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcTempSensSpiSs0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcTempSensSpiSs0, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_0_init();

	Flash_Mem_SPI_init();

	Fvx_I2C_init();

	Heater_I2C_init();

	SPI_Alc_init();

	Misc_Inc_I2C_init();

	I2C_CCu_init();

	FANs_I2C_init();

	SPI_Spare_init();

	MaintUSB_init();
	CCu_CAN_init();
}
