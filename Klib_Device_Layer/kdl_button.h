/**
  ******************************************************************************
  * @date    Jan 17, 2021
  * @file    kdl_button.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Button header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_BUTTON_H_
#define KDL_BUTTON_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
typedef enum {
	KDL_Button_State_Down,
	KDL_Button_State_CriticalDown,
	KDL_Button_State_ContinueDown,
	KDL_Button_State_Up,
	KDL_Button_State_CriticalUp,
	KDL_Button_State_ContinueUp
} KDL_Button_State_t;

typedef struct _KDL_Button_Event_t {
  void(*event)(void* arguments);
  uint32_t offset;
  uint32_t period;
} KDL_Button_Event_t;

typedef struct _KDL_Button_t {
  /* Event */
  void(*press)(void* arguments);
  void(*release)(void* arguments);

  KDL_Button_State_t state;

  KDL_Button_Event_t upevent;
  KDL_Button_Event_t downevent;

  uint32_t last;

} KDL_Button_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_Button_Init(KDL_Button_t* kbt, void* press, void* release);
/* Configuration functions ****************************************************/
KDL_State_t KDL_Button_SetDownEvent(KDL_Button_t* kbt, void* down, uint32_t offset, uint32_t period);
KDL_State_t KDL_Button_DownEvent(KDL_Button_t* kbt);
KDL_State_t KDL_Button_SetUpEvent(KDL_Button_t* kbt, void* up, uint32_t offset, uint32_t period);
KDL_State_t KDL_Button_UpEvent(KDL_Button_t* kbt);
/* IO operation functions *****************************************************/
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_BUTTON_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
