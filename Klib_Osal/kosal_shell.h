/**
  ******************************************************************************
  * @date    May 13, 2021
  * @file    kosal_shell.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Shell on operation system header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KOSAL_SHELL_H_
#define KOSAL_SHELL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kosal.h"
#include "kosal_usart.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
Kosal_State_t Kosal_Shell_Init(Kosal_Usart_t *kuart, UART_HandleTypeDef* huart);
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KOSAL_SHELL_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
