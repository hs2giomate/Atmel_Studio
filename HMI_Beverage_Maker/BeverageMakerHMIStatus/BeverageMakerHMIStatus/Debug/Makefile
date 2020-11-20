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
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../LedStateMachine.cpp \
../main.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
LedStateMachine.o \
main.o

OBJS_AS_ARGS +=  \
LedStateMachine.o \
main.o

C_DEPS +=  \
LedStateMachine.d \
main.d

C_DEPS_AS_ARGS +=  \
LedStateMachine.d \
main.d

OUTPUT_FILE_PATH +=libBeverageMakerHMIStatus.a

OUTPUT_FILE_PATH_AS_ARGS +=libBeverageMakerHMIStatus.a

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
S:/AtmelStudio/HMI_Beverage_Maker/LightBar(19May)/Debug/libBevMakerHmi.a

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./LedStateMachine.o: .././LedStateMachine.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../../LightBar(19May)/src" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAD_DFP\1.1.63\include"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega384d3 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAD_DFP\1.1.63\gcc\dev\atxmega384d3" -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../../LightBar(19May)/src" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAD_DFP\1.1.63\include"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega384d3 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\XMEGAD_DFP\1.1.63\gcc\dev\atxmega384d3" -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	@echo Invoking: AVR8/GNU Archiver : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-ar.exe$(QUOTE) -r  -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) 
	@echo Finished building target: $@
	
	




# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "libBeverageMakerHMIStatus.elf" "libBeverageMakerHMIStatus.a" "libBeverageMakerHMIStatus.hex" "libBeverageMakerHMIStatus.lss" "libBeverageMakerHMIStatus.eep" "libBeverageMakerHMIStatus.map" "libBeverageMakerHMIStatus.srec" "libBeverageMakerHMIStatus.usersignatures"
	