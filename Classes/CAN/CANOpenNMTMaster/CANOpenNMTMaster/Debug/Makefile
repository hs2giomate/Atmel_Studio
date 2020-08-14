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
../Device_Startup/startup_same54.c \
../Device_Startup/system_same54.c \
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o

C_DEPS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d

OUTPUT_FILE_PATH +=CANOpenNMTMaster.elf

OUTPUT_FILE_PATH_AS_ARGS +=CANOpenNMTMaster.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/Applications/ECS_Box/ALU_Driver/ALU_Driver/Debug/libALU_Driver.a \
S:/AtmelStudio/Applications/ECS_Box/ALU_Driver/ALU_Driver/Debug/libALU_Driver.a

LINKER_SCRIPT_DEP+=  \
../Device_Startup/same54p20a_flash.ld \
../Device_Startup/same54p20a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_same54.o: ../Device_Startup/startup_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_same54.o: ../Device_Startup/system_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG -DUSE_HAL_DRIVER -DCAN_TEST_CODE -DCO_USE_GLOBALS -DMASTER=1  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/Config" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/examples" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/can" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/core" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/eic" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pac" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/pm" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/port" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/tc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/hri" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/usb/device" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../../../Applications/ECS_Box/ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"../../../CANOpen_Class/CANOpen_Class"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="CANOpenNMTMaster.map" -Wl,--start-group -lm -lALU_Driver -lALU_Driver  -Wl,--end-group -L"..\\Device_Startup" -L"S:\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\Debug"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "CANOpenNMTMaster.elf" "CANOpenNMTMaster.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "CANOpenNMTMaster.elf" "CANOpenNMTMaster.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "CANOpenNMTMaster.elf" "CANOpenNMTMaster.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "CANOpenNMTMaster.elf" > "CANOpenNMTMaster.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "CANOpenNMTMaster.elf" "CANOpenNMTMaster.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "CANOpenNMTMaster.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "CANOpenNMTMaster.elf" "CANOpenNMTMaster.a" "CANOpenNMTMaster.hex" "CANOpenNMTMaster.bin" "CANOpenNMTMaster.lss" "CANOpenNMTMaster.eep" "CANOpenNMTMaster.map" "CANOpenNMTMaster.srec"
	