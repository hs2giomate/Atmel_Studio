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

OUTPUT_FILE_PATH +=CAN_Asyn_Class.elf

OUTPUT_FILE_PATH_AS_ARGS +=CAN_Asyn_Class.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/Debug/libARINC_Interface_Driver.a \
S:/AtmelStudio/Classes/USB/USB_CDC_Class/USB_CDC_Class/Debug/libUSB_CDC_Class.a

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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/Config" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/examples" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\include" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\utils\include" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/can" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/cmcc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/core" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/dmac" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/eic" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\evsys" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/gclk" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/mclk" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\osc32kctrl" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\oscctrl" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/pm" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/port" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\ramecc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/rtc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\sercom" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\systick" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/usb" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hri" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/usb" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc\device" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\device" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\stdio_redirect" -I"../../../../USB/USB_CDC_Class/USB_CDC_Class"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="CAN_Asyn_Class.map" -Wl,--start-group -lm -lARINC_Interface_Driver -lUSB_CDC_Class  -Wl,--end-group -L"..\\Device_Startup" -L"S:\AtmelStudio\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\Debug" -L"S:\AtmelStudio\Classes\USB\USB_CDC_Class\USB_CDC_Class\Debug"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "CAN_Asyn_Class.elf" "CAN_Asyn_Class.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "CAN_Asyn_Class.elf" "CAN_Asyn_Class.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "CAN_Asyn_Class.elf" "CAN_Asyn_Class.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "CAN_Asyn_Class.elf" > "CAN_Asyn_Class.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "CAN_Asyn_Class.elf" "CAN_Asyn_Class.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "CAN_Asyn_Class.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "CAN_Asyn_Class.elf" "CAN_Asyn_Class.a" "CAN_Asyn_Class.hex" "CAN_Asyn_Class.bin" "CAN_Asyn_Class.lss" "CAN_Asyn_Class.eep" "CAN_Asyn_Class.map" "CAN_Asyn_Class.srec"
	