/* 
* ARINC_Interface.cpp
*
* Created: 3/18/2020 12:01:24 PM
* Author: GMateusDP
*/


#include "ARINC_Interface.h"
const char Mesg1[] = "ARINC Label: ";
const char Mesg2[] = "Data: ";
const char HexToAscii[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

const char HistoryMesg[] = "History FIFO: ";

 char DebugArrayStrings[][25] ={              // Status register dump message headers
	"\n\rREC1 Status Reg \t",
	"REC1 Control Reg \t",
	"REC2 Status Reg \t",
	"REC2 Control Reg \t",
	"Transmit Status Reg \t",
	"Transmit Control Reg \t",
	"FLAG Int Assig \t\t",
	"ACLK Divisor \t\t",
	"PL1 Match 1 Reg \t",
	"PL1 Match 2 Reg \t",
	"PL1 Match 3 Reg \t",
	"PL2 Match 1 Reg \t",
	"PL2 Match 2 Reg \t",
	"PL2 Match 3 Reg \t"
};


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
	HI3893.Init();
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
	HI3893.initReceiver1Labels();     // initial Rec1 labels
	HI3893.initReceiver2Labels();     // initial Rec2 labels
	HI3893.MultiByteRead(REC1MEMORY, 32, LabelsAr1); // read in all Rec-1 labels into the passed array
	HI3893.MultiByteRead(REC2MEMORY, 32, LabelsAr2); // read in all Rec-2 labels into the passed array

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
		HI3893.W_CommandValue(REC1CR,RFLIP | PLON | LABREC | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
		HI3893.W_CommandValue(REC2CR,RFLIP | PLON | LABREC | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
		xprint("\n\rAll labels are Enabled");
	} else
	{
		HI3893.W_CommandValue(REC1CR, RFLIP | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
		HI3893.W_CommandValue(REC2CR, RFLIP | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
		xprint("\n\rAll labels are Disabled");
	}
	
	// Priorty Labels
	TXBuffer[0] =  PL13;   // filter 3
	TXBuffer[1] =  PL12;   // filter 2
	TXBuffer[2] =  PL11;   // filter 1
	HI3893.TransmitCommandAndData(W_PL1Match,TXBuffer);  // initialize the Receiver1 PL match registers
	
	TXBuffer[0] =  PL23;   // filter 3
	TXBuffer[1] =  PL22;   // filter 2
	TXBuffer[2] =  PL21;   // filter 1
	HI3893.TransmitCommandAndData(W_PL2Match,TXBuffer);  // initialize the Receiver2 PL match registers
	
	
	// Prepare the first message in the transmit buffer with [00-00-00-default label]
	TXBuffer[0] = BigCounter.PayLoad[1];
	TXBuffer[1] = BigCounter.PayLoad[2];
	TXBuffer[2] = BigCounter.PayLoad[3];
	TXBuffer[3] = ARINCLabel;            // normal default label
	
	xprint("\n\r\nPress SW-1 to start initially\n\r\n");
	xprint("--- Button Commands ---\n\r");
	xprint("  Space Bar to Display Status and Control Registers\n\r");
	xprint("  p to Pause\n\r");
	xprint("  m to Display Label Memory");
	ConsoleCommandsHelp();
	xprint("\n\r>");
	while(gpio_get_pin_level(OLED_BUTTON1));
	while(!gpio_get_pin_level(OLED_BUTTON1));
	delay_ms(K_1SEC);              // blink for 1 sec
	//g_ledFlashBool=OFF;
	gpio_set_pin_level(OLED_LED1,true);
	//LED_CTL(LED_7,OFF);     // Turn off LED-7. Will use for PL-3 indication
	
	xprint("\n\rTransmitting\n\r>");

	
	for(;;) {     // TEST Transmit loop
		
		do {
			ch=xgetchar();
			
			if(ch==' ')                         // print out the status registers if
			// SW3 pressed or spacebar from PC
			{
				xprint("\n\rHI-3593 Status, Control Registers\r\n");
				cpu_irq_disable();
				printStatusRegisters();                  // print out the status registers
				cpu_irq_enable();
				xprint("Press SW4 or spacebar to continue");
				while(' ' != xgetchar());        // start again if SW-4 pressed/spacebar
			}
			
			if( ch=='p'){                    // pause transmitting
				debounceButton(OLED_BUTTON2);                // debounce this button
				xprint("\n\rTransmit Paused\n\r");
				xprint("Press OLED_BUTTON-2 to transmit one frame and pause\n\r");
				xprint("Press OLED_BUTTON-3 to Resume\n\r");

				TxPaused=YES;
				while(OLED_BUTTON2)                          // wait for button-2 pressed again
				{
					if(!gpio_get_pin_level(OLED_BUTTON3))
					{
						TxPaused=NO;
						xprint("\n\rTransmitter Resumed...\n\r");
						break;
					}
				};
				debounceButton(OLED_BUTTON2);         // debounce this button
			}
			
			if(ch=='m' ){     // print out the Label memory for both receivers if SW3 pushed
				xprint("\n\rReceiver 1 Labels\n\r");
				PrintLabelsOnConsole(LabelsAr1);
				crlf();
				
				xprint("\n\rReceiver 2 Labels\n\r");
				PrintLabelsOnConsole(LabelsAr2);
				xprint("\n\rPress SW-4 to Resume\n\r");
				
				while(gpio_get_pin_level(OLED_BUTTON3));                      // resume if Button-4 pressed or spacebar again
				xprint("\n\rTransmitter Resumed...\n\r");
			}
			
				cpu_irq_disable();
			Status_F = HI3893.R_Register(R_TSR);      // Transmit control register
			cpu_irq_enable();
		} while (Status_F & TFFULL);      // Wait until FIFO not full before loading next byte

		if(ConsoleCommands(ch))                // 1 second delay to see the message on the console
		delay_ms(K_1SEC);
		
			cpu_irq_disable();
		HI3893.TransmitCommandAndData(TXFIFO,TXBuffer);
		cpu_irq_enable();
		printARINCTXData(TXBuffer);                        // print the TX data written to the TXFIFO
		//PORTT= ~TXBuffer[3];                               // show the transmitted nibble of the LSB

		//PORTA |= 0x80;                                     // pulse high for scope
		//PORTA &= 0x7F;
		
		BigCounter.Word32++;

		#if __DEBUG__    // special test patterns for checking the converter
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
		
		
		
		
		#if __DEBUG__     // Test Code: useful for transmitting on-the-fly labels
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
		
		#if __DEBUG__    // use this for debugging. Stops at specified LSB of the message
		if(BigCounter.PayLoad[3]==0x32)
		{
				cpu_irq_disable();
			printStatusRegisters();       // print out the status registers
			cpu_irq_enable();
			for(;;);
			
		}
		#endif
		
		FetchAllMessagesAndDisplay(RXBuffer,RXBufferPL);   // Fetch all Normal and Priority Messages into                                            // the arrays via buffer pointers
		delay_ms(K_10MS);                      // small delay so we can see the PL LEDs flash better
		
		
	} // loop
}  // Transmit Mode End

void ARINC_Interface::xprint(char *string){
	uint8_t	*p;
	p=(uint8_t *)string;
	usb0.write(p,strlen(string));
	}
char	ARINC_Interface::xgetchar(void){
	
	uint8_t	p;
	usb0.read(&p,1);
	char i(p);
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

void	ARINC_Interface::cpu_irq_enable(void){
	__DMB();
	__enable_irq();
}
void	ARINC_Interface::cpu_irq_disable(void){
	__DMB();
	__disable_irq();
}
void	ARINC_Interface::printStatusRegisters(void){
	uint8_t	i;
	char p[25];
	
	getRegStatus();
	for(i=0; i< 14;i++) {
	//	p=DebugArrayStrings[i];
		char *ptr=DebugArrayStrings[i];
	//	char *ptr=p;
		//xprint(ptr);
		//PrintHexByte(DebugArray[i]);  // Output the status registers
		crlf();
	}
	
}
void	ARINC_Interface::crlf(void){
	uint8_t p=0x0A;
	usb0.write(&p,1);
	p=0x0D;
	usb0.write(&p,1);
}
void ARINC_Interface::PrintHexByte(uint8 c)
{
	uint8_t q[]="0x";
	uint8_t *ptr=q;
	usb0.write(ptr,sizeof(q));
	uint8_t p;
	p=HexToAscii[(c>>4)];
	usb0.write(&p,1);
	p=HexToAscii[(c & 0x0f)];
	usb0.write(&p,1);
	*ptr=SPACE;
	usb0.write(ptr,1);
}

void ARINC_Interface::getRegStatus(void){
	uint8 priorityMatchLabels3[3];
	
	DebugArray[0] =HI3893.R_Register(0x90);     // Fetch Receiver 1 Status Reg
	DebugArray[1] =HI3893.R_Register(0x94);     // Fetch Receiver 1 Control Reg
	
	DebugArray[2] =HI3893.R_Register(0xB0);     // Fetch Receiver 2 Status Reg
	DebugArray[3] =HI3893.R_Register(0xB4);     // Fetch Receiver 2 Control Reg
	
	DebugArray[4] = HI3893.R_Register(0x80);     // Fetch Transmit Status Register
	DebugArray[5] = HI3893.R_Register(0x84);     // Fetch Transmit Control Register
	
	DebugArray[6] = HI3893.R_Register(0xD0);     // Fetch Read Flag/ Interrupt Register
	DebugArray[7] = HI3893.R_Register(0xD4);     // Fetch ACLK Divsion Register
	
	HI3893.MultiByteRead(0x9C,3, priorityMatchLabels3);     // Fetch Priority Match Labels
	DebugArray[8] = priorityMatchLabels3[2];         // Fetch Priority Match Label
	DebugArray[9] = priorityMatchLabels3[1];         // Fetch Priority Match Label
	DebugArray[10] = priorityMatchLabels3[0];        // Fetch Priority Match Label
	
	HI3893.MultiByteRead(0xBC,3, priorityMatchLabels3);     //  Fetch Priority Match Labels
	DebugArray[11] = priorityMatchLabels3[2];        //  Fetch Priority Match Label
	DebugArray[12] = priorityMatchLabels3[1];        //  Fetch Priority Match Label
	DebugArray[13] = priorityMatchLabels3[0];        //  Fetch Priority Match Label

}
void  ARINC_Interface::debounceButton(uint8 button)
{
	uint8 debounceCnt;
	
	debounceCnt=10;
	while(debounceCnt){
		delay_ms(10);            // delay 1ms
		if(! (gpio_get_pin_level(button))){
			debounceCnt=10;
		}else{
			debounceCnt--;
		}
		
	}
}
unsigned char   ARINC_Interface::ConsoleCommands(unsigned char ch)
{
	unsigned char TCRTemp, RCR1Temp, RCR2Temp;
	
	if(ch=='P' || ch=='p'|| ch=='R' || ch=='r' || ch=='S' || ch=='s' ||ch=='T' || ch=='t' || ch=='H' || ch=='h')
	{
		TCRTemp =  HI3893.R_Register(R_TCR);    // Read the TCR
		RCR1Temp = HI3893.R_Register(R_RCR1);   // Read the RCR1
		RCR2Temp = HI3893.R_Register(R_RCR2);   // Read the RCR2
		
		if(ch=='P' || ch=='p')           // Toggle the ODD/EVEN parity bit in the TCR reg
		{
			TCRTemp ^= 4;                 // Toggle the TX parity bit
			if(TCRTemp & 4)
			xprint("TX Parity ON\n\r>");
			else
			xprint("TX Parity OFF\n\r>");
			HI3893.W_CommandValue(TCR, TCRTemp);
		}

		if(ch=='T' || ch=='t')          // Toggle the TFLIP bit in the TCR reg
		{
			TCRTemp ^= 0x40;             // Toggle the TFLIP bit
			if(TCRTemp & 0x40)
			xprint("TFLIP Set\n\r>");
			else
			xprint("TFLIP Cleared\n\r>");
			HI3893.W_CommandValue(TCR, TCRTemp);
		}
		
		if(ch=='R' || ch=='r')           // Toggle the RFLIP bit in the RCR1/RCR2 regs
		{
			RCR1Temp ^= 0x80;             // Toggle the RFLIP bit
			RCR2Temp ^= 0x80;             // Toggle the RFLIP bit
			if(RCR1Temp & 0x80)
			xprint("REC-1 RFLIP Set\n\r>");
			else
			xprint("REC-1 RFLIP Cleared\n\r>");
			HI3893.W_CommandValue(REC1CR, RCR1Temp);
			if(RCR2Temp & 0x80)
			xprint("REC-2 RFLIP Set\n\r>");
			else
			xprint("REC-2 RFLIP Cleared\n\r>");
			HI3893.W_CommandValue(REC2CR, RCR2Temp);
		}
		
		if(ch=='S' || ch=='s')          // Toggle the SDON bit in the RCR1/RCR2 regs
		{
			RCR1Temp ^= 0x70;             // Toggle the SDON, SD9, SD10 bits
			RCR2Temp ^= 0x70;             // Toggle the SDON, SD9, SD10 bits
			if(RCR1Temp & 0x10)
			xprint("REC-1 SDON,SD9 and SD10 Set\n\r>");
			else
			xprint("REC-1 SDON,SD9 and SD10 Cleared\n\r>");
			HI3893.W_CommandValue(REC1CR, RCR1Temp);
			if(RCR2Temp & 0x10)
			xprint("REC-2 SDON,SD9 and SD10 Set\n\r>");
			else
			xprint("REC-2 SDON,SD9 and SD10 Cleared\n\r>");
			HI3893.W_CommandValue(REC2CR, RCR2Temp);
		}
		
		if(ch=='H' || ch=='h')            // Print out the help screen
		{
			ConsoleCommandsHelp();
			xprint("\n\r\nPress Space Bar or SW-4 to resume");
			while(OLED_BUTTON3 && ' ' != xgetchar());        // start again if Button-4 pressed/spacebar
			xprint("\n\r>");
		}
		

	} else
	{                                  // No command to process
		return false;
	}
	return true;
	
}

void ARINC_Interface::PrintLabelsOnConsole(unsigned char *labelarray)
{
	uint8 i;
	
	xprint("Labels  0 - 63:  ");
	for(i=31; i != 0xFF;i--) {
		xprint("0x");
		PrintHexByte(labelarray[i]);          // Output the frame data
		xprint(" ");
		if(i==24)
		xprint("\n\rLabels  64-127:  ");
		if(i==16)
		xprint("\n\rLabels 128-191:  ");
		if(i==8)
		xprint("\n\rLabels 192-255:  ");
	}
	
}

void ARINC_Interface::printARINCTXData(unsigned char *array) {
	unsigned char i;
	
	xprint("\n\r>TX    Data:");
	for(i=0; i< 3;i++) {
		PrintHexByte(array[i]);          // Output the data
	}
	xprint("Label:");
	PrintHexByte(array[3]);             // Output the label data
	
	crlf();
	xprint(">");
}

void ARINC_Interface::FetchAllMessagesAndDisplay(unsigned char *RXBuffer,unsigned char *RXBufferPL)
{
	static char Status_F;
	
	//LED_CTL(LED_5,OFF);     // Turn off PL-1 LED
//	LED_CTL(LED_6,OFF);     // Turn off PL-2 LED
//	LED_CTL(LED_7,OFF);     // Turn off PL-3 LED
	
	// Recever -1
	cpu_irq_disable();
	Status_F = HI3893.R_Register(RXSTATUS_1);                       // Poll Receiver1 status register
	cpu_irq_enable();

	if((Status_F & FFEMPTY) == 0)
	{
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_1,RXBuffer );
		(void)memcpy(g_RXBuffer[MessageCount],RXBuffer,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_HEADER,RXBuffer);
		CheckMessageCountMax();
	}

	// Receiver1 Priority Labels
	if(Status_F & PL1)
	{
		//LED_CTL(LED_5,ON);               // Turn on LED5 for PL1
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_1L1,RXBufferPL );
		(void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_L1_HEADER,RXBufferPL);
		CheckMessageCountMax();
	}

	// Receiver1 Priority Label 2
	if(Status_F & PL2)
	{
		//LED_CTL(LED_6,ON);               // Turn on LED6 for PL2
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_1L2,RXBufferPL );
		(void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_L2_HEADER,RXBufferPL);
		CheckMessageCountMax();
	}
	// Receiver1 Priority Label 3
	if(Status_F & PL3)
	{
		//LED_CTL(LED_7,ON);               // Turn on LED7 for PL3
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_1L3,RXBufferPL );
		(void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_L3_HEADER,RXBufferPL);
		CheckMessageCountMax();
	}
	
	// Recever -2
	cpu_irq_disable();
	Status_F = HI3893.R_Register(RXSTATUS_2);
	cpu_irq_enable();
	if((Status_F & FFEMPTY) == 0)
	{
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_2,RXBuffer );
		(void)memcpy(g_RXBuffer[MessageCount],RXBuffer,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_HEADER,RXBuffer);
		CheckMessageCountMax();
	}

	// Receiver2 Priority Label 1
	if(Status_F & PL1)
	{
		//LED_CTL(LED_5,ON);               // Turn on LED5 for PL1
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_2L1,RXBufferPL );
		(void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_L1_HEADER,RXBufferPL);
		CheckMessageCountMax();
	}
	// Receiver2 Priority Label 2
	if(Status_F & PL2)
	{
		//LED_CTL(LED_6,ON);               // Turn on LED6 for PL2
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_2L2,RXBufferPL );
		(void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_L2_HEADER,RXBufferPL);
		CheckMessageCountMax();
	}
	// Receiver2 Priority Label 3
	if(Status_F & PL3)
	{                                // Turn on LED7 for PL3
		//LED_CTL(LED_7,ON);
		cpu_irq_disable();
		HI3893.ArincRead(RXFIFO_2L3,RXBufferPL );
		(void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_L3_HEADER,RXBufferPL);
		CheckMessageCountMax();
	}
	
}
/*

void ARINC_Interface::LED_CTL(uint8 ledNumber, uint8 OnOff){
	#if NEWBOARD
	if(ledNumber>4 && ledNumber<8)          // LEDs 5-7 have reversed HW logic so invert these 3
	#else
	if(ledNumber>4)          // Old board.
	#endif

	OnOff = ~OnOff;
	switch (ledNumber){
		case 1:  LED1=OnOff;  break;
		case 2:  LED2=OnOff;  break;
		case 3:  LED3=OnOff;  break;
		case 4:  LED4=OnOff;  break;
		case 5:  LED5=OnOff;  break;
		case 6:  LED6=OnOff;  break;
		case 7:  LED7=OnOff;  break;
		case 8:  LED8=OnOff;  break;
		default: break;
	}
}
*/
void ARINC_Interface::CheckMessageCountMax(void)
{
	MessageCount++;
	if(MessageCount==MESSAGECOUNTMAX)
	MessageCount=0;
}

