#include "kd_delay.h"
#include "main.h"

void KD_Delay(uint32_t us)
{
	uint64_t tmp;
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOADքֵ
	// ticks=nus*fac_us; 						//ѨҪքޚƄ˽
	tmp = us;
	tmp *= reload +1;
	tmp += 500;
	tmp /= 1000;
	ticks = tmp&0xFFFFFFFF00000000?0xFFFFFFFF:(uint32_t)tmp;
	told=SysTick->VAL;        				//ٕ޸ɫʱք݆˽Ƿֵ
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;	//֢oעӢһЂSYSTICKˇһٶ֝ݵք݆˽ǷߍࠉӔ.
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;			//ʱݤӬڽ/ֈԚҪғԙքʱݤ,ղ΋Զ.
		}
	}
}
