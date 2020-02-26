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
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
FlashMemoryClass.o \
main.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
FlashMemoryClass.o \
main.o

C_DEPS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
FlashMemoryClass.d \
main.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
FlashMemoryClass.d \
main.d

OUTPUT_FILE_PATH +=UpdateParameters.elf

OUTPUT_FILE_PATH_AS_ARGS +=UpdateParameters.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/Bitbucket/ars_2400024_all/scr/Debug/libDoorActuator1.a \
S:/AtmelStudio/SPI_Drivers/SPI_Asyn_Driver/Debug/libSPI_Asyn_Driver.a

LINKER_SCRIPT_DEP+=  \
../Device_Startup/samd21j18a_flash.ld \
../Device_Startup/samd21j18a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_samd21.o: ../Device_Startup/startup_samd21.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../../Bitbucket/ars_2400024_all/scr/Application" -I"../../../../Bitbucket/ars_2400024_all/scr/eCore" -I"../../../SPI0Class/SPI0Class"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_samd21.o: ../Device_Startup/system_samd21.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../../Bitbucket/ars_2400024_all/scr/Application" -I"../../../../Bitbucket/ars_2400024_all/scr/eCore" -I"../../../SPI0Class/SPI0Class"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./FlashMemoryClass.o: .././FlashMemoryClass.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../../Bitbucket/ars_2400024_all/scr/Application" -I"../../../../Bitbucket/ars_2400024_all/scr/eCore" -I"../../SPI_Drivers/SPI_Asyn_Driver/Config" -I"../../SPI_Drivers/SPI_Asyn_Driver" -I"../../SPI_Drivers/SPI_Asyn_Driver/examples" -I"../../SPI_Drivers/SPI_Asyn_Driver/hal/include" -I"../../SPI_Drivers/SPI_Asyn_Driver/hal/utils/include" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/core" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/dmac" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/gclk" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/pm" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/port" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/sercom" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/sysctrl" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/systick" -I"../../SPI_Drivers/SPI_Asyn_Driver/hri" -I"../../../SPI_Asyn_Class"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../../Bitbucket/ars_2400024_all/scr/Application" -I"../../../../Bitbucket/ars_2400024_all/scr/eCore" -I"../../SPI_Drivers/SPI_Asyn_Driver/Config" -I"../../SPI_Drivers/SPI_Asyn_Driver" -I"../../SPI_Drivers/SPI_Asyn_Driver/examples" -I"../../SPI_Drivers/SPI_Asyn_Driver/hal/include" -I"../../SPI_Drivers/SPI_Asyn_Driver/hal/utils/include" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/core" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/dmac" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/gclk" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/pm" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/port" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/sercom" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/sysctrl" -I"../../SPI_Drivers/SPI_Asyn_Driver/hpl/systick" -I"../../SPI_Drivers/SPI_Asyn_Driver/hri" -I"../../../SPI_Asyn_Class"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="UpdateParameters.map" -Wl,--start-group -lm -lDoorActuator1 -l:SPI_Asyn_Class.elf -lSPI_Asyn_Driver  -Wl,--end-group -L"..\\Device_Startup" -L"S:\Bitbucket\ars_2400024_all\scr\Debug" -L"S:\AtmelStudio\SPI_Asyn_Class\Debug" -L"S:\AtmelStudio\SPI_Drivers\SPI_Asyn_Driver\Debug"  -Wl,--gc-sections -mcpu=cortex-m0plus -Tsamd21j18a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "UpdateParameters.elf" "UpdateParameters.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "UpdateParameters.elf" "UpdateParameters.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "UpdateParameters.elf" "UpdateParameters.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "UpdateParameters.elf" > "UpdateParameters.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "UpdateParameters.elf" "UpdateParameters.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "UpdateParameters.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "UpdateParameters.elf" "UpdateParameters.a" "UpdateParameters.hex" "UpdateParameters.bin" "UpdateParameters.lss" "UpdateParameters.eep" "UpdateParameters.map" "UpdateParameters.srec"
	