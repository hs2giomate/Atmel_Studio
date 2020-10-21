/*
 * ByteGenerator.cpp
 *
 * Created: 7/23/2020 5:45:42 PM
 * Author : GMateusDP
 */ 


#include <atmel_start.h>
#include "CDC_Class.h"
#include "GPIOConnector_Class.h"
static	GPIOConnector_Class	gpio;
uint8_t	processValue=0;
float	processFloat=0,outputFloat=0,integralError=0;
uint8_t	SimpleProportionalController(uint8_t sp);

int main(void)
{
	char cmd;
	uint8_t	byte;
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
    /* Replace with your application code */
	serial.Init();
	gpio.Init();
	//	serial.write(test,sizeof(test));
	delay_ms(200);
	serial.println("*** USB Interface Test ***");
    while (1) 
    {
		serial<<"*** Date:  "<<__DATE__<<" Time: "<<__TIME__<<" ***"<<NEWLINE;
		byte=gpio.ReadByteOnEXT2();
		serial<<"Input Value on EXT2: ";
		serial.println(byte,HEX);
			
// 		byte=gpio.RequestByte();
// 		if (byte>0)
// 		{
// 			processValue=byte;
// 			gpio.PutByteOnEXT2(processValue);
// 			
// 		} 
// 		else
// 		{
			while(1){
				byte=gpio.ReadByteOnEXT2();
				serial<<"SetpointPosition: ";
				serial.print(byte,HEX);
				serial<<"\tActualPosition: ";
				serial.println(SimpleProportionalController(byte),HEX);
				gpio.PutByteOnEXT2(processValue);
				asm("nop");
				
			}
			
			
		//}
		delay_ms(500);
	
    }
}

uint8_t	SimpleProportionalController(uint8_t sp){
	//int proportionalError=(sp-processValue);
	float errorFloat=float(sp)-processFloat;
	integralError=errorFloat+integralError;
	errorFloat=2*errorFloat+integralError/32;
	float valueFloat  =((errorFloat)/(64));
	outputFloat=outputFloat+valueFloat;
	if (outputFloat<0)
	{
		processValue=0;
		processFloat=0;
		
	} 
	else
	{
		if (outputFloat>255)
		{
			processValue=0xff;
			processFloat=255;
		} 
		else
		{
			processValue=(uint8_t)outputFloat;
			processFloat=outputFloat;
		}
	}
	delay_ms(100);
	
	return processValue;
	
}
