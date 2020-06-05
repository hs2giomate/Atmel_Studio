/*--------------------------------------------------------------------------

PinCoding.cc

Implementation
Ansteuerung Pin Coding Board

Autor: Steffen Simon

$Date: 2018-03-01 15:56:36 +0100 (Do, 01. Mrz 2018) $
$Revision: 7777 $
$Author: steffen $
$HeadURL: https://svn.s2embedded.at/customers/ipeco/trunk/PSC/Firmware/PinCoding/PinCoding.cc $

--------------------------------------------------------------------------*/
#include "PinCoding.h"
#include "coreI2C.h"
#include "eDeviceEvent.h"
#include "GAINClass.h"
#if defined(__DEBUG__)||defined(HALT_TEST)
#include "eDevice.h"
#endif

uint8	pcaAddress(0x40);
uint8	adcAddress[3] = {0xAA, 0xA8, 0xB2};
//	Old Pincoding board:
//	uint8	adcAddress[3] = {0xAA, 0xAC, 0xB2};

PinCoding::PinCoding(void)
	:code(0)
	{
	}

bool PinCoding::connect(uint16 thresholds[3])
	{
	bool	result(true);
	uint8	i;

#ifndef NO_PINCODING
	uint8	buffer[2];
	
	buffer[1] = 0xFF;
	
	//	Invert register values:
	//	Connected to ground will be annotated by a logical "1"
	for (i=0; i<3; i++)
		{
		//	Set register as input:
		buffer[0] = 0x18 + i;
		coreI2CWrite(kI2C0, pcaAddress, buffer, 2, forever);
		coreSystemTimerWait(1);
		buffer[0] = 0x10 + i;
		coreI2CWrite(kI2C0, pcaAddress, buffer, 2, forever);
		coreSystemTimerWait(1);
		}
#endif

	for (i=0; i<3; i++)
		{
		averagePhaseCurrentValue[i] = 0;
		phaseLossCounter[i] = 0;
		phaseLossDetectionThreshold[i] = thresholds[i];
	#if (__DEBUG__ > 3)
		controller.asc0 << "Phase " << (char)('A'+i) << " threshold: " << phaseLossDetectionThreshold[i] << " mA" << newline;
	#endif
		}

	nextPhaseCheckTime = 0;
	result = updatePinCoding();

	return result;
	}

