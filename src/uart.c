
//����ͨ�ų���

#include "includes.h"

//����һ�����ڽ��ջ�����
Message xdata recMessage;
volatile unsigned char tFlag = 0;		//�޸ĵ�ǰʱ��ı�־λ

//���ڽ�����Ϣ���г�ʼ��
void MsgInit(Message * recMessage)
{
	recMessage->pIn = &(recMessage->rBuffer[0]);
	recMessage->pOut = &(recMessage->rBuffer[0]);
	memset(recMessage->rBuffer,0,Buffer_Size);
	recMessage->rNum = 0;
}

//��Ϣ���
unsigned char MsgIn(Message * recMessage,char recData)
{
	if(MsgIsFull((*recMessage)))
	{
// 		printf("The Msg Queue is Full,can't insert into it\r\n");
		return 0;
	}
	else
	{
		*(recMessage->pIn++) = recData;
		if(recMessage->pIn == &(recMessage->rBuffer[Buffer_Size]))
		{
			recMessage->pIn = &(recMessage->rBuffer[0]);
		}
		recMessage->rNum++;
		return 1;
	}
}

//��Ϣ����
unsigned char MsgOut(Message * recMessage)
{
	unsigned char MsgChar = 0;
	
	if(MsgIsEmpty((*recMessage)))
	{
		printf("The Msg Queue is Empty,can't get out from it\r\n");
// 		return 0;
	}
	else
	{
		MsgChar = *(recMessage->pOut++);
		if(recMessage->pOut == &(recMessage->rBuffer[Buffer_Size]))
		{
			recMessage->pOut = &(recMessage->rBuffer[0]);
		}
		recMessage->rNum--;
	}
	return MsgChar;

}


#ifdef UART_IN_USE

//���ڳ�ʼ��  //9600bps@18.432MHz  12T   //115200bps@18.432MHz   1T
//@baud Ҫ���õĲ�����
//@foscPre ����Ϊ1T����12Tģʽ
void UART_Init(unsigned int baud,unsigned char foscPre)
{
#ifdef USRT_USE_TIMER1		//ʹ�ö�ʱ��1��Ϊ�����ʷ�����
	
	PCON &= 0x7F;		//�����ʲ��ӱ�
	SCON  = 0x50;		//0101 0000 8λ���ݣ��ɱ䲨����
	//AUXR����λѰַ
	if(foscPre == 1)
	{
		AUXR |= 0x40;		//T1x12 = 1����ʱ��ʱ��ΪFosc   ,��1 T		
	}
	else 
	{
		AUXR &= 0xBF;		//T1x12 = 0����ʱ��ʱ��ΪFosc/12,��12T
	}
	AUXR &= 0xFE;		//SIBRS = 0, ����1ѡ��ʱ��1λ�����ʷ�����
	
	TMOD &= 0x0F;		//�����ʱ��1��ģʽλ
	TMOD |= 0x20;		//���ö�ʱ��1Ϊ��λ�Զ���װģʽ
	
// 	TL1   = 0xFB;		//���ö�ʱ����ֵ			2^8 - (Fosc/12/32/baud)
	TL1	  = 256 - (Fosc/12/32/baud);
	
	TH1   = TL1;		//���ö�ʱ���Զ���װֵ
	
	ET1   = 0;			//��ֹ��ʱ��1���ж�
	TR1   = 1;			//������ʱ��1
	
#else					//ʹ�ö��������ʷ�����
	
	PCON &= 0x7F;		//�����ʲ��ӱ�
	SCON  = 0x50;		//����ģʽ1�,8λ���� �ɱ䲨����
	
	if(foscPre == 1)
	{
		AUXR |= 0x04;		//BRTx12 = 1����ʱ��ʱ��ΪFosc   ,��1 T
	}
	else
	{
		AUXR &= 0xFB;		//BRTx12 = 0����ʱ��ʱ��ΪFosc/12,��12T
	}
	
// 	BRT   = 0xFB;		//���� ���������ʷ�������װֵ
	BRT	  = 256 - (Fosc/12/32/baud);
	
	AUXR |= 0x01;		//SIBRS = 1,����1ѡ��ʹ�ö��������ʷ�����
	AUXR |= 0x10;		//BRTR = 1,�������������ʷ�����
	
#endif	
	
#ifdef UART_USE_INT
	ES    = 1;			//�����ж�����
// 	TI = 1;			//��ֲprintf�ķ���2
#else
	ES    = 0;
#endif  
}

#else
//���ô���Ϊ������ʽ0 TXD���ͬ�����壬RXD����������� LSB
void UART_Init(void)
{
	SCON = 0x00;	//ͬ��ͨ�ŷ�ʽ��TXD���ͬ�����壬RXD�����������
	//���ô���ģʽ0 Ϊ12��Ƶ
	AUXR &= ~(1 << 5);		//UART_M0x6 = 0;
	//���ô���ģʽ0 Ϊ2��Ƶ
// 	AUXR |= (1 << 5);
}

#endif






//ͨ�����ڷ���һ���ֽڵ�����
void UART_SendByte(unsigned char sdata)
{
	SBUF = sdata;
	while(!TI);		//�ȴ��������
	TI = 0;			//�巢����ɱ�־λ
}


/*
//ͨ�����ڷ���һ���ַ���
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
//�����жϷ����� //TI��RI���������
void UART_InterruptServer(void) interrupt 4
{
	unsigned char rdata = 0x00;
	if(RI)
	{
		//������жϱ�־λ
		RI = 0;
		rdata = SBUF;			//��������
		UART_SendByte(rdata);	//���ڷ������ݻ���
		MsgIn(&recMessage,rdata);
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
			UART_SendByte(rdata);	//���ڷ������ݻ���
		}
*/
	}
	else	// if(TI)
	{
		//�巢���жϱ�־λ
		TI = 0;			//��ֲprintf�ķ���2
	}
}
#endif



//������ֲprintf ����1
char putchar (char ch)
{
	UART_SendByte(ch);
	return ch;
}



