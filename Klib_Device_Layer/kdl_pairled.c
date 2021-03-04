/**
  ******************************************************************************
  * @date    Feb 18, 2021
  * @file    kdl_pairled.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Template source file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================


  @endverbatim
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "kdl_pairled.h"
#include "math.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_PairLED_Init(KDL_PairLED_t* dled, TIM_HandleTypeDef* port, uint32_t channel_low, uint32_t channel_high)
{
	if(dled == NULL)
		return KDL_ERROR;

	dled->Port = port;
	dled->Channel[0] = channel_low;
	dled->Channel[1] = channel_high;

	dled->Gamma = KDL_PairLED_DEFAULT_GAMMA;
	dled->Brightness = 0;
	dled->Brightness_Range = KDL_PairLED_DEFAULT_BRIGHTNESSRANGE;
	dled->Bound = KDL_PairLED_DEFAULT_BOUNDRANGE;
	dled->Bound_Range = KDL_PairLED_DEFAULT_BOUNDRANGE;
	dled->Proportion = 0;
	dled->Proportion_Range = KDL_PairLED_DEFAULT_PROPORTIONRANGE;

	dled->AutoReloadValue = dled->Port->Instance->ARR;
	if(dled->AutoReloadValue == 0xFFFF)
		dled->AutoReloadValue--;

	KDL_PairLED_ResetCheck(dled);
	KDL_PairLED_Refresh(dled);
	KDL_PairLED_On(dled);

	return KDL_OK;
}

/* Configuration functions ****************************************************/
KDL_State_t KDL_PairLED_On(KDL_PairLED_t* dled)
{
	if(!__IS_KDL_PairLED_T(dled))
		return KDL_ERROR;

	HAL_TIM_PWM_Start(dled->Port, dled->Channel[0]);
	HAL_TIM_PWM_Start(dled->Port, dled->Channel[1]);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_Off(KDL_PairLED_t* dled)
{
	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	HAL_TIM_PWM_Stop(dled->Port, dled->Channel[0]);
	HAL_TIM_PWM_Stop(dled->Port, dled->Channel[1]);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_SetBrightness(KDL_PairLED_t* dled, uint16_t brightness)
{
	if(!__IS_KDL_PairLED_T(dled))
		return KDL_ERROR;

	dled->Brightness = brightness<dled->Brightness_Range?brightness:dled->Brightness_Range & 0xFFFF;
	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_IncBrightness(KDL_PairLED_t* dled, uint16_t brightness)
{
	if(!__IS_KDL_PairLED_T(dled))
		return KDL_ERROR;

	if(!brightness)
		brightness = 1;

	brightness += dled->Brightness;

	dled->Brightness = brightness<dled->Brightness_Range?brightness:dled->Brightness_Range & 0xFFFF;
	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_DecBrightness(KDL_PairLED_t* dled, uint16_t brightness)
{
	if(!__IS_KDL_PairLED_T(dled))
		return KDL_ERROR;

	if(!brightness)
		brightness = 1;


	dled->Brightness = brightness<dled->Brightness?dled->Brightness-brightness:0;
	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_SetBrightnessRange(KDL_PairLED_t* dled, uint16_t range)
{
	double ratio;
	if(!__IS_KDL_PairLED_T(dled))
		return KDL_ERROR;

	ratio = dled->Brightness;
	ratio /= dled->Brightness_Range;

	dled->Brightness_Range = range;
	dled->Brightness = (uint16_t)(ratio * range + 0.5);

	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_SetBound(KDL_PairLED_t* dled, uint16_t bound)
{
	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	dled->Bound = bound<dled->Bound_Range?bound:dled->Bound_Range;
	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_SetBoundRange(KDL_PairLED_t* dled, uint16_t range)
{
	double ratio;

	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	ratio = dled->Bound;
	ratio /= dled->Bound_Range;

	dled->Bound_Range = range;
	dled->Bound = (uint16_t)(ratio * range + 0.5);

	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_SetProportion(KDL_PairLED_t* dled, uint16_t proportion)
{
	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	dled->Proportion = proportion;

	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_IncProportion(KDL_PairLED_t* dled, uint16_t proportion)
{
	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	if(!proportion)
		proportion = 1;

	dled->Proportion += proportion;

	if(dled->Proportion > dled->Proportion_Range)
		dled->Proportion = dled->Proportion_Range;

	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_DecProportion(KDL_PairLED_t* dled, uint16_t proportion)
{
	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	if(!proportion)
		proportion = 1;

	dled->Proportion = proportion<dled->Proportion?dled->Proportion-proportion:0;

	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_SetProportionRange(KDL_PairLED_t* dled, uint16_t range)
{
	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	dled->Proportion_Range = range;

	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}

KDL_State_t KDL_PairLED_SetGamma(KDL_PairLED_t* dled, double gamma)
{
	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	dled->Gamma = (uint16_t)(gamma * KDL_PairLED_GAMMA_SCALER + 0.5);

	KDL_PairLED_Refresh(dled);

	return KDL_OK;
}
/* IO operation functions *****************************************************/
#include "stdio.h"

KDL_State_t KDL_PairLED_Refresh(KDL_PairLED_t* dled)
{
	double tmp;
	double Pulse[2];
	uint16_t uPulse[2];
	uint8_t i;

	if(!__IS_KDL_PairLED_T(dled))
			return KDL_ERROR;

	tmp = ((double)dled->Brightness / dled->Brightness_Range) * ((double)dled->Bound / dled->Bound_Range);
	tmp = pow(tmp, ((double)dled->Gamma)/KDL_PairLED_GAMMA_SCALER);
	tmp *= dled->AutoReloadValue + 1;

	Pulse[0] = tmp * dled->Proportion;
	Pulse[0] /= dled->Proportion_Range;
	Pulse[1] = tmp - Pulse[0];
	for(i=0;i<2;i++)
	{
		uPulse[i] = (uint16_t)(Pulse[i] + 0.5);

		switch (dled->Channel[i])
		{
		case TIM_CHANNEL_1:

		  /* Check the parameters */
		  assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

		  /* Set the capture 1 value */
		  dled->Port->Instance->CCR1 =  i?uPulse[i]:dled->AutoReloadValue+1-uPulse[i];

		  break;

		case TIM_CHANNEL_2:

		  /* Set the parameters */
		  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

		  /* Return the capture 2 value */
		  dled->Port->Instance->CCR2 =  i?uPulse[i]:dled->AutoReloadValue+1-uPulse[i];

		  break;


		case TIM_CHANNEL_3:

		  /* Check the parameters */
		  assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

		  /* Set the capture 3 value */
		  dled->Port->Instance->CCR3 =  i?uPulse[i]:dled->AutoReloadValue+1-uPulse[i];

		  break;


		case TIM_CHANNEL_4:

		  /* Check the parameters */
		  assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

		  /* Set the capture 4 value */
		  dled->Port->Instance->CCR4 =  i?uPulse[i]:dled->AutoReloadValue+1-uPulse[i];

		  break;


		default:
		  break;
		}
	}

	return KDL_OK;
}
/* State and Error functions **************************************************/
KDL_State_t KDL_PairLED_Check(KDL_PairLED_t* dled)
{
	if(dled == NULL)
		return KDL_ERROR;

	return dled->check == (uint32_t)dled ? KDL_OK : KDL_ERROR;
}

KDL_State_t KDL_PairLED_ResetCheck(KDL_PairLED_t* dled)
{
	if(dled == NULL)
		return KDL_ERROR;

	dled->check = (uint32_t)dled;

	return KDL_OK;
}
/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
