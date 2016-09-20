//旋转主板的LED驱动程序

#include "includes.h"




//D1-D8分别连接在P2.0 - P2.7
//D9-D16 由595控制 OEA -P34 RCLA -P43 SCLA -P31 SDAA -P30
//D17-D24由595控制 OEB -P14 RCLB -P15 SCLB -P17 SDAB -P16
//D25-D32由595控制 OEC -P10 RCLC -P11 SCLC -P13 SDAC -P12
/*
void led_Init(void)
{
	//P2默认为传统准双向IO
// 	P2M1 = 0x00;
// 	P2M0 = 0x00;
	
}
*/

//发送数据给595，控制D9-D16
#ifdef UART_IN_USE
void LED_SendDataA(unsigned char temp)
{
	unsigned char k;
	
// 	STRA = 0;

	for(k = 0; k < 8;k++)
	{
		SCKA = 0;	//时钟上升沿移位
#ifdef SEND_DATA_MSB
		SDAA = (bit)(temp & 0x80);	//通过IO口送最高位数据
		temp <<= 1;					//temp左移一位，准备送下一个字节
#else
		SDAA = (bit)(temp & 0x01);	//通过IO口送最低位数据
		temp >>= 1;					//temp右移一位，准备送下一个字节
#endif		
		
		SCKA = 1;
	}
	STRA = 1;			//上升沿锁存
	STRA = 0;			//下降沿保持
// 	OEA = 0;		//595使能
}
#else

//通过串口1的方式0发送数据给595
//LSB
void LED_SendDataA(unsigned char temp)
{
// 	SBUF = temp;		//发送数据
// 	while(!TI);			//等待发送完成
// 	TI = 0;
	UART_SendByte(temp);
	STRA = 1;			//上升沿锁存
	STRA = 0;	
// 	OEA = 0;			//595输出使能
}

#endif



//发送数据给595,控制D17-D24
void LED_SendDataB(unsigned char temp)
{
	unsigned char k;
	
// 	STRB = 0;		
	//一个字节8位，发送给595
	for(k = 0; k < 8; k++)
	{
		SCKB = 0;		//上升沿移位
		
#ifdef SEND_DATA_MSB		
		SDAB = (bit)(temp & 0x80);	//通过IO口送最高位数据
		temp <<= 1;					//temp左移一位，准备送下一个字节
#else
		SDAB = (bit)(temp & 0x01);	//通过IO口送最低位
		temp >>= 1;					//temp右移一位，准备送下一个字节
#endif
		
		SCKB = 1;
	}
	STRB = 1;			//上升沿锁存
	STRB = 0;			//下降沿保持
// 	OEB = 0;			//595使能
}

//发送数据给595,控制D25-D32
void LED_SendDataC(unsigned char temp)
{
	unsigned char k;
	
// 	STRC = 0;		
	//一个字节8位，发送给595
	for(k = 0; k < 8; k++)
	{
		SCKC = 0;		//上升沿移位
		
#ifdef SEND_DATA_MSB		
		SDAC = (bit)(temp & 0x80);	//通过IO口送最高位数据
		temp <<= 1;					//temp左移一位，准备送下一个字节
#else
		SDAC = (bit)(temp & 0x01);	//通过IO口送最低位
		temp >>= 1;					//temp右移一位，准备送下一个字节
#endif
		
		SCKC = 1;
		
	}
	STRC = 1;			//上升沿锁存
	STRC = 0;			//下降沿保持
// 	OEC = 0;			//595使能
}

//将一个字节的高低位互换
unsigned char RollingChar(unsigned char temp)
{
	unsigned char ptemp = 0,k = 0;
	
	for(k = 0; k < 8; k++)
	{
		ptemp <<= 1;	//ptemp左移一位
		//判断temp的最低位
		if(temp & 0x01)
		{
			ptemp |= 0x01;
		}
		
		temp >>= 1;		//temp右移一位，判断下一位
	}
	return ptemp;
}


//发送数据到P2,直接控制LED1-LED8
void LED_SendDataP(unsigned char temp)
{
#ifndef SEND_DATA_MSB
	//如果是没有定义高位在先，就翻转，与595保持一直
	temp = RollingChar(temp);
#endif
	P2 = temp;
}

//LED流水灯程序，测试焊接是否正常
//@time 流水灯延时时间 MS
void LED_Test(unsigned int time)
{
	unsigned char temp = 0xFE;
	unsigned char i = 0;
	
	while(1)
	{
		for(i = 0; i < 8; i++)
		{
			LED_SendDataP(temp);
			temp = _crol_(temp,1);	//temp循环左移一位
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
//效果：D1-D8依次亮，D8-D1依次亮
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
