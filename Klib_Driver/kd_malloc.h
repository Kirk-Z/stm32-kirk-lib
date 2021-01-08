/**
  ******************************************************************************
  * @date    Jan 8, 2021
  * @file    kd_malloc.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Self-developed memory management header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KD_MALLOC_H_
#define KD_MALLOC_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kd.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
 void KD_Mem_Init(void);
/* Configuration functions ****************************************************/
 KD_State_t KD_Free_Base(void* loc, uint32_t size, uint32_t length);
 void* KD_Malloc_Base(uint32_t size, uint32_t length);
 void* KD_Realloc_Base(void* loc, uint32_t size, uint32_t length, uint32_t new_size, uint32_t new_length);
/* IO operation functions *****************************************************/
 KD_State_t KD_Free(void* loc);
 void* KD_Malloc(uint32_t size, uint32_t length);
 void* KD_Realloc(void* loc, uint32_t new_size, uint32_t new_length);
/* State and Error functions **************************************************/
 uint32_t KD_Mem_CalculateWord(uint32_t size, uint32_t length);
 uint32_t KD_Mem_CalculateBlock(uint32_t length);
 uint32_t KD_Mem_UseRate(void);
 uint8_t KD_Mem_First_1(uint32_t num);

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KD_MALLOC_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
