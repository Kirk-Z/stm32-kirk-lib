/**
  ******************************************************************************
  * @date    Jan 8, 2021
  * @file    kd_math.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   General mathematics header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KD_MATH_H_
#define KD_MATH_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kd.h"
#include "math.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define PI ((double)3.14159265358979)

extern const uint32_t KD_MATH_BIT_TRIANGLE[33];

/* Exported macros -----------------------------------------------------------*/


/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
 uint32_t KD_Math_Min(uint32_t, uint32_t);
 uint32_t KD_Math_Max(uint32_t, uint32_t);
/* Initialization and de-initialization functions *****************************/
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KD_MATH_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
