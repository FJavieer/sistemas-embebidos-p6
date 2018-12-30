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
# Colomar Andrés, Alejandro  <1903716@gmail.com>
# García Pedroche, Francisco Javier  <@.>
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
# Do not print "Entering directory ...",
# but we want to display it when entering to the output directory
# so that IDEs/editors are able to understand relative filenames.
MAKEFLAGS += --no-print-directory

################################################################################
PROGRAMVERSION	= $(VERSION)$(if $(PATCHLEVEL),$(PATCHLEVEL)$(if $(SUBLEVEL),$(SUBLEVEL)))$(EXTRAVERSION)
export	PROGRAMVERSION

################################################################################




#  *****  Basic settings of project  *****
# ==============================================================================

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJECT_NAME = main

export	PROJECT_NAME

# ==============================================================================

################################################################################
# directories

MAIN_DIR	= $(CURDIR)
LIBALX_DIR	= $(MAIN_DIR)/libalx/
DRIVERS_DIR	= $(MAIN_DIR)/stm32l4-drivers/
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
AS	= arm-none-eabi-as
AR	= arm-none-eabi-ar
GDB	= arm-none-eabi-gdb
OBJCOPY	= arm-none-eabi-objcopy
OBJDUMP	= arm-none-eabi-objdump
SIZE	= arm-none-eabi-size

export	CC
export	AS
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
#CFLAGS_W       += -Wno-format-truncation
#CFLAGS_W       += -Wno-format-zero-length

CFLAGS_F	= -ffreestanding
CFLAGS_F       += -flto
CFLAGS_F       += -fsingle-precision-constant
CFLAGS_F       += -ffunction-sections
CFLAGS_F       += -fdata-sections

# defining used MCU (instead of in file stm32f00x.h): -DSTM32F051x8//
CFLAGS_D	= -D USE_HAL_DRIVER	# to include file stm32l4xx_hal.h

CFLAGS		= $(CFLAGS_STD)
CFLAGS	       += $(CFLAGS_OPT)
CFLAGS	       += $(CFLAGS_MCU)
CFLAGS	       += $(CFLAGS_W)
CFLAGS	       += $(CFLAGS_F)
CFLAGS	       += $(CFLAGS_D)
CFLAGS	       += $(C_INCLUDES)

export	CFLAGS


################################################################################
# LDFLAGS
# Settings of linker
LDFLAGS		=  -mcpu=cortex-m4 -mthumb
LDFLAGS        += -Wl,--gc-sections -Wl,-Map=$(PROJECT_NAME).map,--cref,--no-warn-mismatch

export	LDFLAGS


################################################################################


.PHONY: all stm32l4-drivers libalx stm32l4-modules objects target flash erase reset clean mrproper

all: stm32l4-drivers libalx stm32l4-modules objects target

stm32l4-drivers:
	@echo	'	MAKE	drivers'
	$(Q)$(MAKE) -C $(DRIVERS_DIR)

libalx:
	@echo	'	MAKE	libalx'
	$(Q)$(MAKE) base	-C $(LIBALX_DIR)

stm32l4-modules:
	@echo	'	MAKE	modules'
	$(Q)$(MAKE) -C $(MODULES_DIR)

objects:
	@echo	"	MAKE	objects"
	$(Q)$(MAKE) -C $(TMP_DIR)

target:
	@echo	"	MAKE	target"
	$(Q)$(MAKE) -C $(BIN_DIR)


flash:
	openocd -f board/stm32l4nucleo.cfg -c "program $(BIN_DIR)/$(PROJECT_NAME).hex verify reset exit"
#second approach:	openocd -f board/stm32f0discovery.cfg -c "init" -c "reset halt" -c "flash write_image erase $(BINDIR)/$(PROJECT_NAME).hex" -c "verify_image $(BINDIR)/$(PROJECT_NAME).hex" -c "reset run" -c "exit"
#third approach:	openocd -f interface/stlink-v2.cfg -f target/stm32f0x_stlink.cfg -c "init" -c "reset halt" -c "flash write_image erase $(BINDIR)/$(PROJECT_NAME).hex" -c "verify_image $(BINDIR)/$(PROJECT_NAME).hex" -c "reset run" -c "exit"

erase:
	openocd -f board/stm32l4nucleo.cfg -c "init" -c "reset halt" -c "stm32l4x mass_erase 0" -c "reset" -c "shutdown"

reset:
	openocd -f board/stm32l4nucleo.cfg -c "init" -c "reset" -c "shutdown"

clean:
	@echo	'	CLEAN	bin'
	$(Q)$(MAKE) clean -C $(BIN_DIR)
	@echo	'	CLEAN	tmp'
	$(Q)$(MAKE) clean -C $(TMP_DIR)

mrproper: clean
	@echo	'	CLEAN	modules'
	$(Q)$(MAKE) clean -C $(MODULES_DIR)
	@echo	'	CLEAN	libalx'
	$(Q)$(MAKE) clean -C $(LIBALX_DIR)
	@echo	'	CLEAN	drivers'
	$(Q)$(MAKE) clean -C $(DRIVERS_DIR)




################################################################################
######## End of file ###########################################################
################################################################################
