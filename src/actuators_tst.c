/******************************************************************************
 *	actuators.c							      *
 *	2018/dec/26							      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C ----------------------------------------------------------------*/
	#include <stdbool.h>
	#include <stddef.h>
	#include <stdint.h>

/* Drivers -------------------------------------------------------------------*/
	#include "stm32l4xx_hal.h"

/* libalx --------------------------------------------------------------------*/
/* STM32L4 modules -----------------------------------------------------------*/
	#include "led.h"
	#include "can.h"
	#include "delay.h"
	#include "errors.h"
	#include "servo.h"
	#include "tim.h"

/* project -------------------------------------------------------------------*/
	#include "actuators_tst.h"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	int8_t	plane_pos [CAN_DATA_LEN];


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	modules_init		(void);
static	int	proc_ref_read		(void *data);
static	int	proc_actuators_set	(int i, void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	proc_actuators_tst_init	(void)
{
	if (modules_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

int	proc_actuators_tst	(void)
{
	int	i;
	i	= 0;

	while (true) {
		i++;
		i	= i % 8;
		if (proc_ref_read(NULL)) {
			error_handle();
		}
		proc_actuators_set(i, NULL);
		delay_us(10000000u - (2*300000u*i));
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	modules_init		(void)
{
	if (delay_us_init()) {
		return	ERROR_NOK;
	}
	led_init();
	if (can_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	proc_ref_read		(void *data)
{
	(void)data;

	if (can_msg_read((uint8_t *)plane_pos)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	proc_actuators_set	(int i, void *data)
{
	int	j;

	(void)data;

	for (j = 0; j < plane_pos[i]; j++) {
		led_set();
		delay_us(300000u);
		led_reset();
		delay_us(300000u);
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
