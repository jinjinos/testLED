//����ң�صĿ����ļ�

#include "includes.h"

sbit RED_CTRL = P3^3;
sbit LED7 = P2^7;

static IR_DATA IR_data;

bit new_code;		//�ж��Ƿ�Ϊ�¼�ֵ


bit TimeOut = 0;						//��ʱ��־
bit Guide = 0;							//��������Ч
bit Continuous = 0;						//������
unsigned char xdata IR_Time[33];		//���������½���֮���ʱ����

unsigned char key_code = 0XFF;

//����ң�����������
void  Decode(void) 
{
	unsigned char i = 0,j,k;
	static unsigned char *p;	//ָ��ṹ��IR_DATA��ָ��
	
	EX1 = 0;		//�ر��ⲿ�ж�
	p = &IR_data.addr1;
	
	for(k = 0;k < 4;k++)
	{
		for(j = 0;j < 8;j++)
		{
			if((IR_Time[i] > D0MinTime) && (IR_Time[i] < D0MaxTime))
            {
                *p >>= 1;           // ����1λ��
                *p &= 0x7f;         // ��0111 1111��0. ���ݡ�0�� 0.56ms�͵�ƽ+0.56ms�ߵ�ƽ
            }
            else if((IR_Time[i] > D1MinTime) && (IR_Time[i] < D1MaxTime))
            {
                *p >>= 1;           // ����1λ��
                *p |= 0x80;         // ��1000 0000��1. ���ݡ�1�� 0.56ms�͵�ƽ+1.69ms�ߵ�ƽ
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
		key_code = 0xFF;	/**�Ǳ���ң���������ݴ���**/
	}
	
	EX1 = 1;		//���ⲿ�ж�
}




//�ⲿ�ж�1���жϴ�����
void INT1Service(void) interrupt 2 using 1
{
	static unsigned char m=0;
	TR1=1;			//��ʱ��1��ʼ����
	
    T1_CNT_bk = T1_CNT;     // ����ʱ�����ֵ����ǰһ���½��ص����½��ص�ʱ����
    T1_CNT = 0x00;          // ���ʱ�����ֵ
    if(TimeOut)             // �����ʱ
    {
        //  TL0 = TIMER0_COUNT;     // ��ʼ����ʱ��0       ��0x1f��31��
        TimeOut = 0;        // �����ʱ��־
        m = 0;              // ��λ����λ
        T1_CNT = 0x00;      // ���ʱ�����ֵ
        Guide = 0;          // ���������־
        Continuous = 0;     // �����_����־
    }
    else       // ��������ʱ��58.5ms��76.5ms
    {
        if(Guide | Continuous)          // �����������Ч
        {
            IR_Time[m++] = T1_CNT_bk;       // ����ʱ����
            if(m == 32)   // ���չ�32���ݺ�
            {
                m = 0;
                Guide = 0;    // ���������־
				 //new_code=1;
				TR1=0;
                /*���н������*/
                Decode();     // ����
					
            }
        }
        if(T1_CNT_bk > GTime)   // ���ʱ����>������ʱ��
        {
            Guide = 1;          // ʹ��������־
            m = 0;
        };
        if(T1_CNT_bk > CTime)    // ���ʱ����>��_����ʱ��
        {
            Continuous = 1;      // ʹ����_����־
            m = 0;
        }
    }//end of ��ʱ
	
}





//ң�ؽ���
unsigned char redCodeReceice(void)
{
	unsigned char i,j,k = 0;
	unsigned char code_value = 0,key = 0;
	unsigned char key_code[4] = {'\0'};
	
	for(i = 0 ;i < 19;i++)
	{
		delayNus(400);			
		if(RED_CTRL)		//9ms���иߵ�ƽ���ж�Ϊ����
		{
			return 0xFF;
		}
	}
	while(!RED_CTRL);		//�ȴ�9ms�͵�ƽ��ȥ
	
	//�ж��Ƿ�������
	for(i = 0;i < 5;i++)
	{
		delayNus(500);
		if(!RED_CTRL)
		{
			return 0xFF;
		}
	}
	
	while(RED_CTRL);		//�ȴ�4.5ms�ߵ�ƽ��ȥ
	
	//�����ĸ��ֽڵ�����
	for(i = 0; i < 4; i++)
	{
		//ÿ���ֽ�8λ����
		for(j = 0; j < 8;j++)
		{
			while(!RED_CTRL);	//�ȴ���Ϊ�ߵ�ƽ
			
			while(RED_CTRL)	//����ߵ�ƽʱ��
			{
				delayNus(100);
				k++;
				//�ߵ�ƽʱ��������˳�
				if(k >= 22)
				{
					return 0xFF;
				}
			}
			
			code_value >>= 1;		//����һλ����
			if( k >= 8)
			{
				code_value |= 0x80;		//�ߵ�ƽ����0.56,��Ϊ����1
			}
			k = 0;			//��ʱ��������
		}
		key_code[i] = code_value;		//�ĸ��ֽ�����
	}
	

	key = key_code[2];

	return key;
}
