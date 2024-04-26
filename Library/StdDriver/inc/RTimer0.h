#ifndef __RTIMER0_H__
#define __RTIMER0_H__

#define TIMER0_MSEC				20
#define LOAD_TIMER0 			TH0 = (Timer0Loadvalue >> 8);	TL0 =  Timer0Loadvalue & 0xFF ;TF0  = 0      //Timer 0 value load
#define TIMER0_INTR_EN  	TF0  = 0; TR0=1 ; ET0  = 1																				//Timer 0 interrupt Enable
#define TIMER0_INTR_DIS   TF0  = 0; TR0=0 ; ET0  = 0

#define TIMER0_1SEC			(1000/TIMER0_MSEC)
#define TIMER0_1MIN			(60000/TIMER0_MSEC)

#define PRE_START_TIME  30
				
void Timer0_Init(UINT8 Msec);
void TimerFunc(void);


#endif//__RTIMER0_H__