/**
  ******************************************************************************
  * @date    Mar 7, 2021
  * @file    kdl_encoder.h
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
#ifndef KDL_ENCODER_H_
#define KDL_ENCODER_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
typedef struct _KDL_Encoder_t {
	TIM_HandleTypeDef* Port;
	TIM_TypeDef* Tim;

	int32_t Position;
	int32_t Velocity;

	uint32_t Last;
} KDL_Encoder_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Encoder_Init(KDL_Encoder_t* decd, TIM_HandleTypeDef *port);
/* Configuration functions ****************************************************/
KDL_State_t KDL_Encoder_Clear(KDL_Encoder_t* decd);
KDL_State_t KDL_Encoder_Start(KDL_Encoder_t* decd);
KDL_State_t KDL_Encoder_Stop(KDL_Encoder_t* decd);

KDL_State_t KDL_Encoder_Convert(KDL_Encoder_t* decd);
/* IO operation functions *****************************************************/
int32_t KDL_Encoder_GetPosition(KDL_Encoder_t* decd);
int32_t KDL_Encoder_GetVelocity(KDL_Encoder_t* decd);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_ENCODER_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
