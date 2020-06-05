/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAME54 has 14 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8
#define GPIO_PIN_FUNCTION_J 9
#define GPIO_PIN_FUNCTION_K 10
#define GPIO_PIN_FUNCTION_L 11
#define GPIO_PIN_FUNCTION_M 12
#define GPIO_PIN_FUNCTION_N 13

#define XIN32 GPIO(GPIO_PORTA, 0)
#define XOUT32 GPIO(GPIO_PORTA, 1)
#define FlashMem_MOSI GPIO(GPIO_PORTA, 4)
#define FlashMem_SCK GPIO(GPIO_PORTA, 5)
#define FlashMem_MISO GPIO(GPIO_PORTA, 6)
#define iAclHeaterI2cScl GPIO(GPIO_PORTA, 8)
#define iAclHeaterI2cSda GPIO(GPIO_PORTA, 9)
#define iAlcMiscIncI2cScl GPIO(GPIO_PORTA, 12)
#define iAlcFMiscIncI2cSda GPIO(GPIO_PORTA, 13)
#define iAlcFvxI2cSda GPIO(GPIO_PORTA, 16)
#define iAlcFvxI2cScl GPIO(GPIO_PORTA, 17)
#define iAlcSpiMiso GPIO(GPIO_PORTA, 18)
#define iAclCcuI2cScl GPIO(GPIO_PORTA, 22)
#define iAclCcuI2cSda GPIO(GPIO_PORTA, 23)
#define iMaintUsbDm GPIO(GPIO_PORTA, 24)
#define iMaintUsbDp GPIO(GPIO_PORTA, 25)
#define iAclHeater1StatusChanged GPIO(GPIO_PORTB, 3)
#define niAlcFansStatusChanged GPIO(GPIO_PORTB, 4)
#define iAclCcuStatusChanged GPIO(GPIO_PORTB, 5)
#define niAlcMiscInStatusChanged GPIO(GPIO_PORTB, 6)
#define iAclHeater2StatusChanged GPIO(GPIO_PORTB, 7)
#define iAclFv1StatusChanged GPIO(GPIO_PORTB, 8)
#define iAclFv2StatusChanged GPIO(GPIO_PORTB, 9)
#define nAlcReset GPIO(GPIO_PORTB, 10)
#define iAlcCcuCanTxd GPIO(GPIO_PORTB, 12)
#define iAlcCcuCanRxd GPIO(GPIO_PORTB, 13)
#define XIN1 GPIO(GPIO_PORTB, 22)
#define XOUT GPIO(GPIO_PORTB, 23)
#define niAlcSPIFramCs GPIO(GPIO_PORTB, 27)
#define iAmmcInt1 GPIO(GPIO_PORTB, 28)
#define iAmmcInt2 GPIO(GPIO_PORTB, 29)
#define iMaintUsbVbus GPIO(GPIO_PORTC, 0)
#define iUcNoFailure GPIO(GPIO_PORTC, 2)
#define iAlcCcuCanStdby GPIO(GPIO_PORTC, 4)
#define SPI_Spare_CS GPIO(GPIO_PORTC, 11)
#define SPI_Spare_MOSI GPIO(GPIO_PORTC, 12)
#define SPI_Spare_SCK GPIO(GPIO_PORTC, 13)
#define iMaintUsbId GPIO(GPIO_PORTC, 19)
#define iAlcSpiSck GPIO(GPIO_PORTC, 22)
#define iAlcSpiMosi GPIO(GPIO_PORTC, 23)
#define niAmmSPIFlashMemCS GPIO(GPIO_PORTC, 24)
#define niAmmcSpiSs GPIO(GPIO_PORTC, 25)
#define niAlcTempSensSpiSs2 GPIO(GPIO_PORTC, 26)
#define niAlcTempSensSpiSs1 GPIO(GPIO_PORTC, 27)
#define niAlcTempSensSpiSs0 GPIO(GPIO_PORTC, 28)
#define iAlcFansI2cScl GPIO(GPIO_PORTD, 8)
#define iAlcFansI2cSda GPIO(GPIO_PORTD, 9)
#define SPI_Spare_MISO GPIO(GPIO_PORTD, 10)

#endif // ATMEL_START_PINS_H_INCLUDED
