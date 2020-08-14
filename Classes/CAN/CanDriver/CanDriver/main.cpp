/*
 * CanDriver.cpp
 *
 * Created: 8/3/2020 3:05:02 PM
 * Author : GMateusDP
 */ 

#include <atmel_start.h>
#include "CDC_Class.h"
bool	rxCallBack=false;
bool	txCallBack=false;
bool	txSTDCallBack=false;
static struct can_message lastMsg;
uint8_t            lastData[64];

/**
 * \brief display configuration menu.
 */
static void display_menu(void)
{
	usb.print("Menu :\r\n");
	usb<<"  -- Select the action:\r\n"
	       <<"  0: Set standard filter ID 0: 0x45A, store into Rx buffer. \r\n"
	       <<"  1: Set standard filter ID 1: 0x469, store into Rx FIFO 0. \r\n"
	       <<"  2: Send FD standard message with ID: 0x45A and 64 byte data 0 to 63. \r\n"
	       <<"  3: Send FD standard message with ID: 0x469 and 64 byte data 128 to 191. \r\n"
	       <<"  a: Send normal standard message with ID: 0x469 and 8 byte data 0 to 7. \r\n"
	       <<"  h: Display menu \r\n\r\n";
}

static void CAN_CCU_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
	txCallBack=true;
	
}

static void CAN_std_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;

	/* Enable the FDOE and BRSE in register configuration*/
	hri_can_set_CCCR_INIT_bit(CAN_CCU.dev.hw);
	while (hri_can_get_CCCR_INIT_bit(CAN_CCU.dev.hw) == 0)
		;
	hri_can_set_CCCR_CCE_bit(CAN_CCU.dev.hw);

	hri_can_set_CCCR_FDOE_bit(CAN_CCU.dev.hw);
	hri_can_set_CCCR_BRSE_bit(CAN_CCU.dev.hw);
	txSTDCallBack=true;
	
}

static void CAN_CCU_rx_callback(struct can_async_descriptor *const descr)
{
	
	
	lastMsg.data = lastData;
	can_async_read(descr, &lastMsg);
	rxCallBack=true;

	return;
}
bool	PrintRxCallBack(void){
		usb.print("\n\r CAN Message received . The received data is: \r\n");
		for (uint8_t i = 0; i < lastMsg.len; i++) {
			usb.print(lastData[i],DEC);
			usb<<" ";
		}
		usb.print("\r\n\r\n");
	
		return rxCallBack;
}

int main(void)
{
	char key;

	struct can_message msg;
	struct can_filter  filter;

	uint8_t tx_message_0[64];
	uint8_t tx_message_1[64];
	uint8_t tx_message_2[8];

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	usb.Init();
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
		if (rxCallBack)
		{
			PrintRxCallBack();
			rxCallBack=false;
		} 
		else if (txCallBack)
		{
			usb.print("  CAN Transmission done \r\n");
			txCallBack=false;
		}
		else if (txSTDCallBack)
		{
			usb.print("  CAN Standard Transmission done \r\n");
			txSTDCallBack=false;
		}
		else
		{
			key=0xff;
			usb.read(&key);

			switch (key) {
			case 'h':
				display_menu();
				break;

			case '0':
				usb.print("  0: Set standard filter ID 0: 0x45A, store into Rx buffer. \r\n");
				can_async_register_callback(&CAN_CCU, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_CCU_rx_callback);
				filter.id   = 0x45A;
				filter.mask = 0;
				can_async_set_filter(&CAN_CCU, 0, CAN_FMT_STDID, &filter);
				break;

			case '1':
				usb.print("  1: Set standard filter ID 1: 0x469, store into Rx FIFO 0. \r\n");
				can_async_register_callback(&CAN_CCU, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_CCU_rx_callback);
				filter.id   = 0x469;
				filter.mask = 0;
				can_async_set_filter(&CAN_CCU, 1, CAN_FMT_STDID, &filter);
				break;

			case '2':
				usb.print("  2: Send standard message with ID: 0x45A and 8 byte data 0 to 63. \r\n");
				msg.id   = 0x45A;
				msg.type = CAN_TYPE_DATA;
				msg.data = tx_message_0;
				msg.len  = 8;
				msg.fmt  = CAN_FMT_STDID;

				can_async_register_callback(&CAN_CCU, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_CCU_tx_callback);
				can_async_enable(&CAN_CCU);
				can_async_write(&CAN_CCU, &msg);
				break;

			case '3':
				usb.print("  3: Send standard message with ID: 0x469 and 8 byte data 128 to 191. \r\n");
				msg.id   = 0x469;
				msg.type = CAN_TYPE_DATA;
				msg.data = tx_message_1;
				msg.len  = 8;
				msg.fmt  = CAN_FMT_STDID;

				can_async_register_callback(&CAN_CCU, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_CCU_tx_callback);
				can_async_enable(&CAN_CCU);
				can_async_write(&CAN_CCU, &msg);
				break;


			case 'a':
				usb.print("  a: Send normal standard message with ID: 0x469 and 8 byte data 0 to 7. \r\n");
				msg.id   = 0x469;
				msg.type = CAN_TYPE_DATA;
				msg.data = tx_message_2;
				msg.len  = 8;
				msg.fmt  = CAN_FMT_STDID;

				/* Disable the FDOE and BRSE from register configuration
				 * and enable them again in callback */
				hri_can_set_CCCR_INIT_bit(CAN_CCU.dev.hw);
				while (hri_can_get_CCCR_INIT_bit(CAN_CCU.dev.hw) == 0)
					;
				hri_can_set_CCCR_CCE_bit(CAN_CCU.dev.hw);

				hri_can_clear_CCCR_FDOE_bit(CAN_CCU.dev.hw);
				hri_can_clear_CCCR_BRSE_bit(CAN_CCU.dev.hw);

				can_async_register_callback(&CAN_CCU, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_std_tx_callback);
				can_async_enable(&CAN_CCU);
				can_async_write(&CAN_CCU, &msg);
				break;

			default:
				break;
			}
		}

	}
}
