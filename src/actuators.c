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
	#include "can.h"
	#include "errors.h"
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
static	int	proc_init		(void);
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

	if (proc_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

int	proc_actuators		(void)
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
	if (servo_init()) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}

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
	(void)data;

	if (servo_position_set(SERVO_S1, pitch)) {
		return	ERROR_NOK;
	}
	if (servo_position_set(SERVO_S2, roll)) {
		return	ERROR_NOK;
	}
	if (servo_position_set(SERVO_S3, yaw)) {
		return	ERROR_NOK;
	}

	return	ERROR_OK;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
