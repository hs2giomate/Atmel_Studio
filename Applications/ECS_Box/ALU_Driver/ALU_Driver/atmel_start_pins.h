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

#define I2CFV1CHANGED GPIO(GPIO_PORTA, 4)
#define I2CFV2CHANGED GPIO(GPIO_PORTA, 5)
#define PA06 GPIO(GPIO_PORTA, 6)
#define FLASH_IO0 GPIO(GPIO_PORTA, 8)
#define FLASH_IO1 GPIO(GPIO_PORTA, 9)
#define FLASH_IO2 GPIO(GPIO_PORTA, 10)
#define FLASH_IO3 GPIO(GPIO_PORTA, 11)
#define CS_FLASH GPIO(GPIO_PORTA, 15)
#define PA16 GPIO(GPIO_PORTA, 16)
#define PA17 GPIO(GPIO_PORTA, 17)
#define PA18 GPIO(GPIO_PORTA, 18)
#define CS_FRAM GPIO(GPIO_PORTA, 19)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define INT_LTC2983 GPIO(GPIO_PORTB, 7)
#define FLASH_CLK GPIO(GPIO_PORTB, 10)
#define FLASH_CS GPIO(GPIO_PORTB, 11)
#define PB12 GPIO(GPIO_PORTB, 12)
#define PB13 GPIO(GPIO_PORTB, 13)
#define PB26 GPIO(GPIO_PORTB, 26)
#define PB27 GPIO(GPIO_PORTB, 27)
#define PB28 GPIO(GPIO_PORTB, 28)
#define CS_SPI_LTC GPIO(GPIO_PORTB, 29)
#define SW0 GPIO(GPIO_PORTB, 31)
#define VBUS_DETECTED GPIO(GPIO_PORTC, 0)
#define CAN_STDBY GPIO(GPIO_PORTC, 13)
#define CS_Holt GPIO(GPIO_PORTC, 14)
#define Ethernet_Led GPIO(GPIO_PORTC, 15)
#define PC16 GPIO(GPIO_PORTC, 16)
#define PC17 GPIO(GPIO_PORTC, 17)
#define LED0 GPIO(GPIO_PORTC, 18)
#define ARINCR2Int GPIO(GPIO_PORTC, 22)
#define ARINCR1Int GPIO(GPIO_PORTC, 23)
#define PD08 GPIO(GPIO_PORTD, 8)
#define PD09 GPIO(GPIO_PORTD, 9)

#endif // ATMEL_START_PINS_H_INCLUDED
