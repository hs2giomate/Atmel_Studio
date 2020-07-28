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
../main.cpp \
../MCP23017_Class.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
MCP23017_Class.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
MCP23017_Class.o

C_DEPS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
MCP23017_Class.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
MCP23017_Class.d

OUTPUT_FILE_PATH +=MPC23017_IO_Expander.elf

OUTPUT_FILE_PATH_AS_ARGS +=MPC23017_IO_Expander.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/Applications/ECS_Box/ALU_Driver/ALU_Driver/Debug/libALU_Driver.a \
S:/AtmelStudio/Applications/ECS_Box/ECS_Box_Classes/ECS_Box_Classes/Debug/libECS_Box_Objects.a

LINKER_SCRIPT_DEP+=  \
../Device_Startup/same54p20a_flash.ld \
../Device_Startup/same54p20a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_same54.o: ../Device_Startup/startup_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_same54.o: ../Device_Startup/system_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include"  -O0 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\Config" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/examples" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hal/include" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hal/utils/include" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/can" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/cmcc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/core" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/dmac" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/dsu" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/eic" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/evsys" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/gclk" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/mclk" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/osc32kctrl" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/oscctrl" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/pac" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/pm" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/port" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/qspi" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/ramecc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/rtc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/sercom" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/tc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/usb" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/wdt" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hri" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb/class/cdc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb/class/cdc/device" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb/device" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/spi_nor_flash" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\spi_nor_flash\n25q256a" -I"../../../ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./MCP23017_Class.o: .././MCP23017_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\Config" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/examples" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hal/include" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hal/utils/include" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/can" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/cmcc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/core" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/dmac" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/dsu" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/eic" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/evsys" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/gclk" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/mclk" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/osc32kctrl" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/oscctrl" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/pac" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/pm" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/port" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/qspi" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/ramecc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/rtc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/sercom" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/tc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/usb" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hpl/wdt" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/hri" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb/class/cdc" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb/class/cdc/device" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/usb/device" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver/spi_nor_flash" -I"\\pdm15\Software\SDrive\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\spi_nor_flash\n25q256a" -I"../../../ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils"  -O0 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="MPC23017_IO_Expander.map" -Wl,--start-group -lm -lALU_Driver -lECS_Box_Objects  -Wl,--end-group -L"..\\Device_Startup" -L"S:\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\Debug" -L"S:\AtmelStudio\Applications\ECS_Box\ECS_Box_Classes\ECS_Box_Classes\Debug"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "MPC23017_IO_Expander.elf" "MPC23017_IO_Expander.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "MPC23017_IO_Expander.elf" "MPC23017_IO_Expander.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "MPC23017_IO_Expander.elf" "MPC23017_IO_Expander.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "MPC23017_IO_Expander.elf" > "MPC23017_IO_Expander.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "MPC23017_IO_Expander.elf" "MPC23017_IO_Expander.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "MPC23017_IO_Expander.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "MPC23017_IO_Expander.elf" "MPC23017_IO_Expander.a" "MPC23017_IO_Expander.hex" "MPC23017_IO_Expander.bin" "MPC23017_IO_Expander.lss" "MPC23017_IO_Expander.eep" "MPC23017_IO_Expander.map" "MPC23017_IO_Expander.srec"
	