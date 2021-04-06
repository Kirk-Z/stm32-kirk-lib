/**
  ******************************************************************************
  * @date    Mar 21, 2021
  * @file    kdl_mpu6050.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   MPU6050 header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_MPU6050_H_
#define KDL_MPU6050_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
/* Exported types ------------------------------------------------------------*/
typedef struct _KDL_MPU6050_t {
	I2C_HandleTypeDef *hi2c;
	I2C_TypeDef *i2c;
	uint16_t address;

	float AccX, AccY, AccZ;
	float AccS;
	float GyrX, GyrY, GyrZ;
	float Temp;

} KDL_MPU6050_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_MPU6050_Init(KDL_MPU6050_t *kmpu, I2C_HandleTypeDef* hi2c, uint16_t addr);

/* Configuration functions ****************************************************/
void KDL_MPU6050_Set_Gyro_Fsr(KDL_MPU6050_t *kmpu, uint8_t fsr);
void KDL_MPU6050_Set_Accel_Fsr(KDL_MPU6050_t *kmpu, uint8_t fsr);
void KDL_MPU6050_Set_LPF(KDL_MPU6050_t *kmpu, uint16_t lpf);
void KDL_MPU6050_Set_Rate(KDL_MPU6050_t *kmpu, uint16_t rate);
void KDL_MPU6050_Set_Fifo(KDL_MPU6050_t *kmpu, uint8_t sens);

/* IO operation functions *****************************************************/
void KDL_MPU6050_Get_Temperature(KDL_MPU6050_t *kmpu);
void KDL_MPU6050_Get_Gyroscope(KDL_MPU6050_t *kmpu);
void KDL_MPU6050_Get_Accelerometer(KDL_MPU6050_t *kmpu);

/* State and Error functions **************************************************/
void KDL_MPU6050_Write_Len(KDL_MPU6050_t *kmpu, uint8_t reg,uint8_t len,uint8_t *buf);                           //IIC连续写
void KDL_MPU6050_Read_Len(KDL_MPU6050_t *kmpu, uint8_t reg,uint8_t len,uint8_t *buf);                         //IIC连续读
void KDL_MPU6050_Write_Byte(KDL_MPU6050_t *kmpu, uint8_t reg,uint8_t data);				//IIC写一个字节
uint8_t KDL_MPU6050_Read_Byte(KDL_MPU6050_t *kmpu, uint8_t reg);					//IIC读一个字节

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KLIB_TEMPLATE_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
