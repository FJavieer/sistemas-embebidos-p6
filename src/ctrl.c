/******************************************************************************
 *	Copyright (C) 2018	Colomar Andrés, Alejandro		      *
 *	Copyright (C) 2018	García Pedroche, Francisco Javier	      *
 *	SPDX-License-Identifier:	GPL-2.0-only			      *
 ******************************************************************************/

/**
 *	@file		ctrl.c
 *	@author		Colomar Andrés, Alejandro
 *	@author		García Pedroche, Francisco Javier
 *	@copyright	GPL-2.0-only
 *	@date		2018/dec/26
 *	@brief		Control
 *		Read values from the nunchuk, and set from them
 *			- pitch
 *			- roll
 *			- yaw
 *		Send those 3 values through CAN
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
	#include "nunchuk.h"
	#include "tim.h"

/* project -------------------------------------------------------------------*/
	#include "ctrl.h"


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
static	bool	level;


/******************************************************************************
 ******* static functions (prototypes) ****************************************
 ******************************************************************************/
static	int	modules_init		(void);
static	int	modules_deinit		(void);
static	int	proc_init		(void);
static	void	proc_deinit		(void);
static	int	proc_ctrl_read		(void *data);
static	int	proc_ctrl_report	(void *data);


/******************************************************************************
 ******* global functions *****************************************************
 ******************************************************************************/
	/**
	 * @brief	Initialize controller process
	 * @return	Error
	 * @note	Sets global variable 'prj_error'
	 */
int	proc_ctrl_init		(void)
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

	level	= true;

	return	ERROR_OK;


err_proc:
	modules_deinit();
err_mod:

	return	ERROR_NOK;
}

	/**
	 * @brief	Deinitialize controller process
	 * @return	Error
	 * @note	Sets global variable 'prj_error'
	 */
int	proc_ctrl_deinit	(void)
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
	 * @brief	Run controller process
	 * @return	Error
	 * @note	Sets global variable 'prj_error'
	 */
int	proc_ctrl		(void)
{

	if (init_pending) {
		if (proc_ctrl_init()) {
			return	ERROR_NOK;
		}
	}

#if CTRL_TASK_MODE_IT
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
		delay_us(CTRL_REFRESH_PERIOD_US);

		if (proc_ctrl_read(NULL)) {
			return	ERROR_NOK;
		}

		if (proc_ctrl_report(NULL)) {
			return	ERROR_NOK;
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
#if CTRL_TASK_MODE_IT
	if (tim_tim3_init(CTRL_REFRESH_PERIOD_US)) {
		goto err_tim;
	}
#endif
	if (can_init()) {
		goto err_can;
	}
	if (nunchuk_init()) {
		goto err_nunchuk;
	}

	return	ERROR_OK;


err_nunchuk:
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

	if (nunchuk_deinit()) {
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

#if CTRL_TASK_MODE_IT
	if (tim_callback_push(&proc_ctrl_read, NULL)) {
		goto err_push;
	}
	if (tim_callback_push(&proc_ctrl_report, NULL)) {
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

#if CTRL_TASK_MODE_IT
	while (tim_callback_pop()) {
	}
#endif
}

static	int	proc_ctrl_read		(void *data)
{
	Nunchuk_Data_s	nunchuk;
	float		tmp;

	(void)data;

	if (nunchuk_read(&nunchuk)) {
		return	ERROR_NOK;
	}

	tmp	= nunchuk.jst.y;
	pitch	= -alx_scale_linear_f(tmp, 0, UINT8_MAX, -40, 40);

	tmp	= nunchuk.jst.x;
	roll	= alx_scale_linear_f(tmp, 0, UINT8_MAX, -35, 35);

	tmp	= nunchuk.acc.x8;
	yaw	= alx_scale_linear_f(tmp, 0, UINT8_MAX, -10, 10);

	level	= nunchuk.btn_c;

	return	ERROR_OK;
}

static	int	proc_ctrl_report	(void *data)
{
	int8_t	plane_pos [CAN_DATA_LEN];
	int	i;

	(void)data;

	for (i = 3; i < CAN_DATA_LEN; i++) {
		plane_pos[i]	= 0;
	}

	if (level) {
		plane_pos[0]	= 0;
		plane_pos[1]	= 0;
		plane_pos[2]	= 0;
	} else {
		if (pitch > INT8_MAX) {
			plane_pos[0]	= INT8_MAX;
		} else if (pitch < INT8_MIN) {
			plane_pos[0]	= INT8_MIN;
		} else {
			plane_pos[0]	= (int8_t)pitch;
		}

		if (roll > INT8_MAX) {
			plane_pos[1]	= INT8_MAX;
		} else if (roll < INT8_MIN) {
			plane_pos[1]	= INT8_MIN;
		} else {
			plane_pos[1]	= (int8_t)roll;
		}

		if (yaw > INT8_MAX) {
			plane_pos[2]	= INT8_MAX;
		} else if (yaw < INT8_MIN) {
			plane_pos[2]	= INT8_MIN;
		} else {
			plane_pos[2]	= (int8_t)yaw;
		}
	}

	if (can_msg_write((uint8_t *)plane_pos)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
