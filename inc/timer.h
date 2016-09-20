
#ifndef __TIMER_H__
#define __TIMER_H__



extern volatile unsigned int T_Count0;

extern void Timer0Init(void);


#ifndef UART_USE_TIMER1

extern volatile unsigned int T_Count1;

extern void Timer1Init(void);

#endif




#endif



