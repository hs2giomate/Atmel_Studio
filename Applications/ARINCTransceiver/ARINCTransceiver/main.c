/*       Main.c
 *       Holt HI-3593 Demonstration Program 
 *
 *
 *      	This file demonstrates a simple ANSI C program structure for 
 *			the initializing and using a Holt HI-3593 device.
 *
 *       Tool: Codewarrior 5.9.0 (build 2836)
 *
 *			HOLT DISCLAIMER
 *
 *			THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *			EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO WARRANTIES
 *			OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *			NONINFRINGEMENT. 
 *			IN NO EVENT SHALL HOLT, INC BE LIABLE FOR ANY CLAIM, DAMAGES
 *			OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 *			OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *			SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *			Copyright (C) 2009 - 2010 by HOLT, Inc.
 *			All Rights Reserved.
 
 *       Revision 1.0 First release 2/11/2011. WS.
 *       Revision 1.1 Correct ACLKDIV ARINC clock divisor from 8 to 4 in header file. 3/3/11. WS. 
         Revision 1.2 define PTM_PTM3 9/7/2011. WS.
 */

/*
 You may see compiler warning like these and they are consider normal:
 Warning : C12056: SP debug info incorrect because of optimization or inline assembler 
*/

#define NEWBOARD 1            // 0= old board.

#define SW_REVISION 0x12      // Software Revision to display on the LEDs and console
                              // format [Revision, Sub revision] for eg. 0x10 = Rev 1.0.
                              

#include "hidef.h"
#include "derivative.h"      /* derivative-specific definitions */
#include <string.h>
#include "main.h"
#include "3593Driver.h"
#include "BoardTest.h"
#include "Uart.h"
#include "common.h"     
#include <stdio.h>                           // to get sprintf function
   

// * Global Variables and arrays *
uint8 DebugArray[16];                        // Global array for 3110 status registers
unsigned char g_RXBuffer[64][4];             // [# of buffers][16 bytes]
unsigned char MessageCount; 
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
   
unsigned char TXBuffer [16];                 // Transmit Buffer
const char WelcomeMesg[] = {"Holt HI-3593 Demonstration."}; 
volatile unsigned int g_count100us;          // Global timer tick for delays.
volatile int g_tick100us;                    // Timer function
volatile char g_ledFlashBool;                // Global LED8 flash enable, YES or NO. 
unsigned char MODES,OPTION;


//------------------------ MAIN ------------------------------------------------
void main() {

 
g_ledFlashBool = OFF;           // Turn off the LED8 active 1 second flasher

PeriphInit();                   // initialize PLL, SPI Clocks and GPIO                

GPIO_InitInterrupts();          // initialize interrupts

Timer_Init();                  // initialize T0 for the main 100us periodic system interrupt
                               //clear all possible false interrupts just in case
PIFP_PIFP0 = 1;                //clear PPO Interrupt 
PIFP_PIFP1 = 1;                //clear PP1 Stat Interrupt 
PIFP_PIFP2 = 1;                //clear PP2  
PIFP_PIFP3 = 1;                //clear PP3   

// Display the software revision on the LEDs for 1 second
PORTT=~SW_REVISION;             // write out to LEDs1-4 
PORTE=SW_REVISION;              // write out to LEDs5-8 
LED_CTL(LED_8,OFF);

EnableInterrupts; 
Delay100us(K_1SEC);             // delay time to show the SW revision on the LEDs
Delay100us(K_1SEC);             // delay time to show the SW revision on the LEDs

LED_CTL(LED_1,OFF);             // Turn off LED1-LED8s
LED_CTL(LED_2,OFF);
LED_CTL(LED_3,OFF);
LED_CTL(LED_4,OFF);
LED_CTL(LED_5,OFF);
LED_CTL(LED_6,OFF);
LED_CTL(LED_7,OFF);
LED_CTL(LED_8,OFF);
g_ledFlashBool = ON;             // Turn ON the LED8 flasher

UartConfig();                    // configure the UART, 115200B, 8 bits, no parity
xputc(XON);                       // Ensure PC Serial can accept data
xputc(NEWPAGE);                   // clear the screen
xprint("Holt HI-3593 Demonstration Software Revision: ");
PrintOneHexNibble (SW_REVISION>>4);
xputc('.');
PrintOneHexNibble(SW_REVISION&0x0F); // print the SW sub revision

crlf();

HW_RESET();                      // issue a hardware reset to the HI-3593
 
for(;;) {
   TEST_OPTION();                // Read and save Test Option switch settings   
   MODE_SELECT();                // Read and save Mode switch settings

   switch (MODES) {              // MODES = 0-7. See Function headers for Dip Switch and SW Button options.

        case TEST_MODE:          // 0  Cycle the LEDs as a simple board test
                BoardTest();
                break;
                                
        case TRANSMIT_RECEIVE_WITH_LABELS_MODE:       // 1  Transmit an incrementing  counter value 
                TransmitReceiveWithLabels_Mode(SELFTEST_OFF);
                break;
                
        case SELF_TEST:                               // 2 SELF TEST (LoopBack Mode) Mode
                TransmitReceiveWithLabels_Mode(SELFTEST_ON);
                break; 
                       
        case HI3593EXPLORER:                          // 3  HI3593 Explorer Mode                
                HI3593Explorer(SELFTEST_OFF);
                break;

        case HI3593EXPLORER_SELFTEST:                 // 4  HI3593Explorer Mode SELFTEST 
                HI3593Explorer(SELFTEST_ON);   
                break;                             
              
        case SERIAL_COMMANDS:                         
               SerialCommands();       // USE THIS FOR SPECIAL TESTING                       
               break;
                                                               
        default:break;        
        };
      
    }; // for

}	// main end


