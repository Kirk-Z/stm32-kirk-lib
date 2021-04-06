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
#include "i2c.h"
#include "math.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KDL_MPU6050_SELF_TESTX_REG		0X0D	//自检寄存器X
#define KDL_MPU6050_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define KDL_MPU6050_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define KDL_MPU6050_SELF_TESTA_REG		0X10	//自检寄存器A
#define KDL_MPU6050_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define KDL_MPU6050_CFG_REG			0X1A	//配置寄存器
#define KDL_MPU6050_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define KDL_MPU6050_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define KDL_MPU6050_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define KDL_MPU6050_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define KDL_MPU6050_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define KDL_MPU6050_I2CSLV0_ADDR_REG	        0X25	//IIC从机0器件地址寄存器
#define KDL_MPU6050_I2CSLV0_REG			0X26	//IIC从机0数据地址寄存器
#define KDL_MPU6050_I2CSLV0_CTRL_REG	        0X27	//IIC从机0控制寄存器
#define KDL_MPU6050_I2CSLV1_ADDR_REG	        0X28	//IIC从机1器件地址寄存器
#define KDL_MPU6050_I2CSLV1_REG			0X29	//IIC从机1数据地址寄存器
#define KDL_MPU6050_I2CSLV1_CTRL_REG	        0X2A	//IIC从机1控制寄存器
#define KDL_MPU6050_I2CSLV2_ADDR_REG	        0X2B	//IIC从机2器件地址寄存器
#define KDL_MPU6050_I2CSLV2_REG			0X2C	//IIC从机2数据地址寄存器
#define KDL_MPU6050_I2CSLV2_CTRL_REG	        0X2D	//IIC从机2控制寄存器
#define KDL_MPU6050_I2CSLV3_ADDR_REG	        0X2E	//IIC从机3器件地址寄存器
#define KDL_MPU6050_I2CSLV3_REG			0X2F	//IIC从机3数据地址寄存器
#define KDL_MPU6050_I2CSLV3_CTRL_REG	        0X30	//IIC从机3控制寄存器
#define KDL_MPU6050_I2CSLV4_ADDR_REG	        0X31	//IIC从机4器件地址寄存器
#define KDL_MPU6050_I2CSLV4_REG			0X32	//IIC从机4数据地址寄存器
#define KDL_MPU6050_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define KDL_MPU6050_I2CSLV4_CTRL_REG	        0X34	//IIC从机4控制寄存器
#define KDL_MPU6050_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define KDL_MPU6050_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define KDL_MPU6050_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define KDL_MPU6050_INT_EN_REG			0X38	//中断使能寄存器
#define KDL_MPU6050_INT_STA_REG			0X3A	//中断状态寄存器

#define KDL_MPU6050_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define KDL_MPU6050_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define KDL_MPU6050_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define KDL_MPU6050_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define KDL_MPU6050_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define KDL_MPU6050_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define KDL_MPU6050_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define KDL_MPU6050_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define KDL_MPU6050_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define KDL_MPU6050_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define KDL_MPU6050_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define KDL_MPU6050_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define KDL_MPU6050_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define KDL_MPU6050_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define KDL_MPU6050_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define KDL_MPU6050_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define KDL_MPU6050_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define KDL_MPU6050_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define KDL_MPU6050_I2CMST_DELAY_REG	        0X67	//IIC主机延时管理寄存器
#define KDL_MPU6050_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define KDL_MPU6050_MDETECT_CTRL_REG	        0X69	//运动检测控制寄存器
#define KDL_MPU6050_USER_CTRL_REG		0X6A	//用户控制寄存器
#define KDL_MPU6050_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define KDL_MPU6050_PWR_MGMT2_REG		0X6C	//电源管理寄存器2
#define KDL_MPU6050_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define KDL_MPU6050_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define KDL_MPU6050_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define KDL_MPU6050_DEVICE_ID_REG		0X75	//器件ID寄存器,who am i寄存器

/* Private macro -------------------------------------------------------------*/
#define KDL_MPU6050_ACC_PRECISION ((double)16384)	// 65536 / 4g
#define KDL_MPU6050_GYR_PRECISION ((double)16.384)  // 65536 / 4000

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
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_PWR_MGMT1_REG,0X00);	//唤醒MPU6050
	KDL_MPU6050_Set_Gyro_Fsr(kmpu, 3);					//陀螺仪传感器,±2000dps
	KDL_MPU6050_Set_Accel_Fsr(kmpu, 0);					//加速度传感器,±2g
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_INT_EN_REG,0X00);	//关闭所有中断
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_USER_CTRL_REG,0X00);	//I2C主模式关闭
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_FIFO_EN_REG,0X00);	//关闭FIFO
	KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=KDL_MPU6050_Read_Byte(kmpu, KDL_MPU6050_DEVICE_ID_REG);
	res <<= 1;
	if(res==addr)//器件ID正确
	{
		KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		KDL_MPU6050_Write_Byte(kmpu, KDL_MPU6050_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
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
  HAL_Delay(1);

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
  HAL_Delay(1);

}
//IIC写一个字节
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
void KDL_MPU6050_Write_Byte(KDL_MPU6050_t *kmpu, uint8_t reg,uint8_t data)
{
  HAL_I2C_Mem_Write(kmpu->hi2c, kmpu->address+1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xfff);
  HAL_Delay(1);
}
//IIC读一个字节
//reg:寄存器地址
//返回值:读到的数据
uint8_t KDL_MPU6050_Read_Byte(KDL_MPU6050_t *kmpu, uint8_t reg)
{
  unsigned char data=0;

  HAL_I2C_Mem_Read(kmpu->hi2c, kmpu->address, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xfff);
  HAL_Delay(1);

  return data;
}

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
