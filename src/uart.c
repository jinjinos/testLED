
//´®¿ÚÍ¨ĞÅ³ÌĞò

#include "includes.h"

//¶¨ÒåÒ»¸ö´®¿Ú½ÓÊÕ»º³åÇø
Message xdata recMessage;
volatile unsigned char tFlag = 0;		//ĞŞ¸Äµ±Ç°Ê±¼äµÄ±êÖ¾Î»

//´®¿Ú½ÓÊÕÏûÏ¢¶ÓÁĞ³õÊ¼»¯
void MsgInit(void)
{
	recMessage.pIn = &(recMessage.rBuffer[0]);
	recMessage.pOut = &(recMessage.rBuffer[0]);
	memset(recMessage.rBuffer,0,Buffer_Size);
	recMessage.rNum = 0;
}

//ÏûÏ¢Èë¶Ó
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

//ÏûÏ¢³ö¶Ó
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

//´®¿Ú³õÊ¼»¯  //9600bps@18.432MHz  12T   //115200bps@18.432MHz   1T
//@baud ÒªÉèÖÃµÄ²¨ÌØÂÊ
//@foscPre ÉèÖÃÎª1T»òÕß12TÄ£Ê½
void UART_Init(unsigned int baud,unsigned char foscPre)
{
#ifdef USRT_USE_TIMER1		//Ê¹ÓÃ¶¨Ê±Æ÷1×÷Îª²¨ÌØÂÊ·¢ÉúÆ÷
	
	PCON &= 0x7F;		//²¨ÌØÂÊ²»¼Ó±¶
	SCON  = 0x50;		//0101 0000 8Î»Êı¾İ£¬¿É±ä²¨ÌØÂÊ
	//AUXR²»¿ÉÎ»Ñ°Ö·
	if(foscPre == 1)
	{
		AUXR |= 0x40;		//T1x12 = 1£¬¶¨Ê±Æ÷Ê±ÖÓÎªFosc   ,¼´1 T		
	}
	else 
	{
		AUXR &= 0xBF;		//T1x12 = 0£¬¶¨Ê±Æ÷Ê±ÖÓÎªFosc/12,¼´12T
	}
	AUXR &= 0xFE;		//SIBRS = 0, ´®¿Ú1Ñ¡Ôñ¶¨Ê±Æ÷1Î»²¨ÌØÂÊ·¢ÉúÆ÷
	
	TMOD &= 0x0F;		//Çå³ş¶¨Ê±Æ÷1µÄÄ£Ê½Î»
	TMOD |= 0x20;		//ÉèÖÃ¶¨Ê±Æ÷1Îª°ËÎ»×Ô¶¯ÖØ×°Ä£Ê½
	
// 	TL1   = 0xFB;		//ÉèÖÃ¶¨Ê±Æ÷³õÖµ			2^8 - (Fosc/12/32/baud)
	TL1	  = 256 - (Fosc/12/32/baud);
	
	TH1   = TL1;		//ÉèÖÃ¶¨Ê±Æ÷×Ô¶¯ÖØ×°Öµ
	
	ET1   = 0;			//½ûÖ¹¶¨Ê±Æ÷1µÄÖĞ¶Ï
	TR1   = 1;			//Æô¶¯¶¨Ê±Æ÷1
	
#else					//Ê¹ÓÃ¶ÀÁ¢²¨ÌØÂÊ·¢ÉúÆ÷
	
	PCON &= 0x7F;		//²¨ÌØÂÊ²»¼Ó±¶
	SCON  = 0x50;		//´®¿ÚÄ£Ê½1£,8Î»Êı¾İ ¿É±ä²¨ÌØÂÊ
	
	if(foscPre == 1)
	{
		AUXR |= 0x04;		//BRTx12 = 1£¬¶¨Ê±Æ÷Ê±ÖÓÎªFosc   ,¼´1 T
	}
	else
	{
		AUXR &= 0xFB;		//BRTx12 = 0£¬¶¨Ê±Æ÷Ê±ÖÓÎªFosc/12,¼´12T
	}
	
// 	BRT   = 0xFB;		//ÉèÖÃ ¶ÀÁ¢²¨ÌØÂÊ·¢ÉúÆ÷ÖØ×°Öµ
	BRT	  = 256 - (Fosc/12/32/baud);
	
	AUXR |= 0x01;		//SIBRS = 1,´®¿Ú1Ñ¡ÔñÊ¹ÓÃ¶ÀÁ¢²¨ÌØÂÊ·¢ÉúÆ÷
	AUXR |= 0x10;		//BRTR = 1,Æô¶¯¶ÀÁ¢²¨ÌØÂÊ·¢ÉúÆ÷
	
#endif	
	
#ifdef UART_USE_INT
	ES    = 1;			//´®¿ÚÖĞ¶ÏÔÊĞí
// 	TI = 1;			//ÒÆÖ²printfµÄ·½·¨2
#else
	ES    = 0;
#endif  
}

#else
//ÉèÖÃ´®¿ÚÎª¹¤×÷·½Ê½0 TXDÊä³öÍ¬²½Âö³å£¬RXDÊä³ö´®ĞĞÊı¾İ LSB
void UART_Init(void)
{
	SCON = 0x00;	//Í¬²½Í¨ĞÅ·½Ê½£¬TXDÊä³öÍ¬²½Âö³å£¬RXDÊä³ö´®ĞĞÊı¾İ
	//ÉèÖÃ´®¿ÚÄ£Ê½0 Îª12·ÖÆµ
//	AUXR &= ~(1 << 5);		//UART_M0x6 = 0;
	//ÉèÖÃ´®¿ÚÄ£Ê½0 Îª2·ÖÆµ
 	AUXR |= (1 << 5);
}

#endif






//Í¨¹ı´®¿Ú·¢ËÍÒ»¸ö×Ö½ÚµÄÊı¾İ
void UART_SendByte(unsigned char sdata)
{
	SBUF = sdata;
	while(!TI);		//µÈ´ı·¢ËÍÍê³É
	TI = 0;			//Çå·¢ËÍÍê³É±êÖ¾Î»
}


/*
//Í¨¹ı´®¿Ú·¢ËÍÒ»¸ö×Ö·û´®
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
//´®¿ÚÖĞ¶Ï·şÎñº¯Êı //TIºÍRIĞèÈí¼şÇåÁã
void UART_InterruptServer(void) interrupt 4
{
	unsigned char rdata = 0x00;
	if(RI)
	{
		//Çå½ÓÊÕÖĞ¶Ï±êÖ¾Î»
		RI = 0;
		rdata = SBUF;			//½ÓÊÕÊı¾İ
		UART_SendByte(rdata);	//´®¿Ú·¢ËÍÊı¾İ»ØÏÔ
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
			UART_SendByte(rdata);	//´®¿Ú·¢ËÍÊı¾İ»ØÏÔ
		}
*/
	}
	else	// if(TI)
	{
		//Çå·¢ËÍÖĞ¶Ï±êÖ¾Î»
		TI = 0;			//ÒÆÖ²printfµÄ·½·¨2
	}
}
#endif



//´®¿ÚÒÆÖ²printf ·½·¨1
char putchar (char ch)
{
	UART_SendByte(ch);
	return ch;
}



