/*
 * SPI_Asyn_Class.cpp
 *
 * Created: 2/5/2020 12:17:26 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
//#include "sam.h"
#include "SPI_Asyn_Class.h"
#define HI 1
#define LO 0
void HW_RESET(void);
static  uint8_t byte2;
int main(void)
{
	
		atmel_start_init();
		
		SPI_Asyn_Class	spi0(0);
		spi0.init();
		uint8_t array[4];
;		uint8_t	clock=0x04;
		uint8_t	wACLK=0x38;
		uint8_t	rACLK=0xD4;
		uint8_t	rTSR=0x80;
		uint8_t	*byte=&byte2;
		HW_RESET();
		spi0.enable();
		gpio_set_pin_level(SPI0_CS,	false);
		while (spi0.isBusy());
		spi0.write(&clock,1);
		while (!spi0.isSPIReady());
		gpio_set_pin_level(SPI0_CS,true);
		spi0.disable();
    /* Replace with your application code */
    while (1) 
    {
	
		spi0.enable();
		gpio_set_pin_level(SPI0_CS,	false);
		while (spi0.isBusy());
		spi0.write(&wACLK,1);
		while (!spi0.isSPIReady());
		spi0.write(&clock,1);
		while (!spi0.isSPIReady());
		gpio_set_pin_level(SPI0_CS,true);
		delay_ms(500);
		gpio_set_pin_level(SPI0_CS,	false);
		while (spi0.isBusy());
		spi0.write(&rACLK,1);

		while (!spi0.isSPIReady());
	
		for (int i=0;i<4;i++)
		{	
			while (!spi0.isSPIReady());
			spi0.read(byte,1);
		
			while (!spi0.isSPIReady());
			array[i]=*byte;
		}

		while (!spi0.isSPIReady());
	//	delay_us(300);
		gpio_set_pin_level(SPI0_CS,true);
		spi0.disable();
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