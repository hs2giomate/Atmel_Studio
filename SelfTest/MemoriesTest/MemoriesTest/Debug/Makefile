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
../main.cpp \
../Parameters.cpp \
../SPI_Syn_Class.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
FlashMemoryClass.o \
main.o \
Parameters.o \
SPI_Syn_Class.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
FlashMemoryClass.o \
main.o \
Parameters.o \
SPI_Syn_Class.o

C_DEPS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
FlashMemoryClass.d \
main.d \
Parameters.d \
SPI_Syn_Class.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
FlashMemoryClass.d \
main.d \
Parameters.d \
SPI_Syn_Class.d

OUTPUT_FILE_PATH +=libMemoriesTest.a

OUTPUT_FILE_PATH_AS_ARGS +=libMemoriesTest.a

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/SelfTest/GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/Debug/libGPIO_SAMD21_TEST.a

LINKER_SCRIPT_DEP+=  \
../Device_Startup/samd21j18a_flash.ld \
../Device_Startup/samd21j18a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_samd21.o: ../Device_Startup/startup_samd21.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_samd21.o: ../Device_Startup/system_samd21.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./FlashMemoryClass.o: .././FlashMemoryClass.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/Config" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/examples" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/utils/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/core" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/dmac" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/gclk" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/pm" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/port" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/rtc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sercom" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sysctrl" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tcc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hri"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/Config" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/examples" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/utils/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/core" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/dmac" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/gclk" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/pm" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/port" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/rtc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sercom" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sysctrl" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tcc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hri"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./Parameters.o: .././Parameters.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/Config" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/examples" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/utils/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/core" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/dmac" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/gclk" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/pm" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/port" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/rtc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sercom" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sysctrl" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tcc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hri"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./SPI_Syn_Class.o: .././SPI_Syn_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/Config" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/examples" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hal/utils/include" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/core" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/dmac" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/gclk" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/pm" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/port" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/rtc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sercom" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/sysctrl" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hpl/tcc" -I"../../../GPIO_SAMD21_TEST/GPIO_SAMD21_TEST/hri"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	rm -rf "libMemoriesTest.elf" "libMemoriesTest.a" "libMemoriesTest.hex" "libMemoriesTest.bin" "libMemoriesTest.lss" "libMemoriesTest.eep" "libMemoriesTest.map" "libMemoriesTest.srec"
	