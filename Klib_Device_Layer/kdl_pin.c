/**
  ******************************************************************************
  * @date    Feb 23, 2021
  * @file    kdl_pin.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Template source file
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
#include "kdl_pin.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Pin_Init(KDL_Pin_t* kpin, GPIO_TypeDef* port, uint16_t pin)
{
	kpin->port = port;
	kpin->pin = pin;

	return KDL_OK;
}
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
KDL_State_t KDL_Pin_Set(KDL_Pin_t* kpin)
{
	HAL_GPIO_WritePin(kpin->port, kpin->pin, GPIO_PIN_SET);

	return KDL_OK;
}

KDL_State_t KDL_Pin_Reset(KDL_Pin_t* kpin)
{
	HAL_GPIO_WritePin(kpin->port, kpin->pin, GPIO_PIN_RESET);

	return KDL_OK;
}

KDL_State_t KDL_Pin_Toggle(KDL_Pin_t* kpin)
{
	HAL_GPIO_TogglePin(kpin->port, kpin->pin);

	return KDL_OK;
}

KDL_State_t KDL_Pin_Write(KDL_Pin_t* kpin, uint8_t level)
{
	HAL_GPIO_WritePin(kpin->port, kpin->pin, level?GPIO_PIN_SET:GPIO_PIN_RESET);

	return KDL_OK;
}

uint8_t KDL_Pin_Read(KDL_Pin_t* kpin)
{
	return HAL_GPIO_ReadPin(kpin->port, kpin->pin);
}
/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
