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

#define PA04 GPIO(GPIO_PORTA, 4)
#define PA05 GPIO(GPIO_PORTA, 5)
#define PA06 GPIO(GPIO_PORTA, 6)
#define LTC_CS GPIO(GPIO_PORTA, 7)
#define PA16 GPIO(GPIO_PORTA, 16)
#define PA17 GPIO(GPIO_PORTA, 17)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define PB12 GPIO(GPIO_PORTB, 12)
#define PB13 GPIO(GPIO_PORTB, 13)
#define SPI1_CS GPIO(GPIO_PORTB, 28)
#define MR GPIO(GPIO_PORTB, 29)
#define OLED_BUTTON2 GPIO(GPIO_PORTC, 2)
#define OLED_BUTTON3 GPIO(GPIO_PORTC, 3)
#define PC04 GPIO(GPIO_PORTC, 4)
#define PC05 GPIO(GPIO_PORTC, 5)
#define PC06 GPIO(GPIO_PORTC, 6)
#define CAN_STBY GPIO(GPIO_PORTC, 13)
#define tick_RTC GPIO(GPIO_PORTC, 15)
#define LED0 GPIO(GPIO_PORTC, 18)
#define OLED_BUTTON1 GPIO(GPIO_PORTC, 30)
#define OLED_LED1 GPIO(GPIO_PORTD, 10)
#define OLED_LED3 GPIO(GPIO_PORTD, 11)

#endif // ATMEL_START_PINS_H_INCLUDED
