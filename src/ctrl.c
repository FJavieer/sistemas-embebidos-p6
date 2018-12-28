/******************************************************************************
 *	ctrl.c								      *
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
	#include "can.h"
	#include "errors.h"
	#include "nunchuk.h"
	#include "tim.h"

/* project -------------------------------------------------------------------*/
	#include "ctrl.h"


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
static	int	proc_init		(void);
static	int	proc_ctrl_read		(void *data);
static	int	proc_ctrl_report	(void *data);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
int	proc_ctrl_init	(void)
{
	if (modules_init()) {
		return	ERROR_NOK;
	}

	if (proc_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

int	proc_ctrl	(void)
{
	while (true) {
		__WFE();
		if (tim_tim3_interrupt) {
			if (tim_callback_exe()) {
				error_handle();
				return	ERROR_NOK;
			}
			tim_tim3_interrupt	= false;
		}
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* static functions (definitions) ***************************************
 ******************************************************************************/
static	int	modules_init		(void)
{
	if (tim_tim3_init(REFRESH_FREQ)) {
		return	ERROR_NOK;
	}
	if (can_init()) {
		return	ERROR_NOK;
	}
	if (nunchuk_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	proc_init		(void)
{
	if (tim_callback_push(&proc_ctrl_read, NULL)) {
		return	ERROR_NOK;
	}
	if (tim_callback_push(&proc_ctrl_report, NULL)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

static	int	proc_ctrl_read		(void *data)
{
	Nunchuk_Data_s	nunchuk;

	(void)data;

	if (nunchuk_read(&nunchuk)) {
		return	ERROR_NOK;
	}

	/* FIXME */
	pitch	= *((int8_t *)&(nunchuk.acc.x8));
	roll	= *((int8_t *)&(nunchuk.acc.y8));
	yaw	= *((int8_t *)&(nunchuk.acc.z8));

	return	ERROR_OK;
}

static	int	proc_ctrl_report	(void *data)
{
	int8_t	plane_pos [CAN_DATA_LEN];
	int	i;

	(void)data;

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

	for (i = 3; i < CAN_DATA_LEN; i++) {
		plane_pos[i]	= 0;
	}

	if (can_msg_write((uint8_t *)plane_pos)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
