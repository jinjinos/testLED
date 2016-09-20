
//ʱ��оƬDS1302����������
#include "includes.h"

//DS1302�������Ӷ���
sbit T_CLK = P3^5;			//����ʱ����
sbit T_IO  = P3^6;			//���ݹܽ�
sbit T_RST = P3^7;			//��λ�ܽ�


void DS1302_Init(void)
{
	unsigned char check = 0xAA;
	unsigned char time_Init[] = {0x16,0x03,0x09,0x14,0x09,0x29,0x00};
	
// 	T_RST_CLR;
// 	T_CLK_CLR;

	if(Read_DS1302(ds1302_check_add) != check)
	{
		//��ʼ��ʱ��
		DS1302_Write_Time(time_Init);
		P2 = 0xFE;
		//д���ʼ����־
		DS1302_Write_Check(check);
	}
	else
	{
		printf("DS1302 is running!\r\n");
	}
}


//��DS1302д��һ���ֽڵ�����  ��λ��ǰ
void RT_Write_Byte(unsigned char sdata)
{
	unsigned char i = 0;
	
// 	T_RST_SET;		//����DS1302����
	
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
		T_CLK_SET;			//ʱ������
		T_CLK_CLR;
		sdata >>= 1;
	}
	
// 	T_RST_CLR;
}


//��DS1302��ȡһ���ֽڵ�����		��λ��ǰ
unsigned char RT_Read_Byte(void)
{
	unsigned char  i = 0;
	unsigned char temp = 0;
// 	T_RST_SET;		//����DS1302ʱ��
	
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

//��DS1302д������ 
//@addr Ҫд��ĵ�ַ��@wdata Ҫд�������
void Write_DS1302(unsigned char addr,unsigned char wdata)
{
	T_RST_CLR;
	T_CLK_CLR;
	T_RST_SET;
	addr &= 0xFE;			//���λ���㣬д��ģʽ
	RT_Write_Byte(addr);	//д���ַ
	RT_Write_Byte(wdata);	//д������
	T_CLK_SET;
	T_RST_CLR;
}


//��DS1302�ж�ȡ����
unsigned char Read_DS1302(unsigned char addr)
{
	unsigned char sdata;
	
	T_RST_CLR;
	T_CLK_CLR;
	T_RST_SET;
	addr |= 0x01;			//���λ��1����ģʽ
	RT_Write_Byte(addr);
	sdata = RT_Read_Byte();	
	T_CLK_SET;
	T_RST_CLR;
	return sdata;
}


//��DS1302д��ʱ��
void DS1302_Write_Time(unsigned char *pTime)
{
	unsigned char i = 0;
	unsigned char ds1302_add = ds1302_year_add;
	
	Write_DS1302(ds1302_control_add,0x00);	//�ر�д����
	Write_DS1302(ds1302_sec_add,0x80);	//��ͣ  ds1302_sec_add�����λCHΪ1����ֹͣ����
// 	Write_DS1302(ds1302_charger_add,0xa9);	//������
	
	for(i = 0; i < 7; i++)
	{
		//�� ���� �� �� ʱ �� ��
		Write_DS1302(ds1302_add,*(pTime + i));
		ds1302_add -= 2;
	}
	
	Write_DS1302(ds1302_control_add,0x80);	//��д����
}


//��DS1302�ж�ȡʱ��
void  DS1302_Read_Time(unsigned char * time_buf)
{
	unsigned char i = 0;
// 	unsigned char time_H,time_L;
	unsigned char * pTime = time_buf;
	unsigned char ds1302_addr = ds1302_year_add;
	
	
	for(i = 0; i < 7; i++)
	{
		//�� ���� �� �� ʱ �� ��
		*(pTime + i) = Read_DS1302(ds1302_addr);
		ds1302_addr -= 2;
	}
	
//  	pTime = time_buf;
	/*
	//��BCD��ת��Ϊʮ������
	for(i = 0; i < 7; i++)
	{
		time_H = *(pTime + i) / 16;		//ʮλ
		time_L = *(pTime + i) % 16;		//��λ
		*(pTime + i) = time_H * 10 + time_L ;
	}
	*/
	
}

//��DS1302д���־λ
void DS1302_Write_Check(unsigned char check)
{
	Write_DS1302(ds1302_control_add,0x00);	//�ر�д����
	Write_DS1302(ds1302_check_add,check);	//д���ʼ����־
	Write_DS1302(ds1302_control_add,0x80);	//��д����
}


//DS1302���Գ���
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
				//����yyyy-MM-dd hh24:mi:ss�ĸ�ʽ�������
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

































