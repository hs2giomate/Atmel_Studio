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
../Device_Startup


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Device_Startup/startup_same54.c \
../Device_Startup/system_same54.c \
../main.cpp \
../SPI_Asyn_Class.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
SPI_Asyn_Class.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
SPI_Asyn_Class.o

C_DEPS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
SPI_Asyn_Class.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
SPI_Asyn_Class.d

OUTPUT_FILE_PATH +=SPI_Asyn_Class.elf

OUTPUT_FILE_PATH_AS_ARGS +=SPI_Asyn_Class.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../Device_Startup/same54p20a_flash.ld \
../Device_Startup/same54p20a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_same54.o: ../Device_Startup/startup_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_same54.o: ../Device_Startup/system_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/Config" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/examples" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/include" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/utils/include" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/adc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/cmcc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/core" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/dmac" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/evsys" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/gclk" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/mclk" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/osc32kctrl" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/oscctrl" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/pm" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/port" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/ramecc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/rtc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/sercom" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/systick" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/usb" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hri" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc/device" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/device" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include" -I".." -I"../Device_Startup"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./SPI_Asyn_Class.o: .././SPI_Asyn_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/Config" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/examples" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/include" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/utils/include" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/adc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/cmcc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/core" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/dmac" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/evsys" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/gclk" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/mclk" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/osc32kctrl" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/oscctrl" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/pm" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/port" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/ramecc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/rtc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/sercom" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/systick" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/usb" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hri" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc/device" -I"../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/device" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include" -I".." -I"../Device_Startup"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="SPI_Asyn_Class.map" -Wl,--start-group -lm  -Wl,--end-group -L"..\\Device_Startup"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "SPI_Asyn_Class.elf" "SPI_Asyn_Class.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "SPI_Asyn_Class.elf" "SPI_Asyn_Class.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "SPI_Asyn_Class.elf" "SPI_Asyn_Class.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "SPI_Asyn_Class.elf" > "SPI_Asyn_Class.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "SPI_Asyn_Class.elf" "SPI_Asyn_Class.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "SPI_Asyn_Class.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SPI_Asyn_Class.elf" "SPI_Asyn_Class.a" "SPI_Asyn_Class.hex" "SPI_Asyn_Class.bin" "SPI_Asyn_Class.lss" "SPI_Asyn_Class.eep" "SPI_Asyn_Class.map" "SPI_Asyn_Class.srec"
	