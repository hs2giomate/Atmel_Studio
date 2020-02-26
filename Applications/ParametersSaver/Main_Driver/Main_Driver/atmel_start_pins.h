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

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define Read_Fail GPIO(GPIO_PORTA, 2)
#define Read_OK GPIO(GPIO_PORTA, 3)
#define Write_OK GPIO(GPIO_PORTA, 4)
#define pa08 GPIO(GPIO_PORTA, 8)
#define PA09 GPIO(GPIO_PORTA, 9)
#define PA10 GPIO(GPIO_PORTA, 10)
#define PA11 GPIO(GPIO_PORTA, 11)
#define PA12 GPIO(GPIO_PORTA, 12)
#define PA13 GPIO(GPIO_PORTA, 13)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define Flash_CS GPIO(GPIO_PORTA, 27)
#define FRAM_CS GPIO(GPIO_PORTA, 28)
#define pa30 GPIO(GPIO_PORTA, 30)
#define pa31 GPIO(GPIO_PORTA, 31)
#define Write_Fail GPIO(GPIO_PORTB, 9)
#define pb31 GPIO(GPIO_PORTB, 31)

#endif // ATMEL_START_PINS_H_INCLUDED