void ARINC_Interface::printARINCData(const uint8 channel, unsigned char *array) {
	unsigned char i;

	
	switch (channel)
	{
		case REC1_HEADER:
		xprint("Rec-1 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the data
		}
		xprint("Label:");
		PrintHexByte(array[3]);          // Output the label data
		break;
		case REC1_L1_HEADER:
		xprint("PL1-1 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the  data
		}
		xprint("Match:");
		PrintHexByte(PL11);              // Output the Match label
		
		break;
		case REC1_L2_HEADER:
		xprint("PL1-2 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the data
		}
		xprint("Match:");
		PrintHexByte(PL12);              // Output the Match label
		
		break;
		case REC1_L3_HEADER:
		xprint("PL1-3 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the data
		}
		xprint("Match:");
		PrintHexByte(PL13);              // Output the Match label
		
		break;
		
		case REC2_HEADER:
		xprint("Rec-2 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);        // Output the data
		}
		xprint("Label:");
		PrintHexByte(array[3]);          // Output the Match label
		break;
		case REC2_L1_HEADER:
		xprint("PL2-1 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the  data
		}
		xprint("Match:");
		PrintHexByte(PL21);              // Output the Match label
		
		break;
		
		case REC2_L2_HEADER:
		xprint("PL2-2 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);        // Output the  data
		}
		xprint("Match:");
		PrintHexByte(PL22);              // Output the Match label
		
		break;
		case REC2_L3_HEADER:
		xprint("PL2-3 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);        // Output the  data
		}
		xprint("Match:");
		PrintHexByte(PL23);              // Output the Match label
		
		break;
		
		
		default:
		break;
	};
	
	crlf();
	xprint(">");
}
