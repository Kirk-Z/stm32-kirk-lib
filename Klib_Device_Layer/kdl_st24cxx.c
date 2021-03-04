/**
  ******************************************************************************
  * @date    Jan 8, 2021
  * @file    kdl_st24cxx.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   ST24Cxx EEPROM source file
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
#include "kdl_st24cxx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/
/**
 * @brief Initialize
 * @param  dst  pointer to the chip
 * @param  port  handler to I2C port
 * @param  address  chip I2C slave address
 * @param  size  size of chip
 *
 * @retval  KDL_State_t whether successful
 */
KDL_State_t KDL_ST24Cxx_Init(KDL_ST24Cxx_t* dst, I2C_HandleTypeDef* port, uint16_t address, KDL_ST24Cxx_Size_t size)
{
  if(dst == NULL)
    return KDL_ERROR;

  dst->Port = port;
  dst->Address = address;
  dst->Size = size;
  dst->Stat = 1;

  return KDL_OK;
}
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
/**
 * @brief Read dual-word from the chip
 * @param  dst  pointer to the chip
 * @param  addr  address to read
 * @param  buffer  buffer to store
 *
 * @retval  KDL_State_t whether successful
 */
KDL_State_t KDL_ST24Cxx_ReadDWord(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer)
{
  if(!__IS_KDL_ST24CXX_T(dst))
    return KDL_ERROR;
  /* Reading interval */
  HAL_Delay(KDL_ST24CXX_OPERATION_INTERVAL);
  /* Read memory */
  if(HAL_OK == HAL_I2C_Mem_Read(dst->Port, dst->Address + 1, addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)buffer, 8, 0x1FFFF))
    return KDL_OK;

  return KDL_ERROR;
}

/**
 * @brief Write dual-word to the chip
 * @param  dst  pointer to the chip
 * @param  addr  address to write
 * @param  buffer  buffer store the data
 *
 * @retval  KDL_State_t whether successful
 */
KDL_State_t KDL_ST24Cxx_WriteDWord(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer)
{
  if(!__IS_KDL_ST24CXX_T(dst))
    return KDL_ERROR;
  /* Writing interval */
  HAL_Delay(KDL_ST24CXX_OPERATION_INTERVAL);
  /* Write memory */
  if(HAL_OK == HAL_I2C_Mem_Write(dst->Port, dst->Address, addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)buffer, 8, 0x1FFFF))
    return KDL_OK;

  return KDL_ERROR;
}


/**
 * @brief Read length bytes from the chip
 * @param  dst  pointer to the chip
 * @param  addr  address to read
 * @param  buffer  buffer to store
 * @param  length  length of data
 *
 * @retval  KDL_State_t whether successful
 *
 * @note  to be completed soon
 */
KDL_State_t KDL_ST24Cxx_Read(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer, uint32_t length)
{
  return KDL_OK;
}

/**
 * @brief  Write length bytes to the chip
 * @param  dst  pointer to the chip
 * @param  addr  address to write
 * @param  buffer  buffer store the data
 * @param  length  length of data
 *
 * @retval  KDL_State_t whether successful
 *
 * @note  to be completed soon
 */
KDL_State_t KDL_ST24Cxx_Write(KDL_ST24Cxx_t* dst, uint32_t addr, void* buffer, uint32_t length)
{
  return KDL_OK;
}

/* State and Error functions **************************************************/

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
