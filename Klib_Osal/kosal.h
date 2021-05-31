/**
  ******************************************************************************
  * @date    May 3, 2021
  * @file    losal.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Klib operation system abstract layer head file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KOSAL_H_
#define KOSAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
/* Exported types ------------------------------------------------------------*/

/**
  * @brief  enum of KOSAL status
  */
typedef enum {
	Kosal_OK       = 0x00,
	Kosal_Error    = 0x01,
	Kosal_Busy     = 0x02,
	Kosal_Timeout  = 0x03,
} Kosal_State_t;

/**
 * @brief  printf return types
 */
typedef struct _Kosal_Printf_Return_t {
	int length;
	int counts;
} Kosal_Printf_Return_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
Kosal_Printf_Return_t Kosal_VsPrintf(char *buf, int limit, const char *fmt, va_list args);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KOSAL_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
