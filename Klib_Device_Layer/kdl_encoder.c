/**
    ******************************************************************************
    * @date    Mar 7, 2021
    * @file    kdl_encoder.c
    * @author  Kirk_Z
    * @name    Kefan Zheng
    * @brief   Template source file
    * @version V0.0.0
    * @email   kirk_z@yeah.net
    @note
    ==============================================================================
                                #### Template timer configuration ####
    ==============================================================================

    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 0;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 65535;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 10;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 10;
    if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

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
#include "kdl_encoder.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Encoder_Init(KDL_Encoder_t* decd, TIM_HandleTypeDef *port)
{
    decd->Port = port;
    decd->Position = 0;
    decd->Velocity = 0;
    decd->Tim = port->Instance;

    return KDL_OK;
}
/* Configuration functions ****************************************************/
KDL_State_t KDL_Encoder_Clear(KDL_Encoder_t* decd)
{
    decd->Position = 0;
    decd->Tim->CNT = 0;
    decd->Velocity = 0;

    return KDL_OK;
}

KDL_State_t KDL_Encoder_Start(KDL_Encoder_t* decd)
{
    HAL_TIM_Encoder_Start(decd->Port, TIM_CHANNEL_1|TIM_CHANNEL_2);

    decd->Last = HAL_GetTick();

    return KDL_OK;
}

KDL_State_t KDL_Encoder_Stop(KDL_Encoder_t* decd)
{
    HAL_TIM_Encoder_Stop(decd->Port, TIM_CHANNEL_1|TIM_CHANNEL_2);

    KDL_Encoder_Convert(decd);

    return KDL_OK;
}


KDL_State_t KDL_Encoder_Convert(KDL_Encoder_t* decd)
{
    uint32_t time;
    int16_t tmp;

    tmp = decd->Tim->CNT;
    decd->Tim->CNT = 0;

    time = HAL_GetTick();

    decd->Position += tmp;
    decd->Velocity = tmp;
    decd->Velocity <<= 16;
    decd->Velocity /= (int16_t)(time - decd->Last);

    decd->Last = time;

    return KDL_OK;
}

/* IO operation functions *****************************************************/
int32_t KDL_Encoder_GetPosition(KDL_Encoder_t* decd)
{
    return decd->Position;
}

int32_t KDL_Encoder_GetVelocity(KDL_Encoder_t* decd)
{
    return decd->Velocity;
}

/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
