/******************************************************************************
 *	main.c								      *
 *	2018/dec/15							      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <stdnoreturn.h>
	#include <stdbool.h>
//	#include <stdio.h>

/* Drivers -------------------------------------------------------------------*/
	#include "stm32l4xx_hal.h"

/* libalx --------------------------------------------------------------------*/
/* STM32L4 modules -----------------------------------------------------------*/
	#include "clk.h"
	#include "delay.h"
	#include "display.h"
	#include "errors.h"
	#include "led.h"
	#include "nunchuk.h"
	#include "servo.h"
	#include "tim.h"

	#include "display_test.h"
	#include "led_test.h"
	#include "nunchuk_test.h"
	#include "servo_test.h"
	#include "tim_test.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	REFRESH_FREQ	(50u)


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	main_init	(void);
static	int	test_units	(void);
static	int	proc_init	(void);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
noreturn int	main	(void)
{
	if (main_init()) {
		while (true) {
			__NOP();
		}
	}

	if (test_units()) {
		while (true) {
			__NOP();
		}
	}

	if (proc_init()) {
		while (true) {
			__NOP();
		}
	}

	while (true) {
		__WFE();
		if (tim_tim3_interrupt) {
			if (tim_callback_exe()) {
				while (true) {
					__NOP();
				}
			}
			tim_tim3_interrupt	= false;
		}
	}

	while (true) {
		__NOP();
	}
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	main_init	(void)
{
	HAL_Init();

	sysclk_config();

	led_init();
	if (tim_tim3_init(REFRESH_FREQ)) {
		return	ERROR_NOK;
	}
	if (delay_us_init()) {
		return	ERROR_NOK;
	}
	if (display_init()) {
		return	ERROR_NOK;
	}
	if (servo_init()) {
		return	ERROR_NOK;
	}
	if (nunchuk_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	test_units	(void)
{
	if (led_test()) {
		return	ERROR_NOK;
	}
	if (tim_test(REFRESH_FREQ)) {
		return	ERROR_NOK;
	}
	if (display_test()) {
		return	ERROR_NOK;
	}
	if (servo_test()) {
		return	ERROR_NOK;
	}
	if (nunchuk_test()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	proc_init	(void)
{
	if (led_test()) {
		return	ERROR_NOK;
	}
	if (tim_test(REFRESH_FREQ)) {
		return	ERROR_NOK;
	}
	if (display_test()) {
		return	ERROR_NOK;
	}
	if (servo_test()) {
		return	ERROR_NOK;
	}
	if (nunchuk_test()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
