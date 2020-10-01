/* 
* ARINC_Interface.cpp
*
* Created: 3/18/2020 12:01:24 PM
* Author: GMateusDP
*/


#include "ARINC_Interface.h"
#include "CDC_Class.h"
#include "hal_gpio.h"


const char Mesg1[] = "ARINC Label: ";
const char Mesg2[] = "Data: ";
const char HexToAscii[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char HexSymbols[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};

const char HistoryMesg[] = "History FIFO: ";

 char DebugArrayStrings[][25] ={              // Status register dump message headers
	"\n\rREC1 Status Reg \t",
	"\n\rREC1 Control Reg \t",
	"\n\rREC2 Status Reg \t",
	"\n\rREC2 Control Reg \t",
	"\n\rTransmit Status Reg \t",
	"\n\rTransmit Control Reg \t",
	"\n\rFLAG Int Assing \t\t",
	"\n\rACLK Divisor \t\t",
	"\n\rPL1 Match 1 Reg \t",
	"\n\rPL1 Match 2 Reg \t",
	"\n\rPL1 Match 3 Reg \t",
	"\n\rPL2 Match 1 Reg \t",
	"\n\rPL2 Match 2 Reg \t",
	"\n\rPL2 Match 3 Reg \t"
};



ARINC_Interface  *ptrARINCInterface;
static void blink_LED0(const struct timer_task *const timer_task){
	gpio_toggle_pin_level(LED0);
}
static void TxTimeout(const struct timer_task *const timer_task){
	ptrARINCInterface->txTimeout=true;
}

static void Receiver1Int(void){
	ptrARINCInterface->newMessageR1=true;
}

// default constructor
ARINC_Interface::ARINC_Interface()
{
	ptrARINCInterface=this;

	//converter=ptrConverter;
} //ARINC_Interface


// default destructor
ARINC_Interface::~ARINC_Interface()
{
} //~ARINC_Interface




bool	ARINC_Interface::Init(void){
	MessageCount=0; MessageCount1=0; MessageCount2=0;
	BITRATE0=false;
	BITRATE1=false;
	BITRATE2=false;

	PARITY=false;
	HW_RESET();
	statusHolt=HI3593.Init();
	if (statusHolt!=TSR_Default){
		usb.println("\n\rHI-3593 failed to Initilize\n\r");
		isOK=false;
	}else{
		isOK=true;
		newMessageR1=false;
		newMessageR2=false;
		BigCounter.Word32 = 0;     // clear all 8 bytes of the data field.
		MessageCount=0;
		ARINCLabel = 0x1D;         // Write ARINC Label
		Arate=0;
		
		(void)memset(TXBuffer, 0, sizeof(TXBuffer));    // clear the buffer
			
		// Setup the Labels
		HI3593.initReceiver1Labels();     // initial Rec1 labels
		HI3593.initReceiver2Labels();     // initial Rec2 labels
		HI3593.MultiByteRead(REC1MEMORY, 32, LabelsArrayTX); // read in all Rec-1 labels into the passed array
		HI3593.MultiByteRead(REC2MEMORY, 32, LabelsArrayRX); // read in all Rec-2 labels into the passed array

		Rec1Parity =0;
		Rec2Parity =0;

		HI3593.W_CommandValue(REC1CR,RFLIP | PLON | LABREC | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
		HI3593.W_CommandValue(REC2CR,RFLIP | PLON | LABREC | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
	}
	
	arincTimer.Init();
	arincTimer.add_periodic_task(FUNC_PTR(blink_LED0),1000);
	arincTimer.add_periodic_task(FUNC_PTR(TxTimeout),500);
	ext_irq_register(ARINCR1Int, Receiver1Int);
	arincTimer.start();
	//HI3893.InitPriorityLabels();
	return isOK;
}

void	ARINC_Interface::SayHello(void){
	//const	char *c="   hs2 ARINC Interface\n\r";
	usb.println("\n\r\n--- hs2 ARINC Interface  ----\n\r");
	//uint8_t_t *ptr=(uint8_t_t *)p;
	//usb.write(&testBanner[0],sizeof(testBanner));
	//usb.write(&p[0],sizeof(p));
	
}
bool	ARINC_Interface::isThereNewMessage(){
	if (newMessageR1)
	{
		return true;
	} 
	else
	{
		return false;
	}
}
uint32_t ARINC_Interface::ReadArincBuffer(){
	usb.println("r\n---!got message!----");
	cpu_irq_disable();
	statusRegister= HI3593.R_Register(RXSTATUS_1);                       // Poll Receiver1 status register
	cpu_irq_enable();
	if((statusRegister & FFEMPTY) == 0)
	{
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_1,RXBuffer );
		(void)memcpy(receiverBuffer[MessageCount],RXBuffer,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_HEADER,RXBuffer);
		CheckMessageCountMax();
	}
	
return statusRegister;	
}

uint32_t ARINC_Interface::ReadFIFO1(){
		// Recever -1
		cpu_irq_disable();
		Status_F = HI3593.R_Register(RXSTATUS_1);                       // Poll Receiver1 status register
		cpu_irq_enable();

		if((Status_F & FFEMPTY) == 0)
		{
			cpu_irq_disable();
			HI3593.ArincRead(RXFIFO_1,RXBuffer );
			(void)memcpy(receiverBuffer[MessageCount],RXBuffer,g_RXBuffSize);  // copy frame to large array for safe keeping
			cpu_irq_enable();
			printARINCData(REC1_HEADER,RXBuffer);
			CheckMessageCountMax();
		}

		// Receiver1 Priority Labels
		if(Status_F & PL1)
		{
			//LED_CTL(LED_5,ON);               // Turn on LED5 for PL1
			cpu_irq_disable();
			HI3593.ArincRead(RXFIFO_1L1,RXBufferPL );
			(void)memcpy(receiverBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
			cpu_irq_enable();
			printARINCData(REC1_L1_HEADER,RXBufferPL);
			CheckMessageCountMax();
		}

		// Receiver1 Priority Label 2
		if(Status_F & PL2)
		{
			//LED_CTL(LED_6,ON);               // Turn on LED6 for PL2
			cpu_irq_disable();
			HI3593.ArincRead(RXFIFO_1L2,RXBufferPL );
			(void)memcpy(receiverBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
			cpu_irq_enable();
			printARINCData(REC1_L2_HEADER,RXBufferPL);
			CheckMessageCountMax();
		}
		// Receiver1 Priority Label 3
		if(Status_F & PL3)
		{
			//LED_CTL(LED_7,ON);               // Turn on LED7 for PL3
			cpu_irq_disable();
			HI3593.ArincRead(RXFIFO_1L3,RXBufferPL );
			(void)memcpy(receiverBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
			cpu_irq_enable();
			printARINCData(REC1_L3_HEADER,RXBufferPL);
			CheckMessageCountMax();
		}
	
	return statusRegister;
}

uint32_t ARINC_Interface::ReadBufferLabel(uint8_t label){
	uint32_t *data;
	uint8_t	j;
	for (uint8_t i=0;i<MESSAGECOUNTMAX;i++){
		j=(MessageCount+i)%MESSAGECOUNTMAX;
		if (receiverBuffer[j][3]==label)
		{
			data=(uint32_t*)receiverBuffer[j];
			return *data;
		}
		
	}

	return 0;
}
uint32_t ARINC_Interface::ReadBufferLabel(int label){
	uint8_t	l=Label2Byte((uint32_t)label);
	return (uint32_t)ReadBufferLabel(l);
}
uint32_t ARINC_Interface::TrasmitSingleLabel(void){
	return	TrasmitSingleLabel(DEFAULT_LABEL);
}

uint32_t ARINC_Interface::TrasmitSingleLabel(uint32_t l){
	ARINCLabel=Label2Byte(l);
	index=GetIndexTXLabelarray(l,LabelsArrayTX);
	uint8_t localBuffer[4];
	memcpy(localBuffer,&transmitBuffer[index][0],4);
	PrepareSingleTXBuffer(TXBuffer,localBuffer);
	usb.println(" transmitting...");
	cpu_irq_disable();
	HI3593.TransmitCommandAndData(TXFIFO,TXBuffer);
	cpu_irq_enable();
	usb.println(" Transmitted!");
	usb.println(">");
	printARINCTXData(TXBuffer);
	txTimeout=false;
	return FourBytesArray2Uint32(TXBuffer);
}

void ARINC_Interface::TransmitReceiveWithLabels_Mode(const uint8_t SELFTEST){
	char Status_F;
	unsigned char TxPaused;


	
	
	 char ch;                   // console character
	

	TxPaused=0;                // start out not paused
	BigCounter.Word32 = 0;     // clear all 8 bytes of the data field.
	MessageCount=0;
	ARINCLabel = 0x1d;         // Write ARINC Label
	Arate=0;
	
	usb.println("\n\rTransmit & Receive With Labels Mode\n\r");
	if(SELFTEST)
	usb.println("\n\rSELF-TEST, Internal Digital Loop-back, No data is transmitted externally\n\r");
	


	(void)memset(TXBuffer, 0, sizeof(TXBuffer));                   // clear the buffer

	//   W_Command(0x48);  // set all bits in label memory to 1
	//   W_Command(0x4C);  // set all bits in label memory to 1
	
	// Setup the Labels
	HI3593.initReceiver1Labels();     // initial Rec1 labels
	HI3593.initReceiver2Labels();     // initial Rec2 labels
	HI3593.MultiByteRead(REC1MEMORY, 32, LabelsArrayTX); // read in all Rec-1 labels into the passed array
	HI3593.MultiByteRead(REC2MEMORY, 32, LabelsArrayRX); // read in all Rec-2 labels into the passed array

	Rec1Parity =0;
	Rec2Parity =0;

	HI3593.W_CommandValue(REC1CR,RFLIP | PLON | LABREC | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
	HI3593.W_CommandValue(REC2CR,RFLIP | PLON | LABREC | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
	usb.println("\n\r on Table labels are Enabled");

	
	HI3593.InitPriorityLabels();
	
	
	
	// Prepare the first message in the transmit buffer with [00-00-00-default label]
	TXBuffer[0] = BigCounter.PayLoad[1];
	TXBuffer[1] = BigCounter.PayLoad[2];
	TXBuffer[2] = BigCounter.PayLoad[3];
	TXBuffer[3] = ARINCLabel;            // normal default label
	
	usb.println("\n\r\nPress SW-1 to start initially\n\r\n");
	usb.println("--- Button Commands ---\n\r");
	usb.println("  Space Bar to Display Status and Control Registers\n\r");
	usb.println("  p to Pause\n\r");
	usb.println("  m to Display Label Memory");
	ConsoleCommandsHelp();
	usb.println("\n\r>");
	while(gpio_get_pin_level(SW0));
	while(!gpio_get_pin_level(SW0));
	delay_ms(K_1SEC);              // blink for 1 sec
	//g_ledFlashBool=OFF;
	gpio_set_pin_level(LED0,false);
	//LED_CTL(LED_7,OFF);     // Turn off LED-7. Will use for PL-3 indication
	
	usb.println("\n\rTransmitting\n\r>");
	arincTimer.start();
	
	for(;;) {     // TEST Transmit loop
			ch=(char)usb.read();
			//usb.println("\n\rTransmitting1\n\r>");
		do {
			
			//usb.println("\n\rTransmitting2\n\r>");
			if(ch==' ')                         // print out the status registers if
			// SW3 pressed or spacebar from PC
			{
				ch=0xff;
				usb.println("\n\rHI-3593 Status, Control Registers\r\n");
			
				printStatusRegisters();                  // print out the status registers
			
				usb.println("\n\r\t  ***** Press  spacebar to continue ******");
				
				while(' ' != ch){ch=xgetchar();};        // start again if SW-4 pressed/spacebar
				ch=0x00;
			}
			
			if( ch=='p'|| TxPaused){  
				ch=0xff;                  // pause transmitting
				debounceButton(SW0);                // debounce this button
				usb.println("\n\n\rTransmit Paused\n\r");
				usb.println("\n\rPress OLED_BUTTON-2 to transmit one frame and pause\n\r");
				usb.println("\n\rPress OLED_BUTTON-3 to Resume\n\r");

				TxPaused=YES;
				while(gpio_get_pin_level(SW0))                          // wait for button-2 pressed again
				{
					if(!gpio_get_pin_level(SW0))
					{
						TxPaused=NO;
						usb.println("\n\rTransmitter Resumed...\n\r");
						break;
					}
				};
				debounceButton(SW0);         // debounce this button
			}
			
			if(ch=='m' ){     // print out the Label memory for both receivers if SW3 pushed
				ch=0xff;
 				usb.println("\n\rReceiver 1 Labels\n\r");
				PrintLabelsOnConsole(LabelsArrayTX);
			//	PrintOctalLabelsOnConsole(LabelsAr1);
				//crlf();
				
				usb.println("\n\rReceiver 2 Labels\n\r");
				PrintLabelsOnConsole(LabelsArrayRX);
				usb.println("\n\rPress OLED_BUTTON-3 to Resume\n\r");
				
				while(gpio_get_pin_level(SW0));                      // resume if Button-4 pressed or spacebar again
				usb.println("\n\rTransmitter Resumed...\n\r");
			}
			
			cpu_irq_disable();
			Status_F = HI3593.R_Register(R_TSR);      // Transmit control register
			cpu_irq_enable();
		} while (Status_F & TFFULL);      // Wait until FIFO not full before loading next byte

		if(ConsoleCommands(ch)){              // 1 second delay to see the message on the console
			delay_ms(K_1SEC);
		}
		
		cpu_irq_disable();
		HI3593.TransmitCommandAndData(TXFIFO,TXBuffer);
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
		delay_ms(K_1SEC);                      // small delay so we can see the PL LEDs flash better
		
		
	} // loop
	arincTimer.stop();
}  // Transmit Mode End


void ARINC_Interface::CommandStatus(){
	unsigned char TxPaused;
	unsigned char RXBuffer[g_RXBuffSize];       // Temp buffer to hold messages data
	unsigned char RXBufferPL[g_RXBuffSize];     // Temp buffer to hold PL messages data
	unsigned char LabelsAr1[32];                // All Rec1 256 labels
	unsigned char LabelsAr2[32];                // All Rec2 256 labels

	
	
	char ch;                   // console character


	TxPaused=0;                // start out not paused
	BigCounter.Word32 = 0;     // clear all 8 bytes of the data field.
	MessageCount=0;
	ARINCLabel = 0x30;         // Write ARINC Label
	Arate=0;
	
	usb.println("\n\rTransmit & Receive Echo Mode\n\r");
	
	if(!BITRATE0)
	{
		Arate = 1;              // BITRATE0 open so set for slow speed
		usb.println("\n\rSlow Speed");
	} else
	{
		usb.println("\n\rFast Speed");
	}
	
	if(PARITY)
	{
		Arate |= TRANSMIT_PARTY_ON << 2;   // enable the transmitter parity
		Arate |= PARITYODD << 3;           // select ODD for now. Expect compiler dead warning if 0.
		usb.println("\n\rTransmitter Parity Enabled");
	}

	//Initilize the TCR and the ACLK Divisor and look for the expected value in the TCR status reg
	if(TSR_Default != HI3593.Init3593(ACLKDIV,  // ARINC clock divisor
	TMODE,    // Transmit mode. Set for "send as soon as possible"
	SELFTEST_OFF, // Selftest defined by Mode switches
	Arate,    // Arinc speed and if Parity is enabled by the switch
	TFLIP ))
	{
		//LED_CTL(LED_8,ON);              // turn on RED LED if failed
		usb.println("\n\rHI-3593 failed to initilize\n\r");
		for(;;);
	}

	(void)memset(TXBuffer, 0, sizeof(TXBuffer));                   // clear the buffer

	//   W_Command(0x48);  // set all bits in label memory to 1
	//   W_Command(0x4C);  // set all bits in label memory to 1
	
	// Setup the Labels
	HI3593.initReceiver1Labels();     // initial Rec1 labels
	HI3593.initReceiver2Labels();     // initial Rec2 labels
	HI3593.MultiByteRead(REC1MEMORY, 32, LabelsAr1); // read in all Rec-1 labels into the passed array
	HI3593.MultiByteRead(REC2MEMORY, 32, LabelsAr2); // read in all Rec-2 labels into the passed array

	Rec1Parity =0;
	Rec2Parity =0;


	HI3593.W_CommandValue(REC1CR,RFLIP | PLON | LABREC | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
	HI3593.W_CommandValue(REC2CR,RFLIP | PLON | LABREC | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
	usb.println("\n\rTable labels are Enabled");

	
	// Priorty Labels
	TXBuffer[0] =  PL13;   // filter 3
	TXBuffer[1] =  PL12;   // filter 2
	TXBuffer[2] =  PL11;   // filter 1
	HI3593.TransmitCommandAndData(W_PL1Match,TXBuffer);  // initialize the Receiver1 PL match registers
	
	TXBuffer[0] =  PL23;   // filter 3
	TXBuffer[1] =  PL22;   // filter 2
	TXBuffer[2] =  PL21;   // filter 1
	HI3593.TransmitCommandAndData(W_PL2Match,TXBuffer);  // initialize the Receiver2 PL match registers
	
	
	// Prepare the first message in the transmit buffer with [00-00-00-default label]
	TXBuffer[0] = BigCounter.PayLoad[1];
	TXBuffer[1] = BigCounter.PayLoad[2];
	TXBuffer[2] = BigCounter.PayLoad[3];
	TXBuffer[3] = ARINCLabel;            // normal default label
	
	usb.println("\n\r\nPress SW-1 to start initially\n\r\n");
	usb.println("--- Button Commands ---\n\r");
	usb.println("  Space Bar to Display Status and Control Registers\n\r");
	usb.println("  p to Pause\n\r");
	usb.println("  m to Display Label Memory");
	ConsoleCommandsHelp();
	usb.println("\n\r>");
	while(gpio_get_pin_level(SW0));
	while(!gpio_get_pin_level(SW0));
	delay_ms(K_1SEC);              // blink for 1 sec
	//g_ledFlashBool=OFF;
	gpio_set_pin_level(LED0,false);
	//LED_CTL(LED_7,OFF);     // Turn off LED-7. Will use for PL-3 indication
	
	usb.println("\n\rTransmitting\n\r>");
	arincTimer.start();
	
	for(;;) {     // TEST Transmit loop
		ch=(char)usb.read();
		//usb.println("\n\rTransmitting1\n\r>");
		do {
			
			//usb.println("\n\rTransmitting2\n\r>");
			if(ch==' ')                         // print out the status registers if
			// SW3 pressed or spacebar from PC
			{
				ch=0xff;
				usb.println("\n\rHI-3593 Status, Control Registers\r\n");
				
				printStatusRegisters();                  // print out the status registers
				
				usb.println("\n\r\t  ***** Press  spacebar to continue ******");
				
				while(' ' != ch){ch=xgetchar();};        // start again if SW-4 pressed/spacebar
				ch=0x00;
			}
			
			if( ch=='p'|| TxPaused){
				ch=0xff;                  // pause transmitting
				debounceButton(SW0);                // debounce this button
				usb.println("\n\n\rTransmit Paused\n\r");
				usb.println("\n\rPress OLED_BUTTON-2 to transmit one frame and pause\n\r");
				usb.println("\n\rPress OLED_BUTTON-3 to Resume\n\r");

				TxPaused=YES;
				while(gpio_get_pin_level(SW0))                          // wait for button-2 pressed again
				{
					if(!gpio_get_pin_level(SW0))
					{
						TxPaused=NO;
						usb.println("\n\rTransmitter Resumed...\n\r");
						break;
					}
				};
				debounceButton(SW0);         // debounce this button
			}
			
			if(ch=='m' ){     // print out the Label memory for both receivers if SW3 pushed
				ch=0xff;
				usb.println("\n\rReceiver 1 Labels\n\r");
				PrintLabelsOnConsole(LabelsAr1);
				//	PrintOctalLabelsOnConsole(LabelsAr1);
				//crlf();
				
				usb.println("\n\rReceiver 2 Labels\n\r");
				PrintLabelsOnConsole(LabelsAr2);
				usb.println("\n\rPress OLED_BUTTON-3 to Resume\n\r");
				
				while(gpio_get_pin_level(SW0));                      // resume if Button-4 pressed or spacebar again
				usb.println("\n\rTransmitter Resumed...\n\r");
			}
			
			cpu_irq_disable();
			Status_F = HI3593.R_Register(R_TSR);      // Transmit control register
			cpu_irq_enable();
		} while (Status_F & TFFULL);      // Wait until FIFO not full before loading next byte

		if(ConsoleCommands(ch)){              // 1 second delay to see the message on the console
			delay_ms(K_1SEC);
		}
		
		cpu_irq_disable();
		HI3593.TransmitCommandAndData(TXFIFO,TXBuffer);
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
		delay_ms(K_1SEC);                      // small delay so we can see the PL LEDs flash better
		
		
	} // loop
	arincTimer.stop();
}  // Transmit Mode End


	

char	ARINC_Interface::xgetchar(){
	char ch=(char)usb.read();
	return ch;

}
void	ARINC_Interface::xgetchar(char *ch){
	
	usb.read(ch);
}


void ARINC_Interface::CE_Low_Holt(void)
{

	//PORT->Group[0].OUTCLR.reg = (1UL << 27);
	gpio_set_pin_level(CS_Holt,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);
}
void ARINC_Interface::CE_High_Holt(void)
{

	//PORT->Group[0].OUTCLR.reg = (1UL << 27);
	gpio_set_pin_level(CS_Holt,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);
}
void ARINC_Interface::ConsoleCommandsHelp(void)
{
	usb.println("\n\r\n--- Console Commands Help ---");
	usb.println("  P = Parity Toggle on/off");
	usb.println("  T = TFLIP  Toggle on/off");
	usb.println("  R = RFLIP  Toggle on/off");
	usb.println("  S = SDON, SD9, SD10 Toggle on/off");
	usb.println("  H = This Help Page");
	usb.println("  Space Bar = Display Status and Control regs");
	usb.println("  I = Increment Message and Transmit. Explorer Mode  - ONLY!");
	usb.println("  M = Enter Custom Message. Explorer Mode  - ONLY!");
	usb.println("  ENTER= Transmit next message. Explorer Mode  - ONLY!");
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
	
	cpu_irq_disable();
	getRegStatus();
	cpu_irq_enable();
	for(i=0; i< 14;i++) {
	//	p=DebugArrayStrings[i];
		char *ptr=DebugArrayStrings[i];
	//	char *ptr=p;
		usb.println(ptr);
		PrintHexByte(DebugArray[i]);  // Output the status registers
		//crlf();
	}
	
}
void	ARINC_Interface::crlf(void){
	uint8_t p=0x0A;
	usb.write(&p,1);
	p=0x0D;
	usb.write(&p,1);
}
void ARINC_Interface::PrintHexByte(uint8_t c)
{
	usb<<"0x ";
	usb.println(c,HEX);
	
}

void ARINC_Interface::PrintOctals(uint8_t c)
{

	uint32_t	octal;
	octal=Hex2Octal(c);
	char asciiOctal[]="&x000 ";
	itoa((int)octal,&asciiOctal[2],10);


	usb.println(&asciiOctal[0]);
	
}

void ARINC_Interface::getRegStatus(void){
	uint8_t priorityMatchLabels3[3];
	
	DebugArray[0] =HI3593.R_Register(0x90);     // Fetch Receiver 1 Status Reg
	DebugArray[1] =HI3593.R_Register(0x94);     // Fetch Receiver 1 Control Reg
	
	DebugArray[2] =HI3593.R_Register(0xB0);     // Fetch Receiver 2 Status Reg
	DebugArray[3] =HI3593.R_Register(0xB4);     // Fetch Receiver 2 Control Reg
	
	DebugArray[4] = HI3593.R_Register(0x80);     // Fetch Transmit Status Register
	DebugArray[5] = HI3593.R_Register(0x84);     // Fetch Transmit Control Register
	
	DebugArray[6] = HI3593.R_Register(0xD0);     // Fetch Read Flag/ Interrupt Register
	DebugArray[7] = HI3593.R_Register(0xD4);     // Fetch ACLK Divsion Register
	
	HI3593.MultiByteRead(0x9C,3, priorityMatchLabels3);     // Fetch Priority Match Labels
	DebugArray[8] = priorityMatchLabels3[2];         // Fetch Priority Match Label
	DebugArray[9] = priorityMatchLabels3[1];         // Fetch Priority Match Label
	DebugArray[10] = priorityMatchLabels3[0];        // Fetch Priority Match Label
	
	HI3593.MultiByteRead(0xBC,3, priorityMatchLabels3);     //  Fetch Priority Match Labels
	DebugArray[11] = priorityMatchLabels3[2];        //  Fetch Priority Match Label
	DebugArray[12] = priorityMatchLabels3[1];        //  Fetch Priority Match Label
	DebugArray[13] = priorityMatchLabels3[0];        //  Fetch Priority Match Label

}
void  ARINC_Interface::debounceButton(uint8_t button)
{
	uint8_t debounceCnt;
	
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
bool   ARINC_Interface::ConsoleCommands(char ch)
{
	unsigned char TCRTemp, RCR1Temp, RCR2Temp;
	
	if(ch=='P' || ch=='p'|| ch=='R' || ch=='r' || ch=='S' || ch=='s' ||ch=='T' || ch=='t' || ch=='H' || ch=='h')
	{
		TCRTemp =  HI3593.R_Register(R_TCR);    // Read the TCR
		RCR1Temp = HI3593.R_Register(R_RCR1);   // Read the RCR1
		RCR2Temp = HI3593.R_Register(R_RCR2);   // Read the RCR2
		
		if(ch=='P' || ch=='p')           // Toggle the ODD/EVEN parity bit in the TCR reg
		{
			TCRTemp ^= 4;                 // Toggle the TX parity bit
			if(TCRTemp & 4)
			usb.println("TX Parity ON\n\r>");
			else
			usb.println("TX Parity OFF\n\r>");
			HI3593.W_CommandValue(TCR, TCRTemp);
		}

		if(ch=='T' || ch=='t')          // Toggle the TFLIP bit in the TCR reg
		{
			TCRTemp ^= 0x40;             // Toggle the TFLIP bit
			if(TCRTemp & 0x40)
			usb.println("TFLIP Set\n\r>");
			else
			usb.println("TFLIP Cleared\n\r>");
			HI3593.W_CommandValue(TCR, TCRTemp);
		}
		
		if(ch=='R' || ch=='r')           // Toggle the RFLIP bit in the RCR1/RCR2 regs
		{
			RCR1Temp ^= 0x80;             // Toggle the RFLIP bit
			RCR2Temp ^= 0x80;             // Toggle the RFLIP bit
			if(RCR1Temp & 0x80)
			usb.println("REC-1 RFLIP Set\n\r>");
			else
			usb.println("REC-1 RFLIP Cleared\n\r>");
			HI3593.W_CommandValue(REC1CR, RCR1Temp);
			if(RCR2Temp & 0x80)
			usb.println("REC-2 RFLIP Set\n\r>");
			else
			usb.println("REC-2 RFLIP Cleared\n\r>");
			HI3593.W_CommandValue(REC2CR, RCR2Temp);
		}
		
		if(ch=='S' || ch=='s')          // Toggle the SDON bit in the RCR1/RCR2 regs
		{
			RCR1Temp ^= 0x70;             // Toggle the SDON, SD9, SD10 bits
			RCR2Temp ^= 0x70;             // Toggle the SDON, SD9, SD10 bits
			if(RCR1Temp & 0x10)
			usb.println("REC-1 SDON,SD9 and SD10 Set\n\r>");
			else
			usb.println("REC-1 SDON,SD9 and SD10 Cleared\n\r>");
			HI3593.W_CommandValue(REC1CR, RCR1Temp);
			if(RCR2Temp & 0x10)
			usb.println("REC-2 SDON,SD9 and SD10 Set\n\r>");
			else
			usb.println("REC-2 SDON,SD9 and SD10 Cleared\n\r>");
			HI3593.W_CommandValue(REC2CR, RCR2Temp);
		}
		
		if(ch=='H' || ch=='h')            // Print out the help screen
		{
			ConsoleCommandsHelp();
			usb.println("\n\r\nPress Space Bar or SW-4 to resume");
			while(SW0 && (' ' != ch)){ch=xgetchar();};        // start again if Button-4 pressed/spacebar
			usb.println();
		}
		

	} else
	{                                  // No command to process
		return false;
	}
	return true;
	
}

void ARINC_Interface::PrintLabelsOnConsole(unsigned char *labelarray)
{
	uint8_t i;
	
	usb.println("Labels  0 - 63:  ");
	for(i=31; i != 0xFF;i--) {
		//usb.println("0x");
		PrintHexByte(labelarray[i]);          // Output the frame data
		usb.println(" ");
		if(i==24)
		usb.println("\n\rLabels  64-127:  ");
		if(i==16)
		usb.println("\n\rLabels 128-191:  ");
		if(i==8)
		usb.println("\n\rLabels 192-255:  ");
	}
	
}
void ARINC_Interface::PrintOctalLabelsOnConsole(unsigned char *labelarray)
{
	uint8_t i;
	
	usb.println("\n\rLabels  0 - 63:  ");
	for(i=31; i != 0xFF;i--) {
		//usb.println("0x");
		PrintOctals(labelarray[i]);          // Output the frame data
		usb.println(" ");
		if(i==24)
		usb.println("\n\rLabels  64-127:  ");
		if(i==16)
		usb.println("\n\rLabels 128-191:  ");
		if(i==8)
		usb.println("\n\rLabels 192-255:  ");
	}
	
}

void ARINC_Interface::printARINCTXData(unsigned char *array) {
	unsigned char i;
	
	usb.println("\n\r>TX    Data:");
	for(i=0; i< 3;i++) {
		PrintHexByte(array[i]);          // Output the data
	}
	usb.println("Label:");
	PrintHexByte(array[3]);             // Output the label data
	
	//crlf();
	usb.println(">");
}

void	ARINC_Interface::FetchAllMessagesReceiver1(void){
	cpu_irq_disable();
	Status_F = HI3593.R_Register(RXSTATUS_1);                       // Poll Receiver1 status register
	cpu_irq_enable();

	if((Status_F & FFEMPTY) == 0)
	{
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_1,RXBuffer1 );
		(void)memcpy(receiverBuffer1[MessageCount1],RXBuffer1,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		RollMessageCount(1);
	}
	for (i = 0; i < 3; i++)
	{
		if(Status_F & (PL1<<(i)))
		{
			cpu_irq_disable();
			HI3593.ArincRead(RXFIFO_1L1+(4*i),RXBufferPL1 );
			(void)memcpy(receiverBuffer1[MessageCount1],RXBufferPL1,g_RXBuffSize);  // copy frame to large array for safe keeping
			cpu_irq_enable();
			RollMessageCount(1);
		}
	}
	// Receiver1 Priority Labels
	newMessageR1=false;
}
void	ARINC_Interface::FetchAllMessagesReceiver2(void){
	cpu_irq_disable();
	Status_F = HI3593.R_Register(RXSTATUS_2);                       // Poll Receiver1 status register
	cpu_irq_enable();

	if((Status_F & FFEMPTY) == 0)
	{
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_2,RXBuffer2 );
		(void)memcpy(receiverBuffer2[MessageCount2],RXBuffer2,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		RollMessageCount(2);
	}

	for (i = 0; i < 3; i++)
	{
		if(Status_F & (PL1<<(i)))
		{
			cpu_irq_disable();
			HI3593.ArincRead(RXFIFO_2L1+(4*i),RXBufferPL2 );
			(void)memcpy(receiverBuffer2[MessageCount2],RXBufferPL2,g_RXBuffSize);  // copy frame to large array for safe keeping
			cpu_irq_enable();
			RollMessageCount(2);
		}
	}
	// Receiver1 Priority Labels
	newMessageR2=false;
}

void ARINC_Interface::FetchAllMessagesAndDisplay(unsigned char *RXBuff,unsigned char *RXBuffPL)
{
	
	
	//LED_CTL(LED_5,OFF);     // Turn off PL-1 LED
//	LED_CTL(LED_6,OFF);     // Turn off PL-2 LED
//	LED_CTL(LED_7,OFF);     // Turn off PL-3 LED
	
	// Recever -1
	cpu_irq_disable();
	Status_F = HI3593.R_Register(RXSTATUS_1);                       // Poll Receiver1 status register
	cpu_irq_enable();

	if((Status_F & FFEMPTY) == 0)
	{
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_1,RXBuff );
		(void)memcpy(receiverBuffer[MessageCount],RXBuff,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_HEADER,RXBuff);
		CheckMessageCountMax();
	}

	// Receiver1 Priority Labels
	if(Status_F & PL1)
	{
		//LED_CTL(LED_5,ON);               // Turn on LED5 for PL1
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_1L1,RXBuffPL );
		(void)memcpy(receiverBuffer[MessageCount],RXBuffPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_L1_HEADER,RXBuffPL);
		CheckMessageCountMax();
	}

	// Receiver1 Priority Label 2
	if(Status_F & PL2)
	{
		//LED_CTL(LED_6,ON);               // Turn on LED6 for PL2
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_1L2,RXBuffPL );
		(void)memcpy(receiverBuffer[MessageCount],RXBuffPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_L2_HEADER,RXBuffPL);
		CheckMessageCountMax();
	}
	// Receiver1 Priority Label 3
	if(Status_F & PL3)
	{
		//LED_CTL(LED_7,ON);               // Turn on LED7 for PL3
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_1L3,RXBuffPL );
		(void)memcpy(receiverBuffer[MessageCount],RXBuffPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC1_L3_HEADER,RXBuffPL);
		CheckMessageCountMax();
	}
	
	// Recever -2
	cpu_irq_disable();
	Status_F = HI3593.R_Register(RXSTATUS_2);
	cpu_irq_enable();
	if((Status_F & FFEMPTY) == 0)
	{
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_2,RXBuff );
		(void)memcpy(receiverBuffer[MessageCount],RXBuff,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_HEADER,RXBuff);
		CheckMessageCountMax();
	}

	// Receiver2 Priority Label 1
	if(Status_F & PL1)
	{
		//LED_CTL(LED_5,ON);               // Turn on LED5 for PL1
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_2L1,RXBuffPL );
		(void)memcpy(receiverBuffer[MessageCount],RXBuffPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_L1_HEADER,RXBuffPL);
		CheckMessageCountMax();
	}
	// Receiver2 Priority Label 2
	if(Status_F & PL2)
	{
		//LED_CTL(LED_6,ON);               // Turn on LED6 for PL2
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_2L2,RXBuffPL );
		(void)memcpy(receiverBuffer[MessageCount],RXBuffPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_L2_HEADER,RXBuffPL);
		CheckMessageCountMax();
	}
	// Receiver2 Priority Label 3
	if(Status_F & PL3)
	{                                // Turn on LED7 for PL3
		//LED_CTL(LED_7,ON);
		cpu_irq_disable();
		HI3593.ArincRead(RXFIFO_2L3,RXBuffPL );
		(void)memcpy(receiverBuffer[MessageCount],RXBuffPL,g_RXBuffSize);  // copy frame to large array for safe keeping
		cpu_irq_enable();
		printARINCData(REC2_L3_HEADER,RXBuffPL);
		CheckMessageCountMax();
	}
	
}
/*

void ARINC_Interface::LED_CTL(uint8_t ledNumber, uint8_t OnOff){
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

uint8_t	ARINC_Interface::RollMessageCount(uint8_t mc){
	switch (mc)
	{
		case 0:
			CheckMessageCountMax();
		/* Your code here */
		break;
		case 1:
			MessageCount1=(MessageCount1+1)%(MESSAGECOUNTMAX/2);
		/* Your code here */
		break;
		case 2:
			MessageCount2=(MessageCount2+1)%(MESSAGECOUNTMAX/2);
		/* Your code here */
		break;
		default:
		/* Your code here */
		break;
	}
	return	MessageCount;
}
void ARINC_Interface::printARINCData(const uint8_t channel, unsigned char *array) {
	unsigned char i;

	
	switch (channel)
	{
		case REC1_HEADER:
		usb.println("Rec-1 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the data
		}
		usb.println("Label:");
		PrintHexByte(array[3]);          // Output the label data
		break;
		case REC1_L1_HEADER:
		usb.println("PL1-1 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the  data
		}
		usb.println("Match:");
		PrintHexByte(PL11);              // Output the Match label
		
		break;
		case REC1_L2_HEADER:
		usb.println("PL1-2 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the data
		}
		usb.println("Match:");
		PrintHexByte(PL12);              // Output the Match label
		
		break;
		case REC1_L3_HEADER:
		usb.println("PL1-3 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the data
		}
		usb.println("Match:");
		PrintHexByte(PL13);              // Output the Match label
		
		break;
		
		case REC2_HEADER:
		usb.println("Rec-2 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);        // Output the data
		}
		usb.println("Label:");
		PrintHexByte(array[3]);          // Output the Match label
		break;
		case REC2_L1_HEADER:
		usb.println("PL2-1 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);       // Output the  data
		}
		usb.println("Match:");
		PrintHexByte(PL21);              // Output the Match label
		
		break;
		
		case REC2_L2_HEADER:
		usb.println("PL2-2 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);        // Output the  data
		}
		usb.println("Match:");
		PrintHexByte(PL22);              // Output the Match label
		
		break;
		case REC2_L3_HEADER:
		usb.println("PL2-3 Data:");
		for(i=0; i< 3;i++) {
			PrintHexByte(array[i]);        // Output the  data
		}
		usb.println("Match:");
		PrintHexByte(PL23);              // Output the Match label
		
		break;
		
		
		default:
		break;
	};
	
	//crlf();
	usb.println("\n\r");
	usb.println(">");
}

void ARINC_Interface::HW_RESET(void){
		
	//gpio_set_pin_level(MR,true);
	delay_us(1);
	//gpio_set_pin_level(MR,false);
}


void ARINC_Interface::CustomMessage(const uint8_t SELFTEST){

	uint32_t	reading;

	bool					done=false;

	
	

	char ch,chL,chR;                    // console character
	uint8_t ARINCLabel;
	uint8_t i;
	

	BigCounter.Word32 = 0;     // clear all 8 bytes of the data field.
	MessageCount=0;
	ARINCLabel = 0x1D;         // Write ARINC Label^^^^^^
	Arate=0;
	usb.println("\n\rExplorer Mode\n\r");
	if(SELFTEST)
	usb.println("\n\rSELF-TEST, Internal Digital Loop-back, No data is transmitted externally\n\r");
	

	(void)memset(TXBuffer, 0, sizeof(TXBuffer));    // clear the buffer
	
	// Setup the Labels
	HI3593.initReceiver1Labels();     // initial Rec1 labels
	HI3593.initReceiver2Labels();     // initial Rec2 labels
	HI3593.MultiByteRead(REC1MEMORY, 32, LabelsArrayTX); // read in all Rec-1 labels into the passed array
	HI3593.MultiByteRead(REC2MEMORY, 32, LabelsArrayRX); // read in all Rec-2 labels into the passed array

	Rec1Parity =0;
	Rec2Parity =0;


	HI3593.W_CommandValue(REC1CR,RFLIP | PLON | LABREC | Arate | Rec1Parity);  // enable PL1 in the Receiver Control Reg
	HI3593.W_CommandValue(REC2CR,RFLIP | PLON | LABREC | Arate | Rec2Parity);  // enable PL2 in the Receiver Control Reg
	usb.println("\n\r labels Table is Enabled");

	HI3593.InitPriorityLabels();
	
	// Prepare the first message in the transmit buffer with [00-00-00-default label]
	TXBuffer[0] = BigCounter.PayLoad[1];
	TXBuffer[1] = BigCounter.PayLoad[2];
	TXBuffer[2] = BigCounter.PayLoad[3];
	TXBuffer[3] = ARINCLabel;            // normal default label

	usb.println("\n\r\n--- Button Commands ---");
	usb.println("\n\r  spacebar to Display Status and Control Registers");
	usb.println("\n\r  OLED-BUTTON2 transmit one message from transmit buffer");
	usb.println("\n\r  L to Print Label Memory\n\r");
	
	
	ConsoleCommandsHelp();
	usb.println("\n\rPress ENTER to Transmit or use the (M) command to enter a custom message");
	usb.println("\n\r>");
	ch=0xff;
	for(;;) {     // TEST Transmit loop
		
		xgetchar(&ch);
		do {
			if(ch==' ')                         // print out the status registers if
			// SW3 pressed or spacebar from console
			{	
				ch=0xff;
				usb.println("\n\rHI-3593 Status, Control Registers\r\n");
			
				printStatusRegisters();                  // print out the status registers
	
				usb.println("Press SW4 or spacebar to continue");
				while(' ' != ch){ch=xgetchar();};      // start again if Button-4 pressed/spacebar
				ch=0xff;
				usb.println("\n\r>");
			}
			
			
			if(ch=='l'||ch=='L'){     // print out the Label memory for both receivers if SW3 pushed
				ch=0xff;
				usb.println("\n\rReceiver 1 Labels\n\r");
				PrintLabelsOnConsole(LabelsArrayTX);
				crlf();
				
				usb.println("\n\rReceiver 2 Labels\n\r");
				PrintLabelsOnConsole(LabelsArrayRX);
				usb.println("Press spacebar to continue");
				
				while(' ' != ch){ch=xgetchar();};      // start again if Button-4 pressed/spacebar
				ch=0xff;
				usb.println("\n\r>");
			}
			
			cpu_irq_disable();
			Status_F =HI3593.R_Register(R_TSR);   // Transmit control register
			cpu_irq_enable();
		} while (Status_F & TFFULL);      // Wait until FIFO not full before loading next byte

		
		(void)ConsoleCommands(ch);             // check for on the fly console commands
		
		// check for a M command to transmit a custom message
		if(ch=='m' || ch=='M')
		{
			ch=0xff;
			usb.println("\n\r>Command M");
			usb.println("\n\rEnter 3 bytes + LABEL to Transmit followed by ENTER\n\r");
			usb.println("\n\rTo Transmit, enter 3 bytes followed by a Label byte followed by ENTER\n\r");
			usb.println("Format: Bits32-25, Bits24-17, Bits16-9, LABEL\n\r:");
			usb.println("Example: 01 23 45 67 ENTER\n\r:");
			
			for(i = 0; i < 4; i++)   // Read in 8 characters from the console to make 4 bytes
			{
				
				do{
					
					done=false;
					chL=xgetchar();
					//while(!usb.rxDone);
					for (int i=0;i<(int)sizeof(HexSymbols);i++)
					{
						if (HexSymbols[i]==chL)
						{
							done=true;
							break;
						}
					}
		
			
				}while(( chL == LF) || (chL == CR) || (chL == SPACE) || (chL==EOF)|| (chL==TAB)||(!done));
				xputc(chL);
				
				do{
					
					done=false;
					chR=xgetchar();
				//	while(!usb.rxDone);
					for (int i=0;i<(int)sizeof(HexSymbols);i++)
					{
						if (HexSymbols[i]==chR)
						{
							done=true;
							break;
						}
					}
					
				}while(( chR == LF) || (chR== CR) || (chR == SPACE) || (chR==EOF)|| (chL==TAB)||(!done));
				
				xputc(chR);
				ch=AtoHex2(chL,chR);
				TXBuffer[i] = ch;
				space();
				chL=0xff;
				chR=0xff;
				
			}
			usb.println(" Press RETURN");
			do{
				chL=xgetchar();
		
			}while(chL!=CR);
			usb.println(" Transmiting...");
			cpu_irq_disable();
			HI3593.TransmitCommandAndData(TXFIFO,TXBuffer);
			cpu_irq_enable();
			usb.println(" Transmited!");
			usb.println(">");
			printARINCTXData(TXBuffer);                     // print the TX data written to the TXFIFO
		//	PORTT= ~TXBuffer[3];                            // show the transmitted upper nibble of the LSB

		} // M command
		
		// if a Enter was pressed just transmit the last message in the buffer
		if(ch==CR || !gpio_get_pin_level(SW0) )
		{
				ch=0xff;
			cpu_irq_disable();
			HI3593.TransmitCommandAndData(TXFIFO,TXBuffer);
			cpu_irq_enable();
			printARINCTXData(TXBuffer);                        // print the TX data written to the TXFIFO
		//	PORTT= ~TXBuffer[3];                               // show the transmitted upper nibble of the LSB
			BigCounter.PayLoad[1] = TXBuffer[0];               // synchronise the Big counter with the new value
			BigCounter.PayLoad[2] = TXBuffer[1];
			BigCounter.PayLoad[3] = TXBuffer[2];
			BigCounter.PayLoad[4] = TXBuffer[3];
		//	PORTA |= 0x80;                                     // pulse high for scope
		//	PORTA &= 0x7F;
			debounceButton(SW0);                           // debounce this button
		}
		
		// Increment the data if "I" pressed
		if(ch=='I' || ch=='i')
		{
				ch=0xff;
			usb.println("Command I: Increment data\n\r>");
			BigCounter.Word32++;
			TXBuffer[0] = BigCounter.PayLoad[1];
			TXBuffer[1] = BigCounter.PayLoad[2];
			TXBuffer[2] = BigCounter.PayLoad[3];
			TXBuffer[3] = BigCounter.PayLoad[3];               // copy the LSB to the label for testing purposes
			
			cpu_irq_disable();
			HI3593.TransmitCommandAndData(TXFIFO,TXBuffer);
			cpu_irq_enable();
			printARINCTXData(TXBuffer);                        // print the TX data written to the TXFIFO
			//PORTT= ~TXBuffer[3];                               // show the transmitted upper nibble of the LSB
		}
		
		
		FetchAllMessagesAndDisplay(RXBuffer,RXBufferPL);   // Fetch all Normal and Priority Messages into                                            // the arrays via buffer pointers
		
		if (newMessageR1)
		{
			reading=ReadArincBuffer();
			newMessageR1=false;
		}
	}
	
}  // Transmit Custom Mode End

void  ARINC_Interface::space(void){
	xputc(0x20);
}
void	ARINC_Interface::xputc(char byte) {
	uint8_t p[]="0";
	p[0]=uint8_t(byte);
	
	usb.write(&p[0],sizeof(p));
}

ARINC_Interface	arinc;