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
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
main.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_samd21.o \
Device_Startup/system_samd21.o \
main.o

C_DEPS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
main.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_samd21.d \
Device_Startup/system_samd21.d \
main.d

OUTPUT_FILE_PATH +=ParametersSaver.elf

OUTPUT_FILE_PATH_AS_ARGS +=ParametersSaver.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/Bitbucket/ars_2400024_all/scr/Debug/libDoorActuator1.a \
S:/AtmelStudio/Classes/FRAM_Memory_Class/FRAM_Memory_Class/Debug/libFRAM_Memory_Class.a \
S:/AtmelStudio/Classes/SPI/SPI_Asyn_Class/Debug/libSPI_Asyn_Class.a \
S:/AtmelStudio/Classes/Usart/Usart_Asyn_Class/Usart_Asyn_Class/Debug/libUsart_Asyn_Class.a \
S:/AtmelStudio/Applications/ParametersSaver/Main_Driver/Main_Driver/Debug/libMain_Driver.a

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
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21J18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAMD21_DFP\1.3.395\samd21a\include" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Config" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\examples" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hal\include" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hal\utils\include" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\core" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\dmac" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\gclk" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\pm" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\port" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\sercom" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\sysctrl" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hpl\systick" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\hri" -I"S:\AtmelStudio\Classes\FlashMemoryClass\FlashMemoryClass" -I"S:\Bitbucket\ars_2400024_all\scr\Application" -I"S:\Bitbucket\ars_2400024_all\scr\eCore" -I"S:\AtmelStudio\Classes\SPI\SPI_Asyn_Class" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\usb" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\usb\class" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\usb\class\cdc" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\usb\class\cdc\device" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\usb\device" -I"S:\AtmelStudio\Classes\FlashMemoryClass" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\Config" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\examples" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hal\include" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hal\utils\include" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\core" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\dmac" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\gclk" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\pm" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\port" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\sercom" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\sysctrl" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hpl\systick" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\hri" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\usb" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\usb\class\cdc" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\usb\class\cdc\device" -I"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\usb\device" -I"../../../../Classes/FRAM_Memory_Class/FRAM_Memory_Class" -I"../../../../Classes/Usart/Usart_Asyn_Class/Usart_Asyn_Class"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="ParametersSaver.map" -Wl,--start-group -lm -lDoorActuator1 -lFRAM_Memory_Class -lSPI_Asyn_Class -lUsart_Asyn_Class -lMain_Driver  -Wl,--end-group -L"..\\Device_Startup" -L"S:\Bitbucket\ars_2400024_all\scr\Debug" -L"S:\AtmelStudio\Classes\FRAM_Memory_Class\FRAM_Memory_Class\Debug" -L"S:\AtmelStudio\Classes\SPI\SPI_Asyn_Class\Debug" -L"S:\AtmelStudio\Classes\Usart\Usart_Asyn_Class\Usart_Asyn_Class\Debug" -L"S:\AtmelStudio\Applications\ParametersSaver\Main_Driver\Main_Driver\Debug"  -Wl,--gc-sections -mcpu=cortex-m0plus -Tsamd21j18a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "ParametersSaver.elf" "ParametersSaver.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "ParametersSaver.elf" "ParametersSaver.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "ParametersSaver.elf" "ParametersSaver.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "ParametersSaver.elf" > "ParametersSaver.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "ParametersSaver.elf" "ParametersSaver.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "ParametersSaver.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "ParametersSaver.elf" "ParametersSaver.a" "ParametersSaver.hex" "ParametersSaver.bin" "ParametersSaver.lss" "ParametersSaver.eep" "ParametersSaver.map" "ParametersSaver.srec"
	