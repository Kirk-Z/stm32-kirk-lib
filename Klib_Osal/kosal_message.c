/*
 * kosal_message.c
 *
 *  Created on: May 3, 2021
 *      Author: kirk_
 */

#include "kosal_message.h"

List_t Kosal_Message_Port_List;

osThreadId_t Kosal_Message_Receive_ErrorCallbackThreadId = NULL;

Kosal_State_t Kosal_Message_Init(Kosal_Message_t *kmsg, UART_HandleTypeDef* huart)
{
	/* Init msg list */
	if(!listLIST_IS_INITIALISED(&Kosal_Message_Port_List))
		vListInitialise(&Kosal_Message_Port_List);
	/* Add to list */
	vListInitialiseItem(&kmsg->item);
	listSET_LIST_ITEM_OWNER(&kmsg->item, kmsg);
	listSET_LIST_ITEM_VALUE(&kmsg->item, (uint32_t)kmsg);
	vListInsert(&Kosal_Message_Port_List, &kmsg->item);
	/* Init uart */
	kmsg->huart = huart;
	/* Reset led */
	kmsg->transmit.led.check = (uint32_t)NULL;
	kmsg->receive.led.check = (uint32_t)NULL;
	kmsg->pack.led.check = (uint32_t)NULL;
	/* Start error handler */
	if(Kosal_Message_Receive_ErrorCallbackThreadId == NULL)
		Kosal_Message_Receive_ErrorCallbackThreadId = osThreadNew(Kosal_Message_Receive_ErrorCallbackThread, NULL, NULL);
	/* Start threads */
	kmsg->transmit.thread = osThreadNew(Kosal_Message_Transmit_Thread, kmsg, NULL);
	kmsg->receive.thread = osThreadNew(Kosal_Message_Receive_Thread, kmsg, NULL);
	kmsg->pack.thread = osThreadNew(Kosal_Message_Pack_Thread, kmsg, NULL);
	/* Finished */
	return Kosal_OK;
}

#if Kosal_Message_UseStateLED
Kosal_State_t Kosal_Message_LEDInit(Kosal_Message_Struct_t *kmsgs, GPIO_TypeDef* port, uint16_t pin, KDL_LED_OnState stat)
{
	KDL_LED_Init(&kmsgs->led, port, pin, stat);
	KDL_LED_TurnOff(&kmsgs->led);
	return Kosal_OK;
}
#endif

Kosal_State_t Kosal_Message_Transmit(Kosal_Message_t *kmsg, uint8_t* payload, uint32_t length, uint8_t state)
{
	Kosal_Message_Transmit_t *p = NULL;
	/* Check arg */
	if(payload == NULL)
		return Kosal_Error;
	if(kmsg == NULL)
		return Kosal_Error;
	if(kmsg->huart == NULL)
		return Kosal_Error;
	/* Apply for memory */
	p = pvPortMalloc(sizeof(Kosal_Message_Transmit_t));
	/* Init trans */
	p->huart = kmsg->huart;
	p->payload = payload;
	p->length = length;
	p->state = state | Kosal_Message_State_OwnerFreeRequired;
	/* Init list item */
	vListInitialiseItem(&p->item);
	/* Set owner */
	listSET_LIST_ITEM_OWNER(&p->item, p);
	/* Insert item */
	vListInsertEnd(&kmsg->transmit.list, &p->item);
	/* Set flag */
	osThreadFlagsSet(kmsg->transmit.thread, Kosal_Message_State_HasData);
	/* Finished */
	return Kosal_OK;
}

Kosal_State_t Kosal_Message_Receive(Kosal_Message_t* kmsg, uint8_t* payload, uint32_t length)
{
	return Kosal_Error;
}

uint8_t  Kosal_Message_Receive_NoCheck(Kosal_Message_t* kmsg)
{
	Kosal_Message_Receive_t *p;
	uint8_t data;
	/* Get current node */
	p = (Kosal_Message_Receive_t*)( (&( (&( kmsg->receive.list ))->xListEnd ))->pxNext->pvOwner );
	/* Read data */
	data = p->buffer[p->hasread++];
	/* Check read out */
	if(p->hasread >= p->length)
	{
		/* Remove from list */
		uxListRemove(&p->item);
		/* Free buffer */
		vPortFree(p->buffer);
		vPortFree(p);
	}
	/* Return data */
	return data;
}

Kosal_State_t Kosal_Message_Receive_Available(Kosal_Message_t* kmsg)
{
	Kosal_Message_Receive_t *p;
	/* Has more entries */
	if(listCURRENT_LIST_LENGTH(&kmsg->receive.list)>=3)
		return Kosal_OK;
	/* Get current node */
	p = (Kosal_Message_Receive_t*)( (&( (&( kmsg->receive.list ))->xListEnd ))->pxNext->pvOwner );
	/* Check length */
	if(p->hasread < (p->length - p->huart->RxXferCount))
		return Kosal_OK;
	/* No data */
	return Kosal_Error;
}