bool PinCoding::updatePinCoding(void)
	{
	bool	result(true);
	uint32	bit0(1);

#ifdef __DEBUG__
	bool	primary(false);
	bool	maintenance(false);
#elif defined NO_PINCODING
	bool	primary(false);
	bool	maintenance(false);
#endif	
#ifdef NO_PINCODING
	code = (bit0 << 24);
	code |= (bit0 << 16);
#ifdef PRIMARY
	primary = PRIMARY;
#else
	primary = true;
#endif
	if (primary)
		code |= (bit0 << 8);

#ifdef MAINTENANCE
	maintenance = MAINTENANCE;
#else
	maintenance = false;
#endif
	if (maintenance)
		code |= (bit0 << 9);
#else
	uint8	i;
	uint32	rawPinCode;
	uint8	bitCount(0);
	uint8	feederID(0);
	uint8	deviceID(0);
	bool	parity(false);
	rawPinCode = 0;

//#ifdef IPECO_PSC_Revision
#if true
	for (i=0; i<3; i++)
		{	
		uint8	buffer[1];
		
		buffer[0] = i;
		coreI2CWrite(kI2C0, pcaAddress, buffer, 1, forever);
		coreI2CRead(kI2C0, pcaAddress, buffer, 1, forever);
		bytes[i] = buffer[0];
		rawPinCode |= ((uint32)buffer[0]) << (i*8);
	#if (__DEBUG__ > 3)
		controller.asc0 << "#" << i << " == " << kModeHex << buffer[0] << ((i<2)?", ":"\r");
	#endif
		}
#else
	coreI2CReadFromRegister(kI2C0, pcaAddress, 0, bytes, 3, forever);

	for (i=0; i<3; i++)
		{
	#if (__DEBUG__ > 3)
		controller.asc0 << "#" << i << " == " << kModeHex << bytes[i] << ((i<2)?", ":"\r");
	#endif
		rawPinCode |= ((uint32)bytes[i]) << (i*8);
		}
#endif
	code = 0;

	//	Decode feeder ID
	for (i=0; i<5; i++)
		{
		if (bytes[0] & (1 << (2 + i)))
			{
			bitCount++;
			feederID |= (bit0 << (4 - i));
			}
		}

	//	Decode device ID
	if (bytes[0] & (1 << 7))
		{
		bitCount++;
		deviceID |= (bit0 << 6);
		}

	for (i=0; i<4; i++)
		{
		if (bytes[1] & (1 << i))
			{
			bitCount++;
			deviceID |= (bit0 << (5 - i));
			}
		}

	for (i=0; i<2; i++)
		{
		if (bytes[1] & (1 << (i + 5)))
			{
			bitCount++;
			deviceID |= (bit0 << (1 - i));
			}
		}
	
	//	Decode primary/secondary
	if (!(bytes[0] & 0x01))
		code |= (bit0 << 8);
	else
		bitCount++;
		
	//	Decode maintenance
	if ((bytes[2] & (1 << 2)))
		{
		code |= (bit0 << 9);
		bitCount++;
		}
		
	//	Decode parity
	parity = ((bytes[1] & (1 << 4)) != 0);
	if (parity)
		code |= (bit0 << 10);
		
	code |= (uint32)feederID << 24;
	code |= (uint32)deviceID << 16;
	
	if (parity != ((bitCount & 0x01) != 0))
		code |= (bit0 << 4);

#if (__DEBUG__ > 0)
	controller.asc0 << "feeder ID is " << kModeHex << feederID << newline;
	controller.asc0 << "device ID is " << kModeHex << deviceID << newline;
	controller.asc0 << (primary?"primary":"secondary") << newline;
	controller.asc0 << (maintenance?"maintenance":"no maintenance") << newline;
	controller.asc0 << (parity?"odd":"even") << " parity" << newline;
	controller.asc0 << "parity is " << ((code & (bit0 << 4))?"odd":"even") << newline;
	controller.asc0 << "rawPinCode code is " << kModeHex << rawPinCode << newline;
	controller.asc0 << "pinCode code is    " << kModeHex << code << newline;
#endif
#endif


#ifndef NO_PINCODING
	if (rawPinCode == 0x00020100)
		code = 0xFFFFFFFF;
#endif
		
	return result;
	}


uint32 PinCoding::getPinCodingValue(void)
	{
	return code;
	}

#ifdef NO_PINCODING
uint16 PinCoding::readoutPhaseCurrentValue(uint8 phase)
	{
    uint16	result(0);

	if (phase < 3)
		{
		adcReady[phase] = true;
	#ifdef OFFLINE
	#ifdef IPECO_PSC_Revision
	#ifdef E_DEVICE
		#if (PARTNUMBER == 921)
		if (!corePortsGetPortPin(kPortC, kPortPin1) && (phase == 1))
			result = 300;
		else
			result = 1200;
		#else
			result = 5500;
	/*
		//if (corePortsGetPortPin(kPortE, kPortPin6))
		if (!corePortsGetPortPin(kPortC, kPortPin6) && (phase == 1))
			result = 300;
		else
			result = 5500;
	*/
		#endif
	#else
		#if (PARTNUMBER == 921)
		result = 900;
		#else
		result = 5500;
		#endif
	#endif
	#else
	#ifdef AVR
		result = 900;
	#else
		if (corePortsGetPortBit(kPort2, kPortPin10))
			result = 5500;
		else
			result = 900;
	#endif
	#endif
	#endif
		}

	return result;
	}
