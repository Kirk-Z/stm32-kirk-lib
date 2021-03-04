/**
  ******************************************************************************
  * @date    Jan 8, 2021
  * @file    kd_malloc.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   Self-developed memory management source file
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
#include "kd_malloc.h"
#ifndef KD_MALLOC_USE_FREERTOS
#include "kd_math.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KD_MEM_MAPSIZE 0x10U
#define KD_MEM_POOLSIZE (KD_MEM_MAPSIZE << 5)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 * @brief Count how much size occupied
 */
uint32_t KD_Mem_Used;

/**
 * @brief Memory pool
 * @note Set the memory section "mempool" in the *.ld file to locate the memory poll specifically
 */
uint32_t KD_Mem_Pool[KD_MEM_POOLSIZE];// __attribute__((section(".mempool")));

/**
 * @brief Memory map
 * @note Set the memory section "memmap" in the *.ld file to locate the memory map specifically
 */
uint32_t KD_Mem_Map[KD_MEM_MAPSIZE];// __attribute__((section(".memmap")));

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* Initialization and de-initialization functions *****************************/

/**
  * @brief  Initialize the memory map
  */
void KD_Mem_Init(void)
{
	uint32_t i;
	for (i = 0; i < KD_MEM_MAPSIZE; i++)
		KD_Mem_Map[i] = 0;
	KD_Mem_Used = 0;
	return;
}
/* Configuration functions ****************************************************/

/**
  * @brief  Basic function for freeing a cell
  * @param  loc  point to where should be freed
  * @param  size  size of every unit
  * @param  length  length of units
  *
  * @note  Total size freed is size*length byte(s).
  * @note  Use 32 uint32_t (128 bytes) as a block.
  *
  * @retval KD_State_t whether freed successfully
  */
KD_State_t KD_Free_Base(void* loc, uint32_t size, uint32_t length)
{
	/* Calculate size in uint32_t (4bytes) */
	length = KD_Mem_CalculateWord(size, length);
	/* If the size smaller than a block */
	if (length < 32)
	{
		uint32_t start;
		uint32_t i;
		/* Where the sector start in the pool */
		start = (uint32_t)loc;
		start -= (uint32_t)KD_Mem_Pool;
		/* If it is not align to 4 bytes, arise an error */
		if (start & 3)
			return KD_ERROR;

		/* the index of block */
		i = start >> 7;
		start >>= 2;
		/* the starting index of cell inside the block */
		start &= 31;

		/* Set 0 to used memory in map to free */
		KD_Mem_Map[i] &= ~((uint32_t)KD_MATH_BIT_TRIANGLE[length] << start);
		/* Decrement the length occupied */
		KD_Mem_Used -= length;

		return KD_OK;
	}
	/* If the size greater than a block */
	else
	{
		uint32_t blocks;
		uint32_t start,end;
		uint32_t i;
		blocks = KD_Mem_CalculateBlock(length);
		start = (uint32_t)loc;
		start -= (uint32_t)KD_Mem_Pool;
		/* If it is not align to block, arise an error */
		if (start & 127)
			return KD_ERROR;

		/* index of starting block */
		start >>= 7;
		/* index of ending block */
		end = start + blocks;
		/* index of ending block out of range */
		if (end >= KD_MEM_MAPSIZE)
			return KD_ERROR;

		/* Free the memory and decrement length occupied */
		for (i = start; i < end; i++)
		{
			KD_Mem_Map[i] = 0;
			KD_Mem_Used -= 32;
		}
		return KD_OK;
	}
}

/**
  * @brief  Basic function for apply a cell
  * @param  size  size of every unit
  * @param  length  length of units
  *
  * @note  Total size freed is size*length byte(s).
  * @note  Use 32 uint32_t (128 bytes) as a block.
  * @note  When needed size is greater than a block,
  *        align will then be block instead of word.
  *
  * @retval void* where the memory is
  *               NULL means fail to access
  */
