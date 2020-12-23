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
struct spi_m_sync_descriptor SPI_Temp;
struct spi_m_sync_descriptor SPI_AMMC;
struct can_async_descriptor  CCu_CAN;

struct i2c_m_sync_desc Shared_I2C;

struct i2c_m_sync_desc Fvx_I2C;

struct i2c_m_sync_desc FANs_I2C;

void EXTERNAL_IRQ_0_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBAMASK_EIC_bit(MCLK);

	// Set pin direction to input
	gpio_set_pin_direction(iMaintUsbVbus, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iMaintUsbVbus,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iMaintUsbVbus, PINMUX_PC00A_EIC_EXTINT0);

	// Set pin direction to input
	gpio_set_pin_direction(iAlcTempSensInt3, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAlcTempSensInt3,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcTempSensInt3, PINMUX_PB01A_EIC_EXTINT1);

	// Set pin direction to input
	gpio_set_pin_direction(SW0, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SW0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SW0, PINMUX_PC18A_EIC_EXTINT2);

	// Set pin direction to input
	gpio_set_pin_direction(SpareInterrupt0, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SpareInterrupt0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SpareInterrupt0, PINMUX_PA03A_EIC_EXTINT3);

	// Set pin direction to input
	gpio_set_pin_direction(iAclFv1StatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclFv1StatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclFv1StatusChanged, PINMUX_PA04A_EIC_EXTINT4);

	// Set pin direction to input
	gpio_set_pin_direction(iAclFv2StatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclFv2StatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclFv2StatusChanged, PINMUX_PA05A_EIC_EXTINT5);

	// Set pin direction to input
	gpio_set_pin_direction(iAmmcInt1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAmmcInt1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAmmcInt1, PINMUX_PC22A_EIC_EXTINT6);

	// Set pin direction to input
	gpio_set_pin_direction(iAmmcInt2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAmmcInt2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAmmcInt2, PINMUX_PC23A_EIC_EXTINT7);

	// Set pin direction to input
	gpio_set_pin_direction(iAlcTempSensInt1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAlcTempSensInt1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcTempSensInt1, PINMUX_PB08A_EIC_EXTINT8);

	// Set pin direction to input
	gpio_set_pin_direction(iAlcTempSensInt2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAlcTempSensInt2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcTempSensInt2, PINMUX_PB09A_EIC_EXTINT9);

	// Set pin direction to input
	gpio_set_pin_direction(SpareInterrupt1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SpareInterrupt1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SpareInterrupt1, PINMUX_PC10A_EIC_EXTINT10);

	// Set pin direction to input
	gpio_set_pin_direction(SpareInterrupt2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SpareInterrupt2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SpareInterrupt2, PINMUX_PC11A_EIC_EXTINT11);

	// Set pin direction to input
	gpio_set_pin_direction(SpareInterrupt3, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SpareInterrupt3,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SpareInterrupt3, PINMUX_PC12A_EIC_EXTINT12);

	// Set pin direction to input
	gpio_set_pin_direction(iAclSharedI2CChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAclSharedI2CChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAclSharedI2CChanged, PINMUX_PA13A_EIC_EXTINT13);

	// Set pin direction to input
	gpio_set_pin_direction(niAlcFansStatusChanged, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(niAlcFansStatusChanged,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(niAlcFansStatusChanged, PINMUX_PA14A_EIC_EXTINT14);

	// Set pin direction to input
	gpio_set_pin_direction(nAlcReset, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(nAlcReset,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(nAlcReset, PINMUX_PC31A_EIC_EXTINT15);

	ext_irq_init();
}

void Flash_Mem_SPI_PORT_init(void)
{

	gpio_set_pin_level(iAlcSpiMemMosi,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiMemMosi, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiMemMosi, PINMUX_PA09D_SERCOM2_PAD0);

	gpio_set_pin_level(iAlcSpiMemSck,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiMemSck, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiMemSck, PINMUX_PA08D_SERCOM2_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(iAlcSpiMemMiso, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAlcSpiMemMiso,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcSpiMemMiso, PINMUX_PA10D_SERCOM2_PAD2);
}

void Flash_Mem_SPI_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

void Flash_Mem_SPI_init(void)
{
	Flash_Mem_SPI_CLOCK_init();
	spi_m_sync_init(&Flash_Mem_SPI, SERCOM2);
	Flash_Mem_SPI_PORT_init();
}

void Shared_I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(iAlcSharedI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcSharedI2cScl, PINMUX_PA17D_SERCOM3_PAD0);

	gpio_set_pin_pull_mode(iAlcSharedI2cSda,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcSharedI2cSda, PINMUX_PA16D_SERCOM3_PAD1);
}

void Shared_I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM3_GCLK_ID_SLOW, CONF_GCLK_SERCOM3_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM3_bit(MCLK);
}

void Shared_I2C_init(void)
{
	Shared_I2C_CLOCK_init();
	i2c_m_sync_init(&Shared_I2C, SERCOM3);
	Shared_I2C_PORT_init();
}

void SPI_Temp_PORT_init(void)
{

	gpio_set_pin_level(iAlcSpiTempMosi,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiTempMosi, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiTempMosi, PINMUX_PB27D_SERCOM4_PAD0);

	gpio_set_pin_level(iAlcSpiTempSck,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiTempSck, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiTempSck, PINMUX_PB26D_SERCOM4_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(iAlcSpiTempMiso, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAlcSpiTempMiso,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcSpiTempMiso, PINMUX_PB28D_SERCOM4_PAD2);
}

void SPI_Temp_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM4_GCLK_ID_SLOW, CONF_GCLK_SERCOM4_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM4_bit(MCLK);
}

void SPI_Temp_init(void)
{
	SPI_Temp_CLOCK_init();
	spi_m_sync_init(&SPI_Temp, SERCOM4);
	SPI_Temp_PORT_init();
}

void Fvx_I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(iAlcFvI2cSda,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFvI2cSda, PINMUX_PA23D_SERCOM5_PAD0);

	gpio_set_pin_pull_mode(iAlcFvI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFvI2cScl, PINMUX_PA22D_SERCOM5_PAD1);
}

void Fvx_I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_CORE, CONF_GCLK_SERCOM5_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM5_GCLK_ID_SLOW, CONF_GCLK_SERCOM5_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM5_bit(MCLK);
}

