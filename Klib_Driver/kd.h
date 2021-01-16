/**
  ******************************************************************************
  * @date    Jan 8, 2021
  * @file    kd.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Klib Drivers header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KD_H_
#define KD_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stddef.h"
/* Exported types ------------------------------------------------------------*/
typedef enum {
	KD_OK       = 0x00,
	KD_ERROR    = 0x01,
	KD_BUSY     = 0x02,
	KD_TIMEOUT  = 0x03,
} KD_State_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KD_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
