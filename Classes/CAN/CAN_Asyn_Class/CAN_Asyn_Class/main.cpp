/*
 * CAN_Asyn_Class.cpp
 *
 * Created: 5/11/2020 9:16:00 AM
 * Author : GMateusDP
 */ 

#include "sam.h"
#include <atmel_start.h>
#include "USB_CDC_Class.h"
#include "stdlib.h"
#include "string.h"
USB_CDC_Class	usb;
bool	gotMessage=false;
 struct can_message msg1;

/**
 * \brief display configuration menu.
 */
void display_menu(void);
 static void CAN_0_tx_callback(struct can_async_descriptor *const descr)
 {
	 (void)descr;
	 usb.xprint("  CAN Transmission done \r\n");
 }
 
  static  void CAN_std_tx_callback(struct can_async_descriptor *const descr)
   {
	   (void)descr;

	   /* Enable the FDOE and BRSE in register configuration*/
	   hri_can_set_CCCR_INIT_bit(CAN_0.dev.hw);
	   while (hri_can_get_CCCR_INIT_bit(CAN_0.dev.hw) == 0)
	   ;
	   hri_can_set_CCCR_CCE_bit(CAN_0.dev.hw);

	   hri_can_set_CCCR_FDOE_bit(CAN_0.dev.hw);
	   hri_can_set_CCCR_BRSE_bit(CAN_0.dev.hw);

	   usb.xprint("  CAN std Transmission done \r\n");
   }
  static void CAN_0_rx_callback(struct can_async_descriptor *const descr)
   {
	   
	   uint8_t            data[64];
	   for (uint8_t  i = 0; i < 64; i++)
	   {
		   data[i]=0;
	   }
	  	   msg1.data = data;
	   can_async_read(descr, &msg1);
		gotMessage=true;
	  
   }
   
   void	print_Message(void){
	    char	q[]="0x    \t";
	    char	p[]="00";
	    usb.xprint("CAN Message received . The received data is: \r\n");
	      // uint8_t            dataIn[64];
		   uint8_t			*dataPtr=msg1.data;
		   uint8_t	dataValue=*dataPtr;
		 //  dataIn=*(msg1.data);
	      for (uint8_t i = 0; i < msg1.len; i++) {
	  //  for (uint8_t i = 0; i < 8; i++) {
		
		   // usb.xprint("0X ");
			
			itoa(dataValue,&p[0],10);
			q[3]=p[0];
			if (p[1]<'0'||p[1]>'9')
			{
				q[4]=' ';
			}else
			{
				q[4]=p[1];
			}
			 
			
		//  q= itoa(msg1.data[i],p,10);
		    usb.xprint(q);
		//	while(!usb.txDone);
			delay_ms(10);
			dataPtr++;
			dataValue=*dataPtr;
			//usb.xprint("\t");
	    }
	    usb.xprint("\r\n\r\n");
	    //return;
   }
  void	print_Message2(void){
	
	      usb.xprint("CAN Message received . The received data is: \r\n");
	      uint8_t			*dataPtr=msg1.data;
	      uint8_t			dataValue=*dataPtr;

	      for (uint8_t i = 0; i < msg1.len; i++) {
		  
		       usb.xprint("0X ");
		        delay_ms(1);
				char buffer[]="   ";
				utoa(dataValue,&buffer[0],10);
			    delay_ms(1);
				usb.xprint(&buffer[0]);
		      delay_ms(1);
		 
		      usb.xprint(" \t");
			   delay_ms(1);
			   	dataPtr++;
			   	dataValue=*dataPtr;
		
	      }
	      usb.xprint("\r\n\r\n");
	 
      }

