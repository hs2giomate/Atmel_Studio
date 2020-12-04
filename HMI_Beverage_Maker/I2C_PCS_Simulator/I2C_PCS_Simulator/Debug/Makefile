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
../I2C_Sync_Class.cpp \
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
I2C_Sync_Class.o \
main.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
I2C_Sync_Class.o \
main.o

C_DEPS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
I2C_Sync_Class.d \
main.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
I2C_Sync_Class.d \
main.d

OUTPUT_FILE_PATH +=I2C_PCS_Simulator.elf

OUTPUT_FILE_PATH_AS_ARGS +=I2C_PCS_Simulator.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/HMI_Beverage_Maker/I2C_Commands_Generator/I2C_Commands_Generator/Debug/libI2C_Commands_Generator.a

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
	

./I2C_Sync_Class.o: .././I2C_Sync_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/Config" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/examples" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hal/include" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hal/utils/include" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/core" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/dmac" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/gclk" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/pm" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/port" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/sercom" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/sysctrl" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hri" -I"../../../../Applications/ECS_Box/ECS_Box_Classes/ECS_Box_Classes/I2C"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/Config" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/examples" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hal/include" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hal/utils/include" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/core" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/dmac" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/gclk" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/pm" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/port" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/sercom" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hpl/sysctrl" -I"../../../I2C_Commands_Generator/I2C_Commands_Generator/hri" -I"../../../../Applications/ECS_Box/ECS_Box_Classes/ECS_Box_Classes/I2C"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="I2C_PCS_Simulator.map" -Wl,--start-group -lm -lI2C_Commands_Generator  -Wl,--end-group -L"..\\Device_Startup" -L"S:\AtmelStudio\HMI_Beverage_Maker\I2C_Commands_Generator\I2C_Commands_Generator\Debug"  -Wl,--gc-sections -mcpu=cortex-m0plus -Tsamd21j18a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "I2C_PCS_Simulator.elf" "I2C_PCS_Simulator.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "I2C_PCS_Simulator.elf" "I2C_PCS_Simulator.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "I2C_PCS_Simulator.elf" "I2C_PCS_Simulator.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "I2C_PCS_Simulator.elf" > "I2C_PCS_Simulator.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "I2C_PCS_Simulator.elf" "I2C_PCS_Simulator.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "I2C_PCS_Simulator.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "I2C_PCS_Simulator.elf" "I2C_PCS_Simulator.a" "I2C_PCS_Simulator.hex" "I2C_PCS_Simulator.bin" "I2C_PCS_Simulator.lss" "I2C_PCS_Simulator.eep" "I2C_PCS_Simulator.map" "I2C_PCS_Simulator.srec"
	