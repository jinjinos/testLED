
#include "includes.h"

void Delay(unsigned int i)
{
	while(i--);
}

void Delay1ms()		//@18.432MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 18;
	j = 234;
	do
	{
		while (--j);
	} while (--i);
}

/*
void Delay1us()		//@18.432MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 1;
	while (--i);
}
*/

void delayNms(unsigned int N)
{
	while(N--)
	{
		Delay1ms();
	}
}


void delayNus(unsigned int N)
{
	while(N--)
	{
// 		Delay1us();
	}
}



/*
void Delay10ms()		//@18.432MHz
{
	unsigned char i, j, k;

	i = 1;
	j = 180;
	k = 71;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay500ms()		//@18.432MHz
{
	unsigned char i, j, k;

	i = 36;
	j = 5;
	k = 211;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
*/