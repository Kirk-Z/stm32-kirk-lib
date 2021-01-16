/**
  ******************************************************************************
  * @date    Jan 16, 2021
  * @file    kdl_bh1750.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Brightness module BH1750 head file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_BH1750_H_
#define KDL_BH1750_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
 /**
   * @brief  type of BH1750
   */
typedef struct _KDL_BH1750_t {
	I2C_HandleTypeDef *hi2c;
	I2C_TypeDef *i2c;
	uint16_t address;
	uint16_t luminance;

	uint32_t check;
} KDL_BH1750_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/**
 * @brief  check whether pointer valid
 */
#define __IS_KDL_BH1750_T(__HANDLE__) (KDL_OK == KDL_BH1750_Check(__HANDLE__))
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_BH1750_Init(KDL_BH1750_t* dbh, I2C_HandleTypeDef *hi2c, uint16_t address);
/* Configuration functions ****************************************************/
KDL_State_t KDL_BH1750_Start(KDL_BH1750_t* dbh);
KDL_State_t KDL_BH1750_Convert(KDL_BH1750_t* dbh);
/* IO operation functions *****************************************************/
uint16_t KDL_BH1750_GetLum(KDL_BH1750_t* dbh);
float KDL_BH1750_fGetLum(KDL_BH1750_t* dbh);
/* State and Error functions **************************************************/
KDL_State_t KDL_BH1750_Check(KDL_BH1750_t* dbh);
KDL_State_t KDL_BH1750_ResetCheck(KDL_BH1750_t* dbh);
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_BH1750_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
