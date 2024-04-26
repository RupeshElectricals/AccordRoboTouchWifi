#include "ML51.h"
#include "RTimer0.h"
#include "RAdc.h"
#include "IO_define.h"
#include <RDataFlash.h>
#include "VoltCurrent.h"
#include "Function.h"
extern UINT8 xdata Dig1,Dig2,Dig3,Dp;

struct Setting Settings;
struct Time Times;
extern struct Flag Flags;
extern UINT16 xdata Rvolt,Yvolt,Bvolt,Current;
extern struct Led LedStatus;

BIT Mode = AUTO;



void Menu(void)
{
	UINT8 MenuNum = 0; 
	UINT8 KeyPressed = 0;
	KeyPressed=Read_Key();
	if(KeyPressed==MENU)
	{
		DisplayMenu(MenuNum);
		Delay_1ms(300);
		//while(Read_Key()==MENU);
		while(MenuNum<TOTAL_MENU)
		{
			//KeyPressed=Read_Key();
			//if(KeyPressed==MENU)
			{ MenuNum++;
				DisplayMenu(MenuNum);
			}
		}
		Write_Flash();
	}
		
}
void DisplayMenu(UINT8 MenuNo)
{
	
	switch(MenuNo)
	{
		case POD:
			DISPLAY_DASH;
	    Delay_1ms(500);
			DISPLAY_POD;
		  Delay_1ms(500);
		  Settings.PodSetVal = Up_DownOnOff(Settings.PodSetVal,POD_STEP,POD_MAX,POD_MIN);
			break;
		case OLP:
			DISPLAY_DASH;
	    Delay_1ms(500);
			DISPLAY_OLS;
		  Delay_1ms(500);
		  Dp = SEG_ON;
		  Settings.OlSetVal = Up_DownOnOff(Settings.OlSetVal,OLP_STEP,OLP_MAX,OLP_MIN);
		  Dp = SEG_OFF;
			break;
		case ONT:
			DISPLAY_DASH;
	    Delay_1ms(500);
			DISPLAY_ONT;
		  Delay_1ms(500);
		  Settings.OntSetVal = Up_DownOnOff( Settings.OntSetVal,ONT_STEP,ONT_MAX,ONT_MIN);
			break;
		case OFT:
			DISPLAY_DASH;
	    Delay_1ms(500);
			DISPLAY_OFT;
		  Delay_1ms(500);
		  Settings.OftSetVal = Up_DownOnOff(Settings.OftSetVal,OFT_STEP,OFT_MAX,OFT_MIN);
			break;
		case SDT:
			DISPLAY_DASH;
	    Delay_1ms(500);
			DISPLAY_SDT;
		  Delay_1ms(500);
		  Settings.SdtSetVal = Up_DownOnOff(Settings.SdtSetVal,SDT_STEP,SDT_MAX,SDT_MIN);
			break;
	}
}


UINT16 Up_DownOnOff(UINT16 count,UINT8 Step,UINT16 Max_L,UINT16 Min_L)
{
	UINT8 xdata Key_Data=0;
	UINT16 xdata Temp_Count = count; 
	while(Read_Key()==MENU);
	CovertToDigi(Temp_Count);
	Delay_1ms(500);
	
	//Flags.KeyOut=1;
	//KeyOutCount=0;
	while( (Key_Data!= MENU))// && (KeyOutCount<10))
	{
		Key_Data=Read_Key();
		
//		if((Key_Data==UP) && (Temp_Count==0) && (Min_L!=0))
//		{
//			Temp_Count = Min_L;
//		}
		
		if((Key_Data==UP) && (Temp_Count<=Max_L))  //@
		{ 
			Temp_Count = Temp_Count+ Step;
		}
		if(Key_Data==SET)
		  Temp_Count=Min_L;
		
//		if((Key_Data==OFF) && (Temp_Count==Min_L) && (Min_L!=0))
//		{
//			Temp_Count = 0;
//		}
//     if((Key_Data==OFF) && (Temp_Count>Min_L) )  //@
//		{  if(Temp_Count==Min_L)
//			  Temp_Count=Max_L + Step;
//			Temp_Count =Temp_Count - Step;
//		}
		if(Temp_Count>Max_L)
			  Temp_Count=Min_L;
		CovertToDigi(Temp_Count);
		Delay_1ms(200);
	}
	//if(Key_Data==ENTER)
	//{
		if(count!=Temp_Count)
		{
			DISPLAY_OFF;
			Delay_1ms(300);
			CovertToDigi(Temp_Count);
			Delay_1ms(300);
			DISPLAY_OFF;
			Delay_1ms(300);
			CovertToDigi(Temp_Count);
		}
		return(Temp_Count);
//	}
}


UINT16 ReadDryPot(void)
{
	UINT32 xdata PotValue=0;
  ADC_Open(ADC_SINGLE,DRY_ADC); 
	
	PotValue = (Read_Adc(DRY_ADC,10,10));
	if(PotValue<50)
		return(0);
	
	if(PotValue>=50 && PotValue<55)
		PotValue=DRY_MIN_CURR;
	else if(PotValue>=1020)
		PotValue=DRY_MAX_CURR;
	else
	{
		PotValue = PotValue*(DRY_MAX_CURR-DRY_MIN_CURR);
		PotValue = (PotValue/1023)+DRY_MIN_CURR;
	}
  if(PotValue>3)
	return (PotValue);
	else
		return(0);
	
}


void ReadDrtSetCurr(void)
{
	UINT16 ReadPotVal = 0;
	UINT8 k=0;
	ReadPotVal = ReadDryPot();
	if(((ReadPotVal > (Settings.DrSetVal+5)) || (ReadPotVal < (Settings.DrSetVal-5))) && (ReadPotVal!=0))
	{
		DISPLAY_DRC;
		Delay_1ms(1000);
		again:
		while(Settings.DrSetVal!= ReadDryPot())
		{
			Settings.DrSetVal = ReadDryPot();
			Dp =SEG_ON;
			CovertToDigi(Settings.DrSetVal);
			Delay_1ms(500);	
			
		}
		k++;
		if(k<4)
		{
			Delay_1ms(1000);	
			goto again;
			
		}
		DISPLAY_OFF;
		Delay_1ms(500);
		CovertToDigi(Settings.DrSetVal);Dp =SEG_ON;
		Delay_1ms(500);
		DISPLAY_OFF;
		Delay_1ms(500);
		CovertToDigi(Settings.DrSetVal);Dp =SEG_ON;
		Delay_1ms(500);
		DISPLAY_OFF;
		//Delay_1ms(500);
		Dp=SEG_OFF;
	}
	
}


//----------------------DR RUN FAULT CHECK-----------------------------//
void DR_Fault(void)
{
	UINT8 k=0;
	if((Current < Settings.DrSetVal) && (Current!=0) && (Times.SdtDelay==0))
	{
		while((k<10) && (Current < Settings.DrSetVal))
		{
			//ReadCurrent();
			LedStatus.Led2 = SEG_ON;
			k++;
			Delay_1ms(1000);
		}
		if(k>=10)
		{
			MOTOROFF;
			ALL_LED_OFF;LedStatus.Led2 = SEG_ON;
			Flags.Fault = DRYRUNFAULT;
			Times.DrtTime = 100;
		}
		
	}
}

//------------------------------UNBALANCE------------------------------//


void CheckFaults(void)
{
	
}
