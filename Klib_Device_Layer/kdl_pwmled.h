/**
  ******************************************************************************
  * @date    Jan 7, 2021
  * @file    kdl_pwmled.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Device PWMLED header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_PWMLED_H_
#define KDL_PWMLED_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief enum of PWMLED status for on
 */
typedef enum {
	KDL_PWMLED_ONSTATE_LOW = 0,
	KDL_PWMLED_ONSTATE_HIGH
} KDL_PWMLED_OnState;

/**
 * @brief struct of PWMLED
 */
typedef struct _KDL_PWMLED_t {

	TIM_HandleTypeDef *Port;
	uint32_t Channel;

	uint16_t Brightness;
	uint16_t Brightness_Range;

	uint16_t Bound;
	uint16_t Bound_Range;

	uint16_t AutoReloadValue;
	KDL_PWMLED_OnState OnState;

	uint16_t Gamma; /* 2W times of Gamma.  2.2 * 2W = 4W4 */

	uint32_t check;

} KDL_PWMLED_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define KDL_PWMLED_GAMMA_SCALER 20000U
#define KDL_PWMLED_DEFAULT_GAMMA 44000U /* 2.2*2W=4W4 */
#define KDL_PWMLED_DEFAULT_BRIGHTNESSRANGE 255U
#define KDL_PWMLED_DEFAULT_BOUNDRANGE 255U
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define __IS_KDL_PWMLED_T(__HANDLE__) (KDL_OK == KDL_PWMLED_Check(__HANDLE__))

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_PWMLED_Init(KDL_PWMLED_t* dled, TIM_HandleTypeDef* port, uint32_t channel, KDL_PWMLED_OnState onstat);

/* Configuration functions ****************************************************/
KDL_State_t KDL_PWMLED_On(KDL_PWMLED_t* dled);
KDL_State_t KDL_PWMLED_Off(KDL_PWMLED_t* dled);

KDL_State_t KDL_PWMLED_SetBrightness(KDL_PWMLED_t* dled, uint16_t brightness);
KDL_State_t KDL_PWMLED_SetBrightnessRange(KDL_PWMLED_t* dled, uint16_t range);
KDL_State_t KDL_PWMLED_SetBound(KDL_PWMLED_t* dled, uint16_t bound);
KDL_State_t KDL_PWMLED_SetBoundRange(KDL_PWMLED_t* dled, uint16_t range);
KDL_State_t KDL_PWMLED_SetGamma(KDL_PWMLED_t* dled, double gamma);

KDL_State_t KDL_PWMLED_Refresh(KDL_PWMLED_t* dled);
/* IO operation functions *****************************************************/

/* State and Error functions **************************************************/
KDL_State_t KDL_PWMLED_Check(KDL_PWMLED_t* dled);
KDL_State_t KDL_PWMLED_ResetCheck(KDL_PWMLED_t* dled);

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_PWMLED_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
