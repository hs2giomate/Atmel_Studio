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
#define SpareInterrupt0 GPIO(GPIO_PORTA, 3)
#define iAclFv1StatusChanged GPIO(GPIO_PORTA, 4)
#define iAclFv2StatusChanged GPIO(GPIO_PORTA, 5)
#define iAlcSpiMemSck GPIO(GPIO_PORTA, 8)
#define iAlcSpiMemMosi GPIO(GPIO_PORTA, 9)
#define iAlcSpiMemMiso GPIO(GPIO_PORTA, 10)
#define iAclSharedI2CChanged GPIO(GPIO_PORTA, 13)
#define niAlcFansStatusChanged GPIO(GPIO_PORTA, 14)
#define iAlcSharedI2cSda GPIO(GPIO_PORTA, 16)
#define iAlcSharedI2cScl GPIO(GPIO_PORTA, 17)
#define iAlcFvI2cScl GPIO(GPIO_PORTA, 22)
#define iAlcFvI2cSda GPIO(GPIO_PORTA, 23)
#define iMaintUsbDm GPIO(GPIO_PORTA, 24)
#define iMaintUsbDp GPIO(GPIO_PORTA, 25)
#define niAlcTempSensSpiSs1 GPIO(GPIO_PORTA, 27)
#define iAlcTempSensInt3 GPIO(GPIO_PORTB, 1)
#define niAnaInSpiSS GPIO(GPIO_PORTB, 2)
#define niAlcTempSensSpiSs2 GPIO(GPIO_PORTB, 7)
#define iAlcTempSensInt1 GPIO(GPIO_PORTB, 8)
#define iAlcTempSensInt2 GPIO(GPIO_PORTB, 9)
#define iAlcCcuCanTxd GPIO(GPIO_PORTB, 12)
#define iAlcCcuCanRxd GPIO(GPIO_PORTB, 13)
#define TestPinOverHeat1 GPIO(GPIO_PORTB, 14)
#define TestPinOverHeat2 GPIO(GPIO_PORTB, 15)
#define SpareOutput1 GPIO(GPIO_PORTB, 16)
#define SpareOutput2 GPIO(GPIO_PORTB, 17)
#define XIN1 GPIO(GPIO_PORTB, 22)
#define XOUT GPIO(GPIO_PORTB, 23)
#define iAlcSpiTempSck GPIO(GPIO_PORTB, 26)
#define iAlcSpiTempMosi GPIO(GPIO_PORTB, 27)
#define iAlcSpiTempMiso GPIO(GPIO_PORTB, 28)
#define niAlcTempSensSpiSs0 GPIO(GPIO_PORTB, 29)
#define iMaintUsbVbus GPIO(GPIO_PORTC, 0)
#define iAlcUcLifePulse GPIO(GPIO_PORTC, 1)
#define iUcNoFailure GPIO(GPIO_PORTC, 2)
#define iAlcSpiAMMCMosi GPIO(GPIO_PORTC, 4)
#define iAlcSpiAiAlcSpiAMMCScl GPIO(GPIO_PORTC, 5)
#define SpareOutput0 GPIO(GPIO_PORTC, 6)
#define iAlcSpiAMMCMiso GPIO(GPIO_PORTC, 7)
#define SpareInterrupt1 GPIO(GPIO_PORTC, 10)
#define SpareInterrupt2 GPIO(GPIO_PORTC, 11)
#define SpareInterrupt3 GPIO(GPIO_PORTC, 12)
#define iAlcCcuCanStdby GPIO(GPIO_PORTC, 13)
#define niAmmcSpiSs GPIO(GPIO_PORTC, 14)
#define SW0 GPIO(GPIO_PORTC, 18)
#define iMaintUsbId GPIO(GPIO_PORTC, 19)
#define iAmmcInt1 GPIO(GPIO_PORTC, 22)
#define iAmmcInt2 GPIO(GPIO_PORTC, 23)
#define niAlcSPIFlashCs GPIO(GPIO_PORTC, 30)
#define nAlcReset GPIO(GPIO_PORTC, 31)
#define SpareOutput3 GPIO(GPIO_PORTD, 0)
#define iAlcFansI2cSda GPIO(GPIO_PORTD, 8)
#define iAlcFansI2cScl GPIO(GPIO_PORTD, 9)
#define niAlcSPIFramCs GPIO(GPIO_PORTD, 11)

#endif // ATMEL_START_PINS_H_INCLUDED
