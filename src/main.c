
#include "includes.h"


sbit LED7 = P2^7;
extern void LED_GO(void);


//ϵͳ��Դ��ʼ������
void SystemInit(void)
{
// 	led_Init();
	//���ڽ��ջ�������ʼ��
	MsgInit(&recMessage);
	
#ifdef UART_IN_USE
	UART_Init(9600,Fosc_12T);		//���ڳ�ʼ��
#else
	UART_Init();
#endif
	
	Timer0Init();		//��ʱ��0��ʼ��
	
#ifndef UART_USE_TIMER1
	Timer1Init();		//��ʱ��1��ʼ��
#endif

	DS1302_Init();		//ʵʱʱ�ӳ�ʼ��
	
	
	
// 	WDT_Init();			//���Ź���ʼ��
	
	OEA = OEB = OEC = 1;		//595�����
//	OEA = OEB = OEC = 0;
	STRA = STRB = STRC = 0;

	EA = 1;			//ȫ���ж�����
}

void main(void)
{
	unsigned int temp = 0;
	SystemInit();
	
	LED33 = 0;		//����Ȧһֱ��
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
	
// 		WDT_Feed();				//ι��
// 		LED_Test(500);			//��ˮ�Ʋ��Գ���

	}
	
}

