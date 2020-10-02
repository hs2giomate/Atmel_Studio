/*
 * ECS_Box_Classes.cpp
 *
 * Created: 6/5/2020 3:51:23 PM
 * Author : GMateusDP
 */ 

#include "main.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	usb.Init();
	delay_ms(100);
	StartLivePulse();
	//QSPIDriverTest();
	//DateTimeTest();
//	EEPROM_Test();
	IO_ExpanderTest();
		/* Replace with your application code */
	while (1) {
		
		
	}
}

uint8_t	StartLivePulse(void){
	usb<<"**** Life Pulse Activated****"<<NEWLINE;
	//pwm_set_parameters(&PWM_0, 500, 1000);
	uint32_t p=pwm_enable(&LIVE_PULSE);
	return p;
}

void usb_test(void){
	if (usb.available())
	{
		usb<<"it should work";
		int inByte=usb.read();
		if (inByte>33)
		{
			//usb.writeData2(&inByte,1);
			usb.println(inByte);
			usb.println((float)inByte/23);
			
			usb<<"it does work"<<NEWLINE;
		}
	}
}
uint8_t EEPROM_Test(void){
	bool	is_corrupted;
	eeprom.Init();
	uint8_t addr=AT24MAC_BUFFER_SIZE/2;
	uint8_t	value;
	usb<<"  Function for testing an i2c EEPROM"<<NEWLINE;
	/* Replace with your application code */
		uint8_t tx_buffer[ AT24MAC_BUFFER_SIZE] ;
		uint8_t rx_buffer[ AT24MAC_BUFFER_SIZE] ;
	while (1) {
		for (int i = 0; i < AT24MAC_BUFFER_SIZE ; i++) {
			tx_buffer[i] = (uint8_t)rand();
			rx_buffer[i] = (uint8_t)(AT24MAC_BUFFER_SIZE-i);
		}

		while(!eeprom.IsReady());
// 		eeprom.WriteAddress(tx_buffer,addr,AT24MAC_BUFFER_SIZE);
		for (int i = 0; i < AT24MAC_BUFFER_SIZE; i++)
		{
			while(!eeprom.IsReady());			
			eeprom.Write_byte(addr+i,tx_buffer[i]);
			delay_ms(2);
			while(!eeprom.AcknolledgePolling());
		}
 		
		eeprom.ReadAddress(rx_buffer,addr,AT24MAC_BUFFER_SIZE);	
// 		for (int i = 0; i < AT24MAC_BUFFER_SIZE; i++)
// 		{
// 			while(!eeprom.IsReady());// 			rx_buffer[i]=eeprom.Read_byte(addr+i);
// 		}

	
		is_corrupted = false;
		for (int i = 0; i < AT24MAC_BUFFER_SIZE; i++) {
			if (tx_buffer[i] != rx_buffer[i]) {
				is_corrupted = true;
				usb.print("EEPROM verification failed. Address: ");
				usb.print(addr,HEX);
				usb<<" bit :"<<i<<NEWLINE;
		    	//flashAddress=0;
			
				break;
			}
		
		}

		if (!is_corrupted) {
			usb.print("Write - Read is successful in EEPROM memory  " );
			usb.println(addr,HEX);
			addr=(addr+AT24MAC_BUFFER_SIZE)%AT24MAC_MEMORY_SIZE;
		}
	}
	return	 value;
}

void	LTC2983_test(void){
	LTC2983_Class	LTC(&SPI_TEMP);
	LTC.Init();
	LTC.print_title();
	LTC.configure_channels();
	LTC.configure_global_parameters();
	while (LTC.ready)
	{
		 LTC.measure_channel(CHIP_SELECT, 4, TEMPERATURE);      // Ch 4: RTD PT-100
		 delay_ms(1);
		 LTC.measure_channel(CHIP_SELECT, 8, TEMPERATURE);      // Ch 8: RTD PT-100
		  delay_ms(1);
		 LTC.measure_channel(CHIP_SELECT, 15, TEMPERATURE);     // Ch 15: RTD PT-100
		  delay_ms(1);
	}
	
}

