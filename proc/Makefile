#!/usr/bin/make -f

################################################################################
# directories

PROC_DIR		= $(CURDIR)
PARENT_DIR		= $(PROC_DIR)/../

STM32_DRIVERS_DIR	= $(PARENT_DIR)/stm32l4-drivers/
STM32_CMSIS_DIR		= $(STM32_DRIVERS_DIR)/CMSIS/
STM32L4_CMSIS_DIR	= $(STM32_CMSIS_DIR)/ST/STM32L4xx/
STM32L4_HAL_DIR		= $(STM32_DRIVERS_DIR)/STM32L4xx_HAL_Driver/

LIBALX_DIR		= $(PARENT_DIR)/libalx/

STM32L4_MODULES_DIR	= $(PARENT_DIR)/stm32l4-modules/

PROC_SENSORS_DIR	= $(PROC_DIR)/proc_sensors/
PROC_ACTUATORS_DIR	= $(PROC_DIR)/proc_actuators/

LIB_DIR			= $(PROC_DIR)/lib/

export	PROC_DIR

export	STM32_DRIVERS_DIR
export	STM32_CMSIS_DIR
export	STM32L4_CMSIS_DIR
export	STM32L4_HAL_DIR

export	LIBALX_DIR

export	PROC_SENSORS_DIR
export	PROC_ACTUATORS_DIR

################################################################################
# target: dependencies
#	action

all:
	@echo	'	MAKE	modules:	sensors'
	$(Q)make -C $(PROC_SENSORS_DIR)
	@echo	'	MAKE	modules:	actuators'
	$(Q)make -C $(PROC_ACTUATORS_DIR)
	@echo	'	MAKE	modules:	lib'
	$(Q)make -C $(LIB_DIR)


clean:
	@echo  "	RM	*.o *.s *.a"
	$(Q)find . -type f -name '*.o' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.s' -exec rm '{}' '+'
	$(Q)find . -type f -name '*.a' -exec rm '{}' '+'

################################################################################
######## End of file ###########################################################
################################################################################
