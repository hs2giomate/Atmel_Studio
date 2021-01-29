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
../HEATERS_HANDLER.cpp \
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
HEATERS_HANDLER.o \
main.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
HEATERS_HANDLER.o \
main.o

C_DEPS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
HEATERS_HANDLER.d \
main.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
HEATERS_HANDLER.d \
main.d

OUTPUT_FILE_PATH +=Dual_Heater_P0.elf

OUTPUT_FILE_PATH_AS_ARGS +=Dual_Heater_P0.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/Applications/ECS_Box/ALU_Driver/ALU_Driver/test_exe/libALU_Driver.a \
S:/AtmelStudio/Applications/ECS_Box/ECS_Box_Classes/ECS_Box_Classes/test_exe/libECS_Box_Objects.a \
S:/AtmelStudio/Applications/ECS_Box/HeaterP0Board/HeaterP0Board/Debug/libHeaterP0Board.a

LINKER_SCRIPT_DEP+=  \
../Device_Startup/same54p20a_flash.ld \
../Device_Startup/same54p20a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_same54.o: ../Device_Startup/startup_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_same54.o: ../Device_Startup/system_same54.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include"  -O1 -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./HEATERS_HANDLER.o: .././HEATERS_HANDLER.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include" -I"../../../ALU_Driver/ALU_Driver/Config" -I"../../../ALU_Driver/ALU_Driver" -I"../../../ALU_Driver/ALU_Driver/examples" -I"../../../ALU_Driver/ALU_Driver/hal/include" -I"../../../ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../ALU_Driver/ALU_Driver/hpl/can" -I"../../../ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../ALU_Driver/ALU_Driver/hpl/core" -I"../../../ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../ALU_Driver/ALU_Driver/hpl/eic" -I"../../../ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../ALU_Driver/ALU_Driver/hpl/pac" -I"../../../ALU_Driver/ALU_Driver/hpl/pm" -I"../../../ALU_Driver/ALU_Driver/hpl/port" -I"../../../ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../ALU_Driver/ALU_Driver/hpl/tc" -I"../../../ALU_Driver/ALU_Driver/hpl/tcc" -I"../../../ALU_Driver/ALU_Driver/hpl/usb" -I"../../../ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../ALU_Driver/ALU_Driver/hri" -I"../../../ALU_Driver/ALU_Driver/usb" -I"../../../ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../ALU_Driver/ALU_Driver/usb/device" -I"../../../ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"../../../ECS_Box_Classes/ECS_Box_Classes/FlapperValve" -I"../../../ECS_Box_Classes/ECS_Box_Classes/SPI" -I"../../../ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils" -I"../../../ECS_Box_Classes/ECS_Box_Classes/Timers" -I"../../../ECS_Box_Classes/ECS_Box_Classes/USBSerial" -I"../../../ECS_Box_Classes/ECS_Box_Classes/Storage" -I"../../../HeaterP0Board/HeaterP0Board"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAME54_DFP\1.1.134\include" -I"../../../ALU_Driver/ALU_Driver/Config" -I"../../../ALU_Driver/ALU_Driver" -I"../../../ALU_Driver/ALU_Driver/examples" -I"../../../ALU_Driver/ALU_Driver/hal/include" -I"../../../ALU_Driver/ALU_Driver/hal/utils/include" -I"../../../ALU_Driver/ALU_Driver/hpl/can" -I"../../../ALU_Driver/ALU_Driver/hpl/cmcc" -I"../../../ALU_Driver/ALU_Driver/hpl/core" -I"../../../ALU_Driver/ALU_Driver/hpl/dmac" -I"../../../ALU_Driver/ALU_Driver/hpl/dsu" -I"../../../ALU_Driver/ALU_Driver/hpl/eic" -I"../../../ALU_Driver/ALU_Driver/hpl/evsys" -I"../../../ALU_Driver/ALU_Driver/hpl/gclk" -I"../../../ALU_Driver/ALU_Driver/hpl/mclk" -I"../../../ALU_Driver/ALU_Driver/hpl/osc32kctrl" -I"../../../ALU_Driver/ALU_Driver/hpl/oscctrl" -I"../../../ALU_Driver/ALU_Driver/hpl/pac" -I"../../../ALU_Driver/ALU_Driver/hpl/pm" -I"../../../ALU_Driver/ALU_Driver/hpl/port" -I"../../../ALU_Driver/ALU_Driver/hpl/qspi" -I"../../../ALU_Driver/ALU_Driver/hpl/ramecc" -I"../../../ALU_Driver/ALU_Driver/hpl/rtc" -I"../../../ALU_Driver/ALU_Driver/hpl/sercom" -I"../../../ALU_Driver/ALU_Driver/hpl/tc" -I"../../../ALU_Driver/ALU_Driver/hpl/tcc" -I"../../../ALU_Driver/ALU_Driver/hpl/usb" -I"../../../ALU_Driver/ALU_Driver/hpl/wdt" -I"../../../ALU_Driver/ALU_Driver/hri" -I"../../../ALU_Driver/ALU_Driver/usb" -I"../../../ALU_Driver/ALU_Driver/usb/class/cdc" -I"../../../ALU_Driver/ALU_Driver/usb/class/cdc/device" -I"../../../ALU_Driver/ALU_Driver/usb/device" -I"../../../ALU_Driver/ALU_Driver/spi_nor_flash" -I"../../../ALU_Driver/ALU_Driver/spi_nor_flash/n25q256a" -I"../../../ECS_Box_Classes/ECS_Box_Classes/FlapperValve" -I"../../../ECS_Box_Classes/ECS_Box_Classes/SPI" -I"../../../ECS_Box_Classes/ECS_Box_Classes/I2C" -I"../../../ECS_Box_Classes/ECS_Box_Classes/EIDE_Utils" -I"../../../ECS_Box_Classes/ECS_Box_Classes/Timers" -I"../../../ECS_Box_Classes/ECS_Box_Classes/USBSerial" -I"../../../ECS_Box_Classes/ECS_Box_Classes/Storage" -I"../../../HeaterP0Board/HeaterP0Board"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="Dual_Heater_P0.map" -Wl,--start-group -lm -lALU_Driver -lECS_Box_Objects -lHeaterP0Board  -Wl,--end-group -L"..\\Device_Startup" -L"S:\AtmelStudio\Applications\ECS_Box\ALU_Driver\ALU_Driver\test_exe" -L"S:\AtmelStudio\Applications\ECS_Box\ECS_Box_Classes\ECS_Box_Classes\test_exe" -L"S:\AtmelStudio\Applications\ECS_Box\HeaterP0Board\HeaterP0Board\Debug"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "Dual_Heater_P0.elf" "Dual_Heater_P0.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "Dual_Heater_P0.elf" "Dual_Heater_P0.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "Dual_Heater_P0.elf" "Dual_Heater_P0.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "Dual_Heater_P0.elf" > "Dual_Heater_P0.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "Dual_Heater_P0.elf" "Dual_Heater_P0.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "Dual_Heater_P0.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Dual_Heater_P0.elf" "Dual_Heater_P0.a" "Dual_Heater_P0.hex" "Dual_Heater_P0.bin" "Dual_Heater_P0.lss" "Dual_Heater_P0.eep" "Dual_Heater_P0.map" "Dual_Heater_P0.srec"
	