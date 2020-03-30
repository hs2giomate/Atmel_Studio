/* 
* ARINC_Interface.cpp
*
* Created: 3/18/2020 12:01:24 PM
* Author: GMateusDP
*/


#include "ARINC_Interface.h"

// default constructor
ARINC_Interface::ARINC_Interface()
{
} //ARINC_Interface

// default destructor
ARINC_Interface::~ARINC_Interface()
{
} //~ARINC_Interface

void	ARINC_Interface::Init(void){
	usb0.init();
	BITRATE0=false;
	BITRATE1=false;
	BITRATE2=false;
	OPT1=true;
	OPT2=false;
	PARITY=false;
}

void	ARINC_Interface::SayHello(void){
	uint8_t p[]="   hs2 ARINC Interface\n\r";
	uint8_t *ptr=p;
	usb0.write(ptr,sizeof(p));
	
}

void ARINC_Interface::TransmitReceiveWithLabels_Mode(const uint8_t SELFTEST){
	static char Status_F;
	static unsigned char TxPaused;
	static  unsigned char RXBuffer[g_RXBuffSize];       // Temp buffer to hold messages data
	static  unsigned char RXBufferPL[g_RXBuffSize];     // Temp buffer to hold PL messages data
	static  unsigned char LabelsAr1[32];                // All Rec1 256 labels
	static  unsigned char LabelsAr2[32];                // All Rec2 256 labels
	uint8_t Arate;
	uint8_t Rec1Parity;
	uint8_t Rec2Parity;
	
	union Buffer32 {
		long Word32;
		unsigned char PayLoad[4];        // We will ignore the MSB byte [0]
	};

	union Buffer32 BigCounter;
	char ch;                   // console character
	uint8_t ARINCLabel;

	TxPaused=0;                // start out not paused
	BigCounter.Word32 = 0;     // clear all 8 bytes of the data field.
	MessageCount=0;
	ARINCLabel = 0x30;         // Write ARINC Label
	Arate=0;
	
	xprint("\n\rTransmit & Receive With Labels Mode\n\r");
	if(SELFTEST)
	xprint("\n\rSELF-TEST, Internal Digital Loop-back, No data is transmitted externally\n\r");
	
	if(BITRATE0)
	{
		Arate = 1;              // BITRATE0 open so set for slow speed
		xprint("\n\rSlow Speed");
	} else
	{
		xprint("\n\rFast Speed");
	}
	
	if(PARITY)
	{
		Arate |= TRANSMIT_PARTY_ON << 2;   // enable the transmitter parity
		Arate |= PARITYODD << 3;           // select ODD for now. Expect compiler dead warning if 0.
		xprint("\n\rTransmitter Parity Enabled");
	}

	//Initilize the TCR and the ACLK Divisor and look for the expected value in the TCR status reg
	if(TSR_Default != HI3893.Init3593(ACLKDIV,  // ARINC clock divisor
	TMODE,    // Transmit mode. Set for "send as soon as possible"
	SELFTEST, // Selftest defined by Mode switches
	Arate,    // Arinc speed and if Parity is enabled by the switch
	TFLIP ))
	{
		//LED_CTL(LED_8,ON);              // turn on RED LED if failed
		xprint("\n\rHI-3593 failed to initilize\n\r");
		for(;;);
	}

	(void)memset(TXBuffer, 0, sizeof(TXBuffer));                   // clear the buffer

	//   W_Command(0x48);  // set all bits in label memory to 1
	//   W_Command(0x4C);  // set all bits in label memory to 1
	
	// Setup the Labels
	initReceiver1Labels();     // initial Rec1 labels
	initReceiver2Labels();     // initial Rec2 labels
	MultiByteRead(REC1MEMORY, 32, LabelsAr1); // read in all Rec-1 labels into the passed array
	MultiByteRead(REC2MEMORY, 32, LabelsAr2); // read in all Rec-2 labels into the passed array

	Rec1Parity =0;
	Rec2Parity =0;
	if(BITRATE1)
	{
		Rec1Parity =8;     // Receiver 1 parity enable
		xprint("\n\rReceiver-1 Parity is Enabled");
	} else
	{
		xprint("\n\rReceiver-1 Parity is Disabled");
	}
	if(BITRATE2)
	{
		Rec2Parity =8;     // Receiver 2 parity enable
		xprint("\n\rReceiver-2 Parity is Enabled");
	} else
	{
		xprint("\n\rReceiver-2 Parity is Disabled");
	}
	
	
	Arate &= 1;           // mask off everything else except the speed bit before using it here
	if(OPT2)      // enable labels if SW OTP2 is open
	{
		W_CommandValue(REC1CR,RFLIP | PLON | LABREC | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
		W_CommandValue(REC2CR,RFLIP | PLON | LABREC | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
		xprint("\n\rAll labels are Enabled");
	} else
	{
		W_CommandValue(REC1CR, RFLIP | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
		W_CommandValue(REC2CR, RFLIP | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
		xprint("\n\rAll labels are Disabled");
	}
	
	// Priorty Labels
	TXBuffer[0] =  PL13;   // filter 3
	TXBuffer[1] =  PL12;   // filter 2
	TXBuffer[2] =  PL11;   // filter 1
	TransmitCommandAndData(W_PL1Match,TXBuffer);  // initialize the Receiver1 PL match registers
	
	TXBuffer[0] =  PL23;   // filter 3
	TXBuffer[1] =  PL22;   // filter 2
	TXBuffer[2] =  PL21;   // filter 1
	TransmitCommandAndData(W_PL2Match,TXBuffer);  // initialize the Receiver2 PL match registers
	
	
	// Prepare the first message in the transmit buffer with [00-00-00-default label]
	TXBuffer[0] = BigCounter.PayLoad[1];
	TXBuffer[1] = BigCounter.PayLoad[2];
	TXBuffer[2] = BigCounter.PayLoad[3];
	TXBuffer[3] = ARINCLabel;            // normal default label
	
	xprint("\n\r\nPress SW-1 to start initially\n\r\n");
	xprint("--- Button Commands ---\n\r");
	xprint("  SW-1 or Space Bar to Display Status and Control Registers\n\r");
	xprint("  SW-2 to Pause\n\r");
	xprint("  SW-3 to Display Label Memory");
	ConsoleCommandsHelp();
	xprint("\n\r>");
	
	delay_ms(1000);              // blink for 1 sec
	g_ledFlashBool=OFF;
	LED_CTL(LED_7,OFF);     // Turn off LED-7. Will use for PL-3 indication
	
	xprint("\n\rTransmitting\n\r>");

	
	for(;;) {     // TEST Transmit loop
		
		do {
			ch=xgetchar();
			
			if(!SW1 || ch==' ')                         // print out the status registers if
			// SW3 pressed or spacebar from PC
			{
				xprint("\n\rHI-3593 Status, Control Registers\r\n");
				DisableInterrupts;
				printStatusRegisters();                  // print out the status registers
				EnableInterrupts;
				xprint("Press SW4 or spacebar to continue");
				while(SW4 && ' ' != xgetchar());        // start again if SW-4 pressed/spacebar
			}
			
			if((!SW2) || TxPaused){                    // pause transmitting
				debounceButton(BUTTON2);                // debounce this button
				xprint("\n\rTransmit Paused\n\r");
				xprint("Press SW-2 to transmit one frame and pause\n\r");
				xprint("Press SW-4 to Resume\n\r");

				TxPaused=YES;
				while(SW2)                          // wait for button-2 pressed again
				{
					if(!SW4)
					{
						TxPaused=NO;
						xprint("\n\rTransmitter Resumed...\n\r");
						break;
					}
				};
				debounceButton(BUTTON2);         // debounce this button
			}
			
			if(!SW3 ){     // print out the Label memory for both receivers if SW3 pushed
				xprint("\n\rReceiver 1 Labels\n\r");
				PrintLabelsOnConsole(LabelsAr1);
				crlf();
				
				xprint("\n\rReceiver 2 Labels\n\r");
				PrintLabelsOnConsole(LabelsAr2);
				xprint("\n\rPress SW-4 to Resume\n\r");
				
				while(SW4);                      // resume if Button-4 pressed or spacebar again
				xprint("\n\rTransmitter Resumed...\n\r");
			}
			
			DisableInterrupts;
			Status_F = R_Register(R_TSR);      // Transmit control register
			EnableInterrupts;
		} while (Status_F & TFFULL);      // Wait until FIFO not full before loading next byte

		if(ConsoleCommands(ch))                // 1 second delay to see the message on the console
		Delay100us(K_1SEC);
		
		DisableInterrupts;
		TransmitCommandAndData(TXFIFO,TXBuffer);
		EnableInterrupts;
		printARINCTXData(TXBuffer);                        // print the TX data written to the TXFIFO
		PORTT= ~TXBuffer[3];                               // show the transmitted nibble of the LSB

		PORTA |= 0x80;                                     // pulse high for scope
		PORTA &= 0x7F;
		
		BigCounter.Word32++;

		#if 0    // special test patterns for checking the converter
		TXBuffer[0] = 0x01;
		TXBuffer[1] = 0x00;
		TXBuffer[2] = 0xff;

		TXBuffer[3] = 0xff;              // *** generate test labels ***
		#else                                 // increment the HUGE counter
		
		TXBuffer[0] = BigCounter.PayLoad[1];
		TXBuffer[1] = BigCounter.PayLoad[2];
		TXBuffer[2] = BigCounter.PayLoad[3];
		TXBuffer[3] = BigCounter.PayLoad[3];              // copy the LSB to the label for testing purposes
		#endif
		
		
		
		
		#if 0    // Test Code: useful for transmitting on-the-fly labels
		if(BigCounter.PayLoad[3]==0x11)
		TXBuffer[3] = BigCounter.PayLoad[3];           // load the label
		if(BigCounter.PayLoad[3]==0x12)
		TXBuffer[3] = BigCounter.PayLoad[3];           // load the label
		if(BigCounter.PayLoad[3]==0x13)
		TXBuffer[3] = BigCounter.PayLoad[3];           // load the label
		if(BigCounter.PayLoad[3]==0x21)
		TXBuffer[3] = BigCounter.PayLoad[3];           // load the label
		if(BigCounter.PayLoad[3]==0x22)
		TXBuffer[3] = BigCounter.PayLoad[3];           // load the label
		if(BigCounter.PayLoad[3]==0x23)
		TXBuffer[3] = BigCounter.PayLoad[3];           // load the label
		#endif
		
		#if 0   // use this for debugging. Stops at specified LSB of the message
		if(BigCounter.PayLoad[3]==0x32)
		{
			DisableInterrupts;
			printStatusRegisters();       // print out the status registers
			EnableInterrupts;
			for(;;);
			
		}
		#endif
		
		FetchAllMessagesAndDisplay(RXBuffer,RXBufferPL);   // Fetch all Normal and Priority Messages into                                            // the arrays via buffer pointers
		Delay100us(K_10MS);                      // small delay so we can see the PL LEDs flash better
		
		
	} // loop
}  // Transmit Mode End

void ARINC_Interface::xprint(char *string){
	uint8_t i;
	
	for(i=0; i < strlen(string);i++) {
		usb0.write(string[i],1);
	}
}

void ARINC_Interface::CE_Low_Holt(void)
{

	//PORT->Group[0].OUTCLR.reg = (1UL << 27);
	gpio_set_pin_level(SPI0_CS,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);
}
void ARINC_Interface::CE_High_Holt(void)
{

	//PORT->Group[0].OUTCLR.reg = (1UL << 27);
	gpio_set_pin_level(SPI0_CS,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);
}
void ARINC_Interface::ConsoleCommandsHelp(void)
{
	xprint("\n\r\n--- Console Commands Help ---");
	xprint("\n\r  P = Parity Toggle on/off");
	xprint("\n\r  T = TFLIP  Toggle on/off");
	xprint("\n\r  R = RFLIP  Toggle on/off");
	xprint("\n\r  S = SDON, SD9, SD10 Toggle on/off");
	xprint("\n\r  H = This Help Page");
	xprint("\n\r  Space Bar = Display Status and Control regs");
	xprint("\n\r  I = Increment Message and Transmit. Explorer Mode  - ONLY!");
	xprint("\n\r  M = Enter Custom Message. Explorer Mode  - ONLY!");
	xprint("\n\r  ENTER= Transmit next message. Explorer Mode  - ONLY!\n\r");
}