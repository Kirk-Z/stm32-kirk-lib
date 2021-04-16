/*
 * kal_firepid.h
 *
 *  Created on: Apr 14, 2021
 *      Author: kirk_
 */

#ifndef KAL_FIREPID_H_
#define KAL_FIREPID_H_

#include "main.h"

#define KAL_FirePID_UART  UART8
#define KAL_FirePID_HUART huart8

typedef enum {
  KAL_FirePID_IndexP,
  KAL_FirePID_IndexI,
  KAL_FirePID_IndexD,
  KAL_FirePID_Target,
  KAL_FirePID_Current,
  KAL_FirePID_Interval,

} KAL_FirePID_Const_t;

#pragma pack (1)             //作用：C编译器将按照n个字节对齐。

typedef struct _KAL_FirePID_t {
	uint32_t head;
	uint8_t channel;
	uint32_t size;
	uint8_t instruct;
	void* payload;
	uint8_t check;
} KAL_FirePID_t;

#pragma pack ()               //作用：取消自定义字节对齐方式。

void KAL_FirePID_Init(KAL_FirePID_t* pack);
void KAL_FirePID_Transmit(KAL_FirePID_t* pack);
void KAL_FirePID_CountCheck(uint8_t* pack, uint8_t size);
void KAL_FirePID_Send(KAL_FirePID_t* pack, uint8_t ch, KAL_FirePID_Const_t index, void* param);
void KAL_FirePID_SendIndexPID(KAL_FirePID_t* pack, uint8_t ch, float p, float i, float d);
void KAL_FirePID_SendInterval(KAL_FirePID_t* pack, uint8_t ch, uint32_t interval);
void KAL_FirePID_SendTarget(KAL_FirePID_t* pack, uint8_t ch, int32_t target);
void KAL_FilePID_SendCurrent(KAL_FirePID_t* pack, uint8_t ch, int32_t current);

#endif /* KAL_FIREPID_H_ */