void* KD_Malloc_Base(uint32_t size, uint32_t length)
{
	/* How many uint32_t needed */
	length = KD_Mem_CalculateWord(size, length);
	/* Overall size less than a block */
	if (length < 32)
	{
		/* Find a block where the first length bits is 0 in map */
		/* In other words, map[block] < (1<<(32-length)) */
		uint32_t i;
		uint8_t j;
		for (i = 0; i < KD_MEM_MAPSIZE; i++)
		{
			if (KD_Mem_Map[i] < ((uint32_t)1 << (32 - length)))
				break;
		}
		/* No available location */
		if (i == KD_MEM_MAPSIZE)
			return NULL;
		/* Note 1 as occupy */
		j = KD_Mem_First_1(KD_Mem_Map[i]) + 1;
		KD_Mem_Map[i] |= ((uint32_t)KD_MATH_BIT_TRIANGLE[length]) << j;
		KD_Mem_Used += length;
		/* Return the pointer for the memory */
		i <<= 5; i += j;
		return &KD_Mem_Pool[i];
	}
	/* Size needed greater than a block */
	else
	{
		uint32_t blocks;
		uint32_t start=0, end=0, num=0;
		uint32_t i;
		/* Total number of blocks needed */
		blocks = KD_Mem_CalculateBlock(length);
		/* Look for the blocks */
		for (end = 0; end < blocks; end++)num += KD_Mem_Map[end] ? 1 : 0;
		while (end < KD_MEM_MAPSIZE && num)
		{
			num -= KD_Mem_Map[start] ? 1 : 0; start++;
			num += KD_Mem_Map[end] ? 1 : 0; end++;
		}
		/* No available location */
		if (end == KD_MEM_MAPSIZE)
			return NULL;
		/* Occupy the blocks */
		for (i = start; i < end; i++)
		{
			KD_Mem_Map[i] = 0xFFFFFFFF;
			KD_Mem_Used += 32;
		}
		return &KD_Mem_Pool[start << 5];
	}
}

/**
  * @brief  Basic function for re-allocating a cell
  * @param  loc  original location
  * @param  size  size of every unit
  * @param  length  length of units
  * @param  new_size  new size of every unit
  * @param  new_length new length of units
  *
  * @note  Total size freed is size*length byte(s).
  * @note  Use 32 uint32_t (128 bytes) as a block.
  *
  * @retval void*  new location
  */
void* KD_Realloc_Base(void* loc, uint32_t size, uint32_t length, uint32_t new_size, uint32_t new_length)
{
	void* new_loc;
	uint8_t *s_new, *s_loc;
	uint32_t i;
	/* If get an invalid original location */
	if (loc == NULL)
		return NULL;
	/* Apply for new location */
	new_loc = KD_Malloc_Base(size, length);
	/* Invalid new location */
	if (new_loc == NULL)
		return NULL;
	/* Copy memory */
	s_new = new_loc; s_loc = loc;
	for (i = 0; i < KD_Math_Min(size*length, new_size*new_length); i++)
		s_new[i] = s_loc[i];
	/* Free the original lcation */
	/* Free successfully */
	if (KD_OK == KD_Free_Base(loc, size, length))
		return new_loc;
	/* Free failed */
	else
	{
		/* Free new location */
		KD_Free_Base(new_loc, size, length);
		return NULL;
	}
}
/* IO operation functions *****************************************************/
/**
  * @brief  External function for freeing a cell
  * @param  loc  point to where should be freed
  *
  * @note  Total size freed is size*length byte(s).
  * @note  Use 32 uint32_t (128 bytes) as a block.
  * @note  The first word (4 bytes) stores the size of this memory block (in word).
  *
  * @retval KD_State_t whether freed successfully
  */
KD_State_t KD_Free(void* loc)
{
	uint32_t* p = loc;
	/* Invalid original location */
	if(loc == NULL)
		return KD_ERROR;
	/* Switch to include the prefix data */
	p--;
	/* Free */
	return KD_Free_Base(p,*p,1);
}

