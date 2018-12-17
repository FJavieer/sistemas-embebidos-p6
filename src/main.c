/******************************************************************************
 *	main.c								      *
 *	2018/dec/15							      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
//	#include <stdbool.h>
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
	#include "servo.h"

	#include "led_test.h"
	#include "display_test.h"
	#include "servo_test.h"


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
static	int	main_init	(void);
static	int	test_units	(void);
//	void	fputc_SetXY		(uint16_t x, uint16_t y);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	main	(void)
{
	/* init */
	if (main_init()) {
		return;
	}

	if (test_units()) {
		return;
	}

#if 0
	/* Infinite loop */
	while (true) {
	}
#endif

	return;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	main_init	(void)
{
	HAL_Init();

	sysclk_config();

	led_init();
	if (delay_us_init()) {
		return	ERROR_NOK;
	}
	if (display_init()) {
		return	ERROR_NOK;
	}
	servo_s1_init();
	servo_s2_init();
	servo_s3_init();
	servo_s4_init();

	return	ERROR_OK;
}

static	int	test_units	(void)
{
	if (led_test()) {
		return	ERROR_NOK;
	}
	if (display_test()) {
		return	ERROR_NOK;
	}
	if (servo_test()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
