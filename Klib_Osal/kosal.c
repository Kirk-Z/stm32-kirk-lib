/**
  ******************************************************************************
  * @date    May 3, 2021
  * @file    kosal.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Klib operation system abstract layer source file
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
#include "kosal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
Kosal_Printf_Return_t Kosal_VsPrintf(char *buf, int limit, const char *fmt, va_list args)
{
	/* Needs to convert to self written codes since it cannot handle arguments properly */
	Kosal_Printf_Return_t res;
	res.counts = sprintf(buf, fmt, args);
	res.length = strlen(buf);
	return res;
}
/* Peripheral State and Error functions ***************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
