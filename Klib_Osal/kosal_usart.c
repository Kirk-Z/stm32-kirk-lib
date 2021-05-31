/**
  ******************************************************************************
  * @date    May 13, 2021
  * @file    kosal_usart.c
  * @author  Kirk_Z
  * @name    Kefan Zheng
  * @brief   USART driver based on operation system source file
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
#include "kosal_usart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
List_t Kosal_Usart_List = {0};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
static void Kosal_Usart_ThreadTx(void* argument)
{
	Kosal_Message_Transmit_t *p = NULL;
	Kosal_Usart_t *kuart;
	uint32_t delay;
	/* Check arg */
	if(argument == NULL)
	{
		/* Exit thread */
		osThreadExit();
		/* Return */
		return;
	}
	/* Pass arg */
	kuart = argument;
	/* Init list */
	vListInitialise(&kuart->ListTx);
	/* Callback functions */
	kuart->huart->TxCpltCallback = Kosal_Usart_TxCpltCallback;
	/* Setup flag */
	osThreadFlagsSet(kuart->TxId, Kosal_Usart_ProcessFinished);
	/* Set to high priority */
	osThreadSetPriority(osThreadGetId(), osPriorityLow);
	/* Infinite loop */
	for(;;)
	{
		/* If have data */
		if(osThreadFlagsWait(Kosal_Usart_HasData, osFlagsWaitAll, 0x1FFFF))
		{
			/* Not finished */
			if(osThreadFlagsGet() & Kosal_Usart_CurrentlyProcessing)
			{
				/* Wait for a while */
				osDelay(1);
				/* Continue */
				continue;
			}
			/* Process */
			while(listCURRENT_LIST_LENGTH(&kuart->ListTx))
			{
				/* Free old */
				if(p != NULL)
				{
					/* Free payload */
					if(p->state & Kosal_Usart_PayloadFreeRequired)
						vPortFree(p->payload);
					/* Free */
					if(p->state & Kosal_Usart_OwnerFreeRequired)
						vPortFree(p);
				}
				/* Get new */
				p = listGET_OWNER_OF_HEAD_ENTRY(&kuart->ListTx);
				/* Configure current transmit */
				HAL_UART_Transmit_DMA(p->huart, p->payload, p->length);
				/* Remove */
				uxListRemove(&p->item);
				/* Clear flag */
				osThreadFlagsClear(Kosal_Usart_ProcessFinished);
				/* No msg */
				if(listLIST_IS_EMPTY(&kuart->ListTx))
					osThreadFlagsClear(Kosal_Usart_HasData);
				/* Calculate delay */
				delay = (p->length * 10) / p->huart->Init.BaudRate;
				/* Wait */
				if(delay)
					osDelay(delay);
			}
		}
		/* Otherwise */
		else
		{
			/* Suspend the task until next pack */
			osThreadSuspend(kuart->TxId);
		}
	}
}

