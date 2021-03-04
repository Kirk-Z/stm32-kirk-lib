/**
  ******************************************************************************
  * @date    Feb 23, 2021
  * @file    kdl_nrf24l01.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Template source file
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
#include "kdl_nrf24l01.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
KDL_State_t KDL_NRF24L01_Init(KDL_NRF24L01_t* knrf, SPI_HandleTypeDef* hspi, uint16_t size)
{
	knrf->hspi = hspi;
	knrf->size = size;

	return KDL_OK;
}

KDL_State_t KDL_NRF24L01_Setup(KDL_NRF24L01_t* knrf)
{
	KDL_Pin_Reset(&knrf->ce);
	KDL_Pin_Set(&knrf->csn);

	return KDL_OK;
}

/* Configuration functions ****************************************************/
KDL_State_t KDL_NRF24L01_PinConfig(KDL_NRF24L01_t* knrf, KDL_NRF24L01_Const_t index, GPIO_TypeDef* port, uint16_t pin)
{
	if(index == KDL_NRF24L01_PIN_CE)
	{
		return KDL_Pin_Init(&knrf->ce, port, pin);
	}
	if(index == KDL_NRF24L01_PIN_CSN)
	{
		return KDL_Pin_Init(&knrf->csn, port, pin);
	}
	if(index == KDL_NRF24L01_PIN_IRQ)
	{
		return KDL_Pin_Init(&knrf->irq, port, pin);
	}

	return KDL_ERROR;
}


const uint8_t TX_ADDRESS[KDL_NRF24L01_ADDR_LENGTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const uint8_t RX_ADDRESS[KDL_NRF24L01_ADDR_LENGTH]={0x34,0x43,0x10,0x10,0x01}; //接收地址

KDL_State_t KDL_NRF24L01_SetMode(KDL_NRF24L01_t* knrf, KDL_NRF24L01_Const_t mode)
{
	if(mode == KDL_NRF24L01_RX_MODE)
	{
		KDL_Pin_Reset(&knrf->ce);
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_CONFIG, 0x0F);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_EN_AA,0x01);    //使能通道0的自动应答
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_EN_RXADDR,0x01);//使能通道0的接收地址
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_RF_CH,40);	     //设置RF通信频率
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启

		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_RX_PW_P0,knrf->size);//选择通道0的有效数据宽度

		KDL_NRF24L01_Write_Buf(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_RX_ADDR_P0,(uint8_t*)RX_ADDRESS,KDL_NRF24L01_ADDR_LENGTH);//写RX节点地址

		KDL_Pin_Set(&knrf->ce); //CE为高,进入接收模式
		// HAL_Delay(1);
		return KDL_OK;
	}
	if(mode == KDL_NRF24L01_TX_MODE)
	{
		KDL_Pin_Reset(&knrf->ce);
		KDL_NRF24L01_Write_Buf(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_TX_ADDR,(uint8_t*)TX_ADDRESS,KDL_NRF24L01_ADDR_LENGTH);//写TX节点地址
		KDL_NRF24L01_Write_Buf(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_RX_ADDR_P0,(uint8_t*)RX_ADDRESS,KDL_NRF24L01_ADDR_LENGTH); //设置TX节点地址,主要为了使能ACK

		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_EN_AA,0x01);     //使能通道0的自动应答
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_EN_RXADDR,0x01); //使能通道0的接收地址
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_SETUP_RETR,0xff);//设置自动重发间隔时间:4000us + 86us;最大自动重发次数:15次
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_RF_CH,40);       //设置RF通道为40
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
		KDL_Pin_Set(&knrf->ce);//CE为高,10us后启动发送
		// HAL_Delay(1);
	}
	return KDL_ERROR;
}


KDL_State_t KDL_NRF24L01_SetTXAddr(KDL_NRF24L01_t* knrf, uint8_t* addr)
{

	return KDL_OK;
}

KDL_State_t KDL_NRF24L01_SetRXAddr(KDL_NRF24L01_t* knrf, uint8_t index, uint8_t* addr)
{

	return KDL_OK;
}

/* IO operation functions *****************************************************/
uint8_t KDL_NRF24L01_ReadWriteSPI(KDL_NRF24L01_t* knrf, uint8_t byte)
{
	uint8_t d_read, d_send=byte;
	if(HAL_SPI_TransmitReceive(knrf->hspi, &d_send, &d_read, 1, 0x1FFF)!=HAL_OK)
	{
		d_read = 0xFF;
	}
	return d_read;
}

