#!/usr/bin/make -f

VERSION		= 0
PATCHLEVEL	= .5
SUBLEVEL	= 
EXTRAVERSION	=

export	VERSION
export	PATCHLEVEL
export	SUBLEVEL

################################################################################
# Project template for development board STM32F0Discovery
# Uses libraries HAL, which are placed in dedicated
# directory (where is also Makefile for them)
# Andrej Bendzo  <andrej.sl@azet.sk>
# 2016.08.11
#
# Modified for development board STM32L476xx
# Alejandro Colomar  <1903716@gmail.com>
# Javi
# 2018
################################################################################

# This Makefile contains:
# rule "flash", which is intended for writing of program into MCU,
# rule "erase" for erasing Flash memory of MCU
# rule "reset" for system resetting of MCU.

################################################################################
# Beautify output
# ---------------------------------------------------------------------------
# Prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)some command here
#
# If BUILD_VERBOSE equals 0 then the above command will be hidden.
# If BUILD_VERBOSE equals 1 then the above command is displayed.
#
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

ifeq ("$(origin V)","command line")
  BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 0
endif

ifeq ($(BUILD_VERBOSE), 1)
  Q =
else
  Q = @
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
  Q = @
endif

export	Q
export	BUILD_VERBOSE

################################################################################
PROGRAMVERSION	= $(VERSION)$(if $(PATCHLEVEL),$(PATCHLEVEL)$(if $(SUBLEVEL),$(SUBLEVEL)))$(EXTRAVERSION)
export	PROGRAMVERSION

################################################################################




#  *****  Basic settings of project  *****
# ==============================================================================

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJECT_NAME = l4template
TARGET = $(PROJECT_NAME).elf

# list of source files
SRCS = main.c system_stm32l4xx.c

# ==============================================================================

################################################################################
# directories

MAIN_DIR	= $(CURDIR)
LIBALX_DIR	= $(MAIN_DIR)/libalx/
DRIVERS_DIR	= $(MAIN_DIR)/stm32-drivers/
MODULES_DIR	= $(MAIN_DIR)/stm32l4-modules/
SRC_DIR		= $(MAIN_DIR)/src/
INC_DIR		= $(MAIN_DIR)/inc/
DEP_DIR		= $(MAIN_DIR)/dep/
TMP_DIR		= $(MAIN_DIR)/tmp/
BIN_DIR		= $(MAIN_DIR)/bin/

export	MAIN_DIR
export	LIBALX_DIR
export	DRIVERS_DIR
export	MODULES_DIR


################################################################################
# Make variables (CC, etc...)
CC	= arm-none-eabi-gcc
AR	= arm-none-eabi-ar
GDB	= arm-none-eabi-gdb
OBJCOPY	= arm-none-eabi-objcopy
OBJDUMP	= arm-none-eabi-objdump
SIZE	= arm-none-eabi-size

export	CC
export	AR
export	GDB
export	OBJCOPY
export	OBJDUMP
export	SIZE


################################################################################
# CFLAGS
CFLAGS_STD	= -std=c11

CFLAGS_OPT	= -Os

CFLAGS_MCU	= -mlittle-endian
CFLAGS_MCU     += -mcpu=cortex-m4
CFLAGS_MCU     += -mthumb
CFLAGS_MCU     += -mfpu=fpv4-sp-d16
CFLAGS_MCU     += -mfloat-abi=hard

CFLAGS_W	= -Wall
CFLAGS_W       += -Wextra
CFLAGS_W       += -Wstrict-prototypes
CFLAGS_W       += -Werror
CFLAGS_W       += -Wno-format-truncation
CFLAGS_W       += -Wno-format-zero-length

CFLAGS_F	= -fsingle-precision-constant
CFLAGS_F       += -ffunction-sections
CFLAGS_F       += -fdata-sections

CFLAGS_D	= -D USE_HAL_DRIVER	# to include file stm32l4xx_hal.h

C_INCLUDES	= -I $(DRIVERS_DIR)/
C_INCLUDES     += -I $(DRIVERS_DIR)/CMSIS/Include/
C_INCLUDES     += -I $(DRIVERS_DIR)/CMSIS/ST/STM32L4xx/Include/
C_INCLUDES     += -I $(DRIVERS_DIR)/STM32L4xx_HAL_Driver/Inc/
C_INCLUDES     += -I $(DRIVERS_DIR)/STM32L4xx_HAL_Driver/Inc/Legacy/
C_INCLUDES     += -I $(LIBALX_DIR)/inc/
C_INCLUDES     += -I $(MODULES_DIR)/pwm/inc/
C_INCLUDES     += -I $(MODULES_DIR)/servo/inc/
C_INCLUDES     += -I $(INC_DIR)/

