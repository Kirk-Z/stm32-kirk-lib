/**
  ******************************************************************************
  * @date    Apr 6, 2021
  * @file    kdl_servo.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Servo driver source file
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
#include "kdl_servo.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Servo_Init(KDL_Servo_t* ksvo, TIM_HandleTypeDef *port, uint32_t channel)
{
	if(ksvo == NULL)
		return KDL_ERROR;
	if(port == NULL)
		return KDL_ERROR;

	ksvo->port = port;
	ksvo->channel = channel;

	KDL_Servo_Set(ksvo, 0.00);

	HAL_TIM_PWM_Start(port, channel);

	return KDL_OK;
}
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
KDL_State_t KDL_Servo_Set(KDL_Servo_t* ksvo, float position)
{
	float ftmp;
	uint32_t utmp;

	if(ksvo == NULL)
		return KDL_ERROR;
	if(ksvo->port == NULL)
		return KDL_ERROR;

	ksvo->position = position;

	utmp = ksvo->port->Instance->ARR;
	utmp ++;

	ftmp = position;
	ftmp += 1.5;

	ftmp *= utmp;
	ftmp /= 20.00;

	ftmp += 0.5;
	utmp = (uint32_t)ftmp;

	if(utmp > 65535)
		utmp = 65535;

	switch (ksvo->channel)
	{
	case TIM_CHANNEL_1:
	{
	  /* Check the parameters */
	  assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

	  /* Set the capture 1 value */
	  ksvo->port->Instance->CCR1 =  utmp;

	  break;
	}
	case TIM_CHANNEL_2:
	{
	  /* Set the parameters */
	  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

	  /* Return the capture 2 value */
	  ksvo->port->Instance->CCR2 =  utmp;

	  break;
	}

	case TIM_CHANNEL_3:
	{
	  /* Check the parameters */
	  assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

	  /* Set the capture 3 value */
	  ksvo->port->Instance->CCR3 =  utmp;

	  break;
	}

	case TIM_CHANNEL_4:
	{
	  /* Check the parameters */
	  assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

	  /* Set the capture 4 value */
	  ksvo->port->Instance->CCR4 =  utmp;

	  break;
	}

	default:
	  break;
	}

	return KDL_OK;
}
/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
