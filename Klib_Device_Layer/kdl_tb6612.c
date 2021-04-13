/**
  ******************************************************************************
  * @date    Apr 7, 2021
  * @file    kdl_tb6612.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   TB6612 motor driver source file
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
#include "kdl_tb6612.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_TB6612_Init(KDL_TB6612_t* ktb)
{
	if(ktb == NULL)
		return KDL_ERROR;

	ktb->Port = NULL;
	ktb->Pin1.port = NULL;
	ktb->Pin2.port = NULL;
	ktb->Stat = 0;
	ktb->Speed = 0;
	ktb->SpeedBound = 1;

	return KDL_OK;
}
/* Configuration functions ****************************************************/
KDL_State_t KDL_TB6612_SetPort(KDL_TB6612_t* ktb, TIM_HandleTypeDef* port, uint32_t channel)
{
	if(ktb == NULL)
		return KDL_ERROR;

	ktb->Port = port;
	ktb->Channel = channel;

	HAL_TIM_PWM_Start(port, channel);

	return KDL_OK;
}

KDL_State_t KDL_TB6612_SetPin1(KDL_TB6612_t* ktb, GPIO_TypeDef* port, uint16_t pin)
{
	if(ktb == NULL)
		return KDL_ERROR;

	KDL_Pin_Init(&ktb->Pin1, port, pin);
	KDL_Pin_Reset(&ktb->Pin1);

	return KDL_OK;
}

KDL_State_t KDL_TB6612_SetPin2(KDL_TB6612_t* ktb, GPIO_TypeDef* port, uint16_t pin)
{
	if(ktb == NULL)
		return KDL_ERROR;

	KDL_Pin_Init(&ktb->Pin2, port, pin);
	KDL_Pin_Reset(&ktb->Pin2);

	return KDL_OK;
}
/* IO operation functions *****************************************************/
KDL_State_t KDL_TB6612_SetSpeed(KDL_TB6612_t *ktb, float speed)
{
	if(ktb == NULL)
		return KDL_ERROR;

	ktb->Speed = speed;

	KDL_TB6612_Refresh(ktb);

	return KDL_OK;
}

KDL_State_t KDL_TB6612_IncSpeed(KDL_TB6612_t *ktb, float speed)
{
	if(ktb == NULL)
		return KDL_ERROR;

	ktb->Speed += speed;

	KDL_TB6612_Refresh(ktb);

	return KDL_OK;
}

KDL_State_t KDL_TB6612_SetSpeedBound(KDL_TB6612_t *ktb, float bound)
{
	if(ktb == NULL)
		return KDL_ERROR;

	ktb->SpeedBound = bound;

	KDL_TB6612_Refresh(ktb);

	return KDL_OK;
}

KDL_State_t KDL_TB6612_Refresh(KDL_TB6612_t *ktb)
{
	float ftmp;
	uint16_t utmp;
	TIM_HandleTypeDef *port;
	uint32_t channel;

	if(ktb == NULL)
		return KDL_ERROR;

	if(ktb->Speed>ktb->SpeedBound)
		ktb->Speed = ktb->SpeedBound;
	if(-ktb->Speed>ktb->SpeedBound)
		ktb->Speed = -ktb->SpeedBound;

	/* Set direction */
	if(ktb->Speed < 0 && ktb->Stat >= 0)
	{
		/* Clear */
		if(ktb->Pin1.port != NULL)
			KDL_Pin_Reset(&ktb->Pin1);
		if(ktb->Pin2.port != NULL)
			KDL_Pin_Reset(&ktb->Pin2);
		/* Set */
		if(ktb->Pin1.port != NULL)
			KDL_Pin_Set(&ktb->Pin1);
		/* State */
		ktb->Stat = -1;
	}
	else if(ktb->Speed > 0 && ktb->Stat <= 0)
	{
		/* Clear */
		if(ktb->Pin1.port != NULL)
			KDL_Pin_Reset(&ktb->Pin1);
		if(ktb->Pin2.port != NULL)
			KDL_Pin_Reset(&ktb->Pin2);
		/* Set */
		if(ktb->Pin2.port != NULL)
			KDL_Pin_Set(&ktb->Pin2);
		/* State */
		ktb->Stat = 1;
	}

	ftmp = ktb->Speed;
	if(ftmp < 0)
		ftmp = -ftmp;

	port = ktb->Port;
	channel = ktb->Channel;
	utmp = port->Instance->ARR;

	ftmp *= utmp;
	ftmp += 0.5;
	utmp = (uint16_t)ftmp;

	__HAL_TIM_SET_COMPARE(port, channel, utmp);

	return KDL_OK;
}

/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
