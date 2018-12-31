#!/usr/bin/make -f

# directories

STM32_CMSIS_DIR		= $(DRIVERS_DIR)/CMSIS/
STM32L4_CMSIS_DIR	= $(STM32_CMSIS_DIR)/ST/STM32L4xx/
STM32L4_HAL_DIR		= $(DRIVERS_DIR)/STM32L4xx_HAL_Driver/

LIBALX_INC_DIR		= $(LIBALX_DIR)/inc/
STM32_CMSIS_INC_DIR	= $(STM32_CMSIS_DIR)/Include/
STM32L4_CMSIS_INC_DIR	= $(STM32L4_CMSIS_DIR)/Include/
STM32L4_HAL_INC_DIR	= $(STM32L4_HAL_DIR)/Inc/

MODULES_BASE_INC_DIR	= $(MODULES_DIR)/inc/
MODULES_DEV_INC_DIR	= $(MODULES_DIR)/inc/dev/
MODULES_TST_INC_DIR	= $(MODULES_DIR)/inc/test/

SRC_DIR = $(MAIN_DIR)/src/
INC_DIR = $(MAIN_DIR)/inc/
DEP_DIR = $(MAIN_DIR)/dep/

# dependencies

ALL		= \
		system_stm32l4xx.o stm32l4xx_it.o main.o \
		ctrl.o actuators.o

COMMON_INC_CMSIS	= core_cm4.h cmsis_gcc.h
COMMON_INC_CMSIS_L4	= stm32l4xx.h
COMMON_INC_HAL		= stm32l4xx_hal.h
COMMON_DEPS		= \
			$(patsubst %,$(STM32_CMSIS_INC_DIR)/%,$(COMMON_INC_CMSIS)) \
			$(patsubst %,$(STM32L4_CMSIS_INC_DIR)/%,$(COMMON_INC_CMSIS_L4)) \
			$(patsubst %,$(STM32L4_HAL_INC_DIR)/%,$(COMMON_INC_HAL))
COMMON_INC_DIRS		= \
			-I $(STM32_CMSIS_INC_DIR) \
			-I $(STM32L4_CMSIS_INC_DIR) \
			-I $(STM32L4_HAL_INC_DIR)


SYS_DEPS		= $(SRC_DIR)/system_stm32l4xx.c \
			$(COMMON_DEPS)
SYS_INC_DIRS		= \
			$(COMMON_INC_DIRS)


IT_DEPS			= $(SRC_DIR)/stm32l4xx_it.c \
			$(COMMON_DEPS)
IT_INC_DIRS		= \
			$(COMMON_INC_DIRS)


MAIN_INC_BASE		= can.h clk.h delay.h errors.h led.h tim.h
MAIN_INC_DEV		= display.h nunchuk.h servo.h
MAIN_INC_TST		= \
			can_test.h led_test.h tim_test.h \
			display_test.h nunchuk_test.h servo_test.h
MAIN_INC		= ctrl.h actuators.h
MAIN_DEPS		= $(SRC_DIR)/main.c \
			$(COMMON_DEPS) \
			$(patsubst %,$(MODULES_BASE_INC_DIR)/%,$(MAIN_INC_BASE)) \
			$(patsubst %,$(MODULES_DEV_INC_DIR)/%,$(MAIN_INC_DEV)) \
			$(patsubst %,$(MODULES_TST_INC_DIR)/%,$(MAIN_INC_TST)) \
			$(patsubst %,$(INC_DIR)/%,$(MAIN_INC))
MAIN_INC_DIRS		= -I $(INC_DIR) \
			-I $(MODULES_BASE_INC_DIR) \
			-I $(MODULES_DEV_INC_DIR) \
			-I $(MODULES_TST_INC_DIR) \
			$(COMMON_INC_DIRS)


CTRL_INC_LIBALX		= alx_math.h
CTRL_INC_BASE		= can.h delay.h errors.h led.h tim.h
CTRL_INC_DEV		= nunchuk.h
CTRL_INC		= ctrl.h
CTRL_DEPS		= $(SRC_DIR)/ctrl.c \
			$(COMMON_DEPS) \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(CTRL_INC_LIBALX)) \
			$(patsubst %,$(MODULES_BASE_INC_DIR)/%,$(CTRL_INC_BASE)) \
			$(patsubst %,$(MODULES_DEV_INC_DIR)/%,$(CTRL_INC_DEV)) \
			$(patsubst %,$(INC_DIR)/%,$(CTRL_INC))
CTRL_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(MODULES_BASE_INC_DIR) \
			-I $(MODULES_DEV_INC_DIR) \
			$(COMMON_INC_DIRS)


ACT_INC_LIBALX		= alx_math.h
ACT_INC_BASE		= can.h delay.h errors.h led.h tim.h
ACT_INC_DEV		= servo.h
ACT_INC			= actuators.h
ACT_DEPS		= $(SRC_DIR)/actuators.c \
			$(COMMON_DEPS) \
			$(patsubst %,$(LIBALX_INC_DIR)/%,$(ACT_INC_LIBALX)) \
			$(patsubst %,$(MODULES_BASE_INC_DIR)/%,$(ACT_INC_BASE)) \
			$(patsubst %,$(MODULES_DEV_INC_DIR)/%,$(ACT_INC_DEV)) \
			$(patsubst %,$(INC_DIR)/%,$(ACT_INC))
ACT_INC_DIRS		= -I $(INC_DIR) \
			-I $(LIBALX_INC_DIR) \
			-I $(MODULES_BASE_INC_DIR) \
			-I $(MODULES_DEV_INC_DIR) \
			$(COMMON_INC_DIRS)


# target: dependencies
#	action

all: $(ALL)


system_stm32l4xx.o: $(SYS_DEPS)
	@echo	"	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(SYS_INC_DIRS) -c $< -o $@

stm32l4xx_it.o: $(IT_DEPS)
	@echo	"	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(IT_INC_DIRS) -c $< -o $@

main.o: $(MAIN_DEPS)
	@echo	"	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(MAIN_INC_DIRS) -c $< -o $@

ctrl.o: $(CTRL_DEPS)
	@echo	"	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(CTRL_INC_DIRS) -c $< -o $@

actuators.o: $(ACT_DEPS)
	@echo	"	CC	$@"
	$(Q)$(CC) $(CFLAGS) $(ACT_INC_DIRS) -c $< -o $@


clean:
	$(Q)rm -f *.o

################################################################################
######## End of file ###########################################################
################################################################################