KDL_State_t KDL_NRF24L01_Write_Buf(KDL_NRF24L01_t* knrf, uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status, uint8_t_ctr;
	KDL_Pin_Reset(&knrf->csn);          //使能SPI传输
	status = KDL_NRF24L01_ReadWriteSPI(knrf, reg);//发送寄存器值(位置),并读取状态值
	for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
	{
		KDL_NRF24L01_ReadWriteSPI(knrf, *pBuf++); //写入数据
	}
	KDL_Pin_Set(&knrf->csn);       //关闭SPI传输
	return status;          //返回读到的状态值

	return KDL_OK;
}

KDL_State_t KDL_NRF24L01_Read_Buf(KDL_NRF24L01_t* knrf, uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	static uint8_t status, uint8_t_ctr;

	KDL_Pin_Reset(&knrf->csn);           //使能SPI传输
	status=KDL_NRF24L01_ReadWriteSPI(knrf, reg);//发送寄存器值(位置),并读取状态值
	for(uint8_t_ctr=0;uint8_t_ctr<len;uint8_t_ctr++)
	{
		pBuf[uint8_t_ctr]=KDL_NRF24L01_ReadWriteSPI(knrf, 0XFF);//读出数据
	}
	KDL_Pin_Set(&knrf->csn);       //关闭SPI传输
	return status;        //返回读到的状态值
	return KDL_OK;
}

KDL_State_t KDL_NRF24L01_Read_Reg(KDL_NRF24L01_t* knrf, uint8_t reg)
{
	uint8_t reg_val;
 	KDL_Pin_Reset(&knrf->csn);          //使能SPI传输
  KDL_NRF24L01_ReadWriteSPI(knrf, reg);   //发送寄存器号
  reg_val=KDL_NRF24L01_ReadWriteSPI(knrf, 0XFF);//读取寄存器内容
  KDL_Pin_Set(&knrf->csn);          //禁止SPI传输
  return(reg_val);           //返回状态值

	return KDL_OK;
}

uint8_t KDL_NRF24L01_Write_Reg(KDL_NRF24L01_t* knrf, uint8_t reg, uint8_t value)
{
	static uint8_t status;

	KDL_Pin_Reset(&knrf->csn);
	// KDL_Pin_Reset(&knrf->csn);

	status = KDL_NRF24L01_ReadWriteSPI(knrf, reg);
	KDL_NRF24L01_ReadWriteSPI(knrf, value);

	KDL_Pin_Set(&knrf->csn);
	// KDL_Pin_Set(&knrf->csn);

	return(status);
}


KDL_State_t KDL_NRF24L01_TxPacket(KDL_NRF24L01_t* knrf, uint8_t *txbuf)
{
	uint8_t sta;

	KDL_Pin_Reset(&knrf->ce);
  KDL_NRF24L01_Write_Buf(knrf, KDL_NRF24L01_WR_TX_PLOAD,txbuf,knrf->size);//写数据到TX BUF  32个字节
 	KDL_Pin_Set(&knrf->ce);//启动发送

	while(KDL_Pin_Read(&knrf->irq)!=0);//等待发送完成

	sta=KDL_NRF24L01_Read_Reg(knrf, KDL_NRF24L01_REG_STATUS);  //读取状态寄存器的值
	KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&KDL_NRF24L01_MAX_TX)//达到最大重发次数
	{
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_FLUSH_TX,0xff);//清除TX FIFO寄存器
		return KDL_NRF24L01_MAX_TX;
	}
	if(sta&KDL_NRF24L01_TX_OK)//发送完成
	{
		return KDL_OK;
	}
	return KDL_ERROR;//其他原因发送失败
}

KDL_State_t KDL_NRF24L01_RxPacket(KDL_NRF24L01_t* knrf, uint8_t *rxbuf)
{
	uint8_t sta;

	sta=KDL_NRF24L01_Read_Reg(knrf, KDL_NRF24L01_REG_STATUS);  //读取状态寄存器的值
	KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&KDL_NRF24L01_RX_OK)//接收到数据
	{
		KDL_NRF24L01_Read_Buf(knrf, KDL_NRF24L01_RD_RX_PLOAD,rxbuf,knrf->size);//读取数据
		KDL_NRF24L01_Write_Reg(knrf, KDL_NRF24L01_FLUSH_RX,0xff);//清除RX FIFO寄存器
		return KDL_OK;
	}

	return KDL_ERROR;
}
/* State and Error functions **************************************************/
KDL_State_t KDL_NRF24L01_Check(KDL_NRF24L01_t* knrf)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;

	KDL_NRF24L01_Write_Buf(knrf, KDL_NRF24L01_WRITE_REG+KDL_NRF24L01_REG_TX_ADDR, buf, 5);
	KDL_NRF24L01_Read_Buf(knrf, KDL_NRF24L01_REG_TX_ADDR, buf, 5);

	for(i=0;i<5;i++)
		if(buf[i]!=0XA5)
			break;

	if(i!=5)
		return KDL_ERROR;
	return KDL_OK;
}

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
