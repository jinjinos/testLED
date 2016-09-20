//红外遥控的控制文件

#include "includes.h"

sbit RED_CTRL = P3^3;
sbit LED7 = P2^7;

static IR_DATA IR_data;

bit new_code;		//判断是否为新键值


bit TimeOut = 0;						//超时标志
bit Guide = 0;							//引导码有效
bit Continuous = 0;						//连发码
unsigned char xdata IR_Time[33];		//保存两个下降沿之间的时间间隔

unsigned char key_code = 0XFF;

//红外遥控器解码程序
void  Decode(void) 
{
	unsigned char i = 0,j,k;
	static unsigned char *p;	//指向结构体IR_DATA的指针
	
	EX1 = 0;		//关闭外部中断
	p = &IR_data.addr1;
	
	for(k = 0;k < 4;k++)
	{
		for(j = 0;j < 8;j++)
		{
			if((IR_Time[i] > D0MinTime) && (IR_Time[i] < D0MaxTime))
            {
                *p >>= 1;           // 右移1位，
                *p &= 0x7f;         // 与0111 1111置0. 数据“0” 0.56ms低电平+0.56ms高电平
            }
            else if((IR_Time[i] > D1MinTime) && (IR_Time[i] < D1MaxTime))
            {
                *p >>= 1;           // 右移1位，
                *p |= 0x80;         // 或1000 0000置1. 数据“1” 0.56ms低电平+1.69ms高电平
            }
            i++;
		}
		p++;
	}
	if(IR_data.data2 + IR_data.data1 == 0xFF)
	{
		key_code = IR_data.data1;
		new_code = 1;
	}
	else
	{
		key_code = 0xFF;	/**非本机遥控器或数据错误**/
	}
	
	EX1 = 1;		//打开外部中断
}




//外部中断1的中断处理函数
void INT1Service(void) interrupt 2 using 1
{
	static unsigned char m=0;
	TR1=1;			//定时器1开始工作
	
    T1_CNT_bk = T1_CNT;     // 备份时间计数值，即前一个下降沿到本下降沿的时间间隔
    T1_CNT = 0x00;          // 清空时间计数值
    if(TimeOut)             // 如果超时
    {
        //  TL0 = TIMER0_COUNT;     // 初始化定时器0       －0x1f（31）
        TimeOut = 0;        // 清除超时标志
        m = 0;              // 复位数据位
        T1_CNT = 0x00;      // 清空时间计数值
        Guide = 0;          // 清除引导标志
        Continuous = 0;     // 清除连_发标志
    }
    else       // 正常按键时长58.5ms～76.5ms
    {
        if(Guide | Continuous)          // 如果引导码有效
        {
            IR_Time[m++] = T1_CNT_bk;       // 保存时间间隔
            if(m == 32)   // 接收够32数据后
            {
                m = 0;
                Guide = 0;    // 清除引导标志
				 //new_code=1;
				TR1=0;
                /*进行解码操作*/
                Decode();     // 解码
					
            }
        }
        if(T1_CNT_bk > GTime)   // 如果时间间隔>引导码时长
        {
            Guide = 1;          // 使能引导标志
            m = 0;
        };
        if(T1_CNT_bk > CTime)    // 如果时间间隔>连_发码时长
        {
            Continuous = 1;      // 使能连_发标志
            m = 0;
        }
    }//end of 超时
	
}





//遥控解码
unsigned char redCodeReceice(void)
{
	unsigned char i,j,k = 0;
	unsigned char code_value = 0,key = 0;
	unsigned char key_code[4] = {'\0'};
	
	for(i = 0 ;i < 19;i++)
	{
		delayNus(400);			
		if(RED_CTRL)		//9ms内有高电平就判断为干扰
		{
			return 0xFF;
		}
	}
	while(!RED_CTRL);		//等待9ms低电平过去
	
	//判断是否连发码
	for(i = 0;i < 5;i++)
	{
		delayNus(500);
		if(!RED_CTRL)
		{
			return 0xFF;
		}
	}
	
	while(RED_CTRL);		//等待4.5ms高电平过去
	
	//接收四个字节的数据
	for(i = 0; i < 4; i++)
	{
		//每个字节8位数据
		for(j = 0; j < 8;j++)
		{
			while(!RED_CTRL);	//等待变为高电平
			
			while(RED_CTRL)	//计算高电平时间
			{
				delayNus(100);
				k++;
				//高电平时间过长就退出
				if(k >= 22)
				{
					return 0xFF;
				}
			}
			
			code_value >>= 1;		//接收一位数据
			if( k >= 8)
			{
				code_value |= 0x80;		//高电平大于0.56,则为数据1
			}
			k = 0;			//定时计数清零
		}
		key_code[i] = code_value;		//四个字节数据
	}
	

	key = key_code[2];

	return key;
}
