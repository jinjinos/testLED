#ifndef __UART_H__
#define __UART_H__

// #define 	UART_IN_USE					//串口工作在模式1或模式0(undefine)
#define 	UART_USE_INT				//启用串口中断
//#define 	USRT_USE_TIMER1				//使用定时器1作为波特率发生器

#define Fosc 		18432000			//晶振频率
#define Fosc_1T			1				//1T模式
#define Fosc_12T		12				//12T模式

#define Buffer_Size  64

typedef struct tagMessage
{
	unsigned char * pIn;
	unsigned char * pOut;
	unsigned char rBuffer[Buffer_Size];
	unsigned char rNum;
}Message;

//判断队列是否已满
#define MsgIsFull(x)	(x.rNum >= Buffer_Size?1:0)
//判断队列是否为空
#define MsgIsEmpty(x)	(x.rNum == 0?1:0)




#ifdef UART_IN_USE
extern void UART_Init(unsigned int baud,unsigned char foscPre);
#else
extern void UART_Init();
#endif

extern Message xdata recMessage; 
extern void MsgInit(Message * recMessage);
extern unsigned char MsgIn(Message * recMessage,char recData);
extern unsigned char MsgOut(Message * recMessage);

//串口输入时间完成的标识
extern volatile unsigned char tFlag;

extern void UART_SendByte(unsigned char sdata);
extern void UART_SendString(unsigned char *);


#endif