// ------------------------------------------------------
// Serial Commands - simple menu to exercise test or special functions
// 
void SerialCommands(void)
{
   for(;;){
   
      char ch;
      xputc(NEWPAGE);

      for (;;) {               
       	xprint("\n\r ******* HOLT HI-3593 Serial Console Commands ******* \n");
       	xprint("\n\r Input String Demo                0");               	
      	xprint("\n\r Reset HI-3593 using the MR pin   1");
      	xprint("\n\r Control and Status Register Dump 2");
      	xprint("\n\r Initialize TCR, RCR1 and RCR2    3");      	
      	xprint("\n\n\r Enter Selection: ");
      
         do{ ch=xgetchar(); 
           }while(ch==EOF);
          
          xputc(ch);
          crlf();
   
                         
          switch (ch) 
            {
            case '0':       
                { 
                // This demonstrates how to read multiple in characters from the serial port.
                // This can be useful to implement additional commands which require input from the console.                     
               char buffer[40];                    
               xprint("\n\r Enter a string then press Return\n\r"); 
               (void) memset(buffer,0,sizeof(buffer));                 
               xscan(buffer);
               crlf();                                 
               xprint( "Input was: ");
               xprint(buffer);
               crlf();  
               }
               break;                                                  
            case '1':      
                {
                 HW_RESET();           
                 xprint("\n\r 1 Hardware Reset Generated\n\r");                                          
                }
                 break;
                 
               case '2': 
                  xprint("\n\rHI-3593 Status and Control Registers\r\n");
                  DisableInterrupts;
                  printStatusRegisters();       // print out the status registers
                  EnableInterrupts;                    
               break;
               
               case '3':
                  {
                  uint8 Arate=0;
                  uint8 selftest=0;
                    
                  xprint("\n\rInitialize TCR and RCR1 and RCR2\r\n");
                                 
                  if(TSR_Default != Init3593(ACLKDIV,  // ARINC clock divisor
                                             TMODE,    // Transmit mode. Set for "send as soon as possible"
                                             selftest, // Selftest defined by Mode switches
                                             Arate,    // Arinc speed and if Parity is enabled by the switch
                                             TFLIP ))                      
                     {
                        
                       LED_CTL(LED_8,ON);              // turn on RED LED if failed
                       xprint("** Transmit Control Register Failed!!! **\n\r");     
                     }                    
                   W_CommandValue(REC1CR,0x00);  // enable PL1 in the Receiver Control Reg
                   W_CommandValue(REC2CR,0xFF);  // enable PL2 in the Receiver Control Reg  
                  }
               break;       
                                   
          default: break;                 
         } // switch

      }  // for
   }   //for
}

