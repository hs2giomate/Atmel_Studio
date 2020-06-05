/* 
* ARINC_Interface.h
*
* Created: 3/18/2020 12:01:24 PM
* Author: GMateusDP
*/


#ifndef __ARINC_INTERFACE_H__
#define __ARINC_INTERFACE_H__
typedef unsigned char uint8;
typedef signed char int8;
#include "USB_CDC_Class.h"
//#include "SPI_Asyn_Class.h"
#include "Holt_3593_Class.h"
#include "Timer_Class.h"
#include "atmel_start_pins.h"
#include "ARINC_Conversions.h"
#include "stdlib.h"
#include "stdio.h"
#define g_RXBuffSize 4
#define OFFF OFF
#define K_1MS  1
#define K_10MS 10
#define K_100MS  100
#define K_1SEC  1000
#define TOGGLE 1
#define DEADLOOP 1
#define MESSAGECOUNTMAX 64
#define SELFTEST_ON 1
#define SELFTEST_OFF 0
#define g_RXBuffSize 4  
#define TRANSMIT_PARTY_ON  1
#define TRANSMIT_PARTY_OFF 0
#define PARITYODD 0
#define PARITYEVEN 1
#define TMODE 1
#define TFLIP 1
#define RFLIP 0x80
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

//const char MyMesg[] = {"Hello World from Holt IC. "};
//const char Mesg1[] ;
//const char Mesg2[] ;



//const char HexToAscii[16] ;
//const char Mesg3717[];
//const char HistoryMesg[];
//const char DebugArrayStrings[][25];              // Status register dump message headers
union Buffer32 {
	long Word32;
	unsigned char PayLoad[4];        // We will ignore the MSB byte [0]
};


class ARINC_Interface
{
//variables
public:
	static USB_CDC_Class	usb0;
	Timer_Class		timer1;

	Holt_3593_Class	HI3893;

	bool	BITRATE0;
	bool	BITRATE1;
	bool	BITRATE2;
	bool	OPT1;
	bool	OPT2;
	bool	PARITY;
	mutable	bool newMessage;
	unsigned char TXBuffer [16];                 // Transmit Buffer
	//const char WelcomeMesg[] = "Holt HI-3593 Demonstration.";
	volatile unsigned int g_count100us;          // Global timer tick for delays.
	volatile int g_tick100us;                    // Timer function
	volatile char g_ledFlashBool;                // Global LED8 flash enable, YES or NO.
	unsigned char MessageCount;
	unsigned char MODES,OPTION;
	uint8 DebugArray[16];                        // Global array for 3110 status registers
	unsigned char g_RXBuffer[64][4];             // [# of buffers][16 bytes]
	

	union Buffer32 BigCounter;

	FUNC_PTR			handler;
	 ARINC_Conversions	converter;
protected:
private:
	uint8_t	receiverArray[4];
	mutable	unsigned char RXBuffer[g_RXBuffSize];       // Temp buffer to hold messages data
	mutable	unsigned char RXBufferPL[g_RXBuffSize];     // Temp buffer to hold PL messages data
		unsigned char LabelsAr1[32];                // All Rec1 256 labels
		unsigned char LabelsAr2[32];                // All Rec2 256 labels
		unsigned char statusRegister;                // All Rec2 256 labels
//functions
public:
	ARINC_Interface();
	//ARINC_Interface(USB_CDC_Class*);
	~ARINC_Interface();
	
	void	Init(void);
	void	SayHello(void);
	void	TransmitReceiveWithLabels_Mode(const uint8_t);
	void	xprint(const char*);
	void	CE_Low_Holt(void);
	void	CE_High_Holt(void);
	void	getRegStatus(void);
	void	LED_CTL(uint8 ledNumber, uint8 OnOff);
	void	SerialCommands(void);
	void	debounceButton(uint8 button);
	char	TimerScheduler100us(void);         // reserved for possible use
	void	printARINCData(const uint8 channel,unsigned char *array);
	void	printStatusRegisters(void);
	void	PrintLabelsOnConsole(unsigned char *labelarray);
	void	FetchAllMessagesAndDisplay(unsigned char *RXBuffer16,unsigned char *RXBufferPL16);
	void	CheckMessageCountMax(void);
	void	printARINCTXData(unsigned char *array);
	bool ConsoleCommands(char ch);
	void	ConsoleCommandsHelp(void);
	void	cpu_irq_enable(void);
	void	cpu_irq_disable(void);
	void	xgetchar(char *);
	char	xgetchar(char *,bool);
	void	crlf(void);
	void	PrintHexByte(uint8);
	void    HW_RESET(void);
	void	blink_LED0(void);
	void	PrintOctalLabelsOnConsole(unsigned char *);
	void	PrintOctals(uint8 );
	void	CustomMessage(const uint8);
	void	space(void);
	void	xputc(char);
	void	ReadArincBuffer(void);

protected:
private:
	ARINC_Interface( const ARINC_Interface &c );
	ARINC_Interface& operator=( const ARINC_Interface &c );

}; //ARINC_Interface

#endif //__ARINC_INTERFACE_H__
