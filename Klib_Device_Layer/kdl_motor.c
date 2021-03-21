/**
  ******************************************************************************
  * @date    Mar 6, 2021
  * @file    kdl_motor.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   General motor control source file
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
#include "kdl_motor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Motor_Init(KDL_Motor_t *dmtr, TIM_HandleTypeDef *port1, uint32_t channel1, TIM_HandleTypeDef *port2, uint32_t channel2)
{
	/* Check valid */
	if(dmtr == NULL)
		return KDL_ERROR;

	/* Pass index */
	dmtr->Port1 = port1;
	dmtr->Channel1 = channel1;
	dmtr->Port2 = port2;
	dmtr->Channel2 = channel2;

	dmtr->SpeedBound = 1;

	KDL_Motor_SetSpeed(dmtr, 0);

	HAL_TIM_PWM_Start(dmtr->Port1, dmtr->Channel1);
	HAL_TIM_PWM_Start(dmtr->Port2, dmtr->Channel2);

	return KDL_OK;
}
/* Configuration functions ****************************************************/
KDL_State_t KDL_Motor_SetSpeed(KDL_Motor_t *dmtr, float speed)
{
	if(speed > dmtr->SpeedBound)
		speed = dmtr->SpeedBound;
	if(-speed > dmtr->SpeedBound)
		speed = -dmtr->SpeedBound;

	dmtr->Speed = speed;

	return KDL_OK;
}

KDL_State_t KDL_Motor_IncSpeed(KDL_Motor_t *dmtr, float speed)
{
	speed += dmtr->Speed;

	return KDL_Motor_SetSpeed(dmtr, speed);
}

KDL_State_t KDL_Motor_SetSpeedBound(KDL_Motor_t *dmtr, float bound)
{
	if(bound<0)
		bound = -bound;
	if(bound>1)
		bound = 1;
	dmtr->SpeedBound = bound;

	return KDL_OK;
}

KDL_State_t KDL_Motor_Refresh(KDL_Motor_t *dmtr)
{
	float ftmp;
	uint16_t dtmp;
	TIM_HandleTypeDef *port;
	uint32_t channel;

	ftmp = dmtr->Speed;

	if(dmtr->Speed > 0)
	{
		__HAL_TIM_SET_COMPARE(dmtr->Port1, dmtr->Channel1, 0);

		port = dmtr->Port2;
		channel = dmtr->Channel2;

		dtmp = port->Instance->ARR;
	}
	else
	{
		__HAL_TIM_SET_COMPARE(dmtr->Port2, dmtr->Channel2, 0);

		port = dmtr->Port1;
		channel = dmtr->Channel1;

		dtmp = port->Instance->ARR;
	}

	if(ftmp<0)
		ftmp = -ftmp;

	ftmp *= dtmp;
	ftmp += 0.5;
	dtmp = (uint16_t)ftmp;

	__HAL_TIM_SET_COMPARE(port, channel, dtmp);

	return KDL_OK;
}
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