int main(void)
{
	uint8_t key;

	struct can_message msg;
	struct can_filter  filter;

	uint8_t tx_message_0[64];
	uint8_t tx_message_1[64];
	uint8_t tx_message_2[8];

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
		usb.init();
		delay_ms(200);
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
		if (gotMessage)
		{			
			print_Message2();
			gotMessage=false;
		}
		key=0xff;
		
		usb.read(&key,1);
		delay_ms(100);
		switch (key) {
		case 'h':
			display_menu();
			break;

		case '0':
			usb.xprint("  0: Set standard filter ID 0: 0x45A, store into Rx buffer. \r\n");
			can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_rx_callback);
			filter.id   = 0x45A;
			filter.mask = 0;
			can_async_set_filter(&CAN_0, 0, CAN_FMT_STDID, &filter);
			break;

		case '1':
			usb.xprint("  1: Set standard filter ID 1: 0x469, store into Rx FIFO 0. \r\n");
			can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_rx_callback);
			filter.id   = 0x469;
			filter.mask = 0;
			can_async_set_filter(&CAN_0, 1, CAN_FMT_STDID, &filter);
			break;

		case '2':
			usb.xprint("  2: Send standard message with ID: 0x45A and 64 byte data 0 to 63. \r\n");
			msg.id   = 0x45A;
			msg.type = CAN_TYPE_DATA;
			msg.data = tx_message_0;
			msg.len  = 8;
			msg.fmt  = CAN_FMT_STDID;

			can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_tx_callback);
			can_async_enable(&CAN_0);
			can_async_write(&CAN_0, &msg);
			break;

		case '3':
			usb.xprint("  3: Send standard message with ID: 0x469 and 64 byte data 128 to 191. \r\n");
			msg.id   = 0x469;
			msg.type = CAN_TYPE_DATA;
			msg.data = tx_message_1;
			msg.len  = 8;
			msg.fmt  = CAN_FMT_STDID;

			can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_tx_callback);
			can_async_enable(&CAN_0);
			can_async_write(&CAN_0, &msg);
			break;

		case '4':
			usb.xprint("  4: Set extended filter ID 0: 0x100000A5, store into Rx buffer. \r\n");
			can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_rx_callback);
			filter.id   = 0x100000A5;
			filter.mask = 0;
			can_async_set_filter(&CAN_0, 0, CAN_FMT_EXTID, &filter);
			break;

		case '5':
			usb.xprint("  5: Set extended filter ID 1: 0x10000096, store into Rx FIFO 1. \r\n");
			can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_rx_callback);
			filter.id   = 0x10000096;
			filter.mask = 0;
			can_async_set_filter(&CAN_0, 1, CAN_FMT_EXTID, &filter);
			break;

		case '6':
			usb.xprint("  6: Send extended message with ID: 0x100000A5 and 64 byte data 0 to 63. \r\n");
			msg.id   = 0x100000A5;
			msg.type = CAN_TYPE_DATA;
			msg.data = tx_message_0;
			msg.len  = 8;
			msg.fmt  = CAN_FMT_EXTID;

			can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_tx_callback);
			can_async_enable(&CAN_0);
			can_async_write(&CAN_0, &msg);
			break;

		case '7':
			usb.xprint("  7: Send extended message with ID: 0x10000096 and 64 byte data 128 to 191. \r\n");
			msg.id   = 0x10000096;
			msg.type = CAN_TYPE_DATA;
			msg.data = tx_message_1;
			msg.len  = 8;
			msg.fmt  = CAN_FMT_EXTID;

			can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_tx_callback);
			can_async_enable(&CAN_0);
			can_async_write(&CAN_0, &msg);
			break;

		case 'a':
			usb.xprint("  a: Send normal standard message with ID: 0x469 and 8 byte data 0 to 7. \r\n");
			msg.id   = 0x469;
			msg.type = CAN_TYPE_DATA;
			msg.data = tx_message_2;
			msg.len  = 8;
			msg.fmt  = CAN_FMT_STDID;

			/* Disable the FDOE and BRSE from register configuration
			 * and enable them again in callback */
			hri_can_set_CCCR_INIT_bit(CAN_0.dev.hw);
			while (hri_can_get_CCCR_INIT_bit(CAN_0.dev.hw) == 0)
				;
			hri_can_set_CCCR_CCE_bit(CAN_0.dev.hw);

			hri_can_clear_CCCR_FDOE_bit(CAN_0.dev.hw);
			hri_can_clear_CCCR_BRSE_bit(CAN_0.dev.hw);

			can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_std_tx_callback);
			can_async_enable(&CAN_0);
			can_async_write(&CAN_0, &msg);
			break;
		case 'f':
			usb.xprint("  a: Send normal standard message with ID: 0x45A and 8 byte data 0 to 7. \r\n");
			msg.id   = 0x45A;
			msg.type = CAN_TYPE_DATA;
			msg.data = tx_message_2;
			msg.len  = 8;
			msg.fmt  = CAN_FMT_STDID;

			/* Disable the FDOE and BRSE from register configuration
			 * and enable them again in callback */
			hri_can_set_CCCR_INIT_bit(CAN_0.dev.hw);
			while (hri_can_get_CCCR_INIT_bit(CAN_0.dev.hw) == 0)
				;
			hri_can_set_CCCR_CCE_bit(CAN_0.dev.hw);

			hri_can_clear_CCCR_FDOE_bit(CAN_0.dev.hw);
			hri_can_clear_CCCR_BRSE_bit(CAN_0.dev.hw);

			can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_std_tx_callback);
			can_async_enable(&CAN_0);
			can_async_write(&CAN_0, &msg);
			break;


		default:
			break;
		}
	}
}
 void display_menu(void)
{
	usb.xprint("Menu :\r\n");
	usb.xprint("  -- Select the action:\r\n");
	usb.xprint("  0: Set standard filter ID 0: 0x45A, store into Rx buffer. \r\n");
	usb.xprint("  1: Set standard filter ID 1: 0x469, store into Rx FIFO 0. \r\n");
	usb.xprint("  2: Send FD standard message with ID: 0x45A and 64 byte data 0 to 63. \r\n");
	usb.xprint("  3: Send FD standard message with ID: 0x469 and 64 byte data 128 to 191. \r\n");
	usb.xprint("  4: Set extended filter ID 0: 0x100000A5, store into Rx buffer. \r\n");
	usb.xprint("  5: Set extended filter ID 1: 0x10000096, store into Rx FIFO 1. \r\n");
	usb.xprint("  6: Send FD extended message with ID: 0x100000A5 and 64 byte data 0 to 63. \r\n");
	usb.xprint("  7: Send FD extended message with ID: 0x10000096 and 64 byte data 128 to 191. \r\n");
	usb.xprint("  a: Send normal standard message with ID: 0x469 and 8 byte data 0 to 7. \r\n");
	usb.xprint("  h: Display menu \r\n\r\n");
}



