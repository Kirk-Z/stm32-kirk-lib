/**
  ******************************************************************************
  * @date    Feb 23, 2021
  * @file    kdl_pin.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Template header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_PIN_H_
#define KDL_PIN_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
#ifndef HAL_GPIO_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#endif /* HAL_GPIO_MODULE_ENABLED */

typedef struct _KDL_Pin_t {
	GPIO_TypeDef* port;
	uint16_t pin;
} KDL_Pin_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Pin_Init(KDL_Pin_t* kpin, GPIO_TypeDef* port, uint16_t pin);
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
KDL_State_t KDL_Pin_Set(KDL_Pin_t* kpin);
KDL_State_t KDL_Pin_Reset(KDL_Pin_t* kpin);
KDL_State_t KDL_Pin_Toggle(KDL_Pin_t* kpin);
KDL_State_t KDL_Pin_Write(KDL_Pin_t* kpin, uint8_t level);

uint8_t KDL_Pin_Read(KDL_Pin_t* kpin);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_PIN_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