int ARINC_test(void)
{
	ARINC_Interface		ECSBox;
	ECSBox.Init();
	delay_ms(100);
	while (1)
	{
		ECSBox.SayHello();
		delay_ms(100);
		ECSBox.CustomMessage(SELFTEST_OFF);
	}
	return 0;
}
void	QSPIDriverTest(void){
		bool is_corrupted = false;
		uint8_t tx_buffer[QSPI_BUFFER_SIZE] ;
		uint8_t rx_buffer[QSPI_BUFFER_SIZE] ;
		qspiFlash.Init();

		usb.print("QSPI Program Started\n\r");
		/* Initialize Tx buffer */
	
	
		/* Erase flash memory */
		uint32_t flashAddress=0;
		uint32_t	delay=1;
		while (1) {
	
			qspiFlash.WaitOnBusy();
			if ((flashAddress % N25Q_SECTOR_SIZE)==0) {
				qspiFlash.Erase(flashAddress);
			}
			for (int i = 0; i <QSPI_BUFFER_SIZE ; i++) {
				tx_buffer[i] = (uint8_t)rand();
				rx_buffer[i] = (uint8_t)(QSPI_BUFFER_SIZE-i);
			}
			/* Write data to flash memory */
			if (ERR_NONE ==qspiFlash.WriteAddress((uint8_t *)tx_buffer,flashAddress,QSPI_BUFFER_SIZE)) {
				//usb.print("Flash write successful \n\r");
			}
			qspiFlash.WaitOnBusy();

			if (ERR_NONE == qspiFlash.ReadAddress((uint8_t *)rx_buffer,flashAddress,QSPI_BUFFER_SIZE)) {
				//while(!memory.xferDone);
				//usb.print("Flash read successful\n\r");
			}
			qspiFlash.WaitOnBusy();
			is_corrupted = false;
			for (int i = 0; i < QSPI_BUFFER_SIZE; i++) {
				if (tx_buffer[i] != rx_buffer[i]) {
					is_corrupted = true;
					usb.print("Flash data verification failed. Address: ");
					usb.print(flashAddress,HEX);
					usb<<" bit :"<<i<<NEWLINE;
					//flashAddress=0;
						
					break;
				}
					
			}

			if (!is_corrupted){
				usb.print("Write - Read is successful in QSPI Flash memory  " );
				usb.println(flashAddress,HEX);
				flashAddress=(flashAddress+QSPI_BUFFER_SIZE)%N25Q_FLASH_SIZE;
			}
					
		}
}

void	DateTimeTest(void){
	usb<<"****  Calendar Update Test***"<<NEWLINE;
	usb<<__TIMESTAMP__<<NEWLINE;
	//eeprom.init();
	//memory.Init();
	calendar.Init();
	calendar_date dia;
	calendar_time tiempo;
	calendar_date_time	cdt;

	while (1)
	{
		calendar.ReadDateTime(&cdt);
		usb<<NEWLINE<<"****last data time:"<<NEWLINE;
		dia=cdt.date;
		usb<<"	year :"<<dia.year<<"	month :"<<dia.month<<"	day: "<<dia.day<<NEWLINE;
		tiempo=cdt.time;
		usb<<"	hour :"<<tiempo.hour<<"	min :"<<tiempo.min<<"	sec: "<<tiempo.sec<<NEWLINE;
		delay_ms(1000);
		
		usb<<"**current data time:"<<NEWLINE;
		calendar.GetDateTime(&cdt);
		dia=cdt.date;
		usb<<"	year :"<<dia.year<<"	month :"<<dia.month<<"	day: "<<dia.day<<NEWLINE;
		tiempo=cdt.time;
		usb<<"	hour :"<<tiempo.hour<<"	min :"<<tiempo.min<<"	sec: "<<tiempo.sec<<NEWLINE;
		
		calendar.SaveCurrentDateTime();
		delay_ms(1000);
		
	}
	
}

void	IO_ExpanderTest(void){
	mcp.Init();
	mcp.SetPortAInput();
	mcp.SetPortBOutput();

	/* Replace with your application code */
	while (1)
	{
		for (int i = 0; i < 8; i++)
		{
			mcp.digitalWrite(8+i,mcp.digitalRead(i));
		}
		
	}
}