void Kosal_Message_Transmit_Thread(void* arg)
{
	Kosal_Message_t *kmsg;
	Kosal_Message_Transmit_t *p = NULL;
	/* Pass param */
	kmsg = arg;
	/* Init list */
	vListInitialise(&kmsg->transmit.list);
	/* Set callback function */
	kmsg->huart->TxCpltCallback = Kosal_Message_Transmit_Callback_Base;
	/* Initialize flag */
	osThreadFlagsSet(kmsg->transmit.thread, Kosal_Message_State_ProcessFinished);
	/* Infinite loop */
	for(;;)
	{
		/* If have data */
		if(osThreadFlagsWait(Kosal_Message_State_HasData | Kosal_Message_State_ProcessFinished, osFlagsWaitAll, 0x1FFFF))
		{
			/* Clear flag */
			osThreadFlagsClear(Kosal_Message_State_HasData | Kosal_Message_State_ProcessFinished);
			/* Process */
			while(listCURRENT_LIST_LENGTH(&kmsg->transmit.list))
			{
				/* Free current */
				if(p!=NULL)
				{
					/* If payload need to free */
					if(p->state & Kosal_Message_State_PayloadFreeRequired)
					{
						vPortFree((void*)p->payload);
					}
					/* If owner need to free */
					if(p->state & Kosal_Message_State_OwnerFreeRequired)
					{
						vPortFree((void*)p);
					}
				}
				/* Get new */
				p = listGET_OWNER_OF_HEAD_ENTRY(&kmsg->transmit.list);
				/* Configure current transmit */
				while(HAL_OK != HAL_UART_Transmit_DMA(p->huart, p->payload, p->length))
					osDelay(1);
				/* Remove item */
				uxListRemove(&p->item);
				/* Flip LED */
				KDL_LED_Toggle(&kmsg->transmit.led);
			}
		}
		/* Otherwise */
		else
		{
			osDelay(10);
		}
	}
}

void Kosal_Message_Receive_Thread(void* arg)
{
	Kosal_Message_t *kmsg;
	Kosal_Message_Receive_t *now;
	/* Pass param */
	kmsg = arg;
	/* Init list */
	vListInitialise(&kmsg->receive.list);
	/* Set callback function */
	kmsg->huart->RxCpltCallback = Kosal_Message_Receive_Callback_Base;
	/* Apply for first item */
	now = pvPortMalloc(sizeof(Kosal_Message_Receive_t));
	/* Apply failed */
	while(now == NULL)
	{
		osDelay(1);
		now = pvPortMalloc(sizeof(Kosal_Message_Receive_t));
	}
	/* Apply for data */
	now->buffer = pvPortMalloc(Kosal_Message_DefaultReceiveLength);
	/* Apply failed */
	while(now->buffer == NULL)
	{
		osDelay(1);
		now->buffer = pvPortMalloc(Kosal_Message_DefaultReceiveLength);
	}
	/* Init item */
	now->length = Kosal_Message_DefaultReceiveLength;
	now->hasread = 0;
	now->huart = kmsg->huart;
	now->state = 0;
	/* Init list item */
	vListInitialiseItem(&now->item);
	/* Set owner */
	listSET_LIST_ITEM_OWNER(&now->item, now);
	/* Insert to list */
	vListInsertEnd(&kmsg->receive.list, &now->item);
	/* Start first receive */
	while(HAL_OK != HAL_UART_Receive_IT(now->huart, now->buffer, now->length))
		osDelay(1);
	/* Mart currently writing */
	now->state |= Kosal_Message_State_CurrentlyProcessing;
	/* Infinite loop */
	for(;;)
	{
		/* Apply for next item */
		now = pvPortMalloc(sizeof(Kosal_Message_Receive_t));
		/* Apply failed */
		while(now == NULL)
		{
			osDelay(1);
			now = pvPortMalloc(sizeof(Kosal_Message_Receive_t));
		}
		/* Apply for data */
		now->buffer = pvPortMalloc(Kosal_Message_DefaultReceiveLength);
		/* Apply failed */
		while(now->buffer == NULL)
		{
			osDelay(1);
			now->buffer = pvPortMalloc(Kosal_Message_DefaultReceiveLength);
		}
		/* Init item */
		now->length = Kosal_Message_DefaultReceiveLength;
		now->hasread = 0;
		now->huart = kmsg->huart;
		now->state = 0;
		/* Init list item */
		vListInitialiseItem(&now->item);
		/* Set owner */
		listSET_LIST_ITEM_OWNER(&now->item, now);
		/* Insert to list */
		vListInsertEnd(&kmsg->receive.list, &now->item);
		/* Wait */
		while(!osThreadFlagsWait(Kosal_Message_State_ProcessFinished, osFlagsWaitAll, 0x1FFFF))
			osDelay(1);
		/* Clear flag */
		osThreadFlagsClear(Kosal_Message_State_ProcessFinished);
		/* Flip LED */
		KDL_LED_Toggle(&kmsg->receive.led);
	}
}

