/**
  ******************************************************************************
  * @date    Feb 8, 2021
  * @file    kdl_pcf8574.h
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
#ifndef KDL_PCF8574_H_
#define KDL_PCF8574_H_

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
typedef struct _KDL_PCF8574_t {
	I2C_HandleTypeDef *hi2c;
	I2C_TypeDef *i2c;
	uint16_t address;

	volatile uint8_t io;

	uint32_t check;
} KDL_PCF8574_t;
/* Exported constants --------------------------------------------------------*/
#define KDL_PCF8574_INTPIN GPIO_PIN_5
/* Exported macros -----------------------------------------------------------*/
/**
 * @brief  get current value
 * @note  directly from buffer
 */
#define KDL_PCF8574_Get(__HANDLE__) ((__HANDEL__)->io)
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/**
 * @brief  check whether pointer valid
 */
#define __IS_KDL_PCF8574_T(__HANDLE__) (KDL_OK == KDL_PCF8574_Check(__HANDLE__))

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_PCF8574_Init(KDL_PCF8574_t* kpcf, I2C_HandleTypeDef *hi2c, uint16_t address);
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
uint8_t KDL_PCF8574_Read(KDL_PCF8574_t* kpcf);
KDL_State_t KDL_PCF8574_Write(KDL_PCF8574_t* kpcf, uint8_t data);
KDL_State_t KDL_PCF8574_Set(KDL_PCF8574_t* kpcf, uint8_t bit);
KDL_State_t KDL_PCF8574_Reset(KDL_PCF8574_t* kpcf, uint8_t bit);
KDL_State_t PCF8574_Toggle(KDL_PCF8574_t* kpcf,uint8_t bit);
uint8_t KDL_PCF8574_ReadBit(KDL_PCF8574_t* kpcf, uint8_t bit);
KDL_State_t KDL_PCF8574_WriteBit(KDL_PCF8574_t* kpcf, uint8_t bit, uint8_t data);
/* State and Error functions **************************************************/
KDL_State_t KDL_PCF8574_Check(KDL_PCF8574_t* dpcf);
KDL_State_t KDL_PCF8574_ResetCheck(KDL_PCF8574_t* dpcf);

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_PCF8574_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
