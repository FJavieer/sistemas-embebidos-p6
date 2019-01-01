/******************************************************************************
 *	Copyright (C) 2018	Colomar Andrés, Alejandro		      *
 *	Copyright (C) 2018	García Pedroche, Francisco Javier	      *
 *	Copyright (C) 2018	Junquera Carrero, Santiago		      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/

/**
 *	@file		main.c
 *	@author		Colomar Andrés, Alejandro
 *	@author		García Pedroche, Francisco Javier
 *	@author		Junquera Carrero, Santiago
 *	@copyright	GPL-2.0-only
 *	@date		2018/dec/15
 *	@brief		Main
 */


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
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
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

#if 1
	if (test()) {
		stuck_forever(true);
	}
#else
	(void)&test;

 #if 0
	if (proc_actuators_init()) {
		stuck_forever(true);
	}
 #elif 1
	if (proc_ctrl_init()) {
		stuck_forever(true);
	}
 #endif

 #if 0
	if (proc_actuators_2()) {
		stuck_forever(true);
	}
 #elif 1
	if (proc_ctrl_2()) {
		stuck_forever(true);
	}
 #endif

#endif

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
#if 1
	if (tim_test()) {
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
