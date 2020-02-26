/*
 * FlashMemoryClass.cpp
 *
 * Created: 2/5/2020 12:35:53 PM
 * Author : GMateusDP
 */ 


//#include "sam.h"
#include <atmel_start.h>
#include "FlashMemoryClass.h"
//static FlashMemoryClass	issi;
int main(void)
{
	FlashMemoryClass	issi;
	userParameters	p;
	p=defaultParameters;
	issi.init(&p);
    /* Replace with your application code */
    while (1) 
    {
    }
}
