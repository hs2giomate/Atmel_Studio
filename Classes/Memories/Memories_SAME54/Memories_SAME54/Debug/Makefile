################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Device_Startup/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Device_Startup/startup_samd21.c \
../Device_Startup/system_samd21.c \
../FlashMemoryClass.cpp \
../FRAM_Memory_Class.cpp \
../main.cpp \
../Parameters.cpp \
../SPI_Syn_Class.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
FlashMemoryClass.o \
FRAM_Memory_Class.o \
main.o \
Parameters.o \
SPI_Syn_Class.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
FlashMemoryClass.o \
FRAM_Memory_Class.o \
main.o \
Parameters.o \
SPI_Syn_Class.o

C_DEPS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
FlashMemoryClass.d \
FRAM_Memory_Class.d \
main.d \
Parameters.d \
SPI_Syn_Class.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
FlashMemoryClass.d \
FRAM_Memory_Class.d \
main.d \
Parameters.d \
SPI_Syn_Class.d

OUTPUT_FILE_PATH +=libMemories_SAME54.a

OUTPUT_FILE_PATH_AS_ARGS +=libMemories_SAME54.a

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../Device_Startup/samd21j18a_flash.ld \
../Device_Startup/samd21j18a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_samd21.o: ../Device_Startup/startup_samd21.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_samd21.o: ../Device_Startup/system_samd21.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./FlashMemoryClass.o: .././FlashMemoryClass.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/Config" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/examples" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hal/include" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\hal\utils\include" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/core" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/dmac" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/gclk" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/pm" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/port" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sercom" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sysctrl" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/usb" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hri" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc\device" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./FRAM_Memory_Class.o: .././FRAM_Memory_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/Config" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/examples" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hal/include" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\hal\utils\include" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/core" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/dmac" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/gclk" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/pm" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/port" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sercom" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sysctrl" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/usb" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hri" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc\device" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/Config" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/examples" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hal/include" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\hal\utils\include" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/core" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/dmac" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/gclk" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/pm" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/port" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sercom" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sysctrl" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/usb" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hri" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc\device" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./Parameters.o: .././Parameters.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/Config" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/examples" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hal/include" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\hal\utils\include" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/core" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/dmac" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/gclk" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/pm" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/port" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sercom" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sysctrl" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/usb" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hri" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc\device" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./SPI_Syn_Class.o: .././SPI_Syn_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/Config" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/examples" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hal/include" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\hal\utils\include" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/core" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/dmac" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/gclk" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/pm" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/port" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sercom" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/sysctrl" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hpl/usb" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/hri" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc" -I"..\..\..\..\..\Applications\Maintenance\SAMD21_DRIVER_INTERFACE\SAMD21_DRIVER_INTERFACE\usb\class\cdc\device" -I"../../../../../Applications/Maintenance/SAMD21_DRIVER_INTERFACE/SAMD21_DRIVER_INTERFACE/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)


$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Archiver : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-ar.exe$(QUOTE) -r  -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) 
	@echo Finished building target: $@
	
	




# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "libMemories_SAME54.elf" "libMemories_SAME54.a" "libMemories_SAME54.hex" "libMemories_SAME54.bin" "libMemories_SAME54.lss" "libMemories_SAME54.eep" "libMemories_SAME54.map" "libMemories_SAME54.srec"
	