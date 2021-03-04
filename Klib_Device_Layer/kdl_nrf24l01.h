/**
  ******************************************************************************
  * @date    Feb 23, 2021
  * @file    kdl_nrf24l01.h
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Template header file
  * @version V0.0.0
  * @email   kirk_z@yeah.net
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KDL_NRF24L01_H_
#define KDL_NRF24L01_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "kdl.h"
#include "kdl_pin.h"
/* Exported types ------------------------------------------------------------*/
#ifndef HAL_SPI_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#endif /* HAL_SPI_MODULE_ENABLED */
typedef struct _KDL_NRF24L01_t {
	SPI_HandleTypeDef *hspi;

	KDL_Pin_t ce;
	KDL_Pin_t csn;
	KDL_Pin_t irq;

	uint16_t size;


} KDL_NRF24L01_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
typedef enum {
	KDL_NRF24L01_REG_CONFIG = 0x00,
	KDL_NRF24L01_REG_EN_AA = 0x01,
	KDL_NRF24L01_REG_EN_RXADDR = 0x02,
	KDL_NRF24L01_REG_SETUP_AW = 0x03,
	KDL_NRF24L01_REG_SETUP_RETR = 0x04,
	KDL_NRF24L01_REG_RF_CH = 0x05,
	KDL_NRF24L01_REG_RF_SETUP = 0x06,
	KDL_NRF24L01_REG_STATUS = 0x07,
	KDL_NRF24L01_REG_OBSERVE_TX = 0x08,
	KDL_NRF24L01_REG_CD = 0x09,
	KDL_NRF24L01_REG_RX_ADDR_P0 = 0x0A,
	KDL_NRF24L01_REG_RX_ADDR_P1 = 0x0B,
	KDL_NRF24L01_REG_RX_ADDR_P2 = 0x0C,
	KDL_NRF24L01_REG_RX_ADDR_P3 = 0x0D,
	KDL_NRF24L01_REG_RX_ADDR_P4 = 0x0E,
	KDL_NRF24L01_REG_RX_ADDR_P5 = 0x0F,
	KDL_NRF24L01_REG_TX_ADDR = 0x10,
	KDL_NRF24L01_REG_RX_PW_P0 = 0x11,
	KDL_NRF24L01_REG_RX_PW_P1 = 0x12,
	KDL_NRF24L01_REG_RX_PW_P2 = 0x13,
	KDL_NRF24L01_REG_RX_PW_P3 = 0x14,
	KDL_NRF24L01_REG_RX_PW_P4 = 0x15,
	KDL_NRF24L01_REG_RX_PW_P5 = 0x16,
	KDL_NRF24L01_REG_NRF_FIFO_STATUS = 0x17,

	KDL_NRF24L01_PIN_CE,
	KDL_NRF24L01_PIN_CSN,
	KDL_NRF24L01_PIN_IRQ,

	KDL_NRF24L01_TX_MODE,
	KDL_NRF24L01_RX_MODE,

	KDL_NRF24L01_RD_RX_PLOAD = 0x61,


	KDL_NRF24L01_WR_TX_PLOAD = 0xA0,


	KDL_NRF24L01_FLUSH_TX = 0xE1,
	KDL_NRF24L01_FLUSH_RX = 0xE2,
	KDL_NRF24L01_REUSE_TX_PL = 0xE3,




	KDL_NRF24L01_NOP = 0xFF
} KDL_NRF24L01_Const_t;


// NRF24L01发送接收数据宽度定义
#define KDL_NRF24L01_ADDR_LENGTH                      5U
// #define TX_ADR_WIDTH                                  5   	//5字节的地址宽度
// #define RX_ADR_WIDTH                                  5   	//5字节的地址宽度
// #define TX_PLOAD_WIDTH                                32  	//32字节的用户数据宽度
// #define RX_PLOAD_WIDTH                                32  	//32字节的用户数据宽度

//NRF24L01寄存器操作命令
#define KDL_NRF24L01_READ_REG                                  0x00  //读配置寄存器,低5位为寄存器地址
#define KDL_NRF24L01_WRITE_REG                                 0x20  //写配置寄存器,低5位为寄存器地址
/*
#define RD_RX_PLOAD                                   0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD                                   0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX                                      0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX                                      0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL                                   0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP                                           0xFF  //空操作,可以用来读状态寄存器
*/
//SPI(NRF24L01)寄存器地址
/*
#define CONFIG                                        0x00  //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
														 //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA                                         0x01  //使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR                                     0x02  //接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW                                      0x03  //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR                                    0x04  //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH                                         0x05  //RF通道,bit6:0,工作通道频率;
#define RF_SETUP                                      0x06  //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS                                        0x07  //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
	*/													 //bit5:数据发送完成中断;bit6:接收数据中断;
#define KDL_NRF24L01_MAX_TX  		                                  0x10  //达到最大发送次数中断
#define KDL_NRF24L01_TX_OK   		                                  0x20  //TX发送完成中断
#define KDL_NRF24L01_RX_OK   		                                  0x40  //接收到数据中断
/*
#define OBSERVE_TX                                    0x08  //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD                                            0x09  //载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0                                    0x0A  //数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1                                    0x0B  //数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2                                    0x0C  //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3                                    0x0D  //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4                                    0x0E  //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5                                    0x0F  //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR                                       0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0                                      0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1                                      0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2                                      0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3                                      0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4                                      0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5                                      0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRF_FIFO_STATUS                               0x17  //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_NRF24L01_Init(KDL_NRF24L01_t* knrf, SPI_HandleTypeDef* hspi, uint16_t size);
KDL_State_t KDL_NRF24L01_Setup(KDL_NRF24L01_t* knrf);
/* Configuration functions ****************************************************/
KDL_State_t KDL_NRF24L01_PinConfig(KDL_NRF24L01_t* knrf, KDL_NRF24L01_Const_t index, GPIO_TypeDef* port, uint16_t pin);
KDL_State_t KDL_NRF24L01_SetMode(KDL_NRF24L01_t* knrf, KDL_NRF24L01_Const_t mode);

KDL_State_t KDL_NRF24L01_SetTXAddr(KDL_NRF24L01_t* knrf, uint8_t* addr);
KDL_State_t KDL_NRF24L01_SetRXAddr(KDL_NRF24L01_t* knrf, uint8_t index, uint8_t* addr);
/* IO operation functions *****************************************************/
uint8_t KDL_NRF24L01_ReadWriteSPI(KDL_NRF24L01_t* knrf, uint8_t byte);

KDL_State_t KDL_NRF24L01_Write_Buf(KDL_NRF24L01_t* knrf, uint8_t reg, uint8_t *pBuf, uint8_t len);
KDL_State_t KDL_NRF24L01_Read_Buf(KDL_NRF24L01_t* knrf, uint8_t reg, uint8_t *pBuf, uint8_t len);
KDL_State_t KDL_NRF24L01_Read_Reg(KDL_NRF24L01_t* knrf, uint8_t reg);
uint8_t KDL_NRF24L01_Write_Reg(KDL_NRF24L01_t* knrf, uint8_t reg, uint8_t value);

KDL_State_t KDL_NRF24L01_TxPacket(KDL_NRF24L01_t* knrf, uint8_t *txbuf);
KDL_State_t KDL_NRF24L01_RxPacket(KDL_NRF24L01_t* knrf, uint8_t *rxbuf);
/* State and Error functions **************************************************/
KDL_State_t KDL_NRF24L01_Check(KDL_NRF24L01_t* knrf);

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* KDL_NRF24L01_H_ */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
