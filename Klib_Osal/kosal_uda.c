/*
 * kosal_uda.c
 *
 *  Created on: May 3, 2021
 *      Author: kirk_
 */

#include "kosal_uda.h"
#include "kosal_message.h"

List_t Kosal_Uda_List;
Kosal_Message_t Kosal_Uda_Message;
osThreadId_t Kosal_Uda_ThreadID;

Kosal_State_t Kosal_Uda_Init(UART_HandleTypeDef* huart)
{
	Kosal_Message_Init(&Kosal_Uda_Message, huart);
	Kosal_Uda_ThreadID = osThreadNew(Kosal_Uda_Thread, NULL, NULL);
	return Kosal_OK;
}

void Kosal_Uda_Thread(void* arg)
{
	Kosal_Message_Pack_t* pack;
	char* payload;
	/* Initialize list */
	Kosal_Uda_AddCommand("welcome", "void", Kosal_Uda_PrintWelcome);
	Kosal_Uda_AddCommand("help", "void", Kosal_Uda_PrintHelp);
	Kosal_Uda_AddCommand("list", "void", Kosal_Uda_PrintList);
	Kosal_Uda_AddCommand("about", "void", Kosal_Uda_PrintAbout);
	/* Wait for a while */
	osDelay(1000);
	/* Print */
	Kosal_Uda_PrintWelcome(0, NULL);
	/* Infinite loop */
	for(;;)
	{
		/* If there is a pack */
		while(!listCURRENT_LIST_LENGTH(&Kosal_Uda_Message.pack.list))
			osDelay(10);
		/* Get the pack */
		pack = listGET_OWNER_OF_HEAD_ENTRY(&Kosal_Uda_Message.pack.list);
		/* Get payload */
		payload = pack->payload;
		/* Remove from list */
		uxListRemove(&pack->item);
		/* Free item */
		vPortFree(pack);
		/* Process */
		osThreadNew(Kosal_Uda_ProcessThread, payload, NULL);
	}
}

void Kosal_Uda_ProcessThread(void* argument)
{
	char* c;
	char** argv;
	int argc;
	Kosal_Uda_t *p;
	ListItem_t *item;
	uint32_t tickstart;
	uint32_t tickend;
	char* log;
	int length;
	int rtval;
	/* Check arg */
	if(argument == NULL)
	{
		osThreadExit();
		return;
	}
	/* Pass arg */
	c = argument;
	/* Split */
	argv = Kosal_Uda_SplitCommand(c, &argc);
	/* Check */
	if(argc != 0)
	{
		/* Get head entry */
		item = 	listGET_HEAD_ENTRY(&Kosal_Uda_List);
		/* Traverse command */
		while(item != listGET_END_MARKER(&Kosal_Uda_List))
		{
			/* Get owner */
			p = listGET_LIST_ITEM_OWNER(item);
			/* Compare */
			if(0 == strcmp(p->command, argv[0]))
			{
				/* Mark time */
				tickstart = HAL_GetTick();
				/* Process */
				rtval = p->process(argc, argv);
				/* Mark time */
				tickend = HAL_GetTick();
				/* Set length */
				length = 70 + strlen(p->command);
				/* Apply log */
				log = pvPortMalloc(length);
				/* Apply failed */
				while(log == NULL)
				{
					osDelay(1);
					log = pvPortMalloc(length);
				}
				/* Print log */
				sprintf(log, "Task \"%s\" processed for %lu ms with return value %d.\r\n", \
						p->command, tickend - tickstart, rtval);
				/* Transmit */
				Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)log, strlen(log),\
						Kosal_Message_State_PayloadFreeRequired);
				/* Break loop */
				break;
			}
			/* Switch to next */
			item = listGET_NEXT(item);
		}
		/* Not found */
		if(item == listGET_END_MARKER(&Kosal_Uda_List))
		{
			/* Assume length */
			length = 24;
			/* Apply log */
			log = pvPortMalloc(length);
			/* Apply failed */
			while(log == NULL)
			{
				osDelay(1);
				log = pvPortMalloc(length);
			}
			/* Print log */
			sprintf(log, "Command not found.\r\n");
			/* Transmit */
			Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)log, strlen(log),\
					Kosal_Message_State_PayloadFreeRequired);
		}
	}
	/* Free pack */
	vPortFree(argument);
	/* Exit thread */
	osThreadExit();
	/* Finished */
	return;
}

Kosal_State_t Kosal_Uda_AddCommand(char* const command, char* const restriction, int(*process)(int argc, char** arg))
{
	char *c, *r;
	Kosal_Uda_t* p;
	/* Init list */
	if(!listLIST_IS_INITIALISED(&Kosal_Uda_List))
		vListInitialise(&Kosal_Uda_List);
	/* Apply for mem */
	c = pvPortMalloc(strlen(command)+1);
	/* Apply failed */
	while(c==NULL)
	{
		osDelay(1);
		c = pvPortMalloc(strlen(command)+1);
	}
	/* Apply for mem */
	r = pvPortMalloc(strlen(restriction)+1);
	/* Apply failed */
	while(c==NULL)
	{
		osDelay(1);
		r = pvPortMalloc(strlen(restriction)+1);
	}
	/* Apply for item */
	p = pvPortMalloc(sizeof(Kosal_Uda_t));
	while(p==NULL)
	{
		osDelay(1);
		p = pvPortMalloc(sizeof(Kosal_Uda_t));
	}
	/* Set params */
	p->command = c;
	p->restriction = r;
	p->process = process;
	/* Copy command */
	strcpy(c, command);
	strcpy(r, restriction);
	/* Init list */
	vListInitialiseItem(&p->item);
	/* Set owner */
	listSET_LIST_ITEM_OWNER(&p->item, p);
	/* Set value */
	listSET_LIST_ITEM_VALUE(&p->item, listCURRENT_LIST_LENGTH(&Kosal_Uda_List));
	/* Into list */
	vListInsert(&Kosal_Uda_List, &p->item);
	/* Finished */
	return Kosal_OK;
}

