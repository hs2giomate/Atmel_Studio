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
../SPI_Syn_Class.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
SPI_Syn_Class.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
SPI_Syn_Class.o

C_DEPS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
SPI_Syn_Class.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
SPI_Syn_Class.d

OUTPUT_FILE_PATH +=libSPI_Syn_Class.a

OUTPUT_FILE_PATH_AS_ARGS +=libSPI_Syn_Class.a

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/Debug/libARINC_Interface_Driver.a

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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/Config" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/examples" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\include" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\utils\include" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/cmcc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/core" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/dmac" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\evsys" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/gclk" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/mclk" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\osc32kctrl" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\oscctrl" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/pm" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/port" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\ramecc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/rtc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\sercom" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\systick" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/usb" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hri" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/usb" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc\device" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\device" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/Config" -I"../../SPI_Syn_Driver/SPI_Syn_Driver" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/examples" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hal/include" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hal/utils/include" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/cmcc" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/core" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/dmac" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/gclk" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/mclk" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/osc32kctrl" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/oscctrl" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/pm" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/port" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/ramecc" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/sercom" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/systick" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hri" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/Config" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/examples" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/include" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/utils/include" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/adc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/cmcc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/core" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/dmac" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/evsys" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/gclk" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/mclk" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/osc32kctrl" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/oscctrl" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/pm" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/port" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/ramecc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/rtc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/sercom" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/systick" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/usb" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hri" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc/device" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/device" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/tc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/gfx_mono" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display\ug2832hsweg04" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller\ssd1306" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\fonts"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./SPI_Syn_Class.o: .././SPI_Syn_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/Config" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/examples" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\include" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\utils\include" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/cmcc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/core" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/dmac" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\evsys" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/gclk" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/mclk" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\osc32kctrl" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\oscctrl" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/pm" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/port" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\ramecc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/rtc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\sercom" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\systick" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/usb" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hri" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/usb" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc\device" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\device" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/Config" -I"../../SPI_Syn_Driver/SPI_Syn_Driver" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/examples" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hal/include" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hal/utils/include" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/cmcc" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/core" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/dmac" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/gclk" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/mclk" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/osc32kctrl" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/oscctrl" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/pm" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/port" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/ramecc" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/sercom" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hpl/systick" -I"../../SPI_Syn_Driver/SPI_Syn_Driver/hri" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/Config" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/examples" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/include" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hal/utils/include" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/adc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/cmcc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/core" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/dmac" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/evsys" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/gclk" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/mclk" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/osc32kctrl" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/oscctrl" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/pm" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/port" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/ramecc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/rtc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/sercom" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/systick" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/usb" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hri" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/class/cdc/device" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/usb/device" -I"../../../../../Applications/ARINCTransceiver/MainDriver/MainDriver/hpl/tc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/gfx_mono" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display\ug2832hsweg04" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller\ssd1306" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\fonts"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	rm -rf "libSPI_Syn_Class.elf" "libSPI_Syn_Class.a" "libSPI_Syn_Class.hex" "libSPI_Syn_Class.bin" "libSPI_Syn_Class.lss" "libSPI_Syn_Class.eep" "libSPI_Syn_Class.map" "libSPI_Syn_Class.srec"
	