
//������DS18B20����������
#include "includes.h"

sbit DQ = P4^0;		//18B20�뵥Ƭ�������ӹܽ�


//DS18B20��λ����ʼ��������Ƿ����
//���ڷ���1�����򷵻�0
bit ds18b20_Init(void)
{
	unsigned char i = 0;
	
	//�������߲�����480us���ϣ����͸�λ����
	DQ = 0;
	Delay(500);
	DQ = 1;
	//60us֮�����DS18B20��������
	Delay(50);
	i = DQ;		//��ȡ60-240us�Ĵ�������
	Delay(450);	//��ʱ450us,���ձ��ִ���480us
	
	if(DQ)
		return 0;		//DQΪ����û�д�������
	else
		return 1;		
}

//��DS18B20�ж�ȡһ���ֽ�
unsigned char DS18B20_ReadByte(void)
{
	unsigned char i = 0;
	unsigned char dat = 0;
	
	for(i = 0;i < 8;i++)
	{
		dat >>= 1;		//ֵ����һλ�����������һλ����
		
		DQ = 0;			//���Ͷ�ʱ��
		Delay(4);
		DQ = 1;
		
		Delay(10);	//��ʱ10us֮���ȡ��ƽ
		
		if(DQ)
		{
			dat |= 0x80;	
		}
		Delay(60);
	}
	
	return dat;
}


//��DS18B20��д��һ���ֽڵ�����
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


//��DS18B20�ж�ȡ���ݲ�ת�� 0x44����
unsigned int DS18B20_ReadTemp(void)
{
	unsigned char temp_H,temp_L;
	unsigned int temp , i;
	
	//�����¶�ת��
	ds18b20_Init();		//DS18B20��ʼ��
	DS18B20_WriteByte(0xcc);//*���������к�*/
	DS18B20_WriteByte(0x44);//*�����¶�ת��*/
	Delay(10);
	
	ds18b20_Init();
	DS18B20_WriteByte(0xCC);//*���������к�*/ 
	DS18B20_WriteByte(0xBE);//*��ȡ�¶�*/ 
	
	temp_L = DS18B20_ReadByte();			//���ֽ�
	temp_H = DS18B20_ReadByte();			//���ֽ�
	for(i=0;i<7;i++)
		DS18B20_ReadByte();
	
	temp = (unsigned int)(temp_H & 0x00FF);
	
	temp = (temp << 8) | temp_L;

/*	
	if(temp < 0x0FFF)
		tFlag = 0;		//������־λΪ��
	else
	{
		tFlag = 1;
		temp = ~temp + 1;	//ȡ����1;
	}
*/	
	//���ǳ���0.0625��Ϊ�˰�С�����һλ����Ҳת��Ϊ������ʾ�����ݣ�����һλС��
 	temp = temp * 0.625;		//�¶�Ϊ29.5 temp = 295
	return temp;
}




