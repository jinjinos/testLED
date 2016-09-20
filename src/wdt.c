
#include "includes.h"

/*
//�������Ź�
void WDT_Start(void)
{
	WDT_CONTR |= 0x20;		//EN_WDT = 1,�������Ź�
}

//ֹͣ���Ź�
void WDT_Stop(void)
{
	WDT_CONTR &= 0xDF;		//EN_WDT = 0,ֹͣ���Ź�
}

//�忴�Ź������־
void WDT_ClrFlag(void)
{
	WDT_CONTR &= 0x7F;		//�忴�Ź������־λ
}


//ι�������¼���
void WDT_Feed(void)
{
	WDT_CONTR |= 0x10;		//CLR_WDT = 1 ,���¼���
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

//���Ź����ʱ������
//Time = (12 * Pre-scale * 32768) / Fosc = 393216 * Pre-scale / 18432000
void WDT_TimeSet(unsigned char Pre_scale)
{
	switch(Pre_scale)
	{
		//�����Ԥ��Ƶ����λ��������Ԥ��Ƶ
		case 2  :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x00; break;		//���ʱ��42.5ms
		case 4  :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x01; break;		//���ʱ��85ms
		case 8  :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x02; break;		//���ʱ��170ms
		case 16 :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x03; break;		//���ʱ��340ms
		case 32 :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x04; break;		//���ʱ��680ms
		case 64 :WDT_CONTR &= 0xF8;WDT_CONTR |= 0x05; break;		//���ʱ��1.36S
		case 128:WDT_CONTR &= 0xF8;WDT_CONTR |= 0x06; break;		//���ʱ��2.72S
		case 256:WDT_CONTR &= 0xF8;WDT_CONTR |= 0x07; break;		//���ʱ��5.44S
		default:break;
	}
}

//���Ź���ʼ��
//Ĭ�Ͽ��Ź��ٿ���ģʽ������
void WDT_Init()
{
	WDT_ClrFlag();			//������Ź������־
	WDT_TimeSet(128);		//128��Ƶ ���ʱ��2.72S
	WDT_IDLE_Count(0);		//����ģʽ������
	WDT_Start();			//�������Ź�
	
}

*/