// ------------------------------------------------------
// TRANSMIT AND RECEIVE WITH LABELS MODE 
// Available SW Button commands:
//    SW-1    Push to start the transmission/receptions initially
//
//    SW-1    Display the status registers. Press SW-4 to continue.
//    SW-2    Pause transmitting. Press SW-4 to continue.
//    SW-3    Print the label memory. Press SW-4 to continue.
//    OPT1 SW     Transmitter Parity Enable - Open=Enable. ODD parity is used.
//    OPT2 SW     Label Enable - Open=Enable Normal and PL label reception.
//
//    Bitrate0    Low Speed - Closed = high speed, Open = low speed.
//    Bitrate1    Parity Checking Receiver-1 - Open = Enable.
//    Bitrate2    Parity Checking Receiver-2 - Open = Enable. 
//    Console Commands:
//       Pressing the space bar will fetch and display the Control and Status 
//       registers on the console. Press space bar again to resume.
// ------------------------------------------------------
void TransmitReceiveWithLabels_Mode(const uint8 SELFTEST){
   static char Status_F;
   static unsigned char TxPaused; 
   static  unsigned char RXBuffer[g_RXBuffSize];       // Temp buffer to hold messages data
   static  unsigned char RXBufferPL[g_RXBuffSize];     // Temp buffer to hold PL messages data
   static  unsigned char LabelsAr1[32];                // All Rec1 256 labels
   static  unsigned char LabelsAr2[32];                // All Rec2 256 labels 
   uint8 Arate;
   uint8 Rec1Parity;
   uint8 Rec2Parity;
     
   union Buffer32 {     
      long Word32;
      unsigned char PayLoad[4];        // We will ignore the MSB byte [0]      
      };

   union Buffer32 BigCounter;
   char ch;                   // console character
   uint8 ARINCLabel;

   TxPaused=0;                // start out not paused
   BigCounter.Word32 = 0;     // clear all 8 bytes of the data field. 
   MessageCount=0;   
   ARINCLabel = 0x30;         // Write ARINC Label   
   Arate=0;
   
   xprint("\n\rTransmit & Receive With Labels Mode\n\r");
   if(SELFTEST)
    xprint("\n\rSELF-TEST, Internal Digital Loop-back, No data is transmitted externally\n\r");
   
   if(PORTB & BITRATE0)
   {
      Arate = 1;              // BITRATE0 open so set for slow speed
      xprint("\n\rSlow Speed");
   } else
   {
      xprint("\n\rFast Speed");     
   }
   
   if(PORTA & OPT1)
   {
      Arate |= TRANSMIT_PARTY_ON << 2;   // enable the transmitter parity 
      Arate |= PARITYODD << 3;           // select ODD for now. Expect compiler dead warning if 0.
      xprint("\n\rTransmitter Parity Enabled");     
   }

   //Initilize the TCR and the ACLK Divisor and look for the expected value in the TCR status reg
   if(TSR_Default != Init3593(ACLKDIV,  // ARINC clock divisor
                              TMODE,    // Transmit mode. Set for "send as soon as possible"
                              SELFTEST, // Selftest defined by Mode switches
                              Arate,    // Arinc speed and if Parity is enabled by the switch
                              TFLIP ))
      {
        LED_CTL(LED_8,ON);              // turn on RED LED if failed
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
   if(PORTA & OPT2)      // enable labels if SW OTP2 is open
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
   while(SW1);                      // wait for key press  
   while(!SW1);
   Delay100us(K_1SEC);              // blink for 1 sec
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

// ------------------------------------------------------
// HI3593Explorer Mode
// This function will transmit one message each time the Enter key is pressed
//    on the Console or if SW-2 is pressed on the board.
//
//    SW-1    Print the status registers. Press SW-4 to continue.
//    SW-3    Print the label memory. Press SW-4 to continue.
//    OPT1 SW     Transmitter Parity Enable - Open=Enable. ODD parity is used.
//    OPT2 SW     Label Enable - Open=Enable Normal and PL label reception.
//
//    Bitrate0    Low Speed - Closed = high speed, Open = low speed.
//    Bitrate1    Parity Checking Receiver-1 - Open = Enable.
//    Bitrate2    Parity Checking Receiver-2 - Open = Enable.
//
// A custom message can be entered by using the (M) command from the console.
// After pressing M or m a prompt message will be displayed. 
// Enter 8 characters representing 4 bytes of data followed by Enter to transmit
// this messaged. From this point pressing Enter will transmit this message.
//
//    Console Commands:
//       ENTER (CR) will transmit the message currently in the transmit buffer.
//       Space Bar - Pressing the space bar will fetch and display the Control and Status 
//          registers on the console. Press space bar again or press SW4 to resume.
//       M or m - Pressing M or m will provide a prompt to enter a custom data and label message.
//
// ------------------------------------------------------

void HI3593Explorer(const uint8 SELFTEST){
   static char Status_F;
   static unsigned char TxPaused; 
   static  unsigned char RXBuffer[g_RXBuffSize];       // Temp buffer to hold messages data
   static  unsigned char RXBufferPL[g_RXBuffSize];     // Temp buffer to hold PL messages data
   static  unsigned char LabelsAr1[32];                // All Rec1 256 labels
   static  unsigned char LabelsAr2[32];                // All Rec2 256 labels 
  uint8 Arate;
  uint8 Rec1Parity;
  uint8 Rec2Parity;
     
   union Buffer32 {     
      long Word32;
      unsigned char PayLoad[4];        // We will ignore the MSB byte [0]      
      };

   union Buffer32 BigCounter;
   char ch,chL,chR;                    // console character
   uint8 ARINCLabel;
   uint8 i;
   
   TxPaused=0;                // start out not paused
   BigCounter.Word32 = 0;     // clear all 8 bytes of the data field. 
   MessageCount=0;   
   ARINCLabel = 0x30;         // Write ARINC Label
   Arate=0;
   xprint("\n\rExplorer Mode\n\r");
   if(SELFTEST)
    xprint("\n\rSELF-TEST, Internal Digital Loop-back, No data is transmitted externally\n\r");   
   
   if(PORTB & BITRATE0)
   {
      Arate = 1;              // BITRATE0 open so set for slow speed
      xprint("\n\rSlow Speed");
   } else
   {
       xprint("\n\rFast Speed");     
   }
   
   if(PORTA & OPT1)
   {
      Arate |= TRANSMIT_PARTY_ON << 2;   // enable the transmitter parity 
      Arate |= PARITYODD << 3;           // select ODD for now. Expect compiler dead warning if 0.
      xprint("\n\rTransmitter Parity Enabled");     
   }


   //Initilize the TCR and the ACLK Divisor and look for the expected value in the TCR status reg
   if(TSR_Default != Init3593(ACLKDIV,  // ARINC clock divisor
                              TMODE,    // Transmit mode. Set for "send as soon as possible"
                              SELFTEST, // Selftest defined by Mode switches
                              Arate,    // Arinc speed and if Parity is enabled by the switch
                              TFLIP ))
      {
        LED_CTL(LED_8,ON);              // turn on RED LED if failed
        xprint("\n\rHI-3593 failed to initilize\n\r");
        for(;;);     
      }

   (void)memset(TXBuffer, 0, sizeof(TXBuffer));    // clear the buffer 
   
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
   if(PORTA & OPT2)      // enable labels if SW OTP2 is open
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

   xprint("\n\r\n--- Button Commands ---");               
   xprint("\n\r  SW-1 or spacebar to Display Status and Control Registers");
   xprint("\n\r  SW-2 to transmit one message from transmit buffer");     
   xprint("\n\r  SW-3 to Print Label Memory\n\r");
   
    
   ConsoleCommandsHelp();
   xprint("\n\rPress ENTER to Transmit or use the (T) command to enter a custom message");    
   xprint("\n\r>");  
                        
   for(;;) {     // TEST Transmit loop
               
    do {
     ch=xgetchar(); 
     
      if(!SW1 || ch==' ')                         // print out the status registers if
                                                  // SW3 pressed or spacebar from console                                           
         {  
         xprint("\n\rHI-3593 Status, Control Registers\r\n");
         DisableInterrupts;
         printStatusRegisters();                  // print out the status registers
         EnableInterrupts;
         xprint("Press SW4 or spacebar to continue");    
         while(SW4 && ' ' != xgetchar());        // start again if Button-4 pressed/spacebar
         xprint("\n\r>");
         } 
                                
                        
      if(!SW3 ){     // print out the Label memory for both receivers if SW3 pushed
            xprint("\n\rReceiver 1 Labels\n\r");    
            PrintLabelsOnConsole(LabelsAr1);    
            crlf();
            
            xprint("\n\rReceiver 2 Labels\n\r");    
            PrintLabelsOnConsole(LabelsAr2);    
            xprint("\n\rPress SW-4 to Resume\n\r");                  
                                     
            while(SW4);                      // resume if Button-4 pressed or spacebar again
            xprint("\n\rResumed...\n\r"); 
            xprint(">");                
          }
            
          DisableInterrupts;        
          Status_F = R_Register(R_TSR);   // Transmit control register
          EnableInterrupts;                 
        } while (Status_F & TFFULL);      // Wait until FIFO not full before loading next byte

                                          
   (void)ConsoleCommands(ch);             // check for on the fly console commands 
   
   // check for a M command to transmit a custom message
   if(ch=='m' || ch=='M')
      {
         xprint("\n\r>Command M");
//         xprint("\n\rEnter 3 bytes + LABEL to Transmit followed by ENTER\n\r");      
         xprint("\n\rTo Transmit, enter 3 bytes followed by a Label byte followed by ENTER\n\r");      
         xprint("Format: Bits32-25, Bits24-17, Bits16-9, LABEL\n\r:");
         xprint("Example: 01 23 45 67 ENTER\n\r:");         
         
          for(i = 0; i < 4; i++)   // Read in 8 characters from the console to make 4 bytes
            {                         
               do{
                  chL=xgetchar();
               }  while(( chL == LF) || (chL == CR) || (chL == SPACE) || (chL==EOF)|| (chL==TAB));        
               xputc(chL);            

               do{
                  chR=xgetchar();
               }  while(( chR == LF) || (chR == CR) || (chR == SPACE) || (chR==EOF)|| (chL==TAB)); 
                      
               xputc(chR);
               ch=atohex2(chL,chR);                 
               TXBuffer[i] = ch;
               space();
               
            } 
         xprint(" Press RETURN");     
         do{
            chL=xgetchar();                     
            }while(chL!=CR);      
         xprint(" Transmitted...\n\r\n");         
         DisableInterrupts;           
         TransmitCommandAndData(TXFIFO,TXBuffer);         
         EnableInterrupts;
         xprint(">");      
         printARINCTXData(TXBuffer);                     // print the TX data written to the TXFIFO             
         PORTT= ~TXBuffer[3];                            // show the transmitted upper nibble of the LSB           

      } // M command
                 
   // if a Enter was pressed just transmit the last message in the buffer
   if(ch==CR || !SW2 )
   {      
      DisableInterrupts;           
      TransmitCommandAndData(TXFIFO,TXBuffer);         
      EnableInterrupts;
      printARINCTXData(TXBuffer);                        // print the TX data written to the TXFIFO      
      PORTT= ~TXBuffer[3];                               // show the transmitted upper nibble of the LSB
      BigCounter.PayLoad[1] = TXBuffer[0];               // synchronise the Big counter with the new value
      BigCounter.PayLoad[2] = TXBuffer[1];
      BigCounter.PayLoad[3] = TXBuffer[2];
      BigCounter.PayLoad[4] = TXBuffer[3];      
      PORTA |= 0x80;                                     // pulse high for scope
      PORTA &= 0x7F;  
      debounceButton(BUTTON2);                           // debounce this button     
   }
   
   // Increment the data if "I" pressed
   if(ch=='I' || ch=='i')
   {      
      xprint("Command I: Increment data\n\r>");     
      BigCounter.Word32++; 
      TXBuffer[0] = BigCounter.PayLoad[1];
      TXBuffer[1] = BigCounter.PayLoad[2];
      TXBuffer[2] = BigCounter.PayLoad[3];      
      TXBuffer[3] = BigCounter.PayLoad[3];               // copy the LSB to the label for testing purposes
  
      DisableInterrupts;           
      TransmitCommandAndData(TXFIFO,TXBuffer);         
      EnableInterrupts;
      printARINCTXData(TXBuffer);                        // print the TX data written to the TXFIFO      
      PORTT= ~TXBuffer[3];                               // show the transmitted upper nibble of the LSB           
   }   
   
         
   FetchAllMessagesAndDisplay(RXBuffer,RXBufferPL);   // Fetch all Normal and Priority Messages into                                            // the arrays via buffer pointers
   

   }
 
}  // Transmit Custom Mode End

// ------------------------------------------------------
// This function allows changing the operating mode On-The-Fly by changing 
// values in either the Transmit Control Register or the Receive Control Registers
// ------------------------------------------------------ 
unsigned char ConsoleCommands(unsigned char ch)
{ 
 unsigned char TCRTemp, RCR1Temp, RCR2Temp;
   
   if(ch=='P' || ch=='p'|| ch=='R' || ch=='r' || ch=='S' || ch=='s' ||ch=='T' || ch=='t' || ch=='H' || ch=='h')
   {
      TCRTemp =  R_Register(R_TCR);    // Read the TCR
      RCR1Temp = R_Register(R_RCR1);   // Read the RCR1
      RCR2Temp = R_Register(R_RCR2);   // Read the RCR2      
             
      if(ch=='P' || ch=='p')           // Toggle the ODD/EVEN parity bit in the TCR reg
      {
         TCRTemp ^= 4;                 // Toggle the TX parity bit
         if(TCRTemp & 4)
            xprint("TX Parity ON\n\r>");
         else
            xprint("TX Parity OFF\n\r>");         
         W_CommandValue(TCR, TCRTemp);      
      }

      if(ch=='T' || ch=='t')          // Toggle the TFLIP bit in the TCR reg
      {
         TCRTemp ^= 0x40;             // Toggle the TFLIP bit
         if(TCRTemp & 0x40)
            xprint("TFLIP Set\n\r>");
         else
            xprint("TFLIP Cleared\n\r>");         
         W_CommandValue(TCR, TCRTemp);       
      }
         
      if(ch=='R' || ch=='r')           // Toggle the RFLIP bit in the RCR1/RCR2 regs
      {      
         RCR1Temp ^= 0x80;             // Toggle the RFLIP bit
         RCR2Temp ^= 0x80;             // Toggle the RFLIP bit                  
         if(RCR1Temp & 0x80)         
            xprint("REC-1 RFLIP Set\n\r>");
         else
            xprint("REC-1 RFLIP Cleared\n\r>");         
         W_CommandValue(REC1CR, RCR1Temp);                  
         if(RCR2Temp & 0x80)
            xprint("REC-2 RFLIP Set\n\r>");
         else
            xprint("REC-2 RFLIP Cleared\n\r>");         
         W_CommandValue(REC2CR, RCR2Temp);                       
      }
           
      if(ch=='S' || ch=='s')          // Toggle the SDON bit in the RCR1/RCR2 regs
      {      
         RCR1Temp ^= 0x70;             // Toggle the SDON, SD9, SD10 bits
         RCR2Temp ^= 0x70;             // Toggle the SDON, SD9, SD10 bits                  
         if(RCR1Temp & 0x10)
            xprint("REC-1 SDON,SD9 and SD10 Set\n\r>");
         else
            xprint("REC-1 SDON,SD9 and SD10 Cleared\n\r>");         
         W_CommandValue(REC1CR, RCR1Temp);                  
         if(RCR2Temp & 0x10)
            xprint("REC-2 SDON,SD9 and SD10 Set\n\r>");
         else
            xprint("REC-2 SDON,SD9 and SD10 Cleared\n\r>");         
         W_CommandValue(REC2CR, RCR2Temp);             
      }
      
      if(ch=='H' || ch=='h')            // Print out the help screen
      {
         ConsoleCommandsHelp();
         xprint("\n\r\nPress Space Bar or SW-4 to resume");
         while(SW4 && ' ' != xgetchar());        // start again if Button-4 pressed/spacebar 
         xprint("\n\r>");                   
      }
                            

   } else
   {                                  // No command to process
      return FALSE;
   }
  return TRUE;
   
}
// ------------------------------------------------------
//  Print out the Console on-the-fly Commands help page
//  Invoked by pressing H. 
// ------------------------------------------------------
void ConsoleCommandsHelp(void)
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



// ------------------------------------------------------
// Manage the message index pointer to the array of all received messages
// The array is not actually used by this program. It is provided as a debug tool.
// ------------------------------------------------------
void CheckMessageCountMax(void)
{
    MessageCount++;
    if(MessageCount==MESSAGECOUNTMAX)
      MessageCount=0;  
}

// ------------------------------------------------------
// Fetch all Normal and Priority Messages
//    Save the last 64 messages in the global array for debugging purposes only
//    Print out the messages on the Console
// ------------------------------------------------------
void FetchAllMessagesAndDisplay(unsigned char *RXBuffer,unsigned char *RXBufferPL)
{
   static char Status_F;
   
   LED_CTL(LED_5,OFF);     // Turn off PL-1 LED
   LED_CTL(LED_6,OFF);     // Turn off PL-2 LED
   LED_CTL(LED_7,OFF);     // Turn off PL-3 LED
   
// Recever -1  
   DisableInterrupts; 
   Status_F = R_Register(RXSTATUS_1);                       // Poll Receiver1 status register
   EnableInterrupts;         

   if((Status_F & FFEMPTY) == 0)
      {    
      DisableInterrupts;
      ArincRead(RXFIFO_1,RXBuffer );            
      (void)memcpy(g_RXBuffer[MessageCount],RXBuffer,g_RXBuffSize);  // copy frame to large array for safe keeping
      EnableInterrupts;   
      printARINCData(REC1_HEADER,RXBuffer);            
      CheckMessageCountMax();               
      }

   // Receiver1 Priority Labels       
   if(Status_F & PL1)
      { 
      LED_CTL(LED_5,ON);               // Turn on LED5 for PL1   
      DisableInterrupts;
      ArincRead(RXFIFO_1L1,RXBufferPL );                        
      (void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
      EnableInterrupts;   
      printARINCData(REC1_L1_HEADER,RXBufferPL);
      CheckMessageCountMax();               
      }

   // Receiver1 Priority Label 2       
   if(Status_F & PL2)
      {    
      LED_CTL(LED_6,ON);               // Turn on LED6 for PL2
      DisableInterrupts;
      ArincRead(RXFIFO_1L2,RXBufferPL );                        
      (void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
      EnableInterrupts;   
      printARINCData(REC1_L2_HEADER,RXBufferPL);
      CheckMessageCountMax();               
      }         
   // Receiver1 Priority Label 3       
   if(Status_F & PL3)
      {
      LED_CTL(LED_7,ON);               // Turn on LED7 for PL3    
      DisableInterrupts;
      ArincRead(RXFIFO_1L3,RXBufferPL );                        
      (void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
      EnableInterrupts;   
      printARINCData(REC1_L3_HEADER,RXBufferPL);
      CheckMessageCountMax();             
      }
            
// Recever -2   
      DisableInterrupts; 
      Status_F = R_Register(RXSTATUS_2);                       
      EnableInterrupts;         
      if((Status_F & FFEMPTY) == 0)
         {    
         DisableInterrupts;
         ArincRead(RXFIFO_2,RXBuffer );                        
         (void)memcpy(g_RXBuffer[MessageCount],RXBuffer,g_RXBuffSize);  // copy frame to large array for safe keeping
         EnableInterrupts;   
         printARINCData(REC2_HEADER,RXBuffer);            
         CheckMessageCountMax();               
         }

      // Receiver2 Priority Label 1       
      if(Status_F & PL1)
         {    
         LED_CTL(LED_5,ON);               // Turn on LED5 for PL1
         DisableInterrupts;
         ArincRead(RXFIFO_2L1,RXBufferPL );                        
         (void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
         EnableInterrupts;   
         printARINCData(REC2_L1_HEADER,RXBufferPL);
         CheckMessageCountMax();               
         }
      // Receiver2 Priority Label 2       
      if(Status_F & PL2)
         {
         LED_CTL(LED_6,ON);               // Turn on LED6 for PL2
         DisableInterrupts;
         ArincRead(RXFIFO_2L2,RXBufferPL );                        
         (void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
         EnableInterrupts;   
         printARINCData(REC2_L2_HEADER,RXBufferPL);
         CheckMessageCountMax();               
         }         
      // Receiver2 Priority Label 3       
      if(Status_F & PL3)
         {                                // Turn on LED7 for PL3
         LED_CTL(LED_7,ON);           
         DisableInterrupts;
         ArincRead(RXFIFO_2L3,RXBufferPL );                        
         (void)memcpy(g_RXBuffer[MessageCount],RXBufferPL,g_RXBuffSize);  // copy frame to large array for safe keeping
         EnableInterrupts;   
         printARINCData(REC2_L3_HEADER,RXBufferPL);
         CheckMessageCountMax();               
         }        
   
}


// ------------------------------------------------------
// Print the Received ARINC Message on the console
// Input: channel - assigns the label for the console
//        passed array - is the data for the printout
//        [byte1, byte2, byte3, label]
// ------------------------------------------------------
void printARINCData(const uint8 channel, unsigned char *array) {   
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

// ------------------------------------------------------
// Print the ARINC Transmitted message on the console
// Input: array - is the data for the printout
//        
// ------------------------------------------------------
void printARINCTXData(unsigned char *array) {   
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


   
// ------------------------------------------------------
// Print the ARINC Labels on the console
//        
// ------------------------------------------------------         
void PrintLabelsOnConsole(unsigned char *labelarray)
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



// ------------------------------------------------------
// Debounce the button on the rising edge only.
// Waits for the button to be released for 10ms
// ------------------------------------------------------
void debounceButton(uint8 button)
{
uint8 debounceCnt;
       
debounceCnt=10;
while(debounceCnt){
   Delay100us(K_1MS);            // delay 1ms
   if(! (PORTB & button))
      debounceCnt=10;
   else
      debounceCnt--;      
   }
}

// ------------------------------------------------------
// Print out the HI-3593status registers on the console
// ------------------------------------------------------
void printStatusRegisters(void)
{
 uint8 i;
getRegStatus();   
for(i=0; i< 14;i++) {               
   xprint(DebugArrayStrings[i]);                        
   PrintHexByte0x(DebugArray[i]);  // Output the status registers
   crlf();                  
   }
   
}
// ------------------------------------------------------
// Get the HI-3593 Status Registers for viewing in the debugger and
// also to print out on the console 
// ------------------------------------------------------
void getRegStatus(void){
uint8 priorityMatchLabels3[3];
   
    DebugArray[0] = R_Register(0x90);     // Fetch Receiver 1 Status Reg
    DebugArray[1] = R_Register(0x94);     // Fetch Receiver 1 Control Reg
    
    DebugArray[2] = R_Register(0xB0);     // Fetch Receiver 2 Status Reg
    DebugArray[3] = R_Register(0xB4);     // Fetch Receiver 2 Control Reg
     
    DebugArray[4] = R_Register(0x80);     // Fetch Transmit Status Register
    DebugArray[5] = R_Register(0x84);     // Fetch Transmit Control Register
          
    DebugArray[6] = R_Register(0xD0);     // Fetch Read Flag/ Interrupt Register
    DebugArray[7] = R_Register(0xD4);     // Fetch ACLK Divsion Register
       
    MultiByteRead(0x9C,3, priorityMatchLabels3);     // Fetch Priority Match Labels
    DebugArray[8] = priorityMatchLabels3[2];         // Fetch Priority Match Label
    DebugArray[9] = priorityMatchLabels3[1];         // Fetch Priority Match Label       
    DebugArray[10] = priorityMatchLabels3[0];        // Fetch Priority Match Label 
     
    MultiByteRead(0xBC,3, priorityMatchLabels3);     //  Fetch Priority Match Labels
    DebugArray[11] = priorityMatchLabels3[2];        //  Fetch Priority Match Label
    DebugArray[12] = priorityMatchLabels3[1];        //  Fetch Priority Match Label      
    DebugArray[13] = priorityMatchLabels3[0];        //  Fetch Priority Match Label    

}

// ------------------------------------------------------
// General timer tick 100us for delays
// ------------------------------------------------------
void Delay100us(unsigned int delay){
    g_count100us=delay;
    while(g_count100us);
    
}
/*
// 
// ------------------------------------------------------
// General timer tick 100us for delays  - reserved for possible use
// ------------------------------------------------------
char TimerScheduler100us(void){
  
    if(g_tick100us==0)
      {
         g_tick100us=1000;       // 100ms time   
         return TRUE;
      } 
      else 
      {
          return FALSE;
      }
}
*/

// ------------------------------------------------------
// Control LED1 - LED8
// ledNumber: LED_1,LED_2,LED_3,LED_4....LED_8 [1-8]
// OnOff: 1=ON, 0=OFF
// ------------------------------------------------------
void LED_CTL(uint8 ledNumber, uint8 OnOff){
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


// ------------------------------------------------------
// Pulse the MR pin of the HI-3593
// ------------------------------------------------------
void HW_RESET(void){
  unsigned char i;
   
  MR = HI;
  for (i=0; i<15; i++);      // ~ 2us at 40MHZ Bus clock
  MR = LO;   
   
}

// ------------------------------------------------------
// Read in the MODE0, MODE1, MODE2 switches and save it.
// ------------------------------------------------------
void MODE_SELECT(void) {
    MODES = PORTA & 0x70;       // get MODE switches on PA4/PA5/PA6
    MODES = MODES >> 4;         // adjust to LSBs

    
}

// ------------------------------------------------------
// Read in the Option1/Option2 switches and save it.
// ------------------------------------------------------
void TEST_OPTION(void){
    OPTION = PORTA & 0x03;       // get OPTION switches on PA0/PA1 
   
}
 
// ------------------------------------------------------
// For now don't init PORTP or interrupts will happen if the line goes low.
// ------------------------------------------------------
void GPIO_InitInterrupts(void){
 PortP_Init();                // initialize portP
// IRQ_Init();                  // Maybe for test 
// XIRQ_Init();                 // Maybe for test    
}

//==============================================================================
//Timer_Init
//==============================================================================
void Timer_Init(void)
{
  TIOS = 0x03;   //channels 0 and 1 as output compare
  TCTL1 = 0x00;  //channels disconnected from pin logic    
  TIE = 0x01;    //interrupt enable for channel 0 (one channel is servicing in interrupt 
                 //routine, the second one in main loop through polling)  
                
  PTPSR = 9;    //precision prescaler for div 10
  TSCR1 = 0xF8;
}


// ------------------------------------------------------

// ------------------------------------------------------
void IRQ_Init(void)
{
// taken out 3/30/10 different MCU  error compile
//  INTCR_IRQE = 1;   // falling edge
//  INTCR_IRQEN = 1;  // enable IRQ    

// added 3/30/10 
IRQCR_IRQE  = 1;   // falling edge         
IRQCR_IRQEN = 1;   // enable IRQ          

}


//==============================================================================
//TIMER_ISR
// Precaler is 10 so:
// Period is 40Mhz/10*400 = 10Khz. Or use: 10 * 400/40Mhz = 100us
//==============================================================================
#pragma CODE_SEG NON_BANKED
interrupt 8 void TIMER_ISR(void)
{
  static int count100us=10; 
  
// Don't change any of this code below
 TC0 +=  400;               // Sets up 100us period
  
 if(g_count100us)
    g_count100us--;
 
 if(g_tick100us)
   g_tick100us--;
 
 
count100us--;

//User Code Section  
 
if (!count100us) 
    {       // 1 second scheduler
    count100us = K_1SEC;
    if(ON==g_ledFlashBool)            // Flash the LED7 if enabled
      LED7 ^= TOGGLE;                 // Alive 1 second blink 
   
    } 
}


// ------------------------------------------------------
// PortP interrupt Service Routine 
// Ensure you disable interrupts around any foreground code that accesses the SPI
// if you want to access the SPI in this interrupt routine.
// Optional interrupt handlers for user code.
// ------------------------------------------------------
#pragma CODE_SEG NON_BANKED
interrupt 56 void PORTP_ISR(void)
{

  PP4 = HI;                         // identify which interrupt with scope

 if(PIFP_PIFP0)                     // R1INT   
     {   
      PP4 = LO;
      PP4 = HI; 
            
      PIFP_PIFP0 = 1;               //clear this interrupt       
     }  

 
 if(PIFP_PIFP1)                     // R2INT   
     {   
      PP4 = LO;
      PP4 = HI; 
      PP4 = LO;
      PP4 = HI;                    
      PIFP_PIFP1 = 1;               //clear this interrupt       
     }  


  
 if(PIFP_PIFP2)                     // MB2-1
     {   
      PP4 = LO;
      PP4 = HI; 
      PP4 = LO;
      PP4 = HI; 
      PP4 = LO;
      PP4 = HI;     
      
                
      PIFP_PIFP2 = 1;               //clear this interrupt       
     }  


 if(PIFP_PIFP3)                     // MB1-1
  {    
      PP4 = LO;
      PP4 = HI; 
      PP4 = LO;
      PP4 = HI; 
      PP4 = LO;
      PP4 = HI;      
      PP4 = LO;
      PP4 = HI; 
       
                             
      PIFP_PIFP3 = 1;                 //clear this interrupt       
  }            

       
   PP4 = LO;  
          
}


// ------------------------------------------------------
// Initialize interrupt routine
// ------------------------------------------------------
void XIRQ_Init(void)
{ 
  asm ANDCC #$BF    //enable XIRQ - clear X bit in CCR
  LED_CTL(LED_7,ON);  
}
 
// ------------------------------------------------------
// Unused interrupt Service Routine
// ------------------------------------------------------
interrupt 6 void IRQ_ISR(void)
{
  LED_CTL(LED_7,ON);
}


// ------------------------------------------------------
// XIRQ unused Interrupt Service Routine
// ------------------------------------------------------
interrupt 5 void XIRQ_ISR(void)
{
  LED_CTL(LED_7,ON);
}






/* End of File */