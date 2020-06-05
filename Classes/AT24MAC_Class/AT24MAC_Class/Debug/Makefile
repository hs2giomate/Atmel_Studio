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
../AT24MAC_Class.cpp \
../Device_Startup/startup_same54.c \
../Device_Startup/system_same54.c \
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
AT24MAC_Class.o \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o

OBJS_AS_ARGS +=  \
AT24MAC_Class.o \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o

C_DEPS +=  \
AT24MAC_Class.d \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d

C_DEPS_AS_ARGS +=  \
AT24MAC_Class.d \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d

OUTPUT_FILE_PATH +=AT24MAC_Class.elf

OUTPUT_FILE_PATH_AS_ARGS +=AT24MAC_Class.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../Device_Startup/same54p20a_flash.ld \
../Device_Startup/same54p20a_sram.ld


# AVR32/GNU C Compiler
./AT24MAC_Class.o: .././AT24MAC_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/Config" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/examples" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hal/include" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hal/utils/include" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/cmcc" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/core" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/dmac" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/gclk" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/mclk" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/osc32kctrl" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/oscctrl" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/pm" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/port" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/ramecc" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/sercom" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/usb" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hri" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb/class/cdc" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb/class/cdc/device" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb/device" -I"../../../I2C/I2C_Asyn_Class/I2C_Asyn_Class" -I"../../../USB/USB_CDC_Class/USB_CDC_Class"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/Config" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/examples" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hal/include" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hal/utils/include" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/cmcc" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/core" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/dmac" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/gclk" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/mclk" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/osc32kctrl" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/oscctrl" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/pm" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/port" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/ramecc" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/sercom" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hpl/usb" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/hri" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb/class/cdc" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb/class/cdc/device" -I"../../../../Applications/LTC2983_Driver/LTC2983_Driver/usb/device" -I"../../../I2C/I2C_Asyn_Class/I2C_Asyn_Class" -I"../../../USB/USB_CDC_Class/USB_CDC_Class"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="AT24MAC_Class.map" -Wl,--start-group -lm  -Wl,--end-group -L"..\\Device_Startup"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "AT24MAC_Class.elf" "AT24MAC_Class.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "AT24MAC_Class.elf" "AT24MAC_Class.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "AT24MAC_Class.elf" "AT24MAC_Class.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "AT24MAC_Class.elf" > "AT24MAC_Class.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "AT24MAC_Class.elf" "AT24MAC_Class.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "AT24MAC_Class.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "AT24MAC_Class.elf" "AT24MAC_Class.a" "AT24MAC_Class.hex" "AT24MAC_Class.bin" "AT24MAC_Class.lss" "AT24MAC_Class.eep" "AT24MAC_Class.map" "AT24MAC_Class.srec"
	