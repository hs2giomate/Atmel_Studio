/* 
* Holt_3593_Class.cpp
*
* Created: 3/18/2020 3:59:33 PM
* Author: GMateusDP
*/


#include "Holt_3593_Class.h"

// default constructor
Holt_3593_Class::Holt_3593_Class()
{
} //Holt_3593_Class

// default destructor
Holt_3593_Class::~Holt_3593_Class()
{
} //~Holt_3593_Class


//==================================================================
/* Initial selections for active ARINC word labels
   Each byte defines 8 label addresses, LS bit = lowest addr 0 of 7
                                        MS bit = highest addr 7 of 7                                        
     [n]   = array pointer value, 
   xxx-yyy = corresponding ARINC label address range 
   
   Example:
   [3] denotes LabelArray[3] which controls label addr 24-31 decimal.
   Bit 0 controls label addr 24, Bit 1 controls label addr 25, etc. */
   
unsigned char LabelArray_1[32] = {      // Receiver-1  
// --------------------------------------------------------------- 
//   [0]     [1]     [2]     [3]     [4]     [5]     [6]     [7]
// 000-007 008-015 016-023 024-031 032-039 040-047 048-055 056-063 
//   270	  272	  204	302		372		367		150		151
     0x1D,   0x5D,   0X21,   0Xc1,   0Xdd,   0XeF,   0X16,   0X96,
// ---------------------------------------------------------------      
//   [8]     [9]     [10]    [11]    [12]    [13]    [14]    [15]  
// 064-071 072-079 080-087 088-095 096-103 104-111 112-119 120-127 
//	 260	   371	  372	 374	375		377
	 0X0d,   0X9f,   0X5f,   0X3F,   0XdF,   0XFF,   0Xb8,   0X00,
// ---------------------------------------------------------------      
//   [16]    [17]    [18]    [19]    [20]    [21]    [22]    [23]       
// 128-135 136-143 144-151 152-159 160-167 168-175 176-183 184-191 
     0X00,   0X00,   0X00,   0X00,   0X00,   0X00,   0X00,   0X00,
// ---------------------------------------------------------------      
//   [24]    [25]    [26]    [27]    [28]    [29]    [30]    [31]            
// 192-119 200-207 208-215 216-223 224-231 232-239 240-247 248-255 
     0X00,   0X00,   0X00,   0X00,   0X00,   0X00,   0x00,   0x00 
// ---------------------------------------------------------------      
};

unsigned char LabelArray_2[32];

// Priority Label-1 Table   
unsigned char LabelPriority1 [3] = {
//   [0]     [1]     [2]  
     PL13,   PL12,   PL11, 
};    


// Priority Label-1 Table     
unsigned char LabelPriority2 [3];

  
 uint8_t	Holt_3593_Class::Init(void){
	 
	 spi.SetDescriptor(&SPI_HI3593);
	  spi.init();
	 uint8_t	status=	Init3593(ACLKDIV,  // ARINC clock divisor
	 TMODE,    // Transmit mode. Set for "send as soon as possible"
	 SELFTEST_OFF, // Selftest defined by Mode switches
	 BITRATE,    // Arinc speed and if Parity is enabled by the switch
	 TFLIP );

	for (int i=0;i<32;i++)
	{
		LabelArray_2[i]=FlipByte(LabelArray_1[i]);
		LabelArray_1[i]=LabelArray_2[i];
	}
	for (int i=0;i<3;i++)
	{
		LabelPriority2[i]=FlipByte(LabelPriority1[i]);
		LabelPriority1[i]=LabelPriority2[i];
	}
	return	status;
 } 
 
 
 // ------------------------------------------------------------------
 // Initialize the HI-3593
 // ------------------------------------------------------------------
 uint8_t Holt_3593_Class::Init3593(uint8_t AclkDiv, uint8_t tmode, uint8_t selftest, uint8_t arate, uint8_t tflip )
 {
	 unsigned char cmd=0;
	 
	 W_Command(RESETCMD);                // Reset the HI-3593
	 W_CommandValue(DivReg, AclkDiv);	   // ACLK div/4 divisor
	 cmd =  arate;
	 cmd |= selftest << 4;
	 cmd |= tmode << 5;
	 cmd |= tflip << 6;                  // TFLIP on
	 W_CommandValue(TCR, cmd);           // Program the Transmit Control Register
	 return R_Register (R_TSR);
	 
	 
 }
 
 void Holt_3593_Class::open(){
	 	spi.enable();
		 SS_Low();
 }
  void Holt_3593_Class::close(){
	    SS_High();
	  spi.disable();
	
  }
 
