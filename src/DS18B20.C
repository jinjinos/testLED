
//单总线DS18B20的驱动程序
#include "includes.h"

sbit DQ = P4^0;		//18B20与单片机的连接管脚


//DS18B20复位，初始化。检测是否存在
//存在返回1，否则返回0
bit ds18b20_Init(void)
{
	unsigned char i = 0;
	
	//拉低总线并保持480us以上，发送复位脉冲
	DQ = 0;
	Delay(500);
	DQ = 1;
	//60us之后接收DS18B20存在脉冲
	Delay(50);
	i = DQ;		//读取60-240us的存在脉冲
	Delay(450);	//延时450us,接收保持大于480us
	
	if(DQ)
		return 0;		//DQ为高则没有存在脉冲
	else
		return 1;		
}

//从DS18B20中读取一个字节
unsigned char DS18B20_ReadByte(void)
{
	unsigned char i = 0;
	unsigned char dat = 0;
	
	for(i = 0;i < 8;i++)
	{
		dat >>= 1;		//值右移一位，方便接收下一位数据
		
		DQ = 0;			//发送读时序
		Delay(4);
		DQ = 1;
		
		Delay(10);	//延时10us之后读取电平
		
		if(DQ)
		{
			dat |= 0x80;	
		}
		Delay(60);
	}
	
	return dat;
}


//向DS18B20中写入一个字节的数据
void DS18B20_WriteByte(unsigned char dat)
{
	unsigned char i = 0;
	
	for(i = 0;i < 8;i++)
	{
		DQ = 0;
		Delay(4);
		
		DQ = dat & 0x01;
		
		Delay(50);
		
		DQ = 1;
		dat >>= 1;
	}
}


//从DS18B20中读取数据并转换 0x44命令
unsigned int DS18B20_ReadTemp(void)
{
	unsigned char temp_H,temp_L;
	unsigned int temp , i;
	
	//启动温度转换
	ds18b20_Init();		//DS18B20初始化
	DS18B20_WriteByte(0xcc);//*跳过读序列号*/
	DS18B20_WriteByte(0x44);//*启动温度转换*/
	Delay(10);
	
	ds18b20_Init();
	DS18B20_WriteByte(0xCC);//*跳过读序列号*/ 
	DS18B20_WriteByte(0xBE);//*读取温度*/ 
	
	temp_L = DS18B20_ReadByte();			//低字节
	temp_H = DS18B20_ReadByte();			//高字节
	for(i=0;i<7;i++)
		DS18B20_ReadByte();
	
	temp = (unsigned int)(temp_H & 0x00FF);
	
	temp = (temp << 8) | temp_L;

/*	
	if(temp < 0x0FFF)
		tFlag = 0;		//正负标志位为正
	else
	{
		tFlag = 1;
		temp = ~temp + 1;	//取反加1;
	}
*/	
	//不是乘以0.0625是为了把小数点后一位数据也转换为可以显示的数据，保留一位小数
 	temp = temp * 0.625;		//温度为29.5 temp = 295
	return temp;
}