#else
uint16 PinCoding::readoutPhaseCurrentValue(uint8 phase)
	{
    uint16	result(0);

	if (phase < 3)
		{
		uint8	buffer[2];
	    uint16	adcReading;
		
	#ifdef AVR
		adcReady[phase] = (coreI2CReadFromRegister(kI2C0, adcAddress[phase], 0, buffer, 2, 100) == 2);
	#if (__DEBUG__ > 1)
		if (!adcReady[phase])
			controller.asc0 << "error reading from ADC #" << phase << newline;
	#endif
	#else
		adcReady[phase] = (coreI2CReadFromRegister(kI2C0, adcAddress[phase], 0, buffer, 2, 100) == 2);
	#if (__DEBUG__ > 1)
		if (!adcReady[phase])
			controller.asc0 << "error reading from ADC #" << phase << newline;
	#endif
	#endif
		if (adcReady[phase])
			{
			float	floatResult;
			
			adcReading = buffer[1];
			adcReading |= ((uint16)buffer[0]) << 8;
			
			floatResult = ((float)adcReading * 5000.0)/203.0;
			result = (uint16)(floatResult + 0.5);
			}
		}

	return result;
	}
#endif

uint16 PinCoding::getPhaseValue(uint8 phase)
	{
	return averagePhaseCurrentValue[phase];
	}

void PinCoding::setPhaseCheckDelayTime(tick_t t)
	{
	nextPhaseCheckTime = coreSystemTimerTicks() + t;
	}

