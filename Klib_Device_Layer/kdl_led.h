/**
  ******************************************************************************
  * @date    Jan 7, 2021
  * @file    kdl_led.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Header file of Device LED
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_LED_H_
#define KDL_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  enum of LED on status
  */
typedef enum {
	KDL_LED_ONSTATE_LOW = 0,
	KDL_LED_ONSTATE_HIGH
} KDL_LED_OnState;

/**
  * @brief  enum of LED status
  */
typedef enum {
	KDL_LED_OFF = 0,
	KDL_LED_ON
} KDL_LED_State;

/**
  * @brief  type of LED
  */
typedef struct _KDL_LED_t {

	GPIO_TypeDef* Port;
	uint16_t Pin;

	KDL_LED_State Onstate;

	uint32_t check;

} KDL_LED_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/**
 * @brief  check whether pointer valid
 */
#define __IS_KDL_LED_T(__HANDLE__) (KDL_OK == KDL_LED_Check(__HANDLE__))

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_LED_Init(KDL_LED_t* dled, GPIO_TypeDef* port, uint16_t pin, KDL_LED_State stat);

/* Configuration functions ****************************************************/

/* IO operation functions *****************************************************/
KDL_State_t KDL_LED_TurnOn(KDL_LED_t* dled);
KDL_State_t KDL_LED_TurnOff(KDL_LED_t* dled);
KDL_State_t KDL_LED_Toggle(KDL_LED_t* dled);
KDL_State_t KDL_LED_Turn(KDL_LED_t* dled, KDL_LED_State stat);

/* State and Error functions **************************************************/
KDL_State_t KDL_LED_Check(KDL_LED_t* dled);
KDL_State_t KDL_LED_ResetCheck(KDL_LED_t* dled);

/* Peripheral State and Error functions ***************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_LED_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
