//��ת�����LED��������

#include "includes.h"




//D1-D8�ֱ�������P2.0 - P2.7
//D9-D16 ��595���� OEA -P34 RCLA -P43 SCLA -P31 SDAA -P30
//D17-D24��595���� OEB -P14 RCLB -P15 SCLB -P17 SDAB -P16
//D25-D32��595���� OEC -P10 RCLC -P11 SCLC -P13 SDAC -P12
/*
void led_Init(void)
{
	//P2Ĭ��Ϊ��ͳ׼˫��IO
// 	P2M1 = 0x00;
// 	P2M0 = 0x00;
	
}
*/

//�������ݸ�595������D9-D16
#ifdef UART_IN_USE
void LED_SendDataA(unsigned char temp)
{
	unsigned char k;
	
// 	STRA = 0;

	for(k = 0; k < 8;k++)
	{
		SCKA = 0;	//ʱ����������λ
#ifdef SEND_DATA_MSB
		SDAA = (bit)(temp & 0x80);	//ͨ��IO�������λ����
		temp <<= 1;					//temp����һλ��׼������һ���ֽ�
#else
		SDAA = (bit)(temp & 0x01);	//ͨ��IO�������λ����
		temp >>= 1;					//temp����һλ��׼������һ���ֽ�
#endif		
		
		SCKA = 1;
	}
	STRA = 1;			//����������
	STRA = 0;			//�½��ر���
// 	OEA = 0;		//595ʹ��
}
#else

//ͨ������1�ķ�ʽ0�������ݸ�595
//LSB
void LED_SendDataA(unsigned char temp)
{
// 	SBUF = temp;		//��������
// 	while(!TI);			//�ȴ��������
// 	TI = 0;
	UART_SendByte(temp);
	STRA = 1;			//����������
	STRA = 0;	
// 	OEA = 0;			//595���ʹ��
}

#endif



//�������ݸ�595,����D17-D24
void LED_SendDataB(unsigned char temp)
{
	unsigned char k;
	
// 	STRB = 0;		
	//һ���ֽ�8λ�����͸�595
	for(k = 0; k < 8; k++)
	{
		SCKB = 0;		//��������λ
		
#ifdef SEND_DATA_MSB		
		SDAB = (bit)(temp & 0x80);	//ͨ��IO�������λ����
		temp <<= 1;					//temp����һλ��׼������һ���ֽ�
#else
		SDAB = (bit)(temp & 0x01);	//ͨ��IO�������λ
		temp >>= 1;					//temp����һλ��׼������һ���ֽ�
#endif
		
		SCKB = 1;
	}
	STRB = 1;			//����������
	STRB = 0;			//�½��ر���
// 	OEB = 0;			//595ʹ��
}

//�������ݸ�595,����D25-D32
void LED_SendDataC(unsigned char temp)
{
	unsigned char k;
	
// 	STRC = 0;		
	//һ���ֽ�8λ�����͸�595
	for(k = 0; k < 8; k++)
	{
		SCKC = 0;		//��������λ
		
#ifdef SEND_DATA_MSB		
		SDAC = (bit)(temp & 0x80);	//ͨ��IO�������λ����
		temp <<= 1;					//temp����һλ��׼������һ���ֽ�
#else
		SDAC = (bit)(temp & 0x01);	//ͨ��IO�������λ
		temp >>= 1;					//temp����һλ��׼������һ���ֽ�
#endif
		
		SCKC = 1;
		
	}
	STRC = 1;			//����������
	STRC = 0;			//�½��ر���
// 	OEC = 0;			//595ʹ��
}

//��һ���ֽڵĸߵ�λ����
unsigned char RollingChar(unsigned char temp)
{
	unsigned char ptemp = 0,k = 0;
	
	for(k = 0; k < 8; k++)
	{
		ptemp <<= 1;	//ptemp����һλ
		//�ж�temp�����λ
		if(temp & 0x01)
		{
			ptemp |= 0x01;
		}
		
		temp >>= 1;		//temp����һλ���ж���һλ
	}
	return ptemp;
}


//�������ݵ�P2,ֱ�ӿ���LED1-LED8
void LED_SendDataP(unsigned char temp)
{
#ifndef SEND_DATA_MSB
	//�����û�ж����λ���ȣ��ͷ�ת����595����һֱ
	temp = RollingChar(temp);
#endif
	P2 = temp;
}

//LED��ˮ�Ƴ��򣬲��Ժ����Ƿ�����
//@time ��ˮ����ʱʱ�� MS
void LED_Test(unsigned int time)
{
	unsigned char temp = 0xFE;
	unsigned char i = 0;
	
	while(1)
	{
		for(i = 0; i < 8; i++)
		{
			LED_SendDataP(temp);
			temp = _crol_(temp,1);	//tempѭ������һλ
			delayNms(time);
		}
		LED_SendDataP(0xFF);
		
		for(i = 0; i < 8; i++)
		{
			LED_SendDataA(temp);
			temp = _crol_(temp,1);
			OEA = 0;
			delayNms(time);
			OEA = 1;
		}
		for(i = 0; i < 8; i++)
		{
			LED_SendDataB(temp);
			temp = _crol_(temp,1);
			OEB = 0;
			delayNms(time);
			OEB = 1;
		}
		for(i = 0; i < 8; i++)
		{
			LED_SendDataC(temp);
			temp = _crol_(temp,1);
			OEC = 0;
			delayNms(time);
			OEC = 1;
		}
	}
}

/*
//Ч����D1-D8��������D8-D1������
void ledrun(void)
{		
 	char i = 0;
	unsigned char temp = 0x01;
	
	while(1)
	{
		temp = 0x01;
		for(i = 0; i < 8;i++)
		{
			P2 = ~temp;
			temp <<= 1;
			delayNms(500);
		}
		temp = 0x80;
		for(i = 8; i > 0;i--)
		{
			P2 = ~temp;
			temp >>= 1;
			delayNms(500);
		}
	}	
}
*/
