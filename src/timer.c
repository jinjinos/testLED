//定时器0和定时器1的

#include "includes.h"

//定时器0 模式1 10ms 18.432MHz  12T
unsigned char Timer0_H = 0xC4;
unsigned char Timer0_L = 0x00;

volatile unsigned int T_Count0 = 0;



void Timer0Init(void)		//1ms@18.432MHz
{
	AUXR &= 0x7F;		//定时器时钟设置为12T模式
	TMOD &= 0xF0;		//清空定时器0的模式设置位
	TMOD |= 0x01;		//设置定时器0为工作模式1
	TL0 = Timer0_L;		//设置定时器初始值
	TH0 = Timer0_H;		//设置定时器初始值
	TF0 = 0;			//清除TF0中断标志
	ET0 = 1;			//允许定时器0的溢出中断
	
//	EA = 1;			//总中断
	TR0 = 1;			//定时器0开始计数
}


void Timer0IntServer(void) interrupt 1
{
	TL0 = Timer0_L;		//设置定时器初始值
	TH0 = Timer0_H;		//设置定时器初始值
	
	T_Count0++;
}


//如果串口不使用定时器1 就做为八位自动重装定时器使用
#ifndef USRT_USE_TIMER1 

//定时器1 模式2 100us 18.432MHz 12T
unsigned char Timer1_H = 0x66;
unsigned char Timer1_L = 0x66;

volatile unsigned int T_Count1 = 0;

void Timer1Init(void)
{
	AUXR &= 0xBF;		//定时器1时钟设置为12T模式
	TMOD &= 0x0F;		//清空定时器1的模式设置位
	TMOD |= 0x20;		//设置定时器1为工作模式2
	TL1= Timer1_L;		//设置定时器初始值
	TH1 = TL1;			//设置定时器重装初始值
	TF1 = 0;			//清除TF1中断标志
	ET1 = 1;			//允许定时器1的溢出中断
	
	TR1 = 1;			//定时器1开始计数
}

//定时器1的中断服务函数
void Timer_Routine(void) interrupt 3
{
	if(T_Count1++ == 5000)
	{
		T_Count1 = 0;
		LED33 = !LED33;
	}
}



#endif




