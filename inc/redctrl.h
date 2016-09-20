#ifndef __RED_CTRL_H__
#define __RED_CTRL_H__


//定义一个结构体 保存一次解码后收到的四个字节数据
typedef struct tagIR_data
{
	unsigned char addr1;
	unsigned char addr2;
	unsigned char data1;
	unsigned char data2;
}IR_DATA;



//红外遥控键值
#define KEY_0		0x16
#define KEY_1		0x0C
#define KEY_2		0x18
#define KEY_3		0x5E
#define KEY_4   	0x08
#define KEY_5  		0x1C
#define KEY_6   	0x5A
#define KEY_7   	0x42
#define KEY_8   	0x52
#define KEY_9   	0x4A
#define KEY_100		0x19
#define KEY_200 	0x0D

#define KEY_SUB		0x07
#define KEY_ADD		0x15
#define KEY_EQ		0x09
#define KEY_LEFT	0x44
#define KEY_RIGHT	0x40
#define KEY_STOP	0x43

#define KEY_CH		0x46
#define KEY_CH0		0x45
#define KEY_CH1		0x47









#endif