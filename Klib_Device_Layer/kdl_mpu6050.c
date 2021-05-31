/**
  ******************************************************************************
  * @date    Mar 21, 2021
  * @file    kdl_mpu6050.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   MPU6050 source file
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
#include "kdl_mpu6050.h"
#include "math.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_MPU6050_Init(KDL_MPU6050_t *kmpu, I2C_HandleTypeDef* hi2c, uint16_t addr)
{
  uint8_t res;

	if(kmpu == NULL)
		return KDL_ERROR;
	if(hi2c == NULL)
		return KDL_ERROR;
	if(!IS_I2C_ALL_INSTANCE(hi2c->Instance))
		return KDL_ERROR;

	/* Pass parameter */
	kmpu->hi2c = hi2c;
	kmpu->i2c = hi2c->Instance;
	kmpu->address = addr;

	/* Initialize */
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_PWR_MGMT1_REG,0X80);	//复位MPU6050
	HAL_Delay(1);
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_PWR_MGMT1_REG,0X00);	//唤醒MPU6050
	HAL_Delay(1);
	KDL_MPU6050_Set_Gyro_Fsr(kmpu, 3);					//陀螺仪传感器,±2000dps
	HAL_Delay(1);
	KDL_MPU6050_Set_Accel_Fsr(kmpu, 0);					//加速度传感器,±2g
	HAL_Delay(1);
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_INT_EN_REG,0X00);	//关闭所有中断
	HAL_Delay(1);
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_USER_CTRL_REG,0X00);	//I2C主模式关闭
	HAL_Delay(1);
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_FIFO_EN_REG,0X00);	//关闭FIFO
	HAL_Delay(1);
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	HAL_Delay(1);
	res=KDL_MPU6050_Read_Byte(kmpu, KDL_MPU6050_DEVICE_ID_REG);
	HAL_Delay(1);
	res <<= 1;
	if(res==addr)//器件ID正确
	{
		KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		HAL_Delay(1);
		KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		HAL_Delay(1);
		KDL_MPU6050_Set_Rate(kmpu, 1000);//Set the sampling frequency to 1KHz
		return KDL_OK;
	}
	return KDL_ERROR;
}
/* Configuration functions ****************************************************/
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
void KDL_MPU6050_Set_Gyro_Fsr(KDL_MPU6050_t *kmpu, uint8_t fsr)
{
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
void KDL_MPU6050_Set_Accel_Fsr(KDL_MPU6050_t *kmpu, uint8_t fsr)
{
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
void KDL_MPU6050_Set_LPF(KDL_MPU6050_t *kmpu, uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_CFG_REG,data);//设置数字低通滤波器
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
void KDL_MPU6050_Set_Rate(KDL_MPU6050_t *kmpu, uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	KDL_MPU6050_Set_LPF(kmpu, rate/2);	//自动设置LPF为采样率的一半
}

/* IO operation functions *****************************************************/
//得到温度值
//返回值:温度值(扩大了100倍)
void KDL_MPU6050_Get_Temperature(KDL_MPU6050_t *kmpu)
{
  unsigned char  buf[2];
  short raw;
  double temp;

  KDL_MPU6050_Read_Len(kmpu, KDL_MPU6050_TEMP_OUTH_REG,2,buf);
  raw = (buf[0]<<8)| buf[1];
  temp = ((double)36.53+((double)raw)/340)*100;
  //  temp = (long)((35 + (raw / 340)) * 65536L);
  kmpu->Temp = temp / 100;
  ///return temp/100.0f;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
void KDL_MPU6050_Get_Gyroscope(KDL_MPU6050_t *kmpu)
{
	int16_t gx, gy, gz;
    uint8_t buf[6],res;
	//res=KDL_MPU6050_Read_Len(KDL_MPU6050_GYRO_XOUTH_REG,6,buf);
	res = HAL_I2C_Mem_Read(kmpu->hi2c, kmpu->address, KDL_MPU6050_GYRO_XOUTH_REG, I2C_MEMADD_SIZE_8BIT, buf, 6, 0xfff);
	if(res==0)
	{
		gx=((uint16_t)buf[0]<<8)|buf[1];
		gy=((uint16_t)buf[2]<<8)|buf[3];
		gz=((uint16_t)buf[4]<<8)|buf[5];
	}

	kmpu->GyrX = (double)gx / KDL_MPU6050_GYR_PRECISION;
	kmpu->GyrY = (double)gy / KDL_MPU6050_GYR_PRECISION;
	kmpu->GyrZ = (double)gz / KDL_MPU6050_GYR_PRECISION;

}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
void KDL_MPU6050_Get_Accelerometer(KDL_MPU6050_t *kmpu)
{
    int16_t ax,ay, az;
    uint8_t buf[6],res;
	//res=KDL_MPU6050_Read_Len(KDL_MPU6050_ACCEL_XOUTH_REG,6,buf);
	res = HAL_I2C_Mem_Read(kmpu->hi2c, kmpu->address, KDL_MPU6050_ACCEL_XOUTH_REG, I2C_MEMADD_SIZE_8BIT, buf, 6, 0xfff);
	if(res==0)
	{
		ax=((uint16_t)buf[0]<<8)|buf[1];
		ay=((uint16_t)buf[2]<<8)|buf[3];
		az=((uint16_t)buf[4]<<8)|buf[5];
	}

	kmpu->AccX = (double)ax / KDL_MPU6050_ACC_PRECISION;
	kmpu->AccY = (double)ay / KDL_MPU6050_ACC_PRECISION;
	kmpu->AccZ = (double)az / KDL_MPU6050_ACC_PRECISION;

	kmpu->AccS = sqrt(kmpu->AccX*kmpu->AccX + kmpu->AccY*kmpu->AccY + kmpu->AccZ*kmpu->AccZ);
}

/* State and Error functions **************************************************/
//IIC连续写
void KDL_MPU6050_Write_Len(KDL_MPU6050_t *kmpu, uint8_t reg,uint8_t len,uint8_t *buf)
{
  HAL_I2C_Mem_Write(kmpu->hi2c, kmpu->address+1, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  //HAL_Delay(1);

}
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
void KDL_MPU6050_Read_Len(KDL_MPU6050_t *kmpu, uint8_t reg,uint8_t len,uint8_t *buf)
{
  HAL_I2C_Mem_Read(kmpu->hi2c, kmpu->address, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  //HAL_Delay(1);

}
//IIC写一个字节
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
void KDL_MPU6050_Write_Byte(KDL_MPU6050_t *kmpu, uint8_t reg,uint8_t data)
{
  HAL_I2C_Mem_Write(kmpu->hi2c, kmpu->address+1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xfff);
  //HAL_Delay(1);
}
//IIC读一个字节
//reg:寄存器地址
//返回值:读到的数据
uint8_t KDL_MPU6050_Read_Byte(KDL_MPU6050_t *kmpu, uint8_t reg)
{
  unsigned char data=0;

  HAL_I2C_Mem_Read(kmpu->hi2c, kmpu->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xfff);
  //HAL_Delay(1);

  return data;
}

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
