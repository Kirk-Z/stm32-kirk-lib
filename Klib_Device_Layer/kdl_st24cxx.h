/**
  ******************************************************************************
  * @date    Jan 8, 2021
  * @file    kdl_st24cxx.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   ST24Cxx EEPROM header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_ST24CXX_H_
#define KDL_ST24CXX_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {

	KDL_ST24C02 = 0,
	KDL_ST24C04 = 1,
	KDL_ST24C08 = 2,
	KDL_ST24C16 = 3,
	KDL_ST24C32 = 4,
	KDL_ST24C64 = 5,
	KDL_ST24C128 = 6,
	KDL_ST24C256 = 7,
	KDL_ST24C512 = 8,

} KDL_ST24Cxx_Size_t;

typedef struct _KDL_ST24Cxx_t {

	I2C_HandleTypeDef* Port;
	uint16_t Address;
	KDL_ST24Cxx_Size_t Size;
	uint8_t Stat;

} KDL_ST24Cxx_t;

/* Exported constants --------------------------------------------------------*/
#define KDL_ST24CXX_OPERATION_INTERVAL (2U)
/* Exported macros -----------------------------------------------------------*/
#define __IS_KDL_ST24CXX_T(__DST__) ((__DST__)->Stat != 0)

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_ST24Cxx_Init(KDL_ST24Cxx_t* dst, I2C_HandleTypeDef* port, uint16_t address, KDL_ST24Cxx_Size_t size);

/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
KDL_State_t KDL_ST24Cxx_ReadDWord(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer);
KDL_State_t KDL_ST24Cxx_WriteDWord(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer);

KDL_State_t KDL_ST24Cxx_Read(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer, uint32_t length);
KDL_State_t KDL_ST24Cxx_Write(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer, uint32_t length);

/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_ST24CXX_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
