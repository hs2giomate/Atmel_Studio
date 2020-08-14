/* 
* Application_Class.cpp
*
* Created: 7/22/2020 12:46:03 PM
* Author: GMateusDP
*/


#include "Application_Class.h"
#include "LedHandler_Class.h"
#include "NMIButton_Class.h"
#include "Timer_Class.h"
#include "stdlib.h"
	
// default constructor
Application_Class::Application_Class()
{
	
} //Application_Class

// default destructor
Application_Class::~Application_Class()
{
} //~Application_Class

void Application_Class::Init(void){
	lighter.poweronSingleTime=POWERON_SINGLE_TIME;
	lighter.Init();
	flash.Init();
	totalPins=lighter.GetTotalPins();
	addressCounter=0;
}
void	Application_Class::Run(void){
	lighter.ClearAll();
	//GreenWave();

	while(1){
		if (nmi.IsButtonPressed())
		{
			
			nmi.OnPressed();
			
			if (!lighter.changeModeRequest)
			{
				
				RunOnMode();
				
			}
			else
			{
				ChangeModus();
				
			}
			
			
		}
		else
		{
			
			if (lighter.changeModeRequest){
				
				ChangeModus();
				
				}else{
				
				RunOnMode();
			}
			
		}
		
	}
}

void	Application_Class::ChangeModus(void){
	lighter.RestartLedModus();
	if (modus==4)
	{
		modus=0;
	}else{
		modus=(modus+1)%NUMBER_STATES;
		if (modus>NUMBER_STATES)
		{
			modus=0;
		}
	}

	
	while(!lighter.restarted);
	//ledTimer.remove_task();
	RunOnMode();
}

void	Application_Class::RunOnMode(void){
	switch (modus)
	{
		case 0:
		lighter.stepTime=POWERON_SINGLE_TIME;
		lighter.GreenWave();
		/* Your code here */
		break;
		case 1:
		lighter.stepTime=POWERON_SINGLE_TIME;
		lighter.RedWave();
		/* Your code here */
		break;
		case 2:
		lighter.stepTime=POWERON_SINGLE_TIME;
		lighter.BlueWave();
		/* Your code here */
		break;
		case 3:
			lighter.stepTime=POWERON_SINGLE_TIME/1000;
			//lighter.LilaWave();
			CheckFlashMemory();
		/* Your code here */
		break;
		case 4:
		lighter.stepTime=POWERON_SINGLE_TIME/1000;
			lighter.LilaWave();
		/* Your code here */
		break;
	
	}
}
bool	Application_Class::CheckFlashMemory(void){
		lighter.request=false;
		lighter.onLila=true;
		FastMemoryCheck();
		errorCounter=0;
		addressCounter=0; succesCounter=0;
		succesLimit=(uint32_t)(TEST_FLASH_SIZE/ISSI_BUFFER_SIZE);
		flash.eraseFlash(0,TEST_FLASH_SIZE*ISSI_BUFFER_SIZE*1);
		while (app.FastMemoryCheck());
		do
		{
			for (int k=2;k<totalPins;k++)
			{
				if (lighter.IsBlue(k))
				{
					if (IsDataCorrupted())
					{
						flash.WriteAddress(tx_buffer,addressCounter,ISSI_BUFFER_SIZE);
						if (!lighter.PowerOnFail(k))
						{
							
							break;
						}
						
						flash.ReadAddress(rx_buffer,addressCounter,ISSI_BUFFER_SIZE);
						errorCounter++;
					}else{
						FillTXBuffer();
						flash.WriteAddress(tx_buffer,addressCounter,ISSI_BUFFER_SIZE);
						if (!lighter.PowerOnLila(k))
						{
							break;
						}
						
						flash.ReadAddress(rx_buffer,addressCounter,ISSI_BUFFER_SIZE);
						succesCounter++;
					}
					NextAddress();
				
				}
				else
				{					
					lighter.PowerOffPin(k);
			
				}
				
				
			}
		}while((!lighter.changeModeRequest)&&(succesCounter<succesLimit)&&(errorCounter<TEST_FLASH_SIZE));
		if (errorCounter>=TEST_FLASH_SIZE)
		{
			modus=1;
		} 
		else
		{
			modus=4;	
		}
		addressCounter=0;
		lighter.request=false;
		lighter.onLila=false;
		return is_corrupted;
}


bool	Application_Class::IsDataCorrupted(void){
	is_corrupted=false;
	for (int i = 0; i < ISSI_BUFFER_SIZE; i++) {
		if (tx_buffer[i] != rx_buffer[i]) {
			is_corrupted=true;
			break;
		}
		
	}
	return	is_corrupted;
}
bool	Application_Class::FastMemoryCheck(void){
	FillTXBuffer();
	flash.WriteAddress(tx_buffer,addressCounter,ISSI_BUFFER_SIZE);
	//delay_ms(WRITE_DELAY);
	//flash.WaitBusy();
	flash.ReadAddress(rx_buffer,addressCounter,ISSI_BUFFER_SIZE);
//	delay_ms(ISSI_BUFFER_SIZE/2);
	NextAddress();
	return	IsDataCorrupted();
}
uint8_t	Application_Class::FillTXBuffer(void){
	for (int i = 0; i <ISSI_BUFFER_SIZE ; i++) {
		tx_buffer[i] = (uint8_t)rand();
	}
	return	tx_buffer[ISSI_BUFFER_SIZE-1];
}
uint32_t	Application_Class::NextAddress(void){
	addressCounter= (addressCounter+ISSI_BUFFER_SIZE)%TEST_FLASH_SIZE;
	return	addressCounter;
}

Application_Class	app;