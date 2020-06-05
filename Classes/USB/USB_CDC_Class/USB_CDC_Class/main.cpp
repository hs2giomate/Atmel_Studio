/*
 * USB_CDC_Class.cpp
 *
 * Created: 2/24/2020 1:15:45 PM
 * Author : GMateusDP
 */ 
#include "sam.h"
#include <atmel_start.h>
#include "USB_CDC_Class.h"

#include "string.h"
#include "stdlib.h"

USB_CDC_Class	usb0;
void display_menu(void);

static uint8_t p[]=" ---Usb port Test ----\n\r";
static uint8_t q[]=",123,456,678 \n\r";
int main(void)
{
	atmel_start_init();

	usb0.init();
	uint8_t *ptr;
	uint8_t  r;
	uint8_t	n,m,l,l2,ind,ind2;

	const char	*test ="123";
	uint8_t	text0[]="Values,";
	uint8_t	text1[]=",";
	uint8_t	text2[]="123\n\r";
	char	array[16];
	uint8_t	palabra[128],finalText[128];
	uint32_t	counter=1;
	delay_ms(100);
	display_menu();
	usb0.write(&p[0],sizeof(p));
		delay_ms(200);
	ind=0;
    /* Replace with your application code */
    while (1) 
    {
		ind=0;
		
		l=sizeof(text0);
		for (int k=0;k<l;k++)
		{

			palabra[k]=text0[k];
		}
		ind=l;
		
		for (int ij=0;ij<4;ij++)
		{
			
			char *c=utoa(counter,array,10);
						
			//usb0.write((uint8_t *)t,10);
			n=sizeof(c);
			ptr=(uint8_t*)c;
			for (int i=ind;i<n+ind;i++)
			{
				palabra[i]=*ptr;
				ptr++;
				
			}
			ind=ind+n;
			m=sizeof(text1);
			for (int j=ind;j<m+ind;j++)
			{
				palabra[j]=text1[j-ind];
			}
			ind=ind+m;
			
		
			
			//usb0.write(&text1[0],sizeof(text1));
			counter++;
		}
		
		l2=sizeof(text2);
		for (int kk=ind;kk<l2+ind;kk++)
		{
			palabra[kk]=text2[kk-ind];
		}
		ind=ind+l2;
		ind2=0;
		for (int i=0;i<ind;i++)
		{
			if (palabra[i]>9)
			{
					finalText[ind2]=palabra[i];
					ind2++;			
			}
		}
		usb0.write(&finalText[0],ind2);
		delay_ms(500);
		//usb0.write(&q[0],sizeof(q));
		//delay_ms(500);
		usb0.read(&r,1);
		delay_ms(200);
		
		
		if (r>33)
		{
		//	text1[0]=r;
		//	usb0.write(&text1[0],sizeof(text1));
			gpio_toggle_pin_level(LED0);
			delay_ms(100);
			r=0x00;
		}
		
		
		delay_ms(500);
    }
}

 void display_menu(void)
 {
	 usb0.xprint("Menu :\r\n");
	usb0.xprint( "  -- Select the action:\r\n");
	usb0.xprint( "  0: Set standard filter ID 0: 0x45A, store into Rx buffer. \r\n");
	usb0.xprint( "  1: Set standard filter ID 1: 0x469, store into Rx FIFO 0. \r\n");
	usb0.xprint( "  2: Send FD standard message with ID: 0x45A and 64 byte data 0 to 63. \r\n");
	usb0.xprint( "  3: Send FD standard message with ID: 0x469 and 64 byte data 128 to 191. \r\n");
	usb0.xprint( "  4: Set extended filter ID 0: 0x100000A5, store into Rx buffer. \r\n");
	usb0.xprint( "  5: Set extended filter ID 1: 0x10000096, store into Rx FIFO 1. \r\n");
	usb0.xprint( "  6: Send FD extended message with ID: 0x100000A5 and 64 byte data 0 to 63. \r\n");
	usb0.xprint( "  7: Send FD extended message with ID: 0x10000096 and 64 byte data 128 to 191. \r\n");
	usb0.xprint( "  a: Send normal standard message with ID: 0x469 and 8 byte data 0 to 7. \r\n");
	usb0.xprint( "  h: Display menu \r\n\r\n");
 }