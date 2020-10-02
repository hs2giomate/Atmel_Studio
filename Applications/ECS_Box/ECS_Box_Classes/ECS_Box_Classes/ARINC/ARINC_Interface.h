/* 
* ARINC_Interface.h
*
* Created: 3/18/2020 12:01:24 PM
* Author: GMateusDP
*/


#ifndef __ARINC_INTERFACE_H__
#define __ARINC_INTERFACE_H__



#include "Holt_3593_Class.h"
#include "Timer_Class.h"
#include "atmel_start_pins.h"
#include "Protocol_Class.h"
#include "ARINC_Conversions.h"

#define g_RXBuffSize 4
#define OFFF OFF
#define K_1MS  1
#define K_10MS 10
#define K_100MS  100
#define K_1SEC  1000
#define TOGGLE 1
#define DEADLOOP 1
#define MESSAGECOUNTMAX 64
#define TX_LABELS_NUMBER 32
#define RX_LABELS_NUMBER 32
#define SELFTEST_ON 1
#define SELFTEST_OFF 0
#define g_RXBuffSize 4  
#define TRANSMIT_PARTY_ON  1
#define TRANSMIT_PARTY_OFF 0
#define PARITYODD 0
#define PARITYEVEN 1
#define TMODE 1
#define ON 1
#define OFF 0
#define SPACE	0x20
#define YES	1
#define	NO	0
// Header idenfifies for the Console output
#define REC1_HEADER    1
#define REC1_L1_HEADER 2
#define REC1_L2_HEADER 3
#define REC1_L3_HEADER 4
#define REC2_HEADER    5
#define REC2_L1_HEADER 6
#define REC2_L2_HEADER 7
#define REC2_L3_HEADER 8

#define ERROR_OK    1
#define ERROR_ERROR 0
#define START_CYCLE 1
#define WAIT_CYCLE  0
#define TAB 9
#define SPACE 0x20         // space
#define CR 0x0D            // carrage return
#define LF 0x0A            // line feed
#define NEWPAGE 0x0C       // new page (clears the full screen)
//#define EOF -1             // ** This may need to be commend out for some codewarrior installs if duplicate declared**
#define XON 0x11           // Flow control ON
#define XOFF 0x13          // Flow control OFF
#define DEFAULT_LABEL 270
#define DEFAULT_MESSAGE 0x1d5d21





union Buffer32 {
	long Word32;
	unsigned char PayLoad[4];        // We will ignore the MSB byte [0]
};




class ARINC_Interface: public virtual ARINC_Conversions, public Protocol_Class
{
//variables
public:
	bool	BITRATE0;
	bool	BITRATE1;
	bool	BITRATE2;
	
	bool	PARITY;
	volatile	bool newMessageR1,newMessageR2, txTimeout;
	unsigned char TXBuffer [16];                 // Transmit Buffer
	//const char WelcomeMesg[] = "Holt HI-3593 Demonstration.";
	volatile unsigned int g_count100us;          // Global timer tick for delays.
	volatile int g_tick100us;                    // Timer function
	volatile char g_ledFlashBool;                // Global LED8 flash enable, YES or NO.
	unsigned char MessageCount;
	unsigned char MODES,OPTION;
	uint8_t DebugArray[16];                        // Global array for 3110 status registers
	uint8_t receiverBuffer[RX_LABELS_NUMBER][4];             // [# of buffers][16 bytes]
	uint8_t	transmitBuffer[TX_LABELS_NUMBER][4];
	
//	Datagram			arincMessage;
	FUNC_PTR			handler;
	uint8_t LabelsArrayTX[TX_LABELS_NUMBER];                // All Rec1 256 labels
	uint8_t LabelsArrayRX1[RX_LABELS_NUMBER];                // All Rec2 256 labels
	uint8_t  LabelsArrayRX2[RX_LABELS_NUMBER];                // All Rec2 256 labels
protected:
private:
	uint8_t	receiverArray[4];
	unsigned char RXBuffer[g_RXBuffSize];       // Temp buffer to hold messages data
	unsigned char RXBufferPL[g_RXBuffSize];     // Temp buffer to hold PL messages data
	uint8_t RXBuffer1[g_RXBuffSize];       // Temp buffer to hold messages data
	uint8_t RXBufferPL1[g_RXBuffSize];     // Temp buffer to hold PL messages data
	uint8_t RXBuffer2[g_RXBuffSize];       // Temp buffer to hold messages data
	uint8_t RXBufferPL2[g_RXBuffSize];     // Temp buffer to hold PL messages data
	uint8_t	MessageCount1;
	uint8_t	MessageCount2;

	unsigned char statusRegister;                // All Rec2 256 labels
	uint8_t	statusHolt;
	bool	isOK;
	Buffer32 BigCounter;
	uint8_t ARINCLabel;
	uint8_t Arate;
	uint8_t Rec1Parity;
	uint8_t Rec2Parity;
	uint8_t	i,j,k,index;
	static char Status_F;
	Holt_3593_Class	HI3593;
	uint8_t receiverBuffer1[RX_LABELS_NUMBER][4];             // [# of buffers][16 bytes]
	uint8_t receiverBuffer2[RX_LABELS_NUMBER][4];
//functions
public:
	ARINC_Interface();
	~ARINC_Interface();
	
	bool	Init(void);
	void	SayHello(void);
	void	TransmitReceiveWithLabels_Mode(const uint8_t);
	void	FetchAllMessagesReceiver1(void);
	void	FetchAllMessagesReceiver2(void);
	void	getRegStatus(void);
	void	PrintLabelsOnConsole(unsigned char *labelarray);
	void	FetchAllMessagesAndDisplay(unsigned char *,unsigned char *);
	void	CheckMessageCountMax(void);
	void	printARINCTXData(unsigned char *array);
	bool	ConsoleCommands(char ch);
	void	ConsoleCommandsHelp(void);
	void	PrintHexByte(uint8_t);
	void    HW_RESET(void);
	void	PrintOctals(uint8_t );
	void	CustomMessage(const uint8_t);
	void	space(void);
	void	xputc(char);
	uint32_t ReadFIFO1(void);
	uint32_t	ReadArincBuffer(void);
	virtual	uint32_t	ReadBufferLabel(uint8_t);
	//virtual uint8_t		Label2Byte(uint32_t);
	uint32_t	ReadBufferLabel(int);
	bool		isThereNewMessage(void);
	uint32_t	Hex2Octal(uint8_t);
	char	AtoHex2(char, char);
	uint32_t TrasmitSingleLabel(void);
	uint32_t TrasmitSingleLabel(uint32_t l);
	

protected:
	
private:
	ARINC_Interface( const ARINC_Interface &c );
	ARINC_Interface& operator=( const ARINC_Interface &c );
	void CommandStatus( void);
	uint8_t	RollMessageCount(uint8_t);
	void	cpu_irq_enable(void);
	void	cpu_irq_disable(void);
	void	PrintOctalLabelsOnConsole(unsigned char *);
	void	LED_CTL(uint8_t ledNumber, uint8_t OnOff);
	void	SerialCommands(void);
	void	debounceButton(uint8_t button);
	char	TimerScheduler100us(void);         // reserved for possible use
	void	printARINCData(const uint8_t channel,unsigned char *array);
	void	printStatusRegisters(void);
	void	xgetchar(char *);
	char	xgetchar();
	void	crlf(void);
	void	CE_Low_Holt(void);
	void	CE_High_Holt(void);

}; //ARINC_Interface
extern	ARINC_Interface arinc;
#endif //__ARINC_INTERFACE_H__