static void Kosal_Usart_ThreadRx(void* argument)
{
	Kosal_Usart_t *kuart;
	Kosal_Usart_Receive_t *p = NULL;
	/* Check arg */
	if(argument == NULL)
	{
		/* Exit thread */
		osThreadExit();
		/* Return */
		return;
	}
	/* Pass arg */
	kuart = argument;
	/* Init list */
	vListInitialise(&kuart->ListRx);
	/* Init callback */
	kuart->huart->RxCpltCallback = Kosal_Usart_RxCpltCallback;
	/* Apply for an item */
	do {
		osDelay(1);
		p = pvPortMalloc(sizeof(Kosal_Usart_Receive_t));
	} while(p == NULL);
	/* Apply for payload */
	do {
		osDelay(1);
		p->payload = pvPortMalloc(Kosal_Usart_RxBufSize);
	} while(p->payload == NULL);
	/* Init params */
	p->length = Kosal_Usart_RxBufSize;
	p->index = 0;
	p->huart = kuart->huart;
	p->state = 0;
	/* Init list item */
	vListInitialiseItem(&p->item);
	/* Set owner */
	listSET_LIST_ITEM_OWNER(&p->item, p);
	/* Insert to list */
	vListInsertEnd(&kuart->ListRx, &p->item);
	/* Set to high priority */
	osThreadSetPriority(osThreadGetId(), osPriorityRealtime);
	/* Start receive */
	HAL_UART_Receive_IT(p->huart, p->payload, p->length);
	/* Mart currently writting */
	p->state = Kosal_Usart_CurrentlyProcessing;
	/* Infinite loop */
	for(;;)
	{
		/* Apply for next item */
		p = pvPortMalloc(sizeof(Kosal_Usart_Receive_t));
		/* Apply failed */
		while(p == NULL)
		{
			osDelay(1);
			p = pvPortMalloc(sizeof(Kosal_Usart_Receive_t));
		}
		/* Apply for data */
		p->payload = pvPortMalloc(Kosal_Usart_RxBufSize);
		/* Apply failed */
		while(p->payload == NULL)
		{
			osDelay(1);
			p->payload = pvPortMalloc(Kosal_Usart_RxBufSize);
		}
		/* Init item */
		p->length = Kosal_Usart_RxBufSize;
		p->index = 0;
		p->huart = kuart->huart;
		p->state = 0;
		/* Init list item */
		vListInitialiseItem(&p->item);
		/* Set owner */
		listSET_LIST_ITEM_OWNER(&p->item, p);
		/* Insert to list */
		vListInsertEnd(&kuart->ListTx, &p->item);
		/* Wait */
		while(!osThreadFlagsWait(Kosal_Usart_ProcessFinished, osFlagsWaitAll, 0x1FFFF))
			osDelay(1);
		/* Clear flag */
		osThreadFlagsClear(Kosal_Usart_ProcessFinished);
	}
}

/* Initialization and de-initialization functions *****************************/
Kosal_State_t Kosal_Usart_Init(Kosal_Usart_t *kuart, UART_HandleTypeDef* huart)
{
	/* Check huart */
	if(!((IS_UART_INSTANCE(huart->Instance)) || (IS_LPUART_INSTANCE(huart->Instance))))
		return Kosal_Error;
	/* Check init list */
	if(!listLIST_IS_INITIALISED(&Kosal_Usart_List))
		vListInitialise(&Kosal_Usart_List);
	/* Check kuart */
	if(kuart == NULL)
		return Kosal_Error;
	/* Init Item */
	vListInitialiseItem(&kuart->Item);
	/* Set owner */
	listSET_LIST_ITEM_OWNER(&kuart->Item, (void*)kuart);
	/* Set value */
	listSET_LIST_ITEM_VALUE(&kuart->Item, (uint32_t)huart->Instance);
	/* Into list */
	vListInsert(&Kosal_Usart_List, &kuart->Item);
	/* Init kuart */
	kuart->huart = huart;
	/* Start tx thread */
	kuart->TxId = osThreadNew(Kosal_Usart_ThreadTx, kuart, NULL);
	/* Start rx thread */
	kuart->RxId = osThreadNew(Kosal_Usart_ThreadRx, kuart, NULL);
	/* Finished */
	return Kosal_OK;
}
/* Configuration functions ****************************************************/
/* IO operation functions *****************************************************/
void Kosal_Usart_Transmit(Kosal_Usart_t *kuart, uint8_t* buf, uint32_t length, uint32_t state)
{
	Kosal_Message_Transmit_t* p;
	/* Apply for memory */
	p = pvPortMalloc(sizeof(Kosal_Message_Transmit_t));
	/* Init trans */
	p->huart = kuart->huart;
	p->payload = buf;
	p->length = length;
	p->state = state | Kosal_Usart_OwnerFreeRequired;
	/* Init list item */
	vListInitialiseItem(&p->item);
	/* Set owner */
	listSET_LIST_ITEM_OWNER(&p->item, p);
	/* Insert item */
	vListInsertEnd(&kuart->ListTx, &p->item);
	/* Set flag */
	osThreadFlagsSet(kuart->TxId, Kosal_Usart_HasData);
	/* Resume */
	osThreadResume(kuart->TxId);
	/* Finished */
	return;
}

