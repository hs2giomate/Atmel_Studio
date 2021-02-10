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
../Commands_Handler.cpp \
../Device_Startup/startup_same54.c \
../Device_Startup/system_same54.c \
../main.cpp \
../MaintenanceDataHandler.cpp \
../Maintenance_Tool.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Commands_Handler.o \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
MaintenanceDataHandler.o \
Maintenance_Tool.o

OBJS_AS_ARGS +=  \
Commands_Handler.o \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
MaintenanceDataHandler.o \
Maintenance_Tool.o

C_DEPS +=  \
Commands_Handler.d \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
MaintenanceDataHandler.d \
Maintenance_Tool.d

C_DEPS_AS_ARGS +=  \
Commands_Handler.d \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
MaintenanceDataHandler.d \
Maintenance_Tool.d

OUTPUT_FILE_PATH +=MaintenanceInterface.elf

OUTPUT_FILE_PATH_AS_ARGS +=MaintenanceInterface.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/Classes/CAN/CANOpen_Class/CANOpen_Class/NMTMaster/libCANOpen_Class.a \
S:/AtmelStudio/Applications/ECS_Box/ALU_Driver/ALU_Driver/Debug/libALU_Driver.a \
S:/AtmelStudio/Applications/ECS_Box/Compresor_Controller/Compresor_Controller/Debug/libCompresor_Controller.a \
S:/AtmelStudio/Applications/ECS_Box/Dual_Heater_P0/Dual_Heater_P0/Debug/libDual_Heater_P0.a \
S:/AtmelStudio/Applications/ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Debug/libECS_Box_Objects.a \
S:/AtmelStudio/Applications/ECS_Box/EvaporatorAndCondesatorFan/EvaporatorAndCondesatorFan/Debug/libEvaporatorAndCondesatorFan.a \
S:/AtmelStudio/Applications/ECS_Box/Event_Manager/Event_Manager/Debug/libEvent_Manager.a \
S:/AtmelStudio/Applications/ECS_Box/FlapperValve/Dual_Flapper_Valve_Controller/Dual_Flapper_Valve_Controller/Debug/libDual_Flapper_Valve_Controller.a \
S:/AtmelStudio/Applications/ECS_Box/FlapperValve/SingleFlapperValveP0/SingleFlapperValveP0/Debug/libSingleFlapperValveP0.a \
S:/AtmelStudio/Applications/ECS_Box/HeaterP0Board/HeaterP0Board/Debug/libHeaterP0Board.a \
S:/AtmelStudio/Applications/ECS_Box/QSPI_Memory_Handler/Debug/libQSPI_Memory_Handler.a \
S:/AtmelStudio/Applications/ECS_Box/Scavenger_Fan_P0/Scavenger_Fan_P0/Debug/libScavenger_Fan_P0.a \
S:/AtmelStudio/Applications/ECS_Box/TemperatureSensors/TemperatureSensorsSampling/TemperatureSensorsSampling/Debug/libTemperatureSensorsSampling.a

LINKER_SCRIPT_DEP+=  \
../Device_Startup/same54p20a_flash.ld \
../Device_Startup/same54p20a_sram.ld


