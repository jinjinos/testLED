//��ʱ��0�Ͷ�ʱ��1��

#include "includes.h"

//��ʱ��0 ģʽ1 10ms 18.432MHz  12T
unsigned char Timer0_H = 0xC4;
unsigned char Timer0_L = 0x00;

volatile unsigned int T_Count0 = 0;



void Timer0Init(void)		//1ms@18.432MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ������Ϊ12Tģʽ
	TMOD &= 0xF0;		//��ն�ʱ��0��ģʽ����λ
	TMOD |= 0x01;		//���ö�ʱ��0Ϊ����ģʽ1
	TL0 = Timer0_L;		//���ö�ʱ����ʼֵ
	TH0 = Timer0_H;		//���ö�ʱ����ʼֵ
	TF0 = 0;			//���TF0�жϱ�־
	ET0 = 1;			//����ʱ��0������ж�
	
//	EA = 1;			//���ж�
	TR0 = 1;			//��ʱ��0��ʼ����
}


void Timer0IntServer(void) interrupt 1
{
	TL0 = Timer0_L;		//���ö�ʱ����ʼֵ
	TH0 = Timer0_H;		//���ö�ʱ����ʼֵ
	
	T_Count0++;
}


//������ڲ�ʹ�ö�ʱ��1 ����Ϊ��λ�Զ���װ��ʱ��ʹ��
#ifndef USRT_USE_TIMER1 

//��ʱ��1 ģʽ2 166.67us 18.432MHz 12T
unsigned char Timer1_H = 0x00;
unsigned char Timer1_L = 0x00;

// volatile unsigned int T_Count1 = 0;
volatile unsigned char T1_CNT;
unsigned char T1_CNT_bk;

void Timer1Init(void)
{
	AUXR &= 0xBF;		//��ʱ��1ʱ������Ϊ12Tģʽ
	TMOD &= 0x0F;		//��ն�ʱ��1��ģʽ����λ
	TMOD |= 0x20;		//���ö�ʱ��1Ϊ����ģʽ2
	TL1= Timer1_L;		//���ö�ʱ����ʼֵ
	TH1 = TL1;			//���ö�ʱ����װ��ʼֵ
	TF1 = 0;			//���TF1�жϱ�־
	ET1 = 1;			//����ʱ��1������ж�
	
	TR1 = 0;			//��ʱ��1ֹͣ����
}

//��ʱ��1���жϷ�����
void Timer_Routine(void) interrupt 3 using 2
{
	if(T1_CNT++ > TimeOut_cnt)
	{
		TimeOut = 1;
	}
}



#endif




