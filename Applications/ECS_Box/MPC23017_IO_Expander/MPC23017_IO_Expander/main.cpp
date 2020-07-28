/*
 * MPC23017_IO_Expander.cpp
 *
 * Created: 7/27/2020 1:01:49 PM
 * Author : GMateusDP
 */ 

#include "atmel_start.h"
#include "MCP23017_Class.h"

MCP23017_Class	mcp(&I2C_EXPANDER);
int main(void)
{
	atmel_start_init();
	mcp.Init();
	mcp.SetPortAInput();
	mcp.SetPortBOutput();

    /* Replace with your application code */
    while (1) 
    {
		for (int i = 0; i < 8; i++)
		{
			mcp.digitalWrite(8+i,mcp.digitalRead(i));
		}
		
    }
}
