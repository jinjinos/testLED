
#include "includes.h"


sbit LED7 = P2^7;
extern void LED_GO(void);


//系统资源初始化函数
void SystemInit(void)
{
// 	led_Init();
	//串口接收缓冲区初始化
	MsgInit(&recMessage);
	
#ifdef UART_IN_USE
	UART_Init(9600,Fosc_12T);		//串口初始化
#else
	UART_Init();
#endif
	
	Timer0Init();		//定时器0初始化
	
#ifndef UART_USE_TIMER1
	Timer1Init();		//定时器1初始化
#endif

	DS1302_Init();		//实时时钟初始化
	
	
	
// 	WDT_Init();			//看门狗初始化
	
	OEA = OEB = OEC = 1;		//595不输出
//	OEA = OEB = OEC = 0;
	STRA = STRB = STRC = 0;

	EA = 1;			//全局中断允许
}

void main(void)
{
	unsigned int temp = 0;
	SystemInit();
	
	LED33 = 0;		//最外圈一直亮
//	LED7 = 0;
	while(1)
	{
		if(T_Count0 == 50)
		{
			T_Count0 = 0;
			
//			LED7 = !LED7;
		}
		
// 		TestDS1302();
//		LED_GO();
		
// 		temp = DS18B20_ReadTemp();
// 		printf("temp = %.1f\r",temp * 0.1);
// 		delayNms(1000);
	
// 		WDT_Feed();				//喂狗
// 		LED_Test(500);			//流水灯测试程序

	}
	
}


