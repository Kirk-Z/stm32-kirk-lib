/**
  ******************************************************************************
  * @date    Feb 22, 2021
  * @file    kdl_osbutton.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Template header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention  FreeRTOS needed
  *
  ******************************************************************************
  */
/**
 * Problems to be solved
 *
 * The first timer (release/press) needed to be started in interrupt callback
 * How to do this in FreeRTOS?
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_OSBUTTON_H_
#define KDL_OSBUTTON_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"

#include "cmsis_os.h"
#include "cmsis_os2.h"
/* Exported types ------------------------------------------------------------*/
typedef struct _KDL_OSButton_Event_t {
	uint32_t ticks;
	void(*event)(void* argument);
	osTimerAttr_t attr;
	osTimerId_t id;
} KDL_OSButton_Event_t;

typedef struct _KDL_OSButton_t {
	KDL_OSButton_Event_t press;
	KDL_OSButton_Event_t offset;
	KDL_OSButton_Event_t period;
	KDL_OSButton_Event_t release;

	uint32_t state;

	uint32_t check;
} KDL_OSButton_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_OSBUTTON_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
