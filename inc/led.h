#ifndef __LED_H__
#define __LED_H__


// #define SEND_DATA_MSB 		//向595传输数据时高位在前

//D9-D16
sbit STRA = P4^3;			//595锁存时钟
sbit OEA  = P3^4;			//595使能
sbit SCKA = P3^1;			//TXD,数据输入时钟
sbit SDAA = P3^0;			//RXD,数据
//D17-D24
sbit STRB = P1^5;	 		//锁存
sbit OEB  = P1^4;  			//595使能
sbit SDAB = P1^6;			//数据
sbit SCKB = P1^7;			//时钟
//D25-D32
sbit STRC = P1^1;	 		//锁存
sbit OEC  = P1^0;  			//595使能
sbit SDAC = P1^2;			//数据
sbit SCKC = P1^3;			//时钟

sbit LED33  = P0^3;			//最外圈LED
sbit KEY = P3^2;		//红外检测起点

extern void LED_SendDataA(unsigned char temp);
extern void LED_SendDataB(unsigned char temp); 
extern void LED_SendDataC(unsigned char temp);
extern void LED_SendDataP(unsigned char temp);

extern void LED_Test(unsigned int time);

#endif