bool PinCoding::handlePeriodicTask(bool throwEventOnPhaseLoss)
	{
	tick_t	now(coreSystemTimerTicks());
	uint8	i;
	
	uint8	nADC(3);

	for (i=0; i<nADC; i++)	
		{
		//	Read out ADC value
		//	Check for correct reading of ADC
		uint16	phaseCurrentValue(readoutPhaseCurrentValue(i));
	
		if (adcReady[i])
			{
		#if (PARTNUMBER == 916)
			uint32	averageValue((uint32)averagePhaseCurrentValue[i] * 3UL);
		#elif (PARTNUMBER == 921)
			uint32	averageValue((uint32)averagePhaseCurrentValue[i] * 4UL);
		#else
			uint32	averageValue((uint32)averagePhaseCurrentValue[i] * 6UL);
		#endif

			averageValue += phaseCurrentValue;

		#if (PARTNUMBER == 916)
			averagePhaseCurrentValue[i] = (uint16)(averageValue/4UL);
		#elif (PARTNUMBER == 921)
			averagePhaseCurrentValue[i] = (uint16)(averageValue/5UL);
		#else
			averagePhaseCurrentValue[i] = (uint16)(averageValue/7UL);
		#endif
		#if (__DEBUG__ > 4)
			controller.asc0 << "#" << i << " == " << averagePhaseCurrentValue[i] << " mA)" << ((i<(nADC-1))?", ":"\r");
		#endif
			}
		else
			{
		#if (__DEBUG__ > 3)
			controller.asc0 << "#" << i << " failed" << newline;
		#endif
			}
		}		

	if (nextPhaseCheckTime < now)
		{
		int8	n;
		
	#if (__DEBUG__ > 4)
		controller.asc0 << "handlePeriodicTask now == " << now << ", " << nextPhaseCheckTime << newline;
	#endif

		//	Count current consuming phases
		n = 0;

	#if (PARTNUMBER == 921)
		uint16	minimumPhaseCurrentValue(0xFFFF);
		
		for (i=0; i<nADC; i++)	
			{
			//	Check for correct reading of ADC
			if (adcReady[i])
				{
			#if (__DEBUG__ > 4)
				controller.asc0 << "Phase " << (char)('A'+i) << ": " << averagePhaseCurrentValue[i] << " mA" << ((i<(nADC-1))?", ":"\r");
				controller.asc0 << "Phase " << (char)('A'+i) << ": " << phaseLossDetectionThreshold[i] << " mA" << ((i<(nADC-1))?", ":"\r");
			#endif
				if (averagePhaseCurrentValue[i] < minimumPhaseCurrentValue)
					minimumPhaseCurrentValue = averagePhaseCurrentValue[i];					
				}
			else
				{
			#if (__DEBUG__ > 2)
				controller.asc0 << "Phase " << (char)('A'+i) << " failed" << newline;
			#endif
				break;
				}
			}		

		for (i=0; i<nADC; i++)	
			{
			//	Check for correct reading of ADC
			if (adcReady[i])
				{
				uint16	phaseImbalance(averagePhaseCurrentValue[i] - minimumPhaseCurrentValue);
				
				if (phaseImbalance < phaseLossDetectionThreshold[i])
					{
    				if (phaseLossCounter[i] > 0)
    					{
					#if (__DEBUG__ > 2)
						controller.asc0 << "Phase" << (char)('A'+i) << ": phase loss counter cleared" << newline;
					#endif
						phaseLossCounter[i] = 0;
						}
					n++;
					}
				else
					phaseLossCounter[i]++;

			#if (__DEBUG__ > 4)
				controller.asc0 << "i == " << ", n == " << n << ", counter == " << phaseLossCounter[i]
					<< ", imbalance == " << phaseImbalance << ", threshold == " << phaseLossDetectionThreshold[i] << newline;
			#endif
				}
			else
				{
				n = -1;
				break;
				}
			}		
	#if (__DEBUG__ > 4)
		controller.asc0 << (throwEventOnPhaseLoss?"will throw event, ":"") << "n == " << n << ", minimumPhaseCurrentValue == " << minimumPhaseCurrentValue << newline;
	#endif
	#else
		for (i=0; i<nADC; i++)	
			{
			//	Check for correct reading of ADC
			if (adcReady[i])
				{
			#if (__DEBUG__ > 4)
				controller.asc0 << "Phase " << (char)('A'+i) << ": " << averagePhaseCurrentValue[i] << " mA" << ((i<(nADC-1))?", ":"\r");
				controller.asc0 << "Phase " << (char)('A'+i) << ": " << phaseLossDetectionThreshold[i] << " mA" << ((i<(nADC-1))?", ":"\r");
			#endif
				if (averagePhaseCurrentValue[i] >= phaseLossDetectionThreshold[i])
					{
    				if (phaseLossCounter[i] > 0)
    					{
					#if (__DEBUG__ > 2)
						controller.asc0 << "Phase " << (char)('A'+i) << ": phase loss counter cleared" << newline;
					#endif
						phaseLossCounter[i] = 0;
						}
					n++;
					}
				else if (throwEventOnPhaseLoss)
					{
				#if (__DEBUG__ > 2)
					controller.asc0 << "Phase " << (char)('A'+i) << ": phase loss counter == " << phaseLossCounter[i]
						  << " is " << averagePhaseCurrentValue[i] << " mA"
						  << ", threshold " << phaseLossDetectionThreshold[i] << " mA" << newline;
				#endif
					phaseLossCounter[i]++;
					}
				}
			else
				{
			#if (__DEBUG__ > 2)
				controller.asc0 << "Phase " << (char)('A'+i) << " failed" << newline;
			#endif
				n = -1;
				break;
				}
			}		
	#endif
		if (throwEventOnPhaseLoss && (n != -1))
			{
			bool	phaseLossDetected(false);
		#if (PARTNUMBER == 916)
			uint8	phaseLossCounterMax(7);
		#elif (PARTNUMBER == 921)
			uint8	phaseLossCounterMax(7);
		#else
			uint8	phaseLossCounterMax(5);
	 	#endif
		#if (__DEBUG__ > 4)
			controller.asc0 << "throwEventOnPhaseLoss: n == " << n << ", nADC == " << nADC << newline;
		#endif
		#if (PARTNUMBER == 916)
			if (n<2)
				{
				i = 0;
				phaseLossCounter[i]++;					
				phaseLossDetected = (phaseLossCounter[i] > phaseLossCounterMax);
			#if (__DEBUG__ > 0)
				controller.asc0 << "Multiple phase loss: counter == " << phaseLossCounter[i] << newline;
			#endif
				}
			else
		#else
			if (n < nADC)
	 	#endif
				{
				for (i=0; i<nADC; i++)
					{
					phaseLossDetected = (phaseLossCounter[i] > phaseLossCounterMax) && (n > 0);
					if (phaseLossDetected)
						{
					#if (__DEBUG__ > 0)
						controller.asc0 << "sending kGAINPhaseLossEvent (triggered by phase " << (char)('A'+i) << ")" << newline;
					#endif
						event	e;
						
						e.eventClass = kGAINEventClass;
						e.eventType = kGAINPhaseLossEvent;
						e.data.data = i;
						sendEventSelf(e);
						break;
						}
					}
				}

		#if (PARTNUMBER == 920)||(PARTNUMBER == 922)
			if (!phaseLossDetected && (n == nADC))
				{
				float	maxCurrent(0.0);
				float	elementFailureThreshold;
					
				for (i=0; i<3; i++)
					{
					if (averagePhaseCurrentValue[i] > maxCurrent)
						maxCurrent = averagePhaseCurrentValue[i];
					}

				elementFailureThreshold = maxCurrent * 0.7;

				for (i=0; (i<3); i++)
					{
					if ((averagePhaseCurrentValue[i] > phaseLossDetectionThreshold[i]) && (averagePhaseCurrentValue[i] < elementFailureThreshold))
						{
					#if (__DEBUG__ > 0)
						controller.asc0 << "Phase" << (char)('A'+i) << ": failure counter == " << elementFailureCounter[i] << newline;
					#endif
						elementFailureCounter[i]++;
						}
					else if (averagePhaseCurrentValue[i] > elementFailureThreshold)
						{
		    			if (elementFailureCounter[i] > 0)
		    				{
						#if (__DEBUG__ > 2)
							controller.asc0 << "Phase" << (char)('A'+i) << ": failure counter cleared" << newline;
						#endif
							elementFailureCounter[i] = 0;
							}
						}
					}
				}
		#endif
			}
		else
			{
			for (i=0; i<nADC; i++)
				{
    			if (phaseLossCounter[i] > 0)
    				{
				#if (__DEBUG__ > 2)
					controller.asc0 << "Phase" << (char)('A'+i) << ": phase loss counter cleared" << newline;
				#endif
					phaseLossCounter[i] = 0;
					}

			#if (PARTNUMBER == 920)||(PARTNUMBER == 922)
    			if (elementFailureCounter[i] > 0)
    				{
				#if (__DEBUG__ > 2)
					controller.asc0 << "Phase" << (char)('A'+i) << ": failure counter cleared" << newline;
				#endif
					elementFailureCounter[i] = 0;
					}
			#endif
				}
			}

	#if (PARTNUMBER == 916)
		nextPhaseCheckTime = now + 500;
	#elif (PARTNUMBER == 921)
		nextPhaseCheckTime = now + 500;
	#else
		nextPhaseCheckTime = now + 1000;
	#endif
	#if (__DEBUG__ > 4)
		controller.asc0 << "handlePeriodicTask now == " << now << ", " << nextPhaseCheckTime << newline;
	#endif
		}
	return 0;
	}

