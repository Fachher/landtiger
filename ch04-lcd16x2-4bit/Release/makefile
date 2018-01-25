################################################################################
# Makefile to generate "elf" compiled binary for embedded targets.
# Author: Asif Iqbal (a.iqbal@ieee.org)
# Date: 7th October, 2010
################################################################################

CPU := cortex-m3
INSTRUCTION_MODE := thumb
TARGET := blinky_lpc1768
TARGET_EXT := elf
LD_SCRIPT := ../lpc1768.ld

LIBS := 

INCLUDE += -I ../src
INCLUDE += -I "/home/fachher/Programme/arm-2011.03/lib/gcc/arm-none-eabi/4.5.2/include"

OBJ_FOLDER := objs

COMPILER_OPTIONS = -O0 -Os -Wall -fno-strict-aliasing \
	-fmessage-length=0 -fno-builtin  -m$(INSTRUCTION_MODE) \
	-mcpu=$(CPU) -MMD -MP -ffunction-sections -fdata-sections
	
DEPEND_OPTS = -MF $(OBJ_FOLDER)/$(patsubst %.o,%.d,$(notdir $@)) \
	-MT $(OBJ_FOLDER)/$(patsubst %.o,%.d,$(notdir $@))

CC = arm-none-eabi-gcc
CFLAGS = $(COMPILER_OPTIONS) $(INCLUDE) $(DEPEND_OPTS) -c

CXX = arm-none-eabi-g++
CXXFLAGS = $(COMPILER_OPTIONS) $(INCLUDE) $(DEPEND_OPTS) -c
AS = arm-none-eabi-gcc
ASFLAGS = $(COMPILER_OPTIONS) $(INCLUDE) $(DEPEND_OPTS)  -c

LD = arm-none-eabi-gcc
LD_OPTIONS = -Wl,--gc-sections,-Map=$(TARGET).map $(COMPILER_OPTIONS)  -L ../ -T $(LD_SCRIPT) $(INCLUDE)

OBJCP = arm-none-eabi-objcopy
OBJCPFLAGS = -O ihex

AR = arm-none-eabi-ar
ARFLAGS = cr

RM := rm -rf

USER_OBJS :=
C_SRCS := 
S_SRCS := 
C_OBJS :=
S_OBJS :=

# Every subdirectory with source files must be described here
SUBDIRS := 	../src 

C_SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))
C_OBJS := $(patsubst %.c,$(OBJ_FOLDER)/%.o,$(notdir $(C_SRCS)))
S_SRCS := $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.s))
S_OBJS := $(patsubst %.c,$(OBJ_FOLDER)/%.o,$(notdir $(S_SRCS)))

VPATH := $(SUBDIRS)

$(OBJ_FOLDER)/%.o : %.c
	@echo 'Building file: $(@F)'
	@echo 'Invoking: MCU C Compiler'
	$(CC) $(CFLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '
	
$(OBJ_FOLDER)/%.o : %.s
	@echo 'Building file: $(@F)'
	@echo 'Invoking: MCU Assembler'
	$(AS) $(ASFLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '

# All Target
all: $(TARGET).$(TARGET_EXT)

# Tool invocations
$(TARGET).$(TARGET_EXT): $(C_OBJS) $(S_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	$(LD) $(LD_OPTIONS) $(C_OBJS) $(S_OBJS) $(USER_OBJS) $(LIBS) -o$(TARGET).$(TARGET_EXT)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(TARGET).$(TARGET_EXT) $(TARGET).bin $(TARGET).map $(OBJ_FOLDER)/*.*
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	arm-none-eabi-size --totals $(TARGET).$(TARGET_EXT); arm-none-eabi-objcopy -O binary $(TARGET).$(TARGET_EXT) $(TARGET).bin ;
	-@echo ' '

flash:
	JLinkExe -CommanderScript flashlpc1768.jlink

.PHONY: all clean dependents flash
.SECONDARY: post-build

