/*
 * kal_filepid.c
 *
 *  Created on: Apr 14, 2021
 *      Author: kirk_
 */

#include "kal_firepid.h"
#include "string.h"
#include "usart.h"

uint8_t KAL_FirePID_Pack[128];

void KAL_FirePID_Init(KAL_FirePID_t* pack)
{
	if(pack == NULL)
		return;

	pack->head = 0x59485A53;
	pack->check = 0;
	pack->size = 0;
	pack->payload = NULL;

	return;
}

void KAL_FirePID_Transmit(KAL_FirePID_t* pack)
{
	uint32_t size;
	uint32_t length;

	uint32_t* p;

	if(pack == NULL)
		return;

	size = pack->size;
	if(size>116)
	{
		size = 116;
		pack->size = size;
	}

	length = size-11;


	p = (uint32_t*)&KAL_FirePID_Pack[0];
	*p = pack->head;
	KAL_FirePID_Pack[4] = pack->channel;
	p = (uint32_t*)&KAL_FirePID_Pack[5];
	*p = pack->size;
	KAL_FirePID_Pack[9] = pack->instruct;

	memcpy(&KAL_FirePID_Pack[10], pack->payload, length);

	KAL_FirePID_CountCheck(KAL_FirePID_Pack, size);

	HAL_UART_Transmit(&KAL_FirePID_HUART, KAL_FirePID_Pack, size, 0x1ffff);
	// HAL_UART_Transmit_IT(&KAL_FirePID_HUART, KAL_FirePID_Pack, size);

	return;
}

void KAL_FirePID_CountCheck(uint8_t* pack, uint8_t size)
{
	uint8_t *p, s;

	if(pack == NULL)
		return;

	s = 0;
	p = (uint8_t*)pack;

	while(--size)
	{
		s += *p;
		p++;
	}

	*p = s;

	return;
}

void KAL_FirePID_Send(KAL_FirePID_t* pack, uint8_t ch, KAL_FirePID_Const_t index, void* param)
{

}

void KAL_FirePID_SendIndexPID(KAL_FirePID_t* pack, uint8_t ch, float p, float i, float d)
{
	static float payload[3];

	payload[0] = p;
	payload[1] = i;
	payload[2] = d;

	pack->channel = ch;
	pack->payload = payload;
	pack->size = 11+3*sizeof(float);
	pack->instruct = 0x03;

	KAL_FirePID_Transmit(pack);
}

void KAL_FirePID_SendInterval(KAL_FirePID_t* pack, uint8_t ch, uint32_t interval)
{

}

void KAL_FirePID_SendTarget(KAL_FirePID_t* pack, uint8_t ch, int32_t target)
{
	static int32_t tmp;

	tmp = target;
	pack->channel = ch;
	pack->payload = &tmp;
	pack->size = 15;
	pack->instruct = 0x01;

	KAL_FirePID_Transmit(pack);

	return;
}

void KAL_FilePID_SendCurrent(KAL_FirePID_t* pack, uint8_t ch, int32_t current)
{
	static int32_t tmp;

	tmp = current;
	pack->channel = ch;
	pack->payload = &tmp;
	pack->size = 15;
	pack->instruct = 0x02;

	KAL_FirePID_Transmit(pack);

	return;
}
