
//串口通信程序

#include "includes.h"

//定义一个串口接收缓冲区
Message xdata recMessage;
volatile unsigned char tFlag = 0;		//修改当前时间的标志位

//串口接收消息队列初始化
void MsgInit(void)
{
	recMessage.pIn = &(recMessage.rBuffer[0]);
	recMessage.pOut = &(recMessage.rBuffer[0]);
	memset(recMessage.rBuffer,0,Buffer_Size);
	recMessage.rNum = 0;
}

//消息入队
unsigned char MsgIn(char recData)
{
	if(MsgIsFull(recMessage))
	{
// 		printf("The Msg Queue is Full,can't insert into it\r\n");
		return 0;
	}
	else
	{
		*(recMessage.pIn++) = recData;
		if(recMessage.pIn == &(recMessage.rBuffer[Buffer_Size]))
		{
			recMessage.pIn = &(recMessage.rBuffer[0]);
		}
		recMessage.rNum++;
		return 1;
	}
}

//消息出队
unsigned char MsgOut(void)
{
	unsigned char MsgChar = 0;
	
	if(MsgIsEmpty(recMessage))
	{
		printf("The Msg Queue is Empty,can't get out from it\r\n");
// 		return 0;
	}
	else
	{
		MsgChar = *(recMessage.pOut++);
		if(recMessage.pOut == &(recMessage.rBuffer[Buffer_Size]))
		{
			recMessage.pOut = &(recMessage.rBuffer[0]);
		}
		recMessage.rNum--;
	}
	return MsgChar;

}


#ifdef UART_IN_USE

//串口初始化  //9600bps@18.432MHz  12T   //115200bps@18.432MHz   1T
//@baud 要设置的波特率
//@foscPre 设置为1T或者12T模式
void UART_Init(unsigned int baud,unsigned char foscPre)
{
#ifdef USRT_USE_TIMER1		//使用定时器1作为波特率发生器
	
	PCON &= 0x7F;		//波特率不加倍
	SCON  = 0x50;		//0101 0000 8位数据，可变波特率
	//AUXR不可位寻址
	if(foscPre == 1)
	{
		AUXR |= 0x40;		//T1x12 = 1，定时器时钟为Fosc   ,即1 T		
	}
	else 
	{
		AUXR &= 0xBF;		//T1x12 = 0，定时器时钟为Fosc/12,即12T
	}
	AUXR &= 0xFE;		//SIBRS = 0, 串口1选择定时器1位波特率发生器
	
	TMOD &= 0x0F;		//清楚定时器1的模式位
	TMOD |= 0x20;		//设置定时器1为八位自动重装模式
	
// 	TL1   = 0xFB;		//设置定时器初值			2^8 - (Fosc/12/32/baud)
	TL1	  = 256 - (Fosc/12/32/baud);
	
	TH1   = TL1;		//设置定时器自动重装值
	
	ET1   = 0;			//禁止定时器1的中断
	TR1   = 1;			//启动定时器1
	
#else					//使用独立波特率发生器
	
	PCON &= 0x7F;		//波特率不加倍
	SCON  = 0x50;		//串口模式1�,8位数据 可变波特率
	
	if(foscPre == 1)
	{
		AUXR |= 0x04;		//BRTx12 = 1，定时器时钟为Fosc   ,即1 T
	}
	else
	{
		AUXR &= 0xFB;		//BRTx12 = 0，定时器时钟为Fosc/12,即12T
	}
	
// 	BRT   = 0xFB;		//设置 独立波特率发生器重装值
	BRT	  = 256 - (Fosc/12/32/baud);
	
	AUXR |= 0x01;		//SIBRS = 1,串口1选择使用独立波特率发生器
	AUXR |= 0x10;		//BRTR = 1,启动独立波特率发生器
	
#endif	
	
#ifdef UART_USE_INT
	ES    = 1;			//串口中断允许
// 	TI = 1;			//移植printf的方法2
#else
	ES    = 0;
#endif  
}

#else
//设置串口为工作方式0 TXD输出同步脉冲，RXD输出串行数据 LSB
void UART_Init(void)
{
	SCON = 0x00;	//同步通信方式，TXD输出同步脉冲，RXD输出串行数据
	//设置串口模式0 为12分频
//	AUXR &= ~(1 << 5);		//UART_M0x6 = 0;
	//设置串口模式0 为2分频
 	AUXR |= (1 << 5);
}

#endif






//通过串口发送一个字节的数据
void UART_SendByte(unsigned char sdata)
{
	SBUF = sdata;
	while(!TI);		//等待发送完成
	TI = 0;			//清发送完成标志位
}


/*
//通过串口发送一个字符串
void UART_SendString(unsigned char *pStr)
{
	while(*pStr != '\0')
	{
		UART_SendByte(*pStr);
		pStr++;
	}
}
	
*/
	

#ifdef UART_USE_INT
//串口中断服务函数 //TI和RI需软件清零
void UART_InterruptServer(void) interrupt 4
{
	unsigned char rdata = 0x00;
	if(RI)
	{
		//清接收中断标志位
		RI = 0;
		rdata = SBUF;			//接收数据
		UART_SendByte(rdata);	//串口发送数据回显
		MsgIn(rdata);
		/*
		if(rdata == 0xFA)
		{
			tFlag = 1;
		}
		else if(rdata == 0xFF)
		{
			tFlag = 2;
		}
		else if(rdata == 0xFB)
		{
			tFlag = 3;
		}
		else
		{
			MsgIn(&recMessage,rdata);
			UART_SendByte(rdata);	//串口发送数据回显
		}
*/
	}
	else	// if(TI)
	{
		//清发送中断标志位
		TI = 0;			//移植printf的方法2
	}
}
#endif



//串口移植printf 方法1
char putchar (char ch)
{
	UART_SendByte(ch);
	return ch;
}



