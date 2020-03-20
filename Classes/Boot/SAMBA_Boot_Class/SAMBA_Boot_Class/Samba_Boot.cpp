/* 
* Samba_Boot.cpp
*
* Created: 3/2/2020 12:43:28 PM
* Author: GMateusDP
*/


#include "Samba_Boot.h"
#include "usart_sam_ba.h"
#include "DeviceClass.h"
//#include "DeviceDefinitions.h"
#include "uart_driver.h"
#include "sam_ba_monitor.h"


//#include "boot_device_configuration.h"

Samba_Boot		Booter;



// default constructor
Samba_Boot::Samba_Boot()
{
	Boot_ptr=this;

} //Samba_Boot

// default destructor
Samba_Boot::~Samba_Boot()
{
} //~Samba_Boot

void Samba_Boot::check_Boot_Key(void){
	int	character=usart_getc();
	
	if((device.state.currentState == kDeviceStateStandbyOFF)){
		
		if ((character== (int) SHARP_CHARACTER)&&(!device.state.bootQuery) ){
			device.state.bootQuery=1;
			uart_inter_disable(BOOT_USART_MODULE);
			
		}else{
			
			
			device.state.bootQuery=0;
			//Door.state.HWCuttOffkey=0;
		}
	}
}

void Samba_Boot::sendSharp(void){
	usart_putc('*');
}

void Samba_Boot::start_booting(void){
	process_booting_request();
}
void SERCOM5_Handler(void){
	Booter.check_Boot_Key();
}