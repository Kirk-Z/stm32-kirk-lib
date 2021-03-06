/**
  ******************************************************************************
  * @date    Mar 7, 2021
  * @file    kd_floatpid.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   PID control in floating numbers
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
#include "kd_floatpid.h"
#include "math.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KD_State_t KD_FloatPID_Init(KD_FloatPID_t* kpid, void* downlink)
{
  if(downlink != NULL)
  {
    kpid->Downlink = downlink;
  }

  kpid->Result = 0;
  kpid->Denominator = 1;

  return KD_OK;
}
/* Configuration functions ****************************************************/
KD_State_t KD_FloatPID_SetIndex(KD_FloatPID_t* kpid, KD_FloatPID_Const_t set, float index)
{
  if(set == KD_FloatPID_IndexP)
  {
    kpid->indexP = index;
    return KD_OK;
  }
  if(set == KD_FloatPID_IndexI)
  {
    kpid->indexI = index;
    return KD_OK;
  }
  if(set == KD_FloatPID_IndexD)
  {
    kpid->indexD = index;
    return KD_OK;
  }
  if(set == KD_FloatPID_Target)
  {
    kpid->Target = index;
    return KD_OK;
  }
  if(set == KD_FloatPID_Tolerance)
  {
    kpid->Tolerance = index;
    return KD_OK;
  }
  if(set == KD_FloatPID_Denominator)
  {
    kpid->Denominator = index;
    return KD_OK;
  }
  return KD_ERROR;
}
/* IO operation functions *****************************************************/
KD_State_t KD_FloatPID_Process(KD_FloatPID_t* kpid, float current, uint32_t interval)
{
  static float Error;
  static float Tmp;

  /* Get Error */
  Error = current - kpid->Target;
  // Error /= interval;
  if(fabs(Error) < kpid->Tolerance)
  {
	  Error = 0;
  }

  Tmp = 0;

  /* Accumulate I */
  Tmp += kpid->indexI * Error;

  /* Accumulate P */
  Tmp += kpid->indexP * (Error - kpid->Error0);

  /* Accumulate D */
  Tmp += kpid->indexD * (Error + kpid->Error1 - kpid->Error0 - kpid->Error0);

  /* Switch Buffer */
  kpid->Error1 = kpid->Error0;
  kpid->Error0 = Error;

  /* Inc */
  Tmp *= interval;
  if(kpid->Denominator)
	  Tmp /= kpid->Denominator;

  /* Output result */
  kpid->Increment = Tmp;
  kpid->Result += Tmp;
  kpid->Period = interval;

  /* Execute downlink */
  if(kpid->Downlink != NULL)
	  kpid->Downlink(kpid);

  return KD_OK;
}
/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
