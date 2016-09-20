#ifndef __LED_H__
#define __LED_H__


// #define SEND_DATA_MSB 		//��595��������ʱ��λ��ǰ

//D9-D16
sbit STRA = P4^3;			//595����ʱ��
sbit OEA  = P3^4;			//595ʹ��
sbit SCKA = P3^1;			//TXD,��������ʱ��
sbit SDAA = P3^0;			//RXD,����
//D17-D24
sbit STRB = P1^5;	 		//����
sbit OEB  = P1^4;  			//595ʹ��
sbit SDAB = P1^6;			//����
sbit SCKB = P1^7;			//ʱ��
//D25-D32
sbit STRC = P1^1;	 		//����
sbit OEC  = P1^0;  			//595ʹ��
sbit SDAC = P1^2;			//����
sbit SCKC = P1^3;			//ʱ��

sbit LED33  = P0^3;			//����ȦLED
sbit KEY = P3^2;		//���������

extern void LED_SendDataA(unsigned char temp);
extern void LED_SendDataB(unsigned char temp); 
extern void LED_SendDataC(unsigned char temp);
extern void LED_SendDataP(unsigned char temp);

extern void LED_Test(unsigned int time);

#endif