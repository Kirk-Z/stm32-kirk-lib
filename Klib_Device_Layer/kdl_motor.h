/**
  ******************************************************************************
  * @date    Mar 6, 2021
  * @file    kdl_motor.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   General motor control header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_MOTOR_H_
#define KDL_MOTOR_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
typedef struct _KDL_Motor_t {

	TIM_HandleTypeDef *Port1;
	uint32_t Channel1;
	TIM_HandleTypeDef *Port2;
	uint32_t Channel2;

	float Speed;
	float SpeedBound;

} KDL_Motor_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Motor_Init(KDL_Motor_t *dmtr, TIM_HandleTypeDef *port1, uint32_t channel1, TIM_HandleTypeDef *port2, uint32_t channel2);
/* Configuration functions ****************************************************/
KDL_State_t KDL_Motor_SetSpeed(KDL_Motor_t *dmtr, float speed);
KDL_State_t KDL_Motor_IncSpeed(KDL_Motor_t *dmtr, float speed);
KDL_State_t KDL_Motor_SetSpeedBound(KDL_Motor_t *dmtr, float bound);
KDL_State_t KDL_Motor_Refresh(KDL_Motor_t *dmtr);
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_MOTOR_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
