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
../MonoDisplay_Class.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
MonoDisplay_Class.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_same54.o \
Device_Startup/system_same54.o \
main.o \
MonoDisplay_Class.o

C_DEPS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
MonoDisplay_Class.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_same54.d \
Device_Startup/system_same54.d \
main.d \
MonoDisplay_Class.d

OUTPUT_FILE_PATH +=MonoDisplayClass.elf

OUTPUT_FILE_PATH_AS_ARGS +=MonoDisplayClass.elf

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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/Config" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/examples" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\include" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\utils\include" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/cmcc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/core" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/dmac" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\evsys" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/gclk" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/mclk" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\osc32kctrl" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\oscctrl" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/pm" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/port" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\ramecc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/rtc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\sercom" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\systick" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/usb" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hri" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/usb" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc\device" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\device" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/gfx_mono" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display\ug2832hsweg04" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller\ssd1306" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\fonts" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I".." -I"../../GFX_Text_Example/Config" -I"../../GFX_Text_Example" -I"../../GFX_Text_Example/examples" -I"../../GFX_Text_Example/hal/include" -I"../../GFX_Text_Example/hal/utils/include" -I"../../GFX_Text_Example/hpl/cmcc" -I"../../GFX_Text_Example/hpl/core" -I"../../GFX_Text_Example/hpl/dmac" -I"../../GFX_Text_Example/hpl/gclk" -I"../../GFX_Text_Example/hpl/mclk" -I"../../GFX_Text_Example/hpl/osc32kctrl" -I"../../GFX_Text_Example/hpl/oscctrl" -I"../../GFX_Text_Example/hpl/pm" -I"../../GFX_Text_Example/hpl/port" -I"../../GFX_Text_Example/hpl/ramecc" -I"../../GFX_Text_Example/hpl/sercom" -I"../../GFX_Text_Example/hri" -I"../../GFX_Text_Example/gfx_mono" -I"../../GFX_Text_Example/gfx_mono/display" -I"../../GFX_Text_Example/gfx_mono/display/ug2832hsweg04" -I"../../GFX_Text_Example/gfx_mono/display_controller" -I"../../GFX_Text_Example/gfx_mono/display_controller/ssd1306" -I"../../GFX_Text_Example/gfx_mono/fonts"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./MonoDisplay_Class.o: .././MonoDisplay_Class.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAME54P20A__ -DDEBUG  -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/Config" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/examples" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\include" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hal\utils\include" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/cmcc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/core" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/dmac" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\evsys" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/gclk" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/mclk" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\osc32kctrl" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\oscctrl" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/pm" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/port" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\ramecc" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/rtc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\sercom" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\hpl\systick" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hpl/usb" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/hri" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/usb" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\class\cdc\device" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\usb\device" -I"../../../../../Applications/ARINCTransceiver/ARINC_Interface_Driver/ARINC_Interface_Driver/gfx_mono" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display\ug2832hsweg04" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\display_controller\ssd1306" -I"..\..\..\..\..\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\gfx_mono\fonts" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\ARM\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\SAME54_DFP\1.1.134\include" -I".." -I"../../GFX_Text_Example/Config" -I"../../GFX_Text_Example" -I"../../GFX_Text_Example/examples" -I"../../GFX_Text_Example/hal/include" -I"../../GFX_Text_Example/hal/utils/include" -I"../../GFX_Text_Example/hpl/cmcc" -I"../../GFX_Text_Example/hpl/core" -I"../../GFX_Text_Example/hpl/dmac" -I"../../GFX_Text_Example/hpl/gclk" -I"../../GFX_Text_Example/hpl/mclk" -I"../../GFX_Text_Example/hpl/osc32kctrl" -I"../../GFX_Text_Example/hpl/oscctrl" -I"../../GFX_Text_Example/hpl/pm" -I"../../GFX_Text_Example/hpl/port" -I"../../GFX_Text_Example/hpl/ramecc" -I"../../GFX_Text_Example/hpl/sercom" -I"../../GFX_Text_Example/hri" -I"../../GFX_Text_Example/gfx_mono" -I"../../GFX_Text_Example/gfx_mono/display" -I"../../GFX_Text_Example/gfx_mono/display/ug2832hsweg04" -I"../../GFX_Text_Example/gfx_mono/display_controller" -I"../../GFX_Text_Example/gfx_mono/display_controller/ssd1306" -I"../../GFX_Text_Example/gfx_mono/fonts"  -O1 -ffunction-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m4 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="MonoDisplayClass.map" -Wl,--start-group -lm -lARINC_Interface_Driver  -Wl,--end-group -L"..\\Device_Startup" -L"S:\AtmelStudio\Applications\ARINCTransceiver\ARINC_Interface_Driver\ARINC_Interface_Driver\Debug"  -Wl,--gc-sections -mcpu=cortex-m4 -Tsame54p20a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "MonoDisplayClass.elf" "MonoDisplayClass.bin"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "MonoDisplayClass.elf" "MonoDisplayClass.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "MonoDisplayClass.elf" "MonoDisplayClass.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "MonoDisplayClass.elf" > "MonoDisplayClass.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "MonoDisplayClass.elf" "MonoDisplayClass.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "MonoDisplayClass.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "MonoDisplayClass.elf" "MonoDisplayClass.a" "MonoDisplayClass.hex" "MonoDisplayClass.bin" "MonoDisplayClass.lss" "MonoDisplayClass.eep" "MonoDisplayClass.map" "MonoDisplayClass.srec"
	