CFLAGS		= $(CFLAGS_STD)
CFLAGS	       += $(CFLAGS_OPT)
CFLAGS	       += $(CFLAGS_MCU)
CFLAGS	       += $(CFLAGS_W)
CFLAGS	       += $(CFLAGS_F)
CFLAGS	       += $(CFLAGS_D)
CFLAGS	       += $(C_INCLUDES)

# defining used MCU (instead of in file stm32f00x.h): -DSTM32F051x8//
CFLAGS += -DUSE_HAL_DRIVER # to include file stm32l4xx_hal.h


################################################################################
# LDFLAGS
# Settings of linker
LDFLAGS =  -mcpu=cortex-m4 -mthumb
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(BIN_DIR)/$(PROJECT_NAME).map,--cref,--no-warn-mismatch


################################################################################
vpath %.a $(DRIVERS_DIR)


# startup file for MCU
STARTUP = $(SRC_DIR)/startup_stm32l476xx.s

# generating of object files and dependencies
OBJS = $(addprefix $(TMP_DIR)/,$(SRCS:.c=.o))
DEPS = $(addprefix $(DEP_DIR)/,$(SRCS:.c=.d))


.PHONY: all stm32-drivers flash erase reset clean entireclean display

all: dirs stm32-drivers $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(PROJECT_NAME).hex $(BIN_DIR)/$(PROJECT_NAME).lst size

stm32-drivers:
	make -C $(DRIVERS_DIR)

dirs:
	mkdir -p $(DEP_DIR) $(TMP_DIR) $(BIN_DIR)

display:
	@echo 'SRCS = $(SRCS)'
	@echo 'OBJS = $(OBJS)'


## Compile:
# independent rule for every source file
$(TMP_DIR)/main.o : $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -MMD -MF $(DEP_DIR)/$(*F).d -c $(SRC_DIR)/main.c -o $@

$(TMP_DIR)/system_stm32l4xx.o : $(SRC_DIR)/system_stm32l4xx.c
	$(CC) $(CFLAGS) -MMD -MF $(DEP_DIR)/$(*F).d -c $(SRC_DIR)/system_stm32l4xx.c -o $@


## Link:
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(STARTUP) -L$(DRIVERS_DIR) -lstm32l4 -TSTM32L476RG.ld

## Post-build steps:
$(BIN_DIR)/$(PROJECT_NAME).hex: $(BIN_DIR)/$(TARGET)
	$(OBJCOPY) -O ihex $(BIN_DIR)/$(TARGET) $@

$(BIN_DIR)/$(PROJECT_NAME).bin: $(BINDIR)/$(TARGET)
	$(OBJCOPY) -O binary $(BINDIR)/$(TARGET) $@

$(BIN_DIR)/$(PROJECT_NAME).lst: $(BIN_DIR)/$(TARGET)
	$(OBJDUMP) -St $(BIN_DIR)/$(TARGET) > $@

size: $(BIN_DIR)/$(TARGET)
	@echo 'size $<'
	@$(SIZE) -B $(BIN_DIR)/$(TARGET)
	@echo


flash:
	openocd -f board/stm32f0discovery.cfg -c "program $(BIN_DIR)/$(PROJECT_NAME).hex verify reset exit"
#second approach:	openocd -f board/stm32f0discovery.cfg -c "init" -c "reset halt" -c "flash write_image erase $(BINDIR)/$(PROJECT_NAME).hex" -c "verify_image $(BINDIR)/$(PROJECT_NAME).hex" -c "reset run" -c "exit"
#third approach:	openocd -f interface/stlink-v2.cfg -f target/stm32f0x_stlink.cfg -c "init" -c "reset halt" -c "flash write_image erase $(BINDIR)/$(PROJECT_NAME).hex" -c "verify_image $(BINDIR)/$(PROJECT_NAME).hex" -c "reset run" -c "exit"

erase:
	openocd -f board/stm32f0discovery.cfg -c "init" -c "reset halt" -c "stm32l4x mass_erase 0" -c "reset" -c "shutdown"

reset:
	openocd -f board/stm32f0discovery.cfg -c "init" -c "reset" -c "shutdown"

clean:
	rm -f $(TMP_DIR)/*.o $(DEP_DIR)/*.d
	rm -f $(BIN_DIR)/$(PROJECT_NAME).elf $(BIN_DIR)/$(PROJECT_NAME).hex $(BIN_DIR)/$(PROJECT_NAME).bin $(BIN_DIR)/$(PROJECT_NAME).map $(BIN_DIR)/$(PROJECT_NAME).lst
	make clean -C $(DRIVERS_DIR)

mrproper: clean
	make -C $(DRIVERS_DIR) clean




################################################################################
######## End of file ###########################################################
################################################################################
