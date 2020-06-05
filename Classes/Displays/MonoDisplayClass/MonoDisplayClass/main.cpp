/*
 * MonoDisplayClass.cpp
 *
 * Created: 4/16/2020 1:29:41 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>

/*
#include <hal_gpio.h>
#include "gfx_mono_main.h"
#include "gfx_mono_font_basic_6x7.h"
*/
//#include "MonoDisplay_Class.h"
////#include "hal_calendar.h"
#include "stdlib.h"
//static MonoDisplay_Class	display;
#include "driver_init.h"
#include "utils.h"
static uint8_t example_SPI_DISPLAY[] = "Hello World!";
int main(void)
{	
		 atmel_start_init();
	//	 uint32_t counter=0xffffffff;
	//	 char *c;
	/*
		char *hh,*mm,*ss;
		
		 char tiempo[]="00:00:00";
		char	pp=':';
		char	*temp=tiempo;

		 char	*hora=__TIME__;
		char	h[]="00";
		//hh=h;mm=h;ss=h;
		atmel_start_init();
		
	calendar_date_time	reloj;
	calendar_time		uhr;
		
	//calendar_get_date_time(&CALENDAR_0,&reloj);
	h[0]=*hora;hora++;
	h[1]=*hora;hora++;
	uhr.hour=atoi(&h[0]); hora++;
		h[0]=*hora;hora++;
		h[1]=*hora;hora++;
		uhr.min=atoi(&h[0]);hora++;
		h[0]=*hora;hora++;
		h[1]=*hora;hora++;
		uhr.sec=atoi(&h[0]);hora++;
	
	calendar_get_date_time(&CALENDAR_0,&reloj);
	reloj.time=uhr;
	calendar_set_time(&CALENDAR_0,&reloj.time);
	calendar_enable(&CALENDAR_0);
	*/
	//display.init();
	//display.makeFrame();
     /* Replace with your application code */
	 
	// const uint8_t str[] = "Hello, world!";

/*

	 spi_m_sync_enable(&SPI_0);
	 gfx_mono_init();
	 gfx_mono_draw_rect(
	 &MONOCHROME_GRAPHICS_0_desc, 0, 0, UG2832HSWEG04_LCD_WIDTH, UG2832HSWEG04_LCD_HEIGHT, GFX_PIXEL_SET);
	 gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc, str, 30, 12, &basic_6x7);
	 */
    while (1) 
    {
		
		struct io_descriptor *io;
		spi_m_sync_get_io_descriptor(&SPI_DISPLAY, &io);

		spi_m_sync_enable(&SPI_DISPLAY);
		io_write(io, example_SPI_DISPLAY, 12);
		/*
		hh=h;
		itoa(reloj.time.hour,hh,10);
		if (reloj.time.hour<10)
		{
			tiempo[0]=' ';
			tiempo[1]=*hh;
		} 
		else
		{
			tiempo[0]=*hh;hh++;
			tiempo[1]=*hh;
		}

		mm=h;
		itoa(reloj.time.min,mm,10);
		if (reloj.time.min<10)
		{
				tiempo[3]=' ';
				tiempo[4]=*mm;
		} 
		else
		{
				tiempo[3]=*mm;mm++;
				tiempo[4]=*mm;
		}
		
	
		ss=h;
		itoa(reloj.time.sec,ss,10);
		if (reloj.time.sec<10)
		{
			tiempo[6]=' ';
			tiempo[7]=*ss;
		} 
		else
		{
			tiempo[6]=*ss;ss++;
			tiempo[7]=*ss;
		}
				*/
		/*
		itoa(counter,c,10);
		display.drawText(c);
		counter--;
		//delay_ms(1000);
			//display.makeFrame();
			delay_ms(1000);
			//calendar_get_date_time(&CALENDAR_0,&reloj);
			//gpio_toggle_pin_level(OLED_LED1);
		*/	
    }
	
	
}
