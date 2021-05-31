/*
 * kosal_message.h
 *
 *  Created on: May 3, 2021
 *      Author: kirk_
 */

#ifndef KOSAL_MESSAGE_H_
#define KOSAL_MESSAGE_H_

#include "kosal.h"

#define Kosal_Message_UseStateLED 0

#if Kosal_Message_UseStateLED
#include "kdl_led.h"
#endif

#define Kosal_Message_State_PayloadFreeRequired  0x01
#define Kosal_Message_State_OwnerFreeRequired    0x02
#define Kosal_Message_State_HasData              0x20
#define Kosal_Message_State_ProcessFinished      0x40
#define Kosal_Message_State_CurrentlyProcessing  0x80

#define Kosal_Message_DefaultReceiveLength 2048U
#define Kosal_Message_MaxMessage_Length 2048U

typedef struct _Kosal_Message_Transmit_t {
	UART_HandleTypeDef* huart;
	ListItem_t item;
	uint8_t* payload;
	uint32_t length;
	uint8_t state;
} Kosal_Message_Transmit_t;

typedef struct _Kosal_Message_Receive_t {
	UART_HandleTypeDef* huart;
	ListItem_t item;
	uint8_t *buffer;
	uint32_t length;
	uint32_t hasread;
	uint8_t state;
} Kosal_Message_Receive_t;

typedef struct _Kosal_Message_Pack_t {
	ListItem_t item;
	char* payload;
	uint32_t length;
	uint8_t state;
} Kosal_Message_Pack_t;

typedef struct _Kosal_Message_Struct_t {
	osThreadId_t thread;
	List_t list;
#if Kosal_Message_UseStateLED
	KDL_LED_t led;
#endif
} Kosal_Message_Struct_t;

typedef struct _Kosal_Message_t {
	UART_HandleTypeDef* huart;
	Kosal_Message_Struct_t transmit;
	Kosal_Message_Struct_t receive;
	Kosal_Message_Struct_t pack;
	ListItem_t item;
} Kosal_Message_t;

Kosal_State_t Kosal_Message_Init(Kosal_Message_t *kmsg, UART_HandleTypeDef* huart);
#if Kosal_Message_UseStateLED
Kosal_State_t Kosal_Message_LEDInit(Kosal_Message_Struct_t *kmsgs, GPIO_TypeDef* port, uint16_t pin, KDL_LED_OnState stat);
#endif

Kosal_State_t Kosal_Message_Transmit(Kosal_Message_t *kmsg, uint8_t* payload, uint32_t length, uint8_t state);
Kosal_State_t Kosal_Message_Receive(Kosal_Message_t* kmsg, uint8_t* payload, uint32_t length);
uint8_t  Kosal_Message_Receive_NoCheck(Kosal_Message_t* kmsg);
Kosal_State_t Kosal_Message_Receive_Available(Kosal_Message_t* kmsg);
void Kosal_Message_Transmit_Thread(void* arg);
void Kosal_Message_Receive_Thread(void* arg);
void Kosal_Message_Pack_Thread(void* arg);
void Kosal_Message_Receive_ErrorCallbackThread(void* arg);
void Kosal_Message_Transmit_Callback(Kosal_Message_t* kmsg);
void Kosal_Message_Receive_Callback(Kosal_Message_t* kmsg);
void Kosal_Message_Transmit_Callback_Base(UART_HandleTypeDef* huart);
void Kosal_Message_Receive_Callback_Base(UART_HandleTypeDef* huart);
#endif /* KOSAL_MESSAGE_H_ */
