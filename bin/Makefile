#!/usr/bin/make -f


################################################################################
# directories

LIBALX_LIB_DIR	= $(LIBALX_DIR)/lib/
DRIVERS_LIB_DIR	= $(DRIVERS_DIR)/lib/
MODULES_LIB_DIR	= $(MODULES_DIR)/lib/

SRC_DIR		= $(MAIN_DIR)/src/
TMP_DIR		= $(MAIN_DIR)/tmp/

################################################################################
# dependencies
ALL	= $(PROJECT_NAME).elf $(PROJECT_NAME).hex $(PROJECT_NAME).lst size

MAIN_OBJ_MODULES	= \
			system_stm32l4xx.o stm32l4xx_it.o main.o \
			ctrl.o actuators.o test.o
MAIN_OBJS		= $(patsubst %,$(TMP_DIR)/%,$(MAIN_OBJ_MODULES))


MAIN_LIB_LIBALX		= libalx-base.a
MAIN_LIB_DRIVERS	= libstm32l4.a
MAIN_LIB_MODULES	= \
			libstm32l4-modules-test.a \
			libstm32l4-modules-dev.a \
			libstm32l4-modules-base.a
MAIN_LIBS		= \
			$(patsubst %,$(LIBALX_LIB_DIR)/%,$(MAIN_LIB_LIBALX)) \
			$(patsubst %,$(DRIVERS_LIB_DIR)/%,$(MAIN_LIB_DRIVERS)) \
			$(patsubst %,$(MODULES_LIB_DIR)/%,$(MAIN_LIB_MODULES))

# static libs
STATIC_LIB_MODULES	= -L $(MODULES_LIB_DIR) \
				-l stm32l4-modules-test \
				-l stm32l4-modules-dev \
				-l stm32l4-modules-base
STATIC_LIB_DRIVERS	= -L $(DRIVERS_LIB_DIR) -l stm32l4
STATIC_LIB_LIBALX	= -L $(LIBALX_LIB_DIR) -l alx-base

STATIC_LIBS	= $(STATIC_LIB_MODULES)
STATIC_LIBS    += $(STATIC_LIB_DRIVERS)
STATIC_LIBS    += $(STATIC_LIB_LIBALX)


# target: dependencies
#	action

all: $(ALL)


$(PROJECT_NAME).elf: $(MAIN_OBJS) $(MAIN_LIBS)
	@echo	"	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $(MAIN_OBJS) -o $@ \
					$(SRC_DIR)/startup_stm32l476xx.s\
					$(STATIC_LIBS) $(LIBS) -TSTM32L476RG.ld

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	@echo	"	OBJCOPY	$@"
	$(Q)$(OBJCOPY) -O ihex $(PROJECT_NAME).elf $@

$(PROJECT_NAME).bin: $(PROJECT_NAME).elf
	@echo	"	OBJCOPY	$@"
	$(Q)$(OBJCOPY) -O binary $(PROJECT_NAME).elf $@

$(PROJECT_NAME).lst: $(PROJECT_NAME).elf
	@echo	"	OBJDUMP	$@"
	$(Q)$(OBJDUMP) -St $(PROJECT_NAME).elf > $@

size: $(PROJECT_NAME).elf
	@echo	"	SIZE	$<"
	$(Q)$(SIZE) -B $(PROJECT_NAME).elf


clean:
	@echo	"	RM	$(ALL)"
	$(Q)rm -f $(ALL)

################################################################################
######## End of file ###########################################################
################################################################################