void Kosal_Message_Pack_Thread(void* arg)
{
	Kosal_Message_t *kmsg;
	char* tmp = NULL;
	char* p;
	uint32_t length;
	Kosal_Message_Pack_t* now;
	/* Pass param */
	kmsg = arg;
	/* Init list */
	vListInitialise(&kmsg->pack.list);
	/* Init pool */
	while(tmp == NULL)
	{
		osDelay(1);
		tmp = pvPortMalloc(Kosal_Message_MaxMessage_Length + 1);
	}
	/* Set last byte 0 */
	tmp[Kosal_Message_MaxMessage_Length] = '\0';
	/* Infinite loop */
	for(;;)
	{
		/* Init converting */
		length = 0;
		p = tmp;
		/* Converting message */
		for(;;)
		{
			/* Wait available */
			while(Kosal_OK != Kosal_Message_Receive_Available(kmsg))
				osDelay(10);
			/* Get a byte */
			*p = Kosal_Message_Receive_NoCheck(kmsg);
			/* Check end */
			if(*p<(char)32 || *p>(char)126)
			{
				/* Set an end */
				*p = '\0';
				break;
			}
			/* Switch pointer */
			p++;
			/* Inc length */
			length++;
			/* Check max length */
			if(length >= Kosal_Message_MaxMessage_Length)
				break;
		}
		/* If needed to add to queue */
		if(length)
		{
			/* Inc length */
			length++;
			/* Apply for mem */
			p = pvPortMalloc(length);
			/* Apply failed */
			while(p==NULL)
			{
				osDelay(1);
				p = pvPortMalloc(length);
			}
			/* Copy string */
			strcpy(p, tmp);
			/* Init item */
			now = pvPortMalloc(sizeof(Kosal_Message_Pack_t));
			/* Set */
			now->length = length;
			now->payload = p;
			now->state = Kosal_Message_State_OwnerFreeRequired | Kosal_Message_State_PayloadFreeRequired;
			/* Init list item */
			vListInitialiseItem(&now->item);
			/* Set owner */
			listSET_LIST_ITEM_OWNER(&now->item, now);
			/* Insert to list */
			vListInsertEnd(&kmsg->pack.list, &now->item);
			/* Flip LED */
			KDL_LED_Toggle(&kmsg->pack.led);
		}
	}
}

void Kosal_Message_Receive_ErrorCallbackThread(void* arg)
{
	for(;;)
	{
		osDelay(1000);
	}
}

void Kosal_Message_Transmit_Callback(Kosal_Message_t* kmsg)
{
	/* Set flag */
	if(kmsg->transmit.thread != NULL)
		osThreadFlagsSet(kmsg->transmit.thread, Kosal_Message_State_ProcessFinished);
	/* Finished */
	return;
}

void Kosal_Message_Receive_Callback(Kosal_Message_t* kmsg)
{
	Kosal_Message_Receive_t * now;
	/* Get owner of the end entry */
	now = (Kosal_Message_Receive_t*)( (&( (&( kmsg->receive.list ))->xListEnd ))->pxPrevious->pvOwner );
	/* Start a new receive */
	while(HAL_OK != HAL_UART_Receive_IT(now->huart, now->buffer, now->length));
	/* Mark current writing */
	now->state |= Kosal_Message_State_CurrentlyProcessing;
	/* Set flag */
	if(kmsg->receive.thread != NULL)
		osThreadFlagsSet(kmsg->receive.thread, Kosal_Message_State_ProcessFinished);
	/* Finished */
	return;
}

void Kosal_Message_Transmit_Callback_Base(UART_HandleTypeDef* huart)
{
	ListItem_t* item;
	item = listGET_HEAD_ENTRY(&Kosal_Message_Port_List);
	while(item != listGET_END_MARKER(&Kosal_Message_Port_List))
	{
		/* Callback */
		if(((Kosal_Message_t*)listGET_LIST_ITEM_OWNER(item))->huart == huart)
		{
			Kosal_Message_Transmit_Callback((Kosal_Message_t*)listGET_LIST_ITEM_OWNER(item));
		}
		/* Switch to next */
		item = listGET_NEXT(item);
	}
}

void Kosal_Message_Receive_Callback_Base(UART_HandleTypeDef* huart)
{
	ListItem_t* item;
	item = listGET_HEAD_ENTRY(&Kosal_Message_Port_List);
	while(item != listGET_END_MARKER(&Kosal_Message_Port_List))
	{
		/* Callback */
		if(((Kosal_Message_t*)listGET_LIST_ITEM_OWNER(item))->huart == huart)
		{
			Kosal_Message_Receive_Callback((Kosal_Message_t*)listGET_LIST_ITEM_OWNER(item));
		}
		/* Switch to next */
		item = listGET_NEXT(item);
	}
}
