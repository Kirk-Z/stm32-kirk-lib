/**
  ******************************************************************************
  * @date    Mar 7, 2021
  * @file    kd_floatpid.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   PID control in floating numbers
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KD_FLOATPID_H_
#define KD_FLOATPID_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kd.h"
/* Exported types ------------------------------------------------------------*/
typedef enum {
  KD_FloatPID_IndexP,
  KD_FloatPID_IndexI,
  KD_FloatPID_IndexD,
  KD_FloatPID_Target,
  KD_FloatPID_Tolerance,
  KD_FloatPID_Denominator,

} KD_FloatPID_Const_t;

typedef struct _KD_FloatPID_t {
  float Target;
  float indexP;
  float indexI;
  float indexD;
  float Tolerance;
  void(*Downlink)(float result);

  uint32_t Denominator;

  /* Private */
  float Error0, Error1;
  float Result;
} KD_FloatPID_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KD_State_t KD_FloatPID_Init(KD_FloatPID_t* kpid, void* downlink);
/* Configuration functions ****************************************************/
KD_State_t KD_FloatPID_SetIndex(KD_FloatPID_t* kpid, KD_FloatPID_Const_t set, float index);
/* IO operation functions *****************************************************/
KD_State_t KD_FloatPID_Process(KD_FloatPID_t* kpid, float current, uint32_t interval);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KD_FLOATPID_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
