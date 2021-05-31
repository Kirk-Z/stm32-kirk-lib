/**
  ******************************************************************************
  * @date    May 13, 2021
  * @file    kosal_usart.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   USART driver based on operation system header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KOSAL_USART_H_
#define KOSAL_USART_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kosal.h"
/* Private constants ---------------------------------------------------------*/
#define Kosal_Usart_RxBufSize 256U
#define Kosal_Usart_MaxLength 256U

#define Kosal_Usart_PayloadFreeRequired 0x01
#define Kosal_Usart_OwnerFreeRequired   0x02
#define Kosal_Usart_CurrentlyProcessing 0x04
#define Kosal_Usart_HasData             0x08
#define Kosal_Usart_ProcessFinished     0x10
/* Exported types ------------------------------------------------------------*/
typedef struct _Kosal_Usart_t {
	UART_HandleTypeDef* huart;
	ListItem_t Item;
	List_t ListTx;
	List_t ListRx;
	osThreadId_t TxId;
	osThreadId_t RxId;
} Kosal_Usart_t;

typedef struct _Kosal_Usart_Transmit_t {
	UART_HandleTypeDef* huart;
	ListItem_t item;
	uint8_t* payload;
	uint32_t length;
	uint8_t state;
} Kosal_Message_Transmit_t;

typedef struct _Kosal_Usart_Receive_t {
	UART_HandleTypeDef* huart;
	ListItem_t item;
	uint8_t* payload;
	uint32_t length;
	uint32_t index;
	uint8_t state;
} Kosal_Usart_Receive_t;
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
Kosal_State_t Kosal_Usart_Init(Kosal_Usart_t *kuart, UART_HandleTypeDef* huart);
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
void Kosal_Usart_Transmit(Kosal_Usart_t *kuart, uint8_t* buf, uint32_t length, uint32_t state);
int Kosal_Usart_Printf(Kosal_Usart_t *kuart, const char *fmt, ...);
/* State and Error functions **************************************************/
Kosal_Usart_t* Kosal_Usart_GetKuart(UART_HandleTypeDef *huart);
Kosal_State_t Kosal_Usart_Receive_Available(Kosal_Usart_t* kuart);
uint8_t  Kosal_Usart_Receive_NoCheck(Kosal_Usart_t* kuart);
/* Private functions ---------------------------------------------------------*/
void Kosal_Usart_TxCpltCallback(UART_HandleTypeDef *huart);
void Kosal_Usart_RxCpltCallback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* KOSAL_USART_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
