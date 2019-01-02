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
 ******* enums ****************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* structs **************************************************************
 ******************************************************************************/


/******************************************************************************
 ******* variables ************************************************************
 ******************************************************************************/
/* Volatile ------------------------------------------------------------------*/
/* Global --------------------------------------------------------------------*/
/* Static --------------------------------------------------------------------*/
static	bool	init_pending	= true;
static	float	pitch;
static	float	roll;
static	float	yaw;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	modules_init		(void);
static	int	modules_deinit		(void);
static	int	proc_init		(void);
static	void	proc_deinit		(void);
static	int	proc_ref_read		(void *data);
static	int	proc_actuators_set	(void *data);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize actuators process
	 * @return	Error
	 * @note	Sets global variable 'prj_error'
	 */
int	proc_actuators_init	(void)
{

	if (init_pending) {
		init_pending	= false;
	} else {
		return	ERROR_OK;
	}

	if (modules_init()) {
		goto err_mod;
	}
	if (proc_init()) {
		goto err_proc;
	}

	return	ERROR_OK;


err_proc:
	modules_deinit();
err_mod:

	return	ERROR_NOK;
}

	/**
	 * @brief	Deinitialize actuators process
	 * @return	Error
	 * @note	Sets global variable 'prj_error'
	 */
int	proc_actuators_deinit	(void)
{
	int	status;

	status	= ERROR_OK;

	if (!init_pending) {
		init_pending	= true;
	} else {
		return	status;
	}

	proc_deinit();
	if (modules_deinit()) {
		status	= ERROR_NOK;
	}

	return	status;
}

	/**
	 * @brief	Run actuators process
	 * @return	Error
	 * @note	Sets global variable 'prj_error'
	 */
int	proc_actuators	(void)
{

	if (init_pending) {
		if (proc_actuators_init()) {
			return	ERROR_NOK;
		}
	}

#if ACT_TASK_MODE_IT
	while (true) {
		__WFI();
		if (tim_tim3_interrupt) {
			if (tim_callback_exe()) {
				return	ERROR_NOK;
			}
			tim_tim3_interrupt	= false;
		}
	}
#else
	while (true) {
		delay_us(ACT_REFRESH_PERIOD_US);

		if (!proc_ref_read(NULL)) {
			if (proc_actuators_set(NULL)) {
				return	ERROR_NOK;
			}
		}
	}
#endif
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	modules_init		(void)
{

	led_init();
	if (delay_us_init()) {
		goto err_delay;
	}
#if ACT_TASK_MODE_IT
	if (tim_tim3_init(ACT_REFRESH_PERIOD_US)) {
		goto err_tim;
	}
#endif
	if (can_init()) {
		goto err_can;
	}
	if (servo_init()) {
		goto err_servo;
	}

	return	ERROR_OK;


err_servo:
	can_deinit();
err_can:
	delay_us_deinit();
err_delay:
	led_deinit();
	tim_tim3_deinit();
err_tim:

	return	ERROR_NOK;
}

static	int	modules_deinit		(void)
{
	int	status;

	status	= ERROR_OK;

	if (servo_deinit()) {
		status	= ERROR_NOK;
	}
	if (can_deinit()) {
		status	= ERROR_NOK;
	}
	if (tim_tim3_deinit()) {
		status	= ERROR_NOK;
	}
	if (delay_us_deinit()) {
		status	= ERROR_NOK;
	}
	led_deinit();

	return	status;
}

static	int	proc_init		(void)
{

#if ACT_TASK_MODE_IT
	if (tim_callback_push(&proc_ref_read, NULL)) {
		goto err_push;
	}
	if (tim_callback_push(&proc_actuators_set, NULL)) {
		goto err_push;
	}

	return	ERROR_OK;


err_push:
	proc_deinit();

	return	ERROR_NOK;
#else
	return	ERROR_OK;
#endif
}

static	void	proc_deinit		(void)
{

#if ACT_TASK_MODE_IT
	while (tim_callback_pop()) {
	}
#endif
}

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
