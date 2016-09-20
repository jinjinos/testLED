#ifndef __UART_H__
#define __UART_H__

// #define 	UART_IN_USE					//���ڹ�����ģʽ1��ģʽ0(undefine)
#define 	UART_USE_INT				//���ô����ж�
//#define 	USRT_USE_TIMER1				//ʹ�ö�ʱ��1��Ϊ�����ʷ�����

#define Fosc 		18432000			//����Ƶ��
#define Fosc_1T			1				//1Tģʽ
#define Fosc_12T		12				//12Tģʽ

#define Buffer_Size  64

typedef struct tagMessage
{
	unsigned char * pIn;
	unsigned char * pOut;
	unsigned char rBuffer[Buffer_Size];
	unsigned char rNum;
}Message;

//�ж϶����Ƿ�����
#define MsgIsFull(x)	(x.rNum >= Buffer_Size?1:0)
//�ж϶����Ƿ�Ϊ��
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

//��������ʱ����ɵı�ʶ
extern volatile unsigned char tFlag;

extern void UART_SendByte(unsigned char sdata);
extern void UART_SendString(unsigned char *);


#endif

