/* 
* MonoDisplay_Class.h
*
* Created: 4/16/2020 1:30:22 PM
* Author: GMateusDP
*/


#ifndef __MONODISPLAY_CLASS_H__
#define __MONODISPLAY_CLASS_H__
#include "hal_gpio.h"
#include "hal_spi_m_sync.h"
#include "gfx_mono_main.h"
#include "gfx_mono_font_basic_6x7.h"
#include "driver_init.h"
#include "string.h"
#include "hal_calendar.h"
class MonoDisplay_Class
{
//variables
public:
	calendar_time Uhr;
	spi_m_sync_descriptor spi;
	gfx_mono_text			textDescriptor;
	font					textFont;
	gfx_mono				descriptor;
protected:
private:

//functions
public:
	MonoDisplay_Class();
	~MonoDisplay_Class();
	void init(void);
	void drawText(char *);
	void	makeFrame(void);
protected:
private:
	MonoDisplay_Class( const MonoDisplay_Class &c );
	MonoDisplay_Class& operator=( const MonoDisplay_Class &c );

}; //MonoDisplay_Class

#endif //__MONODISPLAY_CLASS_H__
