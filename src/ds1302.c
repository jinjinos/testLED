
//时钟芯片DS1302的驱动程序
#include "includes.h"

//DS1302引脚连接定义
sbit T_CLK = P3^5;			//串行时钟线
sbit T_IO  = P3^6;			//数据管脚
sbit T_RST = P3^7;			//复位管脚


void DS1302_Init(void)
{
	unsigned char check = 0xAA;
	unsigned char time_Init[] = {0x16,0x03,0x09,0x14,0x09,0x29,0x00};
	
// 	T_RST_CLR;
// 	T_CLK_CLR;

	if(Read_DS1302(ds1302_check_add) != check)
	{
		//初始化时间
		DS1302_Write_Time(time_Init);
		P2 = 0xFE;
		//写入初始化标志
		DS1302_Write_Check(check);
	}
	else
	{
		printf("DS1302 is running!\r\n");
	}
}


//往DS1302写入一个字节的数据  低位在前
void RT_Write_Byte(unsigned char sdata)
{
	unsigned char i = 0;
	
// 	T_RST_SET;		//启动DS1302总线
	
	for(i = 0;i < 8 ; i++)
	{
// 		T_IO = sdata & 0x01;
		if(sdata & 0x01)
		{
			T_IO_SET;
		}
		else
		{
			T_IO_CLR;
		}
		T_CLK_SET;			//时钟脉冲
		T_CLK_CLR;
		sdata >>= 1;
	}
	
// 	T_RST_CLR;
}


//从DS1302读取一个字节的数据		低位在前
unsigned char RT_Read_Byte(void)
{
	unsigned char  i = 0;
	unsigned char temp = 0;
// 	T_RST_SET;		//启动DS1302时钟
	
	for(i = 0; i < 8;i++)
	{
		temp >>= 1;
		if(T_IO)
		{
			temp |= 0x80;
		}
		else
		{
			temp &= 0x7F;
		}
		T_CLK_SET;
		T_CLK_CLR;
	}
// 	T_RST_CLR;
	return temp;
}

//向DS1302写入数据 
//@addr 要写入的地址，@wdata 要写入的数据
void Write_DS1302(unsigned char addr,unsigned char wdata)
{
	T_RST_CLR;
	T_CLK_CLR;
	T_RST_SET;
	addr &= 0xFE;			//最低位清零，写入模式
	RT_Write_Byte(addr);	//写入地址
	RT_Write_Byte(wdata);	//写入数据
	T_CLK_SET;
	T_RST_CLR;
}


//从DS1302中读取数据
unsigned char Read_DS1302(unsigned char addr)
{
	unsigned char sdata;
	
	T_RST_CLR;
	T_CLK_CLR;
	T_RST_SET;
	addr |= 0x01;			//最低位置1，读模式
	RT_Write_Byte(addr);
	sdata = RT_Read_Byte();	
	T_CLK_SET;
	T_RST_CLR;
	return sdata;
}


//向DS1302写入时间
void DS1302_Write_Time(unsigned char *pTime)
{
	unsigned char i = 0;
	unsigned char ds1302_add = ds1302_year_add;
	
	Write_DS1302(ds1302_control_add,0x00);	//关闭写保护
	Write_DS1302(ds1302_sec_add,0x80);	//暂停  ds1302_sec_add的最高位CH为1，则停止工作
// 	Write_DS1302(ds1302_charger_add,0xa9);	//涓流充电
	
	for(i = 0; i < 7; i++)
	{
		//年 星期 月 日 时 分 秒
		Write_DS1302(ds1302_add,*(pTime + i));
		ds1302_add -= 2;
	}
	
	Write_DS1302(ds1302_control_add,0x80);	//打开写保护
}


//从DS1302中读取时间
void  DS1302_Read_Time(unsigned char * time_buf)
{
	unsigned char i = 0;
// 	unsigned char time_H,time_L;
	unsigned char * pTime = time_buf;
	unsigned char ds1302_addr = ds1302_year_add;
	
	
	for(i = 0; i < 7; i++)
	{
		//年 星期 月 日 时 分 秒
		*(pTime + i) = Read_DS1302(ds1302_addr);
		ds1302_addr -= 2;
	}
	
//  	pTime = time_buf;
	/*
	//将BCD码转换为十进制数
	for(i = 0; i < 7; i++)
	{
		time_H = *(pTime + i) / 16;		//十位
		time_L = *(pTime + i) % 16;		//个位
		*(pTime + i) = time_H * 10 + time_L ;
	}
	*/
	
}

//向DS1302写入标志位
void DS1302_Write_Check(unsigned char check)
{
	Write_DS1302(ds1302_control_add,0x00);	//关闭写保护
	Write_DS1302(ds1302_check_add,check);	//写入初始化标志
	Write_DS1302(ds1302_control_add,0x80);	//打开写保护
}


//DS1302测试程序
void TestDS1302(void)
{
	unsigned char time_buf[7] = {'\0'};
	unsigned char i = 0;
	
	while(1)
	{
		switch(tFlag)
		{
			case 1:
				tFlag = 0;
				if(recMessage.rNum == 7)
				{
					for(i = 0; i < recMessage.rNum;i++)
					{
						time_buf[i] = MsgOut();
					}
					DS1302_Write_Time(time_buf);
					printf("Write Time to DS1302 successed\r\n");
				}
				break;
			case 2:
				tFlag = 0;
				DS1302_Read_Time(time_buf);
				//按照yyyy-MM-dd hh24:mi:ss的格式串口输出
				printf("20%02x-%02x-%02x %02x:%02x:%02x\r\n",\
					(int)time_buf[0],(int)time_buf[2],(int)time_buf[3],(int)time_buf[4],(int)time_buf[5],(int)time_buf[6]);
				break;
			case 3:
				tFlag = 0;
				for(i = 0; i < recMessage.rNum ;i++)
				{
					MsgOut();
				}
				if(MsgIsEmpty(recMessage))
				{
					printf("clear Msg\r\n");
				}
				else
				{
					printf("I don't know what is wrong!");
				}
				break;
			default:break;
		}
		
		
// 		delayNms(10);
		
	}
}

