void Fvx_I2C_init(void)
{
	Fvx_I2C_CLOCK_init();
	i2c_m_sync_init(&Fvx_I2C, SERCOM5);
	Fvx_I2C_PORT_init();
}

void SPI_AMMC_PORT_init(void)
{

	gpio_set_pin_level(iAlcSpiAMMCMosi,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiAMMCMosi, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiAMMCMosi, PINMUX_PC04C_SERCOM6_PAD0);

	gpio_set_pin_level(iAlcSpiAiAlcSpiAMMCScl,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcSpiAiAlcSpiAMMCScl, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcSpiAiAlcSpiAMMCScl, PINMUX_PC05C_SERCOM6_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(iAlcSpiAMMCMiso, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(iAlcSpiAMMCMiso,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcSpiAMMCMiso, PINMUX_PC07C_SERCOM6_PAD3);
}

void SPI_AMMC_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_CORE, CONF_GCLK_SERCOM6_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM6_GCLK_ID_SLOW, CONF_GCLK_SERCOM6_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM6_bit(MCLK);
}

void SPI_AMMC_init(void)
{
	SPI_AMMC_CLOCK_init();
	spi_m_sync_init(&SPI_AMMC, SERCOM6);
	SPI_AMMC_PORT_init();
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

	gpio_set_pin_function(iAlcFansI2cSda, PINMUX_PD08C_SERCOM7_PAD0);

	gpio_set_pin_pull_mode(iAlcFansI2cScl,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(iAlcFansI2cScl, PINMUX_PD09C_SERCOM7_PAD1);
}

void FANs_I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_CORE, CONF_GCLK_SERCOM7_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM7_GCLK_ID_SLOW, CONF_GCLK_SERCOM7_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBDMASK_SERCOM7_bit(MCLK);
}

void FANs_I2C_init(void)
{
	FANs_I2C_CLOCK_init();
	i2c_m_sync_init(&FANs_I2C, SERCOM7);
	FANs_I2C_PORT_init();
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

	// GPIO on PA27

	gpio_set_pin_level(niAlcTempSensSpiSs1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcTempSensSpiSs1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcTempSensSpiSs1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB02

	// Set pin direction to input
	gpio_set_pin_direction(niAnaInSpiSS, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(niAnaInSpiSS,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(niAnaInSpiSS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB07

	gpio_set_pin_level(niAlcTempSensSpiSs2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcTempSensSpiSs2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcTempSensSpiSs2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB14

	gpio_set_pin_level(TestPinOverHeat1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(TestPinOverHeat1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(TestPinOverHeat1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB15

	gpio_set_pin_level(TestPinOverHeat2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(TestPinOverHeat2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(TestPinOverHeat2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB16

	gpio_set_pin_level(SpareOutput1,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SpareOutput1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SpareOutput1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB17

	gpio_set_pin_level(SpareOutput2,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SpareOutput2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SpareOutput2, GPIO_PIN_FUNCTION_OFF);

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

	// GPIO on PB29

	gpio_set_pin_level(niAlcTempSensSpiSs0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcTempSensSpiSs0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcTempSensSpiSs0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC01

	gpio_set_pin_level(iAlcUcLifePulse,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcUcLifePulse, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcUcLifePulse, GPIO_PIN_FUNCTION_OFF);

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

	// GPIO on PC06

	gpio_set_pin_level(SpareOutput0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SpareOutput0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SpareOutput0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC13

	gpio_set_pin_level(iAlcCcuCanStdby,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(iAlcCcuCanStdby, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(iAlcCcuCanStdby, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC14

	gpio_set_pin_level(niAmmcSpiSs,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(niAmmcSpiSs, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAmmcSpiSs, GPIO_PIN_FUNCTION_OFF);

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

	// GPIO on PC30

	gpio_set_pin_level(niAlcSPIFlashCs,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcSPIFlashCs, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcSPIFlashCs, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PD00

	gpio_set_pin_level(SpareOutput3,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SpareOutput3, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SpareOutput3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PD11

	gpio_set_pin_level(niAlcSPIFramCs,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(niAlcSPIFramCs, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(niAlcSPIFramCs, GPIO_PIN_FUNCTION_OFF);

	EXTERNAL_IRQ_0_init();

	Flash_Mem_SPI_init();

	Shared_I2C_init();

	SPI_Temp_init();

	Fvx_I2C_init();

	SPI_AMMC_init();

	FANs_I2C_init();

	MaintUSB_init();
	CCu_CAN_init();
}
