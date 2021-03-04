/**
  ******************************************************************************
  * @date    Jan 16, 2021
  * @file    kdl_sht30.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Temperature and humidity module SHT30 Source file
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
#include "kdl_sht30.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/**
 * @brief  polynomial for checksum
 */
const int16_t KDL_SHT30_Polynomial = 0x131;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/**
 * @brief  initialize the module
 * @param  dsht  pointer to SHT30
 * @param  hi2c  handler of I2C port
 * @param  address  the slave address
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_SHT30_Init(KDL_SHT30_t* dsht, I2C_HandleTypeDef *hi2c, uint16_t address)
{
  uint8_t SHT3X_Modecommand_Buffer[2]={0x22,0x36}; //periodic mode commands
  /* Invalid pointer */
  if(dsht == NULL)
    return KDL_ERROR;
  if(hi2c == NULL)
    return KDL_ERROR;
  if(!IS_I2C_ALL_INSTANCE(hi2c->Instance))
    return KDL_ERROR;

  /* Note the info */
  dsht->hi2c = hi2c;
  dsht->i2c = hi2c->Instance;
  dsht->address = address;

  /* Reset Check value */
  KDL_SHT30_ResetCheck(dsht);

  if(HAL_OK == HAL_I2C_Master_Transmit(hi2c, address, SHT3X_Modecommand_Buffer, 2, 0x1FFF))
    return KDL_OK;
  else
    return KDL_ERROR;
}
/* Configuration functions ****************************************************/
/**
 * @brief  convert data
 * @param  dsht  pointer to SHT30
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_SHT30_Convert(KDL_SHT30_t* dsht)
{
  static uint8_t command[2]={0xE0,0x00};								//read the measurement results
  static uint8_t buffer[6]; 																		//byte 0,1 is temperature byte 4,5 is humidity

  HAL_StatusTypeDef stat;

  /* Check pointer valid */
  if(!__IS_KDL_SHT30_T(dsht))
  {
    return KDL_ERROR;
  }

  stat = HAL_I2C_Master_Transmit(dsht->hi2c,dsht->address,command,2,0x1FFFF); //Read sht30 sensor data
  if(stat!=HAL_OK)
    return KDL_ERROR;

  stat = HAL_I2C_Master_Receive(dsht->hi2c,dsht->address+1,buffer,6,0x1FFFF);
  if(stat!=HAL_OK)
    return KDL_ERROR;

  /* Check and record temperature */
  if( KDL_OK == KDL_SHT30_CheckCRC(&buffer[0], 2, buffer[2]))
  {
    dsht->temp = ((uint16_t)buffer[0] << 8) | buffer[1];
  }

  /* Check and record humidity */
  if( KDL_OK == KDL_SHT30_CheckCRC(&buffer[3], 2, buffer[5]))
  {
    dsht->hum = ((uint16_t)buffer[3] << 8) | buffer[4];
  }

  return KDL_OK;
}
/* IO operation functions *****************************************************/
/**
 * @brief  get temperature
 * @param  dsht  pointer to SHT30
 *
 * @retval int32_t temperature multiplied by 6553600
 */
uint16_t KDL_SHT30_GetTemp(KDL_SHT30_t* dsht)
{
	/* Check pointer valid */
    if(!__IS_KDL_SHT30_T(dsht))
    {
    	return 0xFFFF;
    }

	return dsht->temp;
}

/**
 * @brief  get humidity
 * @param  dsht  pointer to SHT30
 *
 * @retval int32_t humidity multiplied by 6553600
 */
uint16_t KDL_SHT30_GetHum(KDL_SHT30_t* dsht)
{
	/* Check pointer valid */
    if(!__IS_KDL_SHT30_T(dsht))
    {
    	return 0xFFFF;
    }

	return dsht->hum;
}

/**
 * @brief  get temperature
 * @param  dsht  pointer to SHT30
 *
 * @retval float temperature
 */
float KDL_SHT30_fGetTemp(KDL_SHT30_t* dsht)
{
	float tmp;
	tmp = KDL_SHT30_GetTemp(dsht);
	tmp *= 175;
	tmp /= 65535;
	tmp -= 45;
	return (float)tmp;
}

/**
 * @brief  get humidity
 * @param  dsht  pointer to SHT30
 *
 * @retval float humidity
 */
float KDL_SHT30_fGetHum(KDL_SHT30_t* dsht)
{
	float tmp;
	tmp = KDL_SHT30_GetHum(dsht);
	tmp *= 100;
	tmp /= 65535;

	return tmp;
}

/* State and Error functions **************************************************/
/**
 * @brief  check whether pointer valid
 * @param  dsht  pointer to SHT30
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_SHT30_Check(KDL_SHT30_t* dsht)
{
	/* Check pointer valid */
	if(dsht == NULL)
		return KDL_ERROR;
	if(dsht->check == 0)
		return KDL_ERROR;
	return KDL_OK;
}

/**
 * @brief  reset the check value
 * @param  dsht  pointer to SHT30
 *
 * @retval KDL_State_t KDL Status
 */
KDL_State_t KDL_SHT30_ResetCheck(KDL_SHT30_t* dsht)
{
	/* Check pointer valid */
	if(dsht == NULL)
		return KDL_ERROR;
	/* Reset */
	dsht->check = 1;
	return KDL_OK;
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  check crc for result
 * @param  data  array of data
 * @param  length  length of array
 * @param  checksum  supposed check sum
 *
 * @retval KDL_State_t  whether check successful
 */
KDL_State_t KDL_SHT30_CheckCRC(uint8_t data[], uint8_t length, uint8_t checksum)
{
	uint8_t crc = 0xFF;
	uint8_t bit = 0;
    uint8_t byteCtr ;

    /* calculates 8-Bit checksum with given polynomial */
    for(byteCtr = 0; byteCtr < length; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ KDL_SHT30_Polynomial;
            else crc = (crc << 1);
        }
    }

    return (crc == checksum)?KDL_OK:KDL_ERROR;
}

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
