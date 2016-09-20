#include "includes.h"


void IapIdle(void)
{
	IAP_CONTR = 0;                  //Close IAP function
    IAP_CMD = 0;                    //Clear command to standby
    IAP_TRIG = 0;                   //Clear trigger register
    IAP_ADDRH = 0x80;               //Data ptr point to non-EEPROM area
    IAP_ADDRL = 0;                  //Clear IAP address to prevent misuse
}


//从EEPROM读取一个字节
unsigned char EEP_ReadByte(unsigned int addr)
{
	unsigned int dat;	
	
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_READ;
	IAP_ADDRL = (unsigned char)addr;
	IAP_ADDRH = (unsigned char)(addr >> 8);
	//设置IAP命令触发
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0xA5;
	_nop_();
	dat = IAP_DATA;
	IapIdle();
	
	return dat;
}

//从EEPROM中读取多个字节
void EEP_Read(unsigned int addr,unsigned int number,unsigned char *pbuf)
{
	while(number--)
	{
		*pbuf = EEP_ReadByte(addr++);
		pbuf++;
	}
}


//向EEPROM写入一个字节的数据
void EEP_WriteByte(unsigned int addr,unsigned char dat)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_PROGRAM;
	IAP_ADDRL = (unsigned char)addr;
	IAP_ADDRH = (unsigned char)(addr >> 8);
	IAP_DATA  = dat;
	//设置IAP命令触发
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0xA5;
	_nop_();
	IapIdle();
}

//向EEPROM中写入多个字节 1-255
void EEP_Write(unsigned int addr,unsigned int number,unsigned char *pdat)
{
	while(number--)
	{
		EEP_WriteByte(addr++,*pdat);
		pdat++;
	}
}

//扇区擦除
void EEP_EraseSector(unsigned int addr)
{
	IAP_CONTR = ENABLE_IAP;
	IAP_CMD   = CMD_ERASE;
	IAP_ADDRL = (unsigned char)addr;
	IAP_ADDRH = (unsigned char)(addr >> 8);
	
	IAP_TRIG  = 0x5A;
	IAP_TRIG  = 0xA5;
	_nop_();
	//设置IAP命令触发
	IapIdle();
}


