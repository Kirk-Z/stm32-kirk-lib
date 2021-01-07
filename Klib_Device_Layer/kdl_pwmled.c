/**
  ******************************************************************************
  * @date    Jan 7, 2021
  * @file    kdl_pwmled.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Device PWMLED source file
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
#include "kdl_pwmled.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/**
  * @brief  initialize the PWMLED
  * @param  dled  pointer of the PWMLED
  * @param  port  handler of the TIM port
  * @param  channel  channel of the TIM
  * @param  onstat  compare result with the status of LED
  *                 if number in counter is small, LED is on,
  *                 then it should be KDL_PWMLED_ONSTATE_LOW
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_Init(KDL_PWMLED_t* dled, TIM_HandleTypeDef* port, uint32_t channel, KDL_PWMLED_OnState onstat)
{
	// TODO
	if(dled == NULL)
		return KDL_ERROR;

	dled->Port = port;
	dled->Channel = channel;
	dled->OnState = onstat;
	dled->Gamma = KDL_PWMLED_DEFAULT_GAMMA;
	dled->Brightness = 0;
	dled->Brightness_Range = KDL_PWMLED_DEFAULT_BRIGHTNESSRANGE;
	dled->Bound = KDL_PWMLED_DEFAULT_BOUNDRANGE;
	dled->Bound_Range = KDL_PWMLED_DEFAULT_BOUNDRANGE;

	dled->AutoReloadValue = dled->Port->Instance->ARR;
	if(dled->AutoReloadValue == 0xFFFF)
		dled->AutoReloadValue--;

	KDL_PWMLED_ResetCheck(dled);
	KDL_PWMLED_Refresh(dled);
	KDL_PWMLED_On(dled);

	return KDL_OK;
}

/* Configuration functions ****************************************************/
/**
  * @brief  Start output PWM wave
  * @param  dled  pointer of the PWMLED
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_On(KDL_PWMLED_t* dled)
{
	// TODO
	if(!__IS_KDL_PWMLED_T(dled))
		return KDL_ERROR;

	HAL_TIM_PWM_Start(dled->Port, dled->Channel);

	return KDL_OK;
}

/**
  * @brief  Stop output PWM wave
  * @param  dled  pointer of the PWMLED
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_Off(KDL_PWMLED_t* dled)
{
	// TODO
	if(!__IS_KDL_PWMLED_T(dled))
			return KDL_ERROR;

	HAL_TIM_PWM_Stop(dled->Port, dled->Channel);

	return KDL_OK;
}

/**
  * @brief  Set the brightness
  * @param  dled  pointer of the PWMLED
  * @param  brightness  brightness
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_SetBrightness(KDL_PWMLED_t* dled, uint16_t brightness)
{
	// TODO
	if(!__IS_KDL_PWMLED_T(dled))
		return KDL_ERROR;

	dled->Brightness = brightness<dled->Brightness_Range?brightness:dled->Brightness_Range & 0xFFFF;
	KDL_PWMLED_Refresh(dled);

	return KDL_OK;
}

/**
  * @brief  Set the range of brightness
  * @param  dled  pointer of the PWMLED
  * @param  range  range of brightness
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_SetBrightnessRange(KDL_PWMLED_t* dled, uint16_t range)
{
	// TODO
	double ratio;
	if(!__IS_KDL_PWMLED_T(dled))
		return KDL_ERROR;

	ratio = dled->Brightness;
	ratio /= dled->Brightness_Range;

	dled->Brightness_Range = range;
	dled->Brightness = (uint16_t)(ratio * range + 0.5);

	KDL_PWMLED_Refresh(dled);

	return KDL_OK;
}

/**
  * @brief  Set the brightness bound
  * @param  dled  pointer of the PWMLED
  * @param  bound  brightness bound
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_SetBound(KDL_PWMLED_t* dled, uint16_t bound)
{
	// TODO
	if(!__IS_KDL_PWMLED_T(dled))
			return KDL_ERROR;

	dled->Bound = bound<dled->Bound_Range?bound:dled->Bound_Range;
	KDL_PWMLED_Refresh(dled);

	return KDL_OK;
}

/**
  * @brief  Set the bound range of brightness
  * @param  dled  pointer of the PWMLED
  * @param  range  range of bound
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_SetBoundRange(KDL_PWMLED_t* dled, uint16_t range)
{
	// TODO
	double ratio;

	if(!__IS_KDL_PWMLED_T(dled))
			return KDL_ERROR;

	ratio = dled->Bound;
	ratio /= dled->Bound_Range;

	dled->Bound_Range = range;
	dled->Bound = (uint16_t)(ratio * range + 0.5);

	KDL_PWMLED_Refresh(dled);

	return KDL_OK;
}

/**
  * @brief  Set the Gamma
  * @note   Gamma is the power index of brightness
  * @param  dled  pointer of the PWMLED
  * @param  gamma  value of gamma
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_SetGamma(KDL_PWMLED_t* dled, double gamma)
{
	// TODO
	if(!__IS_KDL_PWMLED_T(dled))
			return KDL_ERROR;

	dled->Gamma = (uint16_t)(gamma * KDL_PWMLED_GAMMA_SCALER + 0.5);

	KDL_PWMLED_Refresh(dled);

	return KDL_OK;
}

/**
  * @brief  Refresh the PWM output
  * @param  dled  pointer of the PWMLED
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_Refresh(KDL_PWMLED_t* dled)
{
	// TODO
	double Pulse;
	uint16_t uPulse;

	if(!__IS_KDL_PWMLED_T(dled))
			return KDL_ERROR;

	Pulse = ((double)dled->Brightness / dled->Brightness_Range) * ((double)dled->Bound / dled->Bound_Range);
	Pulse = pow(Pulse, ((double)dled->Gamma)/KDL_PWMLED_GAMMA_SCALER);
	Pulse *= dled->AutoReloadValue + 1;
	uPulse = (uint16_t)(Pulse + 0.5);

	switch (dled->Channel)
	{
	case TIM_CHANNEL_1:
	{
	  /* Check the parameters */
	  assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

	  /* Set the capture 1 value */
	  dled->Port->Instance->CCR1 =  dled->OnState?uPulse:dled->AutoReloadValue+1-uPulse;

	  break;
	}
	case TIM_CHANNEL_2:
	{
	  /* Set the parameters */
	  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

	  /* Return the capture 2 value */
	  dled->Port->Instance->CCR2 =  dled->OnState?uPulse:dled->AutoReloadValue+1-uPulse;

	  break;
	}

	case TIM_CHANNEL_3:
	{
	  /* Check the parameters */
	  assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

	  /* Set the capture 3 value */
	  dled->Port->Instance->CCR3 =  dled->OnState?uPulse:dled->AutoReloadValue+1-uPulse;

	  break;
	}

	case TIM_CHANNEL_4:
	{
	  /* Check the parameters */
	  assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

	  /* Set the capture 4 value */
	  dled->Port->Instance->CCR4 =  dled->OnState?uPulse:dled->AutoReloadValue+1-uPulse;

	  break;
	}

	default:
	  break;
	}

	return KDL_OK;
}


/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/
/**
  * @brief  Check whether the pointer valid
  * @param  dled  pointer of the PWMLED
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_Check(KDL_PWMLED_t* dled)
{
	// TODO
	if(dled == NULL)
		return KDL_ERROR;

	return dled->check == (uint32_t)dled ? KDL_OK : KDL_ERROR;
}

/**
  * @brief  Reset a valid check value
  * @param  dled  pointer of the PWMLED
  *
  * @retval KDL_State_t KDL Status
  */
KDL_State_t KDL_PWMLED_ResetCheck(KDL_PWMLED_t* dled)
{
	// TODO
	if(dled == NULL)
		return KDL_ERROR;

	dled->check = (uint32_t)dled;

	return KDL_OK;
}


/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