char** Kosal_Uda_SplitCommand(char* cmd, int *argc)
{
	int count = 0;
	char* p = cmd;
	int stat = 0;
	char** result;
	/* Convert */
	while(*p!='\0')
	{
		/* Convert invalid */
		if(Kosal_OK != Kosal_Uda_ValidChar(*p))
		{
			*p = '\0';
			stat = 0;
		}
		else
		{
			/* If perious stat is 0, here comes a new string */
			count += !stat;
			stat = 1;
		}
		/* Switch addr */
		p++;
	}
	/* Pass counter */
	*argc = count;
	/* If no argument */
	if(count==0)
		return NULL;
	/* Apply for mem */
	result = pvPortMalloc(count * sizeof(char*));
	/* Apply failed */
	while(result == NULL)
	{
		osDelay(1);
		result = pvPortMalloc(count * sizeof(char*));
	}
	/* Reset pointer */
	p = cmd;
	/* Find where */
	for(stat=0;stat<count;stat++)
	{
		/* Filter */
		while(*p == '\0')
			p++;
		/* Pass pointer */
		result[stat] = p;
		/* Filter */
		while(*p != '\0')
			p++;
	}
	/* Finished */
	return result;
}

Kosal_State_t Kosal_Uda_ValidChar(char c)
{
	/* Check whether valid */
	if(c>='A' && c<='Z')
		return Kosal_OK;
	if(c>='a' && c<='z')
		return Kosal_OK;
	if(c>='0' && c<='9')
		return Kosal_OK;
	if(c=='-' || c=='_')
		return Kosal_OK;
	/* Else */
	return Kosal_Error;
}


int Kosal_Uda_PrintWelcome(int argc, char** arg)
{
	return 0;
}

int Kosal_Uda_PrintHelp(int argc, char** arg)
{
	return 0;
}

int Kosal_Uda_PrintList(int argc, char** arg)
{

	Kosal_Uda_t* p;
	ListItem_t* item;
	char* log;
	/* Get head */
	item = listGET_HEAD_ENTRY(&Kosal_Uda_List);
	/* Apply for mem */
	log = pvPortMalloc(80);
	/* Apply failed */
	while(log == NULL)
	{
		osDelay(1);
		log = pvPortMalloc(100);
	}
	/* Convert */
	sprintf(log, "| Index |  Position  | Command | Arguments |\r\n");
	/* Transmit */
	Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)log, strlen(log), Kosal_Message_State_PayloadFreeRequired);
	/* Traverse */
	while(item != listGET_END_MARKER(&Kosal_Uda_List))
	{
		/* Get owner */
		p = listGET_LIST_ITEM_OWNER(item);

		/* Apply for mem */
		log = pvPortMalloc(40);
		/* Apply failed */
		while(log == NULL)
		{
			osDelay(1);
			log = pvPortMalloc(40);
		}

		/* Construct log */
		sprintf(log, "| %5lu | %p | ", listGET_LIST_ITEM_VALUE(item), p->process);
		/* Transmit */
		Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)log, strlen(log), Kosal_Message_State_PayloadFreeRequired);
		/* Transmit */
		Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)p->command, strlen(p->command), 0);

		/* Apply for mem */
		log = pvPortMalloc(8);
		/* Apply failed */
		while(log == NULL)
		{
			osDelay(1);
			log = pvPortMalloc(8);
		}

		/* Construct log */
		sprintf(log, " | ");
		/* Transmit */
		Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)log, strlen(log), Kosal_Message_State_PayloadFreeRequired);

		/* Transmit */
		Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)p->restriction, strlen(p->restriction), 0);

		/* Apply for mem */
		log = pvPortMalloc(8);
		/* Apply failed */
		while(log == NULL)
		{
			osDelay(1);
			log = pvPortMalloc(8);
		}

		/* Construct log */
		sprintf(log, " |\r\n");
		/* Transmit */
		Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)log, strlen(log), Kosal_Message_State_PayloadFreeRequired);

		/* Switch to next */
		item = listGET_NEXT(item);
	}
	/* Apply for mem */
	log = pvPortMalloc(80);
	/* Apply failed */
	while(log == NULL)
	{
		osDelay(1);
		log = pvPortMalloc(100);
	}
	/* Convert */
	sprintf(log, "| Index |  Position  | Command | Arguments |\r\n");
	/* Transmit */
	Kosal_Message_Transmit(&Kosal_Uda_Message, (uint8_t*)log, strlen(log), Kosal_Message_State_PayloadFreeRequired);
	return 0;
}

int Kosal_Uda_PrintAbout(int argc, char** arg)
{
	return 0;
}

