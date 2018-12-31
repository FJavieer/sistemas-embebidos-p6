/******************************************************************************
 *	main.c								      *
 *	2018/dec/15							      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <stdbool.h>
	#include <stdnoreturn.h>

/* Drivers -------------------------------------------------------------------*/
	#include "stm32l4xx_hal.h"

/* libalx --------------------------------------------------------------------*/
/* STM32L4 modules -----------------------------------------------------------*/
	#include "can.h"
	#include "clk.h"
	#include "delay.h"
	#include "display.h"
	#include "errors.h"
	#include "led.h"
	#include "nunchuk.h"
	#include "servo.h"
	#include "tim.h"

	#include "can_test.h"
	#include "display_test.h"
	#include "led_test.h"
	#include "nunchuk_test.h"
	#include "servo_test.h"
	#include "tim_test.h"

/* project -------------------------------------------------------------------*/
	#include "ctrl.h"
	#include "actuators.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int		test_init	(void);
static	int		test		(void);
static	noreturn void	stuck_forever	(bool led);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
noreturn int	main	(void)
{
	HAL_Init();
	sysclk_config();
	prj_error	= 0;

	(void)&test_init;
#if 0
	if (test_init()) {
#elif 1
	if (proc_actuators_init()) {
#elif 0
	if (proc_ctrl_init()) {
#endif
		stuck_forever(true);
	}

	(void)&test;
#if 0
	if (test()) {
#elif 1
	if (proc_actuators_2()) {
#elif 0
	if (proc_ctrl_2()) {
#endif
		stuck_forever(true);
	}

	stuck_forever(true);
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	noreturn void	stuck_forever	(bool led)
{
	if (led) {
		led_set();
	} else {
		led_reset();
	}

	while (true) {
		__WFI();
		__NOP();
	}
}

static	int		test_init	(void)
{
#if 0
	led_init();
#endif
#if 0
	if (delay_us_init()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (can_init()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (servo_init()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (display_init()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (nunchuk_init()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (tim_tim3_init(1)) {
		return	ERROR_NOK;
	}
#endif

	return	ERROR_OK;
}

static	int		test		(void)
{
#if 0
	if (led_test()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	can_r_test();
#endif
#if 0
	can_w_test();
#endif
#if 0
	if (servo_test_2()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (display_test()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (nunchuk_test_2()) {
		return	ERROR_NOK;
	}
#endif
#if 0
	if (tim_test(1)) {
		return	ERROR_NOK;
	}
#endif
#if 0
	prj_error_handle();
#endif

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
