
/*!
LT_I2C: Routines to communicate with ATmega328P's hardware I2C port.

@verbatim

LT_I2C contains the low level routines to communicate with devices using the
ATMega328's onboard hardware I2C port. Each routine checks the Two Wire Status
Register (TWSR) at the end of the transaction and returns 0 if successful and 1
if not successful.

I2C Frequency = (CPU Clock frequency)/(16+2(TWBR)*Prescaler)

TWBR-Two Wire Bit Rate Register
TWCR=Two Wire Control Register (TWINT TWEA TWSTA TWSTO TWWC TWEN - TWIE)
TWSR=Two Wire Status Register

Prescaler Values:
TWSR1  TWSR0  Prescaler
   0      0      1
   0      1      4
   1      0      16
   1      1      64

Examples:
CPU Frequency = 16Mhz on Arduino Uno
I2C Frequency  Prescaler  TWSR1  TWSR0  TWBR
  1khz         64         1      1      125
  10khz        64         1      1      12
  50khz        16         1      0      10
  100khz        4         0      1      18
  400khz        1         0      0      12

@endverbatim


Copyright 2018(c) Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.
 - Neither the name of Analog Devices, Inc. nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.
 - The use of this software may or may not infringe the patent rights
   of one or more patent holders.  This license does not release you
   from the requirement that you obtain separate licenses from these
   patent holders to use this software.
 - Use of the software either in source or binary form, must be run
   on or directly connected to an Analog Devices Inc. component.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//! @ingroup Linduino
//! @{
//! @defgroup LT_I2C LT_I2C: Routines to Communicate With ATmega328P's hardware I2C port.
//! @}

/*! @file
    @ingroup LT_I2C
    Library for LT_I2C: Routines to Communicate With ATmega328P's hardware I2C port.
*/


#include "LT_I2C.h"

//! CPU master clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