// ------------------------------------------------------------------
//  SPI function to load 8 bits of label data
// ------------------------------------------------------------------   
unsigned char Holt_3593_Class::txrx8bits_8(unsigned char txbyte, unsigned char return_when_done) {
  unsigned char rxbyte;    
 // rxbyte = Get_Byte();         // clear SPI status register
  uint8_t	p(txbyte);
  Send_Byte(p)    ;   // write Data Register to begin transfer  
	  
  if (return_when_done) {  // optional wait for SPIF flag
    while (!spi.isSPIReady());     
  }
  return rxbyte = txbyte;
  
}

// ------------------------------------------------------------------
//  Initilize Receiver 2 labels with contents of array
// ------------------------------------------------------------------                              */
void Holt_3593_Class::initReceiver1Labels(void)
{
   
  unsigned char dummy;
  signed char i; 
 	
  open();

  dummy = txrx8bits_8(0x14,1);  
  // send 32 bytes of ARINC label data
  for (i=31; i>=0; i--) {
    // send 1 byte of label data, ignore returned data byte            
    dummy = txrx8bits_8(LabelArray_1[i],1);       
  }    
 	  
	close();
}
void Holt_3593_Class::InitPriorityLabels(void)
{
	
	// Priorty Labels
	TXBuffer[0] =  LabelPriority1[0];   // filter 3
	TXBuffer[1] =  LabelPriority1[1];   // filter 2
	TXBuffer[2] =  LabelPriority1[2];   // filter 1
	TransmitCommandAndData(W_PL1Match,TXBuffer);  // initialize the Receiver1 PL match registers
	
	TXBuffer[0] =  LabelPriority2[0];   // filter 3
	TXBuffer[1] =   LabelPriority2[1];   // filter 2
	TXBuffer[2] =   LabelPriority2[2];   // filter 1
	TransmitCommandAndData(W_PL2Match,TXBuffer);  // initialize the Receiver2 PL match registers
}

unsigned char Holt_3593_Class::FlipByte(unsigned char byte){
	unsigned char rev=0;
	for (int i=0;i<8;i++)
	{
		rev=rev<<1;
		rev|=(byte>>i)&(0x01);
	}
	return rev;
}

uint8_t Holt_3593_Class::Send_Byte(uint8_t byte)
{
	while (!spi.isSPIReady()); 
	spi.write(&byte,1);
	while (!spi.isSPIReady());  
	return byte;
}

uint8_t Holt_3593_Class::Get_Byte(void)
{
	uint8_t byte;
	
	while (!spi.isSPIReady());
	spi.read(&byte,1);
	while (!spi.isSPIReady());


	
	return byte;
}	
 
// ------------------------------------------------------------------
// Initilize Receiver 2 labels with contents of array
// ------------------------------------------------------------------ 
void Holt_3593_Class::initReceiver2Labels(void)
{
   
  unsigned char dummy;
  signed char i; 
 	
	open();	
  // send op code (ignore returned data byte)
  dummy = txrx8bits_8(0x28,1);  
  // send 32 bytes of ARINC label data
  for (i=31; i>=0; i--) {
    // send 1 byte of label data, ignore returned data byte            
    dummy = txrx8bits_8(LabelArray_2[i],1);       
  }    
 	  
	close();
}





// ------------------------------------------------------
// Fetch the SPI FIFO data and load the data into the passed array pointer
// This is a more generic version of this function which could be used to read 
// other SPI multibyte reads.
// Inputs:  ReadCommand=SPI Command
//          count=number of bytes to read
//
// Output:  *passedArray = pointer to the array that gets loaded
// ------------------------------------------------------
void Holt_3593_Class::MultiByteRead(uint8_t ReadCommand, uint8_t count, unsigned char *passedArray)
{
   uint8_t dummy, ByteCount;
   open();
   //dummy = Get_Byte();                  // clear SPI status register    
   Send_Byte(ReadCommand);
     
   while (!spi.isSPIReady());            // wait for SPIF flag assertion    
  // dummy = Get_Byte();                  // read/ignore Rx data in Data Reg, resets SPIF                    
   for (ByteCount=0; ByteCount < count; ByteCount++) {
      dummy =Get_Byte(); 
      passedArray[ByteCount] = dummy; 
     }

  close();
  
}

// ------------------------------------------------------
// Fetch the FIFO ARINC data from the desired receiver channel 
//
// ------------------------------------------------------
void Holt_3593_Class::ArincRead(uint8_t source, unsigned char *passedArray)
{
   uint8_t dummy, ByteCount, count;
   
   count = 3;                // assume it's a Priority Label Word?
   if(source==0xA0 || source == 0xC0){  // is it a normal Word?
       count++;								 // then it has 4 bytes.
	   }                        
  open();
  // dummy = Get_Byte();                   // clear SPI status register    
   Send_Byte(source) ;                  // source command to read FIFO data
   
 //  dummy = Get_Byte();                  // read/ignore Rx data in Data Reg, resets SPIF                    
   for (ByteCount=0; ByteCount < count; ByteCount++) {
      dummy = Get_Byte(); 
      passedArray[ByteCount] = dummy; 
     }

  close();
  
}



