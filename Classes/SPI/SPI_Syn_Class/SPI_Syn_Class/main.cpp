/*
 * SPI_Syn_Class.cpp
 *
 * Created: 4/6/2020 10:21:44 AM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>

#include "SPI_Syn_Class.h"
#define HI 1
#define LO 0
void HW_RESET(void);

static  uint8_t byte2;
int main(void)
{
   atmel_start_init();
   
   SPI_Syn_Class	spi1(&SPI_Holt);
   spi1.init();
   uint8_t array[4];
   ;		uint8_t	clock=0x04;
   uint8_t	wACLK=0x38;
   uint8_t	rACLK=0xD4;
   uint8_t	rTSR=0x80;
   uint8_t	*byte=&byte2;
   HW_RESET();
   spi1.enable();
   gpio_set_pin_level(SPI1_CS,	false);
   while (!spi1.isSPIReady());
   spi1.write(&clock,1);
   while (!spi1.isSPIReady());
   gpio_set_pin_level(SPI1_CS,true);
   spi1.disable();
   /* Replace with your application code */
   while (1)
   {
	   
	   spi1.enable();
	   gpio_set_pin_level(SPI1_CS,	false);
	   while (!spi1.isSPIReady());
	   spi1.write(&wACLK,1);
	   while (!spi1.isSPIReady());
	   spi1.write(&clock,1);
	   while (!spi1.isSPIReady());
	   gpio_set_pin_level(SPI1_CS,true);
	   delay_ms(1000);
	   gpio_set_pin_level(SPI1_CS,	false);
	    while (!spi1.isSPIReady());
	   spi1.write(&rACLK,1);

	   while (!spi1.isSPIReady());
	   spi1.read(byte,1);


	   while (!spi1.isSPIReady());
	   //	delay_us(300);
	   gpio_set_pin_level(SPI1_CS,true);
	   spi1.disable();
	   if (byte&&clock){
		   gpio_set_pin_level(LED0,false);
		   }else{
		   gpio_set_pin_level(LED0,true);
	   }
	   delay_ms(1000);
	   gpio_set_pin_level(LED0,true);
   }
}
void HW_RESET(void){
	
	
	gpio_set_pin_level(MR,true);
	delay_us(2);
	gpio_set_pin_level(MR,false);
}