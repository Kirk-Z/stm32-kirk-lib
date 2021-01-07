/**
  ******************************************************************************
  * @date    Jan 7, 2021
  * @file    kdl_led.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Source file of Device LED
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
#include "kdl_led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/**
  * @brief  Initialize the LED
  * @param  dled  pointer of the led
  * @param  port  GPIO port
  * @param  pin  GPIO pin number
  * @param  stat  indicate whether low/high level associates the LED on
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_LED_Init(KDL_LED_t* dled, GPIO_TypeDef* port, uint16_t pin, KDL_LED_State stat)
{
	if(dled == NULL)
		return KDL_ERROR;

	dled->Port = port;
	dled->Pin = pin;
	dled->Onstate = stat;

	KDL_LED_ResetCheck(dled);

	return KDL_OK;
}

/* Configuration functions ****************************************************/

/* IO operation functions *****************************************************/
/**
  * @brief  Turn on the LED
  * @param  dled  pointer of the led
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_LED_TurnOn(KDL_LED_t* dled)
{
	if(!__IS_KDL_LED_T(dled))
		return KDL_ERROR;

	HAL_GPIO_WritePin(dled->Port, dled->Pin, (GPIO_PinState)(dled->Onstate));

	return KDL_OK;
}

/**
  * @brief  Turn off the LED
  * @param  dled  pointer of the led
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_LED_TurnOff(KDL_LED_t* dled)
{
	if(!__IS_KDL_LED_T(dled))
		return KDL_ERROR;

	HAL_GPIO_WritePin(dled->Port, dled->Pin, (GPIO_PinState)(!dled->Onstate));

	return KDL_OK;
}

/**
  * @brief  Toggle the LED
  * @param  dled  pointer of the led
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_LED_Toggle(KDL_LED_t* dled)
{
	if(!__IS_KDL_LED_T(dled))
		return KDL_ERROR;

	HAL_GPIO_TogglePin(dled->Port, dled->Pin);

	return KDL_OK;
}

/**
  * @brief  Turn on/off the LED
  * @param  dled  pointer of the led
  * @param  stat  on/off
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_LED_Turn(KDL_LED_t* dled, KDL_LED_State stat)
{
	if(!__IS_KDL_LED_T(dled))
		return KDL_ERROR;

	HAL_GPIO_WritePin(dled->Port, dled->Pin, (GPIO_PinState)(!(stat ^ dled->Onstate)));

	return KDL_OK;
}

/* Peripheral State and Error functions ***************************************/
/**
  * @brief  Check whether it is legal LED
  * @param  dled  pointer of the led
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_LED_Check(KDL_LED_t* dled)
{
	return dled->check == (uint32_t)dled ? KDL_OK : KDL_ERROR;
}

/**
  * @brief  Reset the check value of the LED
  * @param  dled  pointer of the led
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_LED_ResetCheck(KDL_LED_t* dled)
{
	if(dled == NULL)
		return ERROR;

	dled->check = (uint32_t)dled;

	return KDL_OK;
}


/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
