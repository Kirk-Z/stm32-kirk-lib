/**
  ******************************************************************************
  * @date    Feb 18, 2021
  * @file    kdl_pairled.h
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
#ifndef KDL_PAIRLED_H_
#define KDL_PAIRLED_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
typedef struct _KDL_PairLED_t {

	TIM_HandleTypeDef *Port;
	uint32_t Channel[2];

	uint16_t Brightness;
	uint16_t Brightness_Range;

	uint16_t Bound;
	uint16_t Bound_Range;

	uint16_t Proportion;
	uint16_t Proportion_Range;

	uint16_t AutoReloadValue;

	uint16_t Gamma; /* 2W times of Gamma.  2.2 * 2W = 4W4 */

	uint32_t check;
} KDL_PairLED_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define KDL_PairLED_GAMMA_SCALER 20000U
#define KDL_PairLED_DEFAULT_GAMMA 44000U /* 2.2*2W=4W4 */
#define KDL_PairLED_DEFAULT_BRIGHTNESSRANGE 255U
#define KDL_PairLED_DEFAULT_BOUNDRANGE 255U
#define KDL_PairLED_DEFAULT_PROPORTIONRANGE 100U
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define __IS_KDL_PairLED_T(__HANDLE__) (KDL_OK == KDL_PairLED_Check(__HANDLE__))

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_PairLED_Init(KDL_PairLED_t* dled, TIM_HandleTypeDef* port, uint32_t channel_low, uint32_t channel_high);

/* Configuration functions ****************************************************/
KDL_State_t KDL_PairLED_On(KDL_PairLED_t* dled);
KDL_State_t KDL_PairLED_Off(KDL_PairLED_t* dled);

KDL_State_t KDL_PairLED_SetBrightness(KDL_PairLED_t* dled, uint16_t brightness);
KDL_State_t KDL_PairLED_IncBrightness(KDL_PairLED_t* dled, uint16_t brightness);
KDL_State_t KDL_PairLED_DecBrightness(KDL_PairLED_t* dled, uint16_t brightness);
KDL_State_t KDL_PairLED_SetBrightnessRange(KDL_PairLED_t* dled, uint16_t range);
KDL_State_t KDL_PairLED_SetBound(KDL_PairLED_t* dled, uint16_t bound);
KDL_State_t KDL_PairLED_SetBoundRange(KDL_PairLED_t* dled, uint16_t range);
KDL_State_t KDL_PairLED_SetProportion(KDL_PairLED_t* dled, uint16_t proportion);
KDL_State_t KDL_PairLED_IncProportion(KDL_PairLED_t* dled, uint16_t proportion);
KDL_State_t KDL_PairLED_DecProportion(KDL_PairLED_t* dled, uint16_t proportion);
KDL_State_t KDL_PairLED_SetProportionRange(KDL_PairLED_t* dled, uint16_t range);
KDL_State_t KDL_PairLED_SetGamma(KDL_PairLED_t* dled, double gamma);

KDL_State_t KDL_PairLED_Refresh(KDL_PairLED_t* dled);
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/
KDL_State_t KDL_PairLED_Check(KDL_PairLED_t* dled);
KDL_State_t KDL_PairLED_ResetCheck(KDL_PairLED_t* dled);

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_PAIRLED_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
