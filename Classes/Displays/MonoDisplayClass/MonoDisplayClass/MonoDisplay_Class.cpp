/* 
* MonoDisplay_Class.cpp
*
* Created: 4/16/2020 1:30:22 PM
* Author: GMateusDP
*/


#include "MonoDisplay_Class.h"



// default constructor
MonoDisplay_Class::MonoDisplay_Class()
{
	spi=SPI_DISPLAY;
	textDescriptor=MONOCHROME_TEXT_0_desc;
	textFont=basic_6x7;
	descriptor=PRIMITIVES_INSTANCE_desc;
} //MonoDisplay_Class

// default destructor
MonoDisplay_Class::~MonoDisplay_Class()
{
} //~MonoDisplay_Class

 void MonoDisplay_Class::init(){
	 spi_m_sync_enable(&SPI_DISPLAY);
	 gfx_mono_init();
	 
	 
 }
 void MonoDisplay_Class::drawText(char * text){
	 uint8_t	t[24];
	 char *p=text;
	 uint8_t	n=strlen(text);
	for (int i=0;i<n;i++)
	{
		t[i]=*p;
		p++;
	}
	  gfx_mono_text_draw_string(&MONOCHROME_TEXT_0_desc,&t[0],4,11,&basic_6x7);
 }
  void MonoDisplay_Class::makeFrame(void){
	  
	 gfx_mono_draw_rect(&PRIMITIVES_INSTANCE_desc, 0, 0, UG2832HSWEG04_LCD_WIDTH, UG2832HSWEG04_LCD_HEIGHT, GFX_PIXEL_SET);
  }