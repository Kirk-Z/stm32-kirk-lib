/**
  ******************************************************************************
  * @date    Apr 7, 2021
  * @file    kdl_tb6612.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   TB6612 TB6612 driver header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_TB6612_H_
#define KDL_TB6612_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
#include "kdl_pin.h"
/* Exported types ------------------------------------------------------------*/
typedef struct KDL_TB6612_t {
	TIM_HandleTypeDef *Port;
	uint32_t Channel;

	KDL_Pin_t Pin1, Pin2;

	float Speed;
	float SpeedBound;

	int16_t Stat;

} KDL_TB6612_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_TB6612_Init(KDL_TB6612_t* ktb);
/* Configuration functions ****************************************************/
KDL_State_t KDL_TB6612_SetPort(KDL_TB6612_t* ktb, TIM_HandleTypeDef* port, uint32_t channel);
KDL_State_t KDL_TB6612_SetPin1(KDL_TB6612_t* ktb, GPIO_TypeDef* port, uint16_t pin);
KDL_State_t KDL_TB6612_SetPin2(KDL_TB6612_t* ktb, GPIO_TypeDef* port, uint16_t pin);
/* IO operation functions *****************************************************/
KDL_State_t KDL_TB6612_SetSpeed(KDL_TB6612_t *ktb, float speed);
KDL_State_t KDL_TB6612_IncSpeed(KDL_TB6612_t *ktb, float speed);
KDL_State_t KDL_TB6612_SetSpeedBound(KDL_TB6612_t *ktb, float bound);
KDL_State_t KDL_TB6612_Refresh(KDL_TB6612_t *ktb);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_TB6612_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