/**
  * @brief  External function for apply a cell
  * @param  size  size of every unit
  * @param  length  length of units
  *
  * @note  Total size freed is size*length byte(s).
  * @note  Use 32 uint32_t (128 bytes) as a block.
  * @note  When needed size is greater than a block,
  *        align will then be block instead of word.
  * @note  The first word (4 bytes) stores the size of this memory block (in word).
  *
  * @retval void* where the memory is
  *               NULL means fail to access
  */
void* KD_Malloc(uint32_t size, uint32_t length)
{
	uint32_t* p;
	/* Calculate length */
	size *= length;
	/* Include the size of prefix word */
	size += 4;
	/* Apply for memory */
	p = KD_Malloc_Base(size, 1);
	/* Invalid location */
	if(p == NULL)
		return NULL;

	/* Note the size of the memory block in prefix word */
	*p = size;
	/* Switch to payload location */
	p++;
	return p;
}

/**
  * @brief  External function for re-allocating a cell
  * @param  loc  original location
  * @param  new_size  new size of every unit
  * @param  new_length new length of units
  *
  * @note  Total size freed is size*length byte(s).
  * @note  Use 32 uint32_t (128 bytes) as a block.
  *
  * @retval void*  new location
  */
void* KD_Realloc(void* loc, uint32_t new_size, uint32_t new_length)
{
	uint32_t* p;
	p = loc;
	/* Invalid location */
	if(loc == NULL)
		return NULL;
	/* Switch to include prefix */
	p--;
	/* Re-allocation */
	return KD_Realloc_Base(loc,*p, 1, new_size, new_length);
}
/* State and Error functions **************************************************/

/**
 * @brief  Transfor bytes to words
 * @param  size  size of every unit
 * @param  length  length of units
 *
 * @note  have size*length bytes in total
 * @note  return the celling
 *
 * @retval  uint32_t number of words
 */
uint32_t KD_Mem_CalculateWord(uint32_t size, uint32_t length)
{
	return ((size * length + 3) >> 2);
}

/**
 * @brief  Transfor words to blocks
 * @param  length  length of words
 *
 * @note  have length*4 bytes in total
 * @note  return the celling
 *
 * @retval  uint32_t number of blocks
 */
uint32_t KD_Mem_CalculateBlock(uint32_t length)
{
	return ((length + 31) >> 5);
}

/**
 * @brief  Calculate the used rate of memory pool
 *
 * @retval  uint32_t rate in 0.01%
 */
uint32_t KD_Mem_UseRate(void)
{
	return (KD_Mem_Used * 10000 + (KD_MEM_POOLSIZE >> 1)) / KD_MEM_POOLSIZE;
}

/**
 * @brief  Get the index of first 1 appears in a word
 * @param  num  original data
 *
 * @retval  uint8_t location of the 1
 */
uint8_t KD_Mem_First_1(uint32_t num)
{
	uint8_t n = 255;
	if (num & 0xFFFF0000)
	{
		n += 16;
		num >>= 16;
	}
	if (num & 0xFF00)
	{
		n += 8;
		num >>= 8;
	}
	if (num & 0xF0)
	{
		n += 4;
		num >>= 4;
	}
	if (num & 0xC)
	{
		n += 2;
		num >>= 2;
	}
	if (num & 0x2)
	{
		n += 1;
		num >>= 1;
	}
	if (num & 0x1)
	{
		n += 1;
		num >>= 1;
	}
	return n;
}

void *Malloc(size_t size)
{
	return KD_Malloc(size, 1);
}

void Free(void* pv)
{
	KD_Free(pv);
}

#else /* KD_MALLOC_USE_FREERTOS */
void *Malloc(size_t size)
{
	return pvPortMalloc(size);
}

void Free(void* pv)
{
	vPortFree(pv);
}
#endif /* KD_MALLOC_USE_FREERTOS */

/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
