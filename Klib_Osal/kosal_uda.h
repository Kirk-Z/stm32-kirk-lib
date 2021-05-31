/*
 * kosal_uda.h
 *
 *  Created on: May 3, 2021
 *      Author: kirk_
 */

#ifndef KOSAL_UDA_H_
#define KOSAL_UDA_H_

#include "kosal.h"

typedef struct _Kosal_Uda_t {
	char* command;
	char* restriction;
	int(*process)(int argc, char** arg);
	ListItem_t item;
} Kosal_Uda_t;

Kosal_State_t Kosal_Uda_Init(UART_HandleTypeDef* huart);
void Kosal_Uda_Thread(void* arg);
void Kosal_Uda_ProcessThread(void* argument);
Kosal_State_t Kosal_Uda_AddCommand(char* const command, char* const restriction, int(*process)(int argc, char** arg));

char** Kosal_Uda_SplitCommand(char* cmd, int *argc);
Kosal_State_t Kosal_Uda_ValidChar(char c);

int Kosal_Uda_PrintWelcome(int argc, char** arg);
int Kosal_Uda_PrintHelp(int argc, char** arg);
int Kosal_Uda_PrintList(int argc, char** arg);
int Kosal_Uda_PrintAbout(int argc, char** arg);

#endif /* KOSAL_UDA_H_ */
