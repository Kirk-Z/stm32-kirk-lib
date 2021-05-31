/**
  ******************************************************************************
  * @date    Jan 16, 2021
  * @file    kdl_bh1750.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Brightness module BH1750 source file
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
#include "kdl_bh1750.h"

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
 * @param  dbh  pointer to BH1750
 * @param  hi2c  handler of I2C port
 * @param  address  the slave address
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_BH1750_Init(KDL_BH1750_t* dbh, I2C_HandleTypeDef *hi2c, uint16_t address)
{
	uint8_t t_Data = 0x01;

	/* Invalid pointer */
	if(dbh == NULL)
		return KDL_ERROR;
	if(hi2c == NULL)
		return KDL_ERROR;
	if(!IS_I2C_ALL_INSTANCE(hi2c->Instance))
		return KDL_ERROR;

	/* Record basic parameters */
	dbh->hi2c = hi2c;
	dbh->i2c = hi2c->Instance;
	dbh->address = address;

	/* Configuration */
	HAL_I2C_Master_Transmit(dbh->hi2c, dbh->address, &t_Data, 1, 0x1FFFF);

	/* Reset check */
	KDL_BH1750_ResetCheck(dbh);

	return KDL_OK;
}
/* Configuration functions ****************************************************/
/**
 * @brief start preparing data
 * @param  dbh  pointer to BH1750
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_BH1750_Start(KDL_BH1750_t* dbh)
{
	uint8_t t_Data = 0x10;

	/* Check pointer valid */
	if(!__IS_KDL_BH1750_T(dbh))
	{
		return KDL_ERROR;
	}

	/* Configure */
	HAL_I2C_Master_Transmit(dbh->hi2c, dbh->address, &t_Data, 1, 0xff);

	return KDL_OK;
}

/*
 * @brief  get and convert data from BH1750
 * @param  dbh  pointer to BH1750
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_BH1750_Convert(KDL_BH1750_t *dbh)
{
	uint16_t result;
	uint8_t BUF[2];

	/* Check pointer valid */
	if(!__IS_KDL_BH1750_T(dbh))
	{
		return KDL_ERROR;
	}

	/* Start preparation */
	KDL_BH1750_Start(dbh);
	/* Wait 180ms for preparation */
	HAL_Delay(180);
	/* Receive data */
	HAL_I2C_Master_Receive(dbh->hi2c, dbh->address+1, BUF, 2, 0xff);

	/* Convert result */
	result=BUF[0];
	result=(result<<8)+BUF[1];
	dbh->luminance = result;

	return KDL_OK;
}

/* IO operation functions *****************************************************/
/**
 * @brief get raw luminance data
 * @param  dbh  pointer to BH1750
 *
 * @retval  uint16_t raw data
 */
uint16_t KDL_BH1750_GetLum(KDL_BH1750_t* dbh)
{
	/* Check pointer valid */
	if(!__IS_KDL_BH1750_T(dbh))
	{
		return 0xFFFF;
	}
	return dbh->luminance;
}

/**
 * @brief get luminance in float
 * @param  dbh  pointer to BH1750
 *
 * @retva float the result
 */
float KDL_BH1750_fGetLum(KDL_BH1750_t* dbh)
{
	return (float)KDL_BH1750_GetLum(dbh)/1.2;
}
/* State and Error functions **************************************************/
/**
 * @brief  check whether pointer valid
 * @param  dbh  pointer to BH1750
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_BH1750_Check(KDL_BH1750_t* dbh)
{
	/* Check pointer valid */
	if(dbh == NULL)
		return KDL_ERROR;
	if(dbh->check == 0)
		return KDL_ERROR;
	return KDL_OK;
}

/**
 * @brief  reset the check value
 * @param  dbh  pointer to BH1750
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_BH1750_ResetCheck(KDL_BH1750_t* dbh)
{
	if(dbh == NULL)
		return KDL_ERROR;
	dbh->check = 1;
	return KDL_OK;
}
/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
