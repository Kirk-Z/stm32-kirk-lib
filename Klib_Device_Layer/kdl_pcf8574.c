/**
  ******************************************************************************
  * @date    Feb 8, 2021
  * @file    kdl_pcf8574.c
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
#include "kdl_pcf8574.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/**
 * @brief  initialize the module
 * @param  kpcf  pointer to PCF8574
 * @param  hi2c  handler of I2C port
 * @param  address  the slave address
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_PCF8574_Init(KDL_PCF8574_t* kpcf, I2C_HandleTypeDef *hi2c, uint16_t address)
{
	if(kpcf == NULL)
		return KDL_ERROR;
	if(hi2c == NULL)
		return KDL_ERROR;
	if(!IS_I2C_ALL_INSTANCE(hi2c->Instance))
		return KDL_ERROR;

	KDL_PCF8574_ResetCheck(kpcf);

	kpcf->hi2c = hi2c;
	kpcf->i2c = hi2c->Instance;
	kpcf->address = address;

	return KDL_PCF8574_Write(kpcf, 0xFF);
}
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
/**
 * @brief  read IO
 * @param  kpcf  pointer to PCF8574
 *
 * @retval uint8_t io value
 */
uint8_t KDL_PCF8574_Read(KDL_PCF8574_t* kpcf)
{
	uint8_t data;

    if(!__IS_KDL_PCF8574_T(kpcf))
    {
    	return KDL_ERROR;
    }

	HAL_I2C_Master_Receive(kpcf->hi2c, kpcf->address, &data, 1, 0xFF);
	kpcf->io = data;
	return data;
}

/**
 * @brief  write IO
 * @param  kpcf  pointer to PCF8574
 * @param  data  value
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_PCF8574_Write(KDL_PCF8574_t* kpcf, uint8_t data)
{
    if(!__IS_KDL_PCF8574_T(kpcf))
    {
    	return KDL_ERROR;
    }

    kpcf->io = data;
	HAL_I2C_Master_Transmit(kpcf->hi2c, kpcf->address, &data, 1, 0xFF);

	return KDL_OK;
}

/**
 * @brief  read IO and set bit
 * @param  kpcf  pointer to PCF8574
 * @param  bit  bit order
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_PCF8574_Set(KDL_PCF8574_t* kpcf, uint8_t bit)
{
    if(!__IS_KDL_PCF8574_T(kpcf))
    {
    	return KDL_ERROR;
    }

    if(bit >= 8)
    	return KDL_ERROR;

    bit = 1 << bit;

	return KDL_PCF8574_Write(kpcf, KDL_PCF8574_Read(kpcf) | bit);
}

/**
 * @brief  read IO and reset bit
 * @param  kpcf  pointer to PCF8574
 * @param  bit  bit order
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_PCF8574_Reset(KDL_PCF8574_t* kpcf, uint8_t bit)
{
    if(!__IS_KDL_PCF8574_T(kpcf))
    {
    	return KDL_ERROR;
    }

    if(bit >= 8)
    	return KDL_ERROR;

    bit = 1 << bit;

	return KDL_PCF8574_Write(kpcf, KDL_PCF8574_Read(kpcf) & ~bit);
}

/**
 * @brief  read IO and toggle bit
 * @param  kpcf  pointer to PCF8574
 * @param  bit  bit order
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t PCF8574_Toggle(KDL_PCF8574_t* kpcf, uint8_t bit)
{
    if(!__IS_KDL_PCF8574_T(kpcf))
    {
    	return KDL_ERROR;
    }

    if(bit >= 8)
    	return KDL_ERROR;

    bit = 1 << bit;

	return KDL_PCF8574_Write(kpcf, KDL_PCF8574_Read(kpcf) ^ bit);
}

/**
 * @brief  read IO in bit
 * @param  kpcf  pointer to PCF8574
 * @param  bit  bit order
 *
 * @retval uint8_t high or low
 */
uint8_t KDL_PCF8574_ReadBit(KDL_PCF8574_t* kpcf, uint8_t bit)
{
    if(!__IS_KDL_PCF8574_T(kpcf))
    {
    	return KDL_ERROR;
    }

    if(bit >= 8)
    	return KDL_ERROR;

    bit = 1 << bit;

	return (KDL_PCF8574_Read(kpcf) & bit)?1:0;
}

/**
 * @brief  read IO and set bit
 * @param  kpcf  pointer to PCF8574
 * @param  bit  bit order
 * @param  data  high or low
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_PCF8574_WriteBit(KDL_PCF8574_t* kpcf, uint8_t bit, uint8_t data)
{
	if(!__IS_KDL_PCF8574_T(kpcf))
	{
		return KDL_ERROR;
	}

	if(bit >= 8)
		return KDL_ERROR;

	if(data)
		return KDL_PCF8574_Set(kpcf, (uint8_t)1 << bit);
	else
		return KDL_PCF8574_Reset(kpcf, (uint8_t)1 << bit);
}

/* State and Error functions **************************************************/
/**
 * @brief  check whether pointer valid
 * @param  dpcf  pointer to PCF8574
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_PCF8574_Check(KDL_PCF8574_t* dpcf)
{
	/* Check pointer valid */
	if(dpcf == NULL)
		return KDL_ERROR;
	if(dpcf->check == 0)
		return KDL_ERROR;
	return KDL_OK;
}

/**
 * @brief  reset the check value
 * @param  dpcf  pointer to PCF8574
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_PCF8574_ResetCheck(KDL_PCF8574_t* dpcf)
{
	/* Check pointer valid */
	if(dpcf == NULL)
		return KDL_ERROR;
	/* Reset */
	dpcf->check = 1;
	return KDL_OK;
}

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