#if (PARTNUMBER == 920)||(PARTNUMBER == 922)
bool PinCoding::elementFailureDetected(uint8& id)
	{
	int8	i;
	uint8	failureBits(0);
	
	for (i=0; i<3; i++)
		{
		if (elementFailureCounter[i] >= 5)
			failureBits |= (1 << i);
		}

	if ((failureBits & 0x3) == 0x03)	 	//	Phase 1+2
		{
		id = 1;
		}
	else if ((failureBits & 0x05) == 0x05)	//	Phase 1+3
		{
		id = 3;
		}
	else if ((failureBits & 0x06) == 0x06)	//	Phase 2+3
		{
		id = 2;
		}
	else
		{
		id = 0;
		}
		
	if (id)
		{
		for (i=0; i<3; i++)
			{
			elementFailureCounter[i] = 0;
			}
	#if (__DEBUG__ > 2)
		if (id)
			controller.asc0 << "elementFailureDetected id == " << id << ", failureBits == " << failureBits << newline;
	#endif
		}

	return (id != 0);
	}

bool PinCoding::thermalFuseFailureDetected(void)
	{
	int8	i;
	uint8	failureBits(0);
	
	for (i=0; i<3; i++)
		{
	#if (PARTNUMBER == 920)
		if (phaseLossCounter[i] >= 5)
	#else
	#ifdef OFFLINE
		if (phaseLossCounter[i] >= 10)
	#else
		if (phaseLossCounter[i] >= 120)
	#endif
	#endif
			failureBits |= (1 << i);
		}


#if (__DEBUG__ > 2)
	if (failureBits)
		controller.asc0 << "thermalFuseFailureDetected failureBits == " << kModeHex << failureBits << newline;
#endif

	return (failureBits == 0x07);
	}
#endif

