/**
  ******************************************************************************
  * @date    Jan 16, 2021
  * @file    kdl_sht30.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Temperature and humidity module SHT30 Header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_SHT30_H_
#define KDL_SHT30_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"

/**
 * @brief  to ensure I2C is enabled
 */
#ifndef HAL_I2C_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#endif /* HAL_I2C_MODULE_ENABLED */

/* Exported types ------------------------------------------------------------*/
 /**
   * @brief  type of SHT30
   */
typedef struct _KDL_SHT30_t {
	I2C_HandleTypeDef *hi2c;
	I2C_TypeDef *i2c;
	uint16_t address;
	uint16_t temp;
	uint16_t hum;

	uint32_t check;
} KDL_SHT30_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/**
 * @brief  check whether pointer valid
 */
#define __IS_KDL_SHT30_T(__HANDLE__) (KDL_OK == KDL_SHT30_Check(__HANDLE__))

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_SHT30_Init(KDL_SHT30_t* dsht, I2C_HandleTypeDef *hi2c, uint16_t address);
/* Configuration functions ****************************************************/
KDL_State_t KDL_SHT30_Convert(KDL_SHT30_t* dsht);
/* IO operation functions *****************************************************/
uint16_t KDL_SHT30_GetTemp(KDL_SHT30_t* dsht);
uint16_t KDL_SHT30_GetHum(KDL_SHT30_t* dsht);
float KDL_SHT30_fGetTemp(KDL_SHT30_t* dsht);
float KDL_SHT30_fGetHum(KDL_SHT30_t* dsht);
/* State and Error functions **************************************************/
KDL_State_t KDL_SHT30_Check(KDL_SHT30_t* dsht);
KDL_State_t KDL_SHT30_ResetCheck(KDL_SHT30_t* dsht);
/* Private functions ---------------------------------------------------------*/
KDL_State_t KDL_SHT30_CheckCRC(uint8_t data[], uint8_t length, uint8_t checksum);

#ifdef __cplusplus
}
#endif

#endif /* KDL_SHT30_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
