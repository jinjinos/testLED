
#include "includes.h"

/*
//启动看门狗
void WDT_Start(void)
{
	WDT_CONTR |= 0x20;		//EN_WDT = 1,启动开门狗
}

//停止看门狗
void WDT_Stop(void)
{
	WDT_CONTR &= 0xDF;		//EN_WDT = 0,停止看门狗
}

//清看门狗溢出标志
void WDT_ClrFlag(void)
{
	WDT_CONTR &= 0x7F;		//清看门狗溢出标志位
}


//喂狗，重新计数
void WDT_Feed(void)
{
	WDT_CONTR |= 0x10;		//CLR_WDT = 1 ,重新计数
}

void WDT_IDLE_Count(bit c)
{
	if(c)
	{
		WDT_CONTR |= 0x08;
	}
	else
	{
		WDT_CONTR &= 0xF7;
	}
}

//看门狗溢出时间设置
//Time = (12 * Pre-scale * 32768) / Fosc = 393216 * Pre-scale / 18432000
void WDT_TimeSet(unsigned char Pre_scale)
{
	switch(Pre_scale)
	{
		//先清空预分频设置位，再设置预分频
		case 2  :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x00; break;		//溢出时间42.5ms
		case 4  :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x01; break;		//溢出时间85ms
		case 8  :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x02; break;		//溢出时间170ms
		case 16 :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x03; break;		//溢出时间340ms
		case 32 :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x04; break;		//溢出时间680ms
		case 64 :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x05; break;		//溢出时间1.36S
		case 128:WDT_CONTR &= 0xF8;WDT_CONTR |= 0x06; break;		//溢出时间2.72S
		case 256:WDT_CONTR &= 0xF8;WDT_CONTR |= 0x07; break;		//溢出时间5.44S
		default:break;
	}
}

//看门狗初始化
//默认看门狗再空闲模式不计数
void WDT_Init()
{
	WDT_ClrFlag();			//清除看门狗溢出标志
	WDT_TimeSet(128);		//128分频 溢出时间2.72S
	WDT_IDLE_Count(0);		//空闲模式不计数
	WDT_Start();			//启动看门狗
	
}

*/