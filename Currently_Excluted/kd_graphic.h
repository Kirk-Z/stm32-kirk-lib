/**
  ******************************************************************************
  * @date    Jan 15, 2021
  * @file    kd_graphic.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Graphic memory header
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KD_GRAPHIC_H_
#define KD_GRAPHIC_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kd.h"
/* Exported types ------------------------------------------------------------*/
 typedef enum {
 	KD_GRAPHIC_MODE_BIT,
 	KD_GRAPHIC_MODE_BYTE,

 	KD_GRAPHIC_MODE_RGB222,
 	KD_GRAPHIC_MODE_RGB565,
 	KD_GRAPHIC_MODE_RGB888,

 	KD_GRAPHIC_MODE_ARGB2222,
 	KD_GRAPHIC_MODE_ARGB1555,
 	KD_GRAPHIC_MODE_ARGB8888,

 	// TODO More color modes.
 } KD_Graphic_Mode_t;

typedef struct _KD_Graphic_t {
	uint8_t *buffer;
	uint16_t width;
	uint16_t height;
	uint8_t symmstate;

	KD_Graphic_Mode_t mode;

} KD_Graphic_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/* Initialization and de-initialization functions *****************************/
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
KD_State_t KD_Graphic_DrawPoint(int16_t Horizontal, int16_t Vertical, uint32_t Color);
KD_State_t KD_Graphic_DrawLine(int16_t HorizontalStart, int16_t VerticalStart, int16_t HorizontalEnd, int16_t VerticalEnd, uint32_t Color);
KD_State_t KD_Graphic_DrawVector(int16_t Horizontal, int16_t Vertical, int16_t Angle, int16_t Length, uint32_t Color);
KD_State_t KD_Graphic_DrawArc(int16_t CenterHorizontal, int16_t CenterVertical, int16_t AngleStart, int16_t AngleEnd, int16_t Radius, uint32_t Color);
KD_State_t KD_Graphic_DrawCross(int16_t Horizontal, int16_t Vertical, int16_t Angle, int16_t Length, uint32_t Color);

KD_State_t KD_Graphic_DrawCircle(int16_t CenterHorizontal, int16_t CenterVertical, int16_t Radius, uint32_t Color);
KD_State_t KD_Graphic_FillCircle(int16_t CenterHorizontal, int16_t CenterVertical, int16_t Radius, uint32_t Color);
KD_State_t KD_Graphic_DrawIncircle(int16_t HorizontalStart, int16_t VerticalStart, int16_t HorizontalEnd, int16_t VerticalEnd, uint32_t Color);
KD_State_t KD_Graphic_FillIncircle(int16_t HorizontalStart, int16_t VerticalStart, int16_t HorizontalEnd, int16_t VerticalEnd, uint32_t Color);
KD_State_t KD_Graphic_DrawEllipse(int16_t Focus1Horizontal, int16_t Focus1Vertical, int16_t Focus2Horizontal, int16_t Focus2Vertical, int16_t LongAxis, uint32_t Color);
KD_State_t KD_Graphic_FillEllipse(int16_t Focus1Horizontal, int16_t Focus1Vertical, int16_t Focus2Horizontal, int16_t Focus2Vertical, int16_t LongAxis, uint32_t Color);

KD_State_t KD_Graphic_DrawRect(int16_t HorizontalStart, int16_t VerticalStart, int16_t HorizontalEnd, int16_t VerticalEnd, uint32_t Color);
KD_State_t KD_Graphic_FillRect(int16_t HorizontalStart, int16_t VerticalStart, int16_t HorizontalEnd, int16_t VerticalEnd, uint32_t Color);
KD_State_t KD_Graphic_DrawSquare(int16_t CenterHorizontal, int16_t CenterVertical, uint32_t color);
KD_State_t KD_Graphic_FillSquare(int16_t CenterHorizontal, int16_t CenterVertical, uint32_t color);
/* State and Error functions **************************************************/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KD_GRAPHIC_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
