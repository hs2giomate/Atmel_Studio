/*
 * CAN_Asyn_Class.cpp
 *
 * Created: 5/11/2020 9:16:00 AM
 * Author : GMateusDP
 */ 

#include <atmel_start.h>
#include "CDC_Class.h"
#include "CAN_Asyn_Class.h"

CAN_Asyn_Class	can(&CAN_CCU);

static void display_menu(void)
{
	usb.print("Menu :\r\n");
	usb<<"  -- Select the action:\r\n"

	<<"  2: Send FD standard message with ID: 0x45A and 64 byte data 0 to 63. \r\n"
	<<"  3: Send FD standard message with ID: 0x469 and 64 byte data 128 to 191. \r\n"
	<<"  a: Send normal standard message with ID: 0x469 and 8 byte data 0 to 7. \r\n"
	<<"  h: Display menu \r\n\r\n";
}

static	bool	PrintRxCallBack(void){
	usb.print("\n\r CAN Message received . The received data is: \r\n");
	for (uint8_t i = 0; i < can.receivedMsg.len; i++) {
		usb.print(can.rx_buffer_64[i],DEC);
		usb<<" ";
	}
	usb.print("\r\n\r\n");
	return can.rxCallBack;
}

int main(void)
{
	char key;

	

	uint8_t tx_message_0[64];
	uint8_t tx_message_1[64];
	uint8_t tx_message_2[8];

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	usb.Init();
	delay_ms(200);
	can.Init();
	display_menu();

	/* Initialize the data to be used later */
	for (uint8_t i = 0; i < 64; i++) {
		tx_message_0[i] = i;
	}

	for (uint8_t j = 128; j < 192; j++) {
		tx_message_1[j - 128] = j;
	}

	for (uint8_t k = 0; k < 8; k++) {
		tx_message_2[k] = k;
	}

	while (1) {
		if (can.rxCallBack)
		{
			PrintRxCallBack();
			can.rxCallBack=false;
		} 
		else if (can.txCallBack)
		{
			usb.print("  CAN Transmission done \r\n");
			can.txCallBack=false;
		}
		else if (can.txSTDCallBack)
		{
			usb.print("  CAN Standard Transmission done \r\n");
			can.txSTDCallBack=false;
		}
		else
		{
			key=0xff;
			usb.read(&key);

			switch (key) {
			case 'h':
				display_menu();
				break;

			case '2':
				usb.print("  0: Set standard filter ID 0: 0x45A, store into Rx buffer. \r\n");
				can.Write(tx_message_0,0x45a,8);
				break;

			case '3':
				can.Write(tx_message_1,0x469,8);
				break;

			case 'a':
				can.WriteStandard(tx_message_2,0x469,8);
				break;

			default:
				break;
			}
		}

	}
}
