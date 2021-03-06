/**
  ******************************************************************************
  * @date    Jan 17, 2021
  * @file    kdl_button.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Button source file
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
#include "kdl_button.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Button_Init(KDL_Button_t* kbt, void* press, void* release)
{
	kbt->press = press;
	kbt->release = release;

	kbt->downevent.event = NULL;
	kbt->downevent.offset = 0xFFFFFFFF;
	kbt->downevent.period = 0xFFFFFFFF;
	kbt->upevent = kbt->downevent;


	kbt->state = KDL_Button_State_Up;

	kbt->last = HAL_GetTick();

	return KDL_OK;
}
/* Configuration functions ****************************************************/
KDL_State_t KDL_Button_SetDownEvent(KDL_Button_t* kbt, void* down, uint32_t offset, uint32_t period)
{
	kbt->downevent.event = down;
	kbt->downevent.offset = offset;
	kbt->downevent.period = period;

	return KDL_OK;
}
/* For Debug */
#include "stdio.h"

KDL_State_t KDL_Button_DownEvent(KDL_Button_t* kbt)
{

	return KDL_OK;
}

KDL_State_t KDL_Button_SetUpEvent(KDL_Button_t* kbt, void* up, uint32_t offset, uint32_t period)
{
	kbt->upevent.event = up;
	kbt->upevent.offset = offset;
	kbt->upevent.period = period;

	return KDL_OK;
}

KDL_State_t KDL_Button_UpEvent(KDL_Button_t* kbt)
{

	return KDL_OK;
}
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