# AVR32/GNU C Compiler
./Commands_Handler.o: .././Commands_Handler.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG -DSELF_STANDALONE -DATP_TEST  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/SPI" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Storage" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/USBSerial" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tcc" -I".." -I"../../../../../ECS_Box/HeaterP0Board/HeaterP0Board" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/FlapperValve" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../../../ECS_Box/EvaporatorAndCondesatorFan/EvaporatorAndCondesatorFan" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/LTC2983" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Timers" -I"../../../../../ECS_Box/TemperatureSensors/TemperatureSensorsSampling/TemperatureSensorsSampling" -I"../../../../../ECS_Box/FlapperValve/SingleFlapperValveP0/SingleFlapperValveP0" -I"../../../../../ECS_Box/QSPI_Memory_Handler" -I"../../../../../ECS_Box/FlapperValve/Dual_Flapper_Valve_Controller/Dual_Flapper_Valve_Controller" -I"../../../../../ECS_Box/Dual_Heater_P0/Dual_Heater_P0" -I"../../../../../ECS_Box/Scavenger_Fan_P0/Scavenger_Fan_P0" -I"../../../../../ECS_Box/Compresor_Controller/Compresor_Controller" -I"../../../../../../Classes/CAN/CANOpen_Class/CANOpen_Class" -I"../../../../../ECS_Box/Event_Manager/Event_Manager"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/startup_same54.o: ../Device_Startup/startup_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG -DSELF_STANDALONE  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_same54.o: ../Device_Startup/system_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG -DSELF_STANDALONE  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG -DSELF_STANDALONE -DATP_TEST  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/SPI" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Storage" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/USBSerial" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tcc" -I".." -I"../../../../../ECS_Box/HeaterP0Board/HeaterP0Board" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/FlapperValve" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../../../ECS_Box/EvaporatorAndCondesatorFan/EvaporatorAndCondesatorFan" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/LTC2983" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Timers" -I"../../../../../ECS_Box/TemperatureSensors/TemperatureSensorsSampling/TemperatureSensorsSampling" -I"../../../../../ECS_Box/FlapperValve/SingleFlapperValveP0/SingleFlapperValveP0" -I"../../../../../ECS_Box/QSPI_Memory_Handler" -I"../../../../../ECS_Box/FlapperValve/Dual_Flapper_Valve_Controller/Dual_Flapper_Valve_Controller" -I"../../../../../ECS_Box/Dual_Heater_P0/Dual_Heater_P0" -I"../../../../../ECS_Box/Scavenger_Fan_P0/Scavenger_Fan_P0" -I"../../../../../ECS_Box/Compresor_Controller/Compresor_Controller" -I"../../../../../../Classes/CAN/CANOpen_Class/CANOpen_Class" -I"../../../../../ECS_Box/Event_Manager/Event_Manager"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./MaintenanceDataHandler.o: .././MaintenanceDataHandler.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG -DSELF_STANDALONE -DATP_TEST  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/SPI" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Storage" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/USBSerial" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tcc" -I".." -I"../../../../../ECS_Box/HeaterP0Board/HeaterP0Board" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/FlapperValve" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../../../ECS_Box/EvaporatorAndCondesatorFan/EvaporatorAndCondesatorFan" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/LTC2983" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Timers" -I"../../../../../ECS_Box/TemperatureSensors/TemperatureSensorsSampling/TemperatureSensorsSampling" -I"../../../../../ECS_Box/FlapperValve/SingleFlapperValveP0/SingleFlapperValveP0" -I"../../../../../ECS_Box/QSPI_Memory_Handler" -I"../../../../../ECS_Box/FlapperValve/Dual_Flapper_Valve_Controller/Dual_Flapper_Valve_Controller" -I"../../../../../ECS_Box/Dual_Heater_P0/Dual_Heater_P0" -I"../../../../../ECS_Box/Scavenger_Fan_P0/Scavenger_Fan_P0" -I"../../../../../ECS_Box/Compresor_Controller/Compresor_Controller" -I"../../../../../../Classes/CAN/CANOpen_Class/CANOpen_Class" -I"../../../../../ECS_Box/Event_Manager/Event_Manager"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./Maintenance_Tool.o: .././Maintenance_Tool.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG -DSELF_STANDALONE -DATP_TEST  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/SPI" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Storage" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/USBSerial" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils" -I"../../../../../ECS_Box/ALU_Driver/ALU_Driver/hpl/tcc" -I".." -I"../../../../../ECS_Box/HeaterP0Board/HeaterP0Board" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/FlapperValve" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../../../ECS_Box/EvaporatorAndCondesatorFan/EvaporatorAndCondesatorFan" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/LTC2983" -I"../../../../../ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Timers" -I"../../../../../ECS_Box/TemperatureSensors/TemperatureSensorsSampling/TemperatureSensorsSampling" -I"../../../../../ECS_Box/FlapperValve/SingleFlapperValveP0/SingleFlapperValveP0" -I"../../../../../ECS_Box/QSPI_Memory_Handler" -I"../../../../../ECS_Box/FlapperValve/Dual_Flapper_Valve_Controller/Dual_Flapper_Valve_Controller" -I"../../../../../ECS_Box/Dual_Heater_P0/Dual_Heater_P0" -I"../../../../../ECS_Box/Scavenger_Fan_P0/Scavenger_Fan_P0" -I"../../../../../ECS_Box/Compresor_Controller/Compresor_Controller" -I"../../../../../../Classes/CAN/CANOpen_Class/CANOpen_Class" -I"../../../../../ECS_Box/Event_Manager/Event_Manager"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="MaintenanceInterface.map" -Wl,--start-group -lm -lCANOpen_Class -lALU_Driver -lCompresor_Controller -lDual_Heater_P0 -lECS_Box_Objects -lEvaporatorAndCondesatorFan -lEvent_Manager -lDual_Flapper_Valve_Controller -lSingleFlapperValveP0 -lHeaterP0Board -lQSPI_Memory_Handler -lScavenger_Fan_P0 -lTemperatureSensorsSampling  -Wl,--end-group -L"..\\Device_Startup" -L"S:\AtmelStudio\Classes\CAN\CANOpen_Class\CANOpen_Class\NMTMaster" -L"S:\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\Compresor_Controller\Compresor_Controller\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\Dual_Heater_P0\Dual_Heater_P0\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\ECS_Box_Classes\ECS_Box_Classes\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\EvaporatorAndCondesatorFan\EvaporatorAndCondesatorFan\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\Event_Manager\Event_Manager\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\FlapperValve\Dual_Flapper_Valve_Controller\Dual_Flapper_Valve_Controller\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\FlapperValve\SingleFlapperValveP0\SingleFlapperValveP0\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\HeaterP0Board\HeaterP0Board\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\QSPI_Memory_Handler\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\Scavenger_Fan_P0\Scavenger_Fan_P0\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\TemperatureSensors\TemperatureSensorsSampling\TemperatureSensorsSampling\Debug"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "MaintenanceInterface.elf" "MaintenanceInterface.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "MaintenanceInterface.elf" "MaintenanceInterface.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "MaintenanceInterface.elf" "MaintenanceInterface.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "MaintenanceInterface.elf" > "MaintenanceInterface.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "MaintenanceInterface.elf" "MaintenanceInterface.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "MaintenanceInterface.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "MaintenanceInterface.elf" "MaintenanceInterface.a" "MaintenanceInterface.hex" "MaintenanceInterface.bin" "MaintenanceInterface.lss" "MaintenanceInterface.eep" "MaintenanceInterface.map" "MaintenanceInterface.srec"
	