/**
  ******************************************************************************
  * @date    Jan 7, 2021
  * @file    kdl.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Header file of Device layer
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_H_
#define KDL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  enum of KDL status
  */
typedef enum {
	KDL_OK       = 0x00,
	KDL_ERROR    = 0x01,
	KDL_BUSY     = 0x02,
	KDL_TIMEOUT  = 0x03,
} KDL_State_t;

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

#endif /* KDL_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
