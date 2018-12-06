/**
  ******************************************************************************
  * This is the template for Informatica Industrial II. Lunes tarde group.
  * Based on the STMicroelectronics HAL template
  *
  * See http://aperles.blogs.upv.es/arm-cortex-m-practico-1-introduccion-a-los-microcontroladores-stm32-de-st/
  *
  * Good look,
  * Angel Perles
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include "stm32l4xx_hal.h"
#include "delay.h"
#include "led.h"
#include "servo.h"
#include "servo_test.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
	static	void	SystemClock_Config	(void);
//	static	void	Error_Handler		(void);
//		void	fputc_SetXY		(uint16_t x, uint16_t y);


/* Private functions ---------------------------------------------------------*/

/** Main program **************************************************************/
int	main	(void)
{

	/* STM32F4xx HAL library initialization */
	HAL_Init();

	/* Configure the System clock to 180 MHz */
	SystemClock_Config();

//	servo_test();

	/* init */
	led_init();
//	delay_us_init();
/*	servo_s1_init();
	servo_s2_init();
	servo_s3_init();
	servo_s4_init();*/

	led_set();
//	delay_us(5000000u);
HAL_Delay(1000);
	led_reset();
HAL_Delay(1000);
	led_set();
HAL_Delay(1000);
	led_reset();
HAL_Delay(1000);
	led_set();
HAL_Delay(1000);
	led_reset();
HAL_Delay(1000);
	led_set();

	delay_us_init();

	/* Infinite loop */
	while (true) {
		led_reset();
/*		servo_sX_position_set(-850, SERVO_S1);
		servo_sX_position_set(-850, SERVO_S2);
		servo_sX_position_set(-850, SERVO_S3);
		servo_sX_position_set(-850, SERVO_S4);*/
		delay_us(500000u);

		led_set();
/*		servo_sX_position_set(850, SERVO_S1);
		servo_sX_position_set(650, SERVO_S2);
		servo_sX_position_set(450, SERVO_S3);
		servo_sX_position_set(250, SERVO_S4);*/
		delay_us(500000u);
	}
	return	0;
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static	void	SystemClock_Config	(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* MSI is enabled after System reset, activate PLL with MSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLP = 7;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		/* Initialization Error */
		while(true) {
			;
		}
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1
	 * and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		/* Initialization Error */
		while(true) {
			;
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/