/******************************************************************************
 *	Copyright (C) 2018	Colomar Andrés, Alejandro		      *
 *	Copyright (C) 2018	García Pedroche, Francisco Javier	      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/

/**
 *	@file		actuators.c
 *	@author		Colomar Andrés, Alejandro
 *	@author		García Pedroche, Francisco Javier
 *	@copyright	GPL-2.0-only
 *	@date		2018/dec/26
 *	@brief		Actuators
 *		Set the actuators of the plane to the position received by CAN
 */


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
	#include "alx_math.h"

/* STM32L4 modules -----------------------------------------------------------*/
	#include "can.h"
	#include "delay.h"
	#include "errors.h"
	#include "led.h"
	#include "servo.h"
	#include "tim.h"

/* project -------------------------------------------------------------------*/
	#include "actuators.h"


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
static	float	pitch;
static	float	roll;
static	float	yaw;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	modules_init		(void);
#if 0
static	int	proc_init		(void);
#endif
static	int	proc_ref_read		(void *data);
static	int	proc_actuators_set	(void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	proc_actuators_init	(void)
{
	if (modules_init()) {
		return	ERROR_NOK;
	}

#if 0
	if (proc_init()) {
		return	ERROR_NOK;
	}
#endif


	servo_position_set(SERVO_S1, 0);
	servo_position_set(SERVO_S2, -45);
	servo_position_set(SERVO_S3, 90);
	servo_position_set(SERVO_S4, -90);
	delay_us(1000000u);

	return	ERROR_OK;
}

int	proc_actuators_1	(void)
{
	while (true) {
		__WFE();
		if (tim_tim3_interrupt) {
			if (tim_callback_exe()) {
				prj_error_handle();
				return	ERROR_NOK;
			}
			tim_tim3_interrupt	= false;
		}
	}

	return	ERROR_OK;
}

int	proc_actuators_2	(void)
{
	delay_us(200000u);

	while (true) {
		delay_us(100000u);

		if (!proc_ref_read(NULL)) {
			if (proc_actuators_set(NULL)) {
				return	ERROR_NOK;
			}
		}
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	modules_init		(void)
{
#if 0
	if (tim_tim3_init(REFRESH_FREQ)) {
		return	ERROR_NOK;
	}
#endif
	if (delay_us_init()) {
		return	ERROR_NOK;
	}
	if (can_init()) {
		return	ERROR_NOK;
	}
	if (servo_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

#if 0
static	int	proc_init		(void)
{
	if (tim_callback_push(&proc_ref_read, NULL)) {
		return	ERROR_NOK;
	}
	if (tim_callback_push(&proc_actuators_set, NULL)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}
#endif

static	int	proc_ref_read		(void *data)
{
	int8_t	plane_pos [CAN_DATA_LEN];

	(void)data;

	if (can_msg_read((uint8_t *)plane_pos)) {
		return	ERROR_NOK;
	}

	pitch	= plane_pos[0];
	roll	= plane_pos[1];
	yaw	= plane_pos[2];

	return	ERROR_OK;
}

static	int	proc_actuators_set	(void *data)
{
	float	tmp;

	(void)data;

	tmp	= alx_scale_linear_f(pitch, -40, 40, -90, 90);
	if (servo_position_set(SERVO_S1, tmp)) {
		return	ERROR_NOK;
	}

	tmp	= alx_scale_linear_f(roll, -35, 35, -90, -20);
	if (servo_position_set(SERVO_S2, tmp)) {
		return	ERROR_NOK;
	}

	if (yaw > 0) {
		if (servo_position_set(SERVO_S3, 90)) {
			return	ERROR_NOK;
		}

		tmp	= alx_scale_linear_f(yaw, 0, 10, -90, 90);
		if (servo_position_set(SERVO_S4, tmp)) {
			return	ERROR_NOK;
		}
	} else {
		tmp	= alx_scale_linear_f(yaw, -10, 0, -90, 90);
		if (servo_position_set(SERVO_S3, tmp)) {
			return	ERROR_NOK;
		}

		if (servo_position_set(SERVO_S4, -90)) {
			return	ERROR_NOK;
		}
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
