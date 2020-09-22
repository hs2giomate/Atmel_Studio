/* 
* SerialTerminal_Class.cpp
*
* Created: 7/23/2020 6:43:18 PM
* Author: GMateusDP
*/


#include "SerialTerminal_Class.h"
#include "TimerSerial_Class.h"
#include "CDC_Class.h"

SerialTerminal_Class	*ptrSerialTerminalClass;

static void	USBTimeoutTask(const struct timer_task *const timer_task){
	ptrSerialTerminalClass->timeout=true;
	ptrSerialTerminalClass->terminalStarted=false;
}

// default constructor
SerialTerminal_Class::SerialTerminal_Class()
{
	ptrSerialTerminalClass=this;
} //SerialTerminal_Class

// default destructor
SerialTerminal_Class::~SerialTerminal_Class()
{
} //~SerialTerminal_Class

void	SerialTerminal_Class::OnInit(void){
		//vbusDetected=gpio_get_pin_level(VBUS_DETECTED);
		if (vbusDetected)
		{
			timeout=false;
			terminalStarted=true;
			usbTerminalTimer.Add_oneShot_task((FUNC_PTR)USBTimeoutTask,USB_TIMEOUT*60);
			usbTerminalTimer.Start();
			while (!usb.IsEnabled()){
				if (timeout)
				{
					plugged=false;
					return;
				}
				else
				{
				}
			}
			
			usbTerminalTimer.Stop();
		} 
		else
		{
			plugged=false;
			terminalStarted=false;
		}
		
}
void	SerialTerminal_Class::OnPlugged(void){
	usbTerminalTimer.Stop();
	usbTerminalTimer.Remove_task();
	usbTerminalTimer.Add_oneShot_task((FUNC_PTR)USBTimeoutTask,USB_TIMEOUT*40);
	usbTerminalTimer.Start();
	while (!plugged){
		if (timeout)
		{
			plugged=false;
			terminalStarted=false;
			return;
		}
		else
		{
		}
	}
	usbTerminalTimer.Stop();
	
}
bool	SerialTerminal_Class::CheckTerminal(void){
	if (terminalStarted)
	{
		if (plugged||usb.IsEnabled())
		{
			terminalStarted=usbTerminalTimer.Start()==0;
		}
		else
		{
			terminalStarted=false;
		}
	}
	else
	{
		return	terminalStarted;
	}
	
	return	terminalStarted;
}
bool	SerialTerminal_Class::CheckTerminal(uint32_t size){
	if (terminalStarted)
	{
		if (plugged||usb.IsEnabled())
		{
			usbTerminalTimer.Stop();
			usbTerminalTimer.Remove_task();
			usbTerminalTimer.Add_oneShot_task((FUNC_PTR)USBTimeoutTask,USB_TIMEOUT*size);
			terminalStarted=usbTerminalTimer.Start()==0;
		}
		else
		{
			terminalStarted=false;
		}
	}
	else
	{
		return	terminalStarted;
	}
	
	return	terminalStarted;
}