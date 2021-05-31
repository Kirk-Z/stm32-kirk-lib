/**
  ******************************************************************************
  * @date    May 13, 2021
  * @file    kosal_shell.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Shell on operation system source file
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
#include "kosal_shell.h"
#include "kosal_usart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Kosal_Shell_Thread(void* argument)
{
	/* Set priority to lowest */
	osThreadSetPriority(osThreadGetId(), osPriorityLow-1);
	/* Infinite loop */
	for(;;)
	{

	}
}
/* Initialization and de-initialization functions *****************************/
Kosal_State_t Kosal_Shell_Init(Kosal_Usart_t *kuart, UART_HandleTypeDef* huart)
{
	/* Init usart */
	Kosal_Usart_Init(kuart, huart);
	/* Start thread */
	osThreadNew(Kosal_Shell_Thread, kuart, NULL);
	/* Finished */
	return Kosal_OK;
}
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
