/**
  ******************************************************************************
  * @date    Mar 24, 2021
  * @file    kdl_hcsr04.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   HC-SR04 ultrasonic distance module header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_HCSR04_H_
#define KDL_HCSR04_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
#include "kdl_pin.h"
/* Exported types ------------------------------------------------------------*/
typedef struct _KDL_HCSR04_t {
	KDL_Pin_t Trig, Echo;
	uint32_t Ticks;
} KDL_HCSR04_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
#define KDL_HCSR04_TimeOut 0xFFFFFU
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_HCSR04_Init(
		KDL_HCSR04_t* khcsr,
		GPIO_TypeDef* Tport,
		uint16_t Tpin,
		GPIO_TypeDef* Eport,
		uint16_t Epin
		);
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
uint32_t KDL_HCSR04_Measure(KDL_HCSR04_t* khcsr);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_HCSR04_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
