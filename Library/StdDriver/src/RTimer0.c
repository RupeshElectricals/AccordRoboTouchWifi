#include "ML51.h"
#include "RTimer0.h"
#include "IO_define.h"


UINT16 Timer0Loadvalue=0,Timer1Loadvalue=0;
UINT8 LoadingTime=0;

UINT8 PreStartTime=0;
UINT16 Timer0Count=0;

extern unsigned int TimerForChair;
extern UINT8 OffTime;
extern bit FlagChairWorking;
extern bit FlagChairlastMin;
extern bit FlagOffAud,FlagStopAud;
extern struct Flag Flags;
extern code char Remaining_time [];
/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/
void Timer0_ISR(void) interrupt 1        // Vector @  0x0B
{
	//static  UINT16 Timer0Count=0;
	Timer0Count++;
	
	if(((Timer0Count%TIMER0_1SEC)==0) && OffTime>0)
	{
		OffTime--;
		//ChairAnimation();
		if(OffTime==0)
		{
			FlagChairWorking = 0;
			GLCD_ClearAll();
			
			RELAY = RELAY_ON;
			GLCD_Image(0,0,0, Remaining_time,128,16);
			TimerForChair = 60;
			FlagChairlastMin=1;
		}
	}
	
	if(((Timer0Count%TIMER0_1SEC)==0) && TimerForChair>0)
	{
		if(PreStartTime==0)
		{
			TimerForChair--;
			
			//ChairAnimation();
			if(TimerForChair==0)
			{
				RELAY = RELAY_OFF;
				if(FlagChairWorking==0)
				{
					FlagChairlastMin=0;
					FlagStopAud=1;
					QrScreen();
				}
				else 
				{
					FlagOffAud=1;
					//FlagChairWorking=0;
					
					OffTime = 30;
				}
			}
		}
		else if(PreStartTime>0)
		{
			PreStartTime--;
			if(PreStartTime==0)
			{
				GLCD_ClearAll();
				GLCD_Image(0,0,0, Remaining_time,128,16);
				RELAY = RELAY_ON;
			}
		}
	}
	
	
	if(((Timer0Count%TIMER0_1SEC)==0))
	{
	  LoadingTime++;
		Flags.LoadScreen=1;
	//	LoadingScreen();
		Flags.GetPidCnt++;
	}
	
	
	
	if(Timer0Count>60000)
		Timer0Count=0;
	
  LOAD_TIMER0;  
	clr_TCON_TF0;  
}



void TimerFunc(void)
{
	if(((Timer0Count%TIMER0_1SEC)==0) && TimerForChair>0)
	{
		if(PreStartTime==0)
		{
			TimerForChair--;
			
			//ChairAnimation();
			if(TimerForChair==0)
			{
				RELAY = RELAY_OFF;
				if(FlagChairWorking==0)
				{
					FlagChairlastMin=0;
					FlagStopAud=1;
					QrScreen();
				}
				else 
				{
					FlagOffAud=1;
					//FlagChairWorking=0;
					
					OffTime = 30;
				}
			}
		}
		else if(PreStartTime>0)
		{
			PreStartTime--;
			if(PreStartTime==0)
			{
				GLCD_ClearAll();
				GLCD_Image(0,0,0, Remaining_time,128,16);
				RELAY = RELAY_ON;
			}
		}
	}
	
	
	if(((Timer0Count%TIMER0_1SEC)==0))
	{
	  LoadingTime++;
	//	LoadingScreen();
		Flags.GetPidCnt++;
	}
}




//----------------TIMER 0 INITIALISATION-----------------//
//@ INPUT Msec : Timer0 msec configuration (0  - 48), change value of  TIMER0_MSEC for different MSEC
// Note : this function not Enable the Timer 0 Interrupt
// For interrupt enable use TIMER0_INTR_EN

void Timer0_Init(UINT8 Msec)
{
	TMOD |=0x01;                      //TIME0 16 bit
	Timer0Loadvalue = 65535 - (2000 * Msec);
	LOAD_TIMER0;
}
