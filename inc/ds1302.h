#ifndef __DS1302_H__
#define __DS1302_H__


#define T_CLK_SET		T_CLK = 1
#define T_CLK_CLR		T_CLK = 0
#define T_IO_SET		T_IO  = 1
#define T_IO_CLR		T_IO  = 0
#define T_RST_SET		T_RST = 1
#define T_RST_CLR		T_RST = 0


//DS1302命令相关地址
#define ds1302_sec_add			0x80		//秒
#define ds1302_min_add			0x82		//分
#define ds1302_hr_add			0x84		//时
#define ds1302_date_add			0x86		//日
#define ds1302_month_add		0x88		//月
#define ds1302_day_add			0x8a		//星期
#define ds1302_year_add			0x8c		//年
#define ds1302_control_add		0x8e		//控制
#define ds1302_charger_add		0x90 		//涓流充电			 
#define ds1302_clkburst_add		0xbe
#define ds1302_check_add		0xc0		//第00个RAM位置
 

extern void DS1302_Init(void);
extern void Write_DS1302(unsigned char addr,unsigned char wdata);
extern unsigned char Read_DS1302(unsigned char addr);
extern void DS1302_Write_Time(unsigned char *pTime);
extern void  DS1302_Read_Time(unsigned char * time_buffer);
extern void DS1302_Write_Check(unsigned char check);
extern void TestDS1302(void);


#endif


