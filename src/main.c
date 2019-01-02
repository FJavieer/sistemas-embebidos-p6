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
	# define	TESTING_MODULES	(false)
	# define	PROJECT_CTRL	(false)
	# define	PROJECT_ACT	(!PROJECT_CTRL)


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
static	int		proc_init	(void);
static	int		proc		(void);
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

#if	TESTING_MODULES
	if (test()) {
		goto err;
	}
#else
	(void)&test;

	if (proc_init()) {
		goto err;
	}

	if (proc()) {
		goto err;
	}

#endif

err:
	stuck_forever(true);
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int		proc_init	(void)
{

#if	PROJECT_ACT
	if (proc_actuators_init()) {
		return	ERROR_NOK;
	}
#elif	PROJECT_CTRL
	if (proc_ctrl_init()) {
		return	ERROR_NOK;
	}
#endif

	return	ERROR_OK;
}

static	int		proc		(void)
{

#if	PROJECT_ACT
	if (proc_actuators()) {
		return	ERROR_NOK;
	}
#elif	PROJECT_CTRL
	if (proc_ctrl()) {
		return	ERROR_NOK;
	}
#endif

	return	ERROR_OK;
}

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
