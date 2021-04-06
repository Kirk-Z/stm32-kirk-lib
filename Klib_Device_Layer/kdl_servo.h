/**
  ******************************************************************************
  * @date    Apr 6, 2021
  * @file    kdl_servo.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Servo driver header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_SERVO_H_
#define KDL_SERVO_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
typedef struct _KDL_Servo_t {
	TIM_HandleTypeDef *port;
	uint32_t channel;

	float position;
} KDL_Servo_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Servo_Init(KDL_Servo_t* ksvo, TIM_HandleTypeDef *port, uint32_t channel);
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
KDL_State_t KDL_Servo_Set(KDL_Servo_t* ksvo, float position);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_SERVO_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
