/**
  ******************************************************************************
  * @date    Mar 24, 2021
  * @file    kdl_hcsr04.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   HC-SR04 ultrasonic distance module source file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================


  @endverbatim
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "kdl_hcsr04.h"
#include "kd_delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_HCSR04_Init(
		KDL_HCSR04_t* khcsr,
		GPIO_TypeDef* Tport,
		uint16_t Tpin,
		GPIO_TypeDef* Eport,
		uint16_t Epin
		)
{
	if(khcsr == NULL)
		return KDL_ERROR;

	KDL_Pin_Init(&khcsr->Trig, Tport, Tpin);
	KDL_Pin_Init(&khcsr->Echo, Eport, Epin);

	KDL_Pin_Reset(&khcsr->Trig);

	return KDL_OK;
}
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
uint32_t KDL_HCSR04_Measure(KDL_HCSR04_t* khcsr)
{
	uint32_t tick_start, tick_end, ms_start, ms_end, timeout;
	double dtmp;

	/* 10us of pulse */
	KDL_Pin_Set(&khcsr->Trig);
	KD_Delay(10);
	KDL_Pin_Reset(&khcsr->Trig);

	/* Wait for echo high */
	timeout = KDL_HCSR04_TimeOut;
	while(timeout-- && !KDL_Pin_Read(&khcsr->Echo))
		tick_start = SysTick->VAL;
	ms_start = HAL_GetTick();

	/* Wait for echo low */
	timeout = KDL_HCSR04_TimeOut;
	while(timeout-- && KDL_Pin_Read(&khcsr->Echo))
		tick_end = SysTick->VAL;
	ms_end = HAL_GetTick();

	dtmp = tick_end;
	dtmp -= tick_start;
	dtmp /= SysTick -> LOAD + 1;
	dtmp += ms_end - ms_start;
	dtmp *= 1000;

	khcsr ->Ticks = (uint32_t)dtmp;

	return khcsr->Ticks;
}
/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