int Kosal_Usart_Printf(Kosal_Usart_t *kuart, const char *fmt, ...)
{
	Kosal_Printf_Return_t res = {0};
	va_list args;
	char *buf;

	va_start(args, fmt);
	buf = pvPortMalloc(Kosal_Usart_MaxLength);
	if(buf == NULL)
		return -1;
	res = Kosal_VsPrintf(buf, Kosal_Usart_MaxLength, fmt, args);
	va_end(args);

	Kosal_Usart_Transmit(kuart, (uint8_t*)buf, res.length, Kosal_Usart_PayloadFreeRequired);

	return res.counts;
}
/* State and Error functions **************************************************/
Kosal_Usart_t* Kosal_Usart_GetKuart(UART_HandleTypeDef *huart)
{
	ListItem_t* item;
	/* Get head */
	item = listGET_HEAD_ENTRY(&Kosal_Usart_List);
	/* Traverse */
	while(item != listGET_END_MARKER(&Kosal_Usart_List))
	{
		/* Check */
		if(((Kosal_Usart_t*)listGET_LIST_ITEM_OWNER(item))->huart == huart)
			return ((Kosal_Usart_t*)listGET_LIST_ITEM_OWNER(item));
		/* Next */
		item = listGET_NEXT(item);
	}
	/* Not found */
	return NULL;
}

Kosal_State_t Kosal_Usart_Receive_Available(Kosal_Usart_t* kuart)
{
	Kosal_Usart_Receive_t *p;
	/* Has more entries */
	if(listCURRENT_LIST_LENGTH(&kuart->ListRx)>=3)
		return Kosal_OK;
	/* Get current node */
	p = ( (&( (&(kuart->ListRx))->xListEnd ))->pxNext->pvOwner );
	/* Check length */
	if(p->index < (p->length - p->huart->RxXferCount))
		return Kosal_OK;
	/* No data */
	return Kosal_Error;
}

uint8_t  Kosal_Usart_Receive_NoCheck(Kosal_Usart_t* kuart)
{
	Kosal_Usart_Receive_t *p;
	uint8_t data;
	/* Get current node */
	p = ( (&( (&(kuart->ListRx))->xListEnd ))->pxNext->pvOwner );
	/* Read data */
	data = p->payload[p->index++];
	/* Check read out */
	if(p->index >= p->length)
	{
		/* Remove from list */
		uxListRemove(&p->item);
		/* Free buffer */
		vPortFree(p->payload);
		/* Free item */
		vPortFree(p);
	}
	/* Return data */
	return data;
}
/* Private functions ---------------------------------------------------------*/
void Kosal_Usart_TxCpltCallback(UART_HandleTypeDef *huart)
{
	Kosal_Usart_t* kuart;
	/* Get kuart */
	kuart = Kosal_Usart_GetKuart(huart);
	/* Check */
	if(kuart != NULL)
	{
		/* Set flag */
		osThreadFlagsSet(kuart->TxId, Kosal_Usart_ProcessFinished);
	}
}

void Kosal_Usart_RxCpltCallback(UART_HandleTypeDef *huart)
{
	Kosal_Usart_t* kuart;
	Kosal_Usart_Receive_t *p;
	/* Get kuart */
	kuart = Kosal_Usart_GetKuart(huart);
	/* Check */
	if(kuart != NULL)
	{
		/* Get end owner */
		p = ( (&( (&(kuart->ListRx))->xListEnd ))->pxPrevious->pvOwner );
		/* Start a new */
		HAL_UART_Receive_IT(p->huart, p->payload, p->length);
		/* Mart current */
		p->state |= Kosal_Usart_CurrentlyProcessing;
		/* Set flag */
		osThreadFlagsSet(kuart->RxId, Kosal_Usart_ProcessFinished);
	}
}
/************************ (C) COPYRIGHT kirkz.tech *****END OF FILE****/
