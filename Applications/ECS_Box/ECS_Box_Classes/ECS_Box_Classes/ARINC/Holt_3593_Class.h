/* 
* Holt_3593_Class.h
*
* Created: 3/18/2020 3:59:33 PM
* Author: GMateusDP
*/


#ifndef __HOLT_3593_CLASS_H__
#define __HOLT_3593_CLASS_H__
#include "driver_init.h"
#include "SPI_Syn_Class.h"
typedef unsigned char uint8;
#define RESETCMD 0x04

#define DivReg  0x38
#define ACLKDIV 4                 // Divide Aclk by 4 we use a 4Mhz Osc
#define TCR 0x08                  // Transmit Control Register
#define R_TSR 0x80                // Read Transmit Status Register

#define R_RCR1 0x94
#define R_RCR2 0xB4
#define R_TCR  0x84

#define TFFULL 4                   // Transmit FIFO Full
#define TXFIFO 0x0C                // Transmit FIFO

#define RXSTATUS_1 0x90            // Receiver 1 status reg
#define RXSTATUS_2 0xB0            // Receiver 2 status reg
#define FFEMPTY 1
#define FFHALF  2
#define FFFULL  4

#define PL1     8                  // PL1 flag in receive status register
#define PL2     0x10               // PL2 flag in receive status register
#define PL3     0x20               // PL3 flag in receive status register


#define RXFIFO_1    0xA0           // Receiver-1 FIFO op-codes
#define RXFIFO_1L1  0xA4
#define RXFIFO_1L2  0xA8
#define RXFIFO_1L3  0xAC

#define RXFIFO_2    0xC0           // Receiver-2 FIFO op-codes
#define RXFIFO_2L1  0xC4
#define RXFIFO_2L2  0xC8
#define RXFIFO_2L3  0xCC

#define W_PL1Match 0x18           // PL match opcode to write 3 labels
#define W_PL2Match 0x2C           // PL match opcode to write 3 labels


#define TMODE 1
#ifndef TFLIP
	//#define TFLIP 0
	#define TFLIP 0x40
#endif

#ifndef RFLIP
	//#define RFLIP 0x00             // Reverses the label data if high
	#define RFLIP 0x80             // Reverses the label data if high
#endif
#define SELFTEST_ON 1
#define SELFTEST_OFF 0


#define PL11 0x1D                // Receiver1 PL 1 programmed match register value
#define PL12 0x9f
#define PL13 0xef
#define PL21 0x1D                // Receiver2 PL 1 programmed match register value
#define PL22 0x9f
#define PL23 0xef
#define PLON 0x02
//#define LABREC 0x00
#define LABREC 0x04
#define REC1CR 0x10
#define REC2CR 0x24
#define REC1MEMORY 0x98
#define REC2MEMORY 0xB8

#define TRANSMIT_PARTY_ON  1
#define TRANSMIT_PARTY_OFF 0
#define PARITYODD 0
#define PARITYEVEN 1
#define BITRATE 0

#define REC1PARITY_ON   1
#define REC1PARITY_OFF  0
#define REC2PARITY_ON   1
#define REC2PARITY_OFF  0
#define  LABEL_ARRAY_SIZE 32




// Flag bits
#define TSR_Default 0x01                 // TSR status after init.

// define chip select GPIO output for SPI port which is handled by GPIO.
#define SPI0_nSS    PTM_PTM3             // updated for Rev 1.2 from PTP_PTP3

class Holt_3593_Class
{
//variables
public:
	SPI_Syn_Class	spi;

protected:
private:
	uint8_t	rate;
	uint8_t	receiverArray[4];
	unsigned char TXBuffer [16];                 // Transmit Buffer
	
//functions
public:
	Holt_3593_Class();
	~Holt_3593_Class();
	
	
	
	void W_Command (char cmd);
	uint8_t Init(void);
	uint8_t Init3593(uint8_t AclkDiv, uint8_t tmode, uint8_t selftest, uint8_t arate, uint8_t tflip );
	void ArincRead(uint8_t source, unsigned char *passedArray);
	void CS_HL (void);
	unsigned char txrx8bits(unsigned char, unsigned char );
	unsigned char txrx8bits_8(unsigned char, unsigned char );
	unsigned char fetch8bits(unsigned char txbyte, unsigned char return_when_done);
	unsigned char R_Register(char Reg);
	void W_CommandValue (uint8_t cmd, uint8_t value);
	void MultiByteRead(uint8_t,uint8_t count, unsigned char *passedArray);
	void TransmitCommandAndData(uint8_t cmd, uint8_t *TXBuffer);
	void initReceiver1Labels(void);
	void initReceiver2Labels(void);

	uint8_t Get_Byte(void);
	uint8_t Send_Byte(uint8_t byte);
	void write_priorioty_labels();
	void InitPriorityLabels(void);
	
	
	

	
	friend class	 ARINC_Interface;
protected:
	unsigned char FlipByte(unsigned char);
private:
	void	SS_Low(void);
	void	SS_High(void);
	void	open(void);
	void	close(void);
	Holt_3593_Class( const Holt_3593_Class &c );
	Holt_3593_Class& operator=( const Holt_3593_Class &c );

}; //Holt_3593_Class

#endif //__HOLT_3593_CLASS_H__