/* ------------------------------------------------------------------
/  SPI0 8-Bit Send Data / Receive Data Function
/  ------------------------------------------------------------------

Argument(s):  txbyte, return_when_done

     Return:  rxbyte
     
     Action:  Using SPI0, this function sends txbyte and returns rxbyte
              as part of a chained multi-byte transfer. The calling 
              program controls the /SS chip select instead of using the
              automatic /SS option available in the Freescale SPI port.
              This permits simple chaining of op code commands and Tx/Rx
              data as a series of 8-bit read/writes followed by /SS 
              deassertion by the calling program.
              
              If return_when_done is True, the function waits for transfer
              completion before returning, which may be needed for back-to-
              back commands. If return_when_done is False, the function
              returns immediately after initiating the transfer. 
              
Example Call: rcv_byte = txrx8bits(0xFF,1) // sends data 0xFF then returns 
                                           // data when xfer is done  */
unsigned char Holt_3593_Class::txrx8bits (unsigned char txbyte, unsigned char return_when_done) {
  unsigned char rxbyte;  
    uint8_t	p(txbyte);
   // rxbyte = Get_Byte();         // clear SPI status register  
	Send_Byte(p);        // write Data Register to begin transfer   
	//rxbyte = Get_Byte(); 
	if (return_when_done) {  // optional wait for SPIF flag
		while (!spi.isSPIReady());      
    }

  rxbyte = txbyte;  // get received data byte from Data Register

  return rxbyte;
  
 }
 

// ------------------------------------------------------------------
// Transmits the Message Command and data contained in the passed array pointer
// Transmit the 0x0C Opcode command + 4 bytes of ARINC data per Figure 1, pg 8 of the data sheet
// ------------------------------------------------------------------
 void Holt_3593_Class::TransmitCommandAndData(uint8_t cmd, uint8_t *TXBuffer)
 {              
 uint8_t static ByteCount,dummy,transmitCount;
 
	open();
   
   transmitCount=4;                                // Standard messages are 4 bytes
   if(cmd==W_PL1Match || cmd==W_PL2Match)          // if writing PL Match registers send only 3 bytes
      transmitCount--;
   
   dummy = txrx8bits(cmd, 1);      // Transmit the whole message, ignore return values 

   for(ByteCount=0; ByteCount< transmitCount; ByteCount++)// Transmit command=0x0C +  4 bytes        
      {              
         dummy = txrx8bits(TXBuffer[ByteCount], 1);      // Transmit the whole message, ignore return values 
      }   
 close();
}

// Primitive SPI Commands below
//
// ------------------------------------------------------------------
// Single command byte write
// ------------------------------------------------------------------
void Holt_3593_Class::W_Command (char cmd) {

 	  uint8_t p(cmd); 
  open();
  //dummy = Get_Byte();               // clear SPI status register    
 
  Send_Byte(p);             // Test Mode SPI Instruction - MASTER RESET CMD 
  while (!spi.isSPIReady()) {;}      // Wait for data to come back in.
  //dummy = char(Get_Byte());               // read Rx data in Data Reg to reset SPIF 
  close();
}

// ------------------------------------------------------------------
// Write SPI Command (Opcode) with a Value to HI-3593
//  ------------------------------------------------------------------
void Holt_3593_Class::W_CommandValue (uint8_t cmd, uint8_t value){

  	
  open();                             // assert the SPI0 /SS strobe
 // dummy = Get_Byte();                           // clear SPI status register    
  Send_Byte(cmd);                             // SPI  command 
  while (!spi.isSPIReady());    
 // dummy = Get_Byte();                           // read Rx data in Data Reg to reset SPIF
                         // clear SPI status register    
  Send_Byte(value);                           // Reset values     
  while (!spi.isSPIReady());    
 // dummy = Get_Byte();                           // read Rx data in Data Reg to reset SPIF

  close();
}


/* ------------------------------------------------------------------
/  Read HI-3110 Register Read Function
/  ------------------------------------------------------------------

Argument(s):  Register to read

     Return:  8-bit Register Value 
*/

unsigned char Holt_3593_Class::R_Register(char Reg){                     
  unsigned char R_Reg;
	
  open();
  txrx8bits(Reg,1);        // send op code (ignore returned data byte)
              
  R_Reg  = Get_Byte();  
         
  close();
  return R_Reg;
}
// ------------------------------------------------------
// Set the SPI nCS high then low
// ------------------------------------------------------
void Holt_3593_Class::CS_HL (void) {
	close();
  //--------------------------------------------    
	open();                             // assert the SPI0 /SS strobe
  } 

void Holt_3593_Class::SS_Low(void)
{

	
	gpio_set_pin_level(CS_Holt,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);
}
void Holt_3593_Class::SS_High(void)
{

	
	gpio_set_pin_level(CS_Holt,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);
}
