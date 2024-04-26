#include "ML51.h"
#include "IO_define.h"
#include "RTimer0.h"
#include "RAdc.h"
#include "RDataFlash.h"
#include "Function.h"
#include "VoltCurrent.h"


float xdata RVoltFactor=1,YVoltFactor=1,BVoltFactor=1,CurrFactor=1;
UINT16 xdata Rvolt=0,Yvolt=0,Bvolt=0,Current=0;
extern struct Setting Settings;
extern UINT8 xdata Dig1,Dig2,Dig3,Dp;
UINT8 xdata Timer00Count=0;
struct Flag Flags;
extern struct Led LedStatus;
void Calibration(void)
{		UINT8 t=0;
	  if(Read_Key()==SET)
		{
			for(t=0;t<3;t++)
			{
			DISPLAY_CAL;
			Delay_1ms(300);
			DISPLAY_OFF;
			Delay_1ms(300);
			}
			ReadVolts();
			//for(t=0;t<3;t++)
			//ReadCurrent();
	
			RVoltFactor = 425/(Rvolt/RVoltFactor);
			YVoltFactor = 425/(Yvolt/YVoltFactor);
			BVoltFactor = 425/(Bvolt/BVoltFactor);
			//CurrFactor  = 15/(Current/CurrFactor);
			Write_Flash();
					
		}
}



void ReadVolts(void)
{
	UINT16 xdata VoltCount=0;
	uint8_t xdata t=0;
	static VoltRead=0;
	//float CosRadian = (120*3.1416)/180.0;
	VoltRead++;
	switch(VoltRead)
	{
		case 1:
			for(t=0;t<10;t++)
				VoltCount  += ReadVolt(R_ADC);
			 Rvolt = (VoltCount/10)*RVOLTMUL*RVoltFactor;
	  break;
		
		case 2:
			for(t=0;t<10;t++)
				VoltCount += ReadVolt(Y_ADC);
	      Yvolt =(VoltCount/10)*YVOLTMUL*YVoltFactor;
		break;
		
		case 3:
			for(t=0;t<10;t++)
				VoltCount  += ReadVolt(B_ADC);
				Bvolt = (VoltCount/10)*BVOLTMUL*BVoltFactor;
		    VoltRead = 0;
		break;
	}
}

void ReadCurrent(void)
{
	UINT16 xdata PreCurrent=0;
	PreCurrent = ReadCurrentCount(C_ADC);// *CURMUL * CurrFactor;
	PreCurrent += ReadCurrentCount(C_ADC);
	PreCurrent += ReadCurrentCount(C_ADC);
	PreCurrent += ReadCurrentCount(C_ADC);
	PreCurrent += ReadCurrentCount(C_ADC);
	PreCurrent += ReadCurrentCount(C_ADC);
	PreCurrent += ReadCurrentCount(C_ADC);
	PreCurrent += ReadCurrentCount(C_ADC);
	Current = (((PreCurrent/8)*CURMUL * CurrFactor));
	Current = (Current);
	//Precur=Current;
	//Current = (PreCurrent+ Current)/2;
}


UINT16 ReadVolt(uint8_t Channel)
{
	UINT16 AdcValue =0x0000,TempValue=0,TempCount=0;
	ADC_Close();
	ADC_Open(ADC_SINGLE,Channel); 
  AdcValue = Read_Adc(Channel,1,ADC_BITS);
	
	Flags.Apresent=0;
	Timer00Count=0;
	Flags.AcMeasure=1;
	
	while((AdcValue>4) && (Flags.Apresent==0))
		AdcValue = Read_Adc(Channel,1,ADC_BITS);
	while((AdcValue<5) && (Flags.Apresent==0))
		AdcValue = Read_Adc(Channel,1,ADC_BITS);
	
	if(Flags.Apresent==1)
	{
		ADC_Close();
		return 0;
	}
	
	Flags.Apresent=0;
	Timer00Count=0;
	Flags.AcMeasure=1;
	
	while((AdcValue>4) && (Flags.Apresent==0))
	{
		AdcValue = Read_Adc(Channel,1,10);
		TempValue = TempValue + AdcValue;
		TempCount++;
	}
	
	if(Flags.Apresent==1)
	{
		ADC_Close();
		return 0;
	}
	ADC_Close();
	return (TempValue/TempCount);	
}

UINT16 ReadCurrentCount(uint8_t Channel)
{
  UINT16 AdcValue =0x0000,TempValue=0,TempCount=0;
	ADC_Close();
	ADC_Open(ADC_SINGLE,Channel); 
	Flags.Apresent=0;
	Timer00Count=0;
	Flags.AcMeasure=1;
		AdcValue = Read_Adc(Channel,1,CURR_ADC_BITS);
	while((AdcValue>4) && (Flags.Apresent==0))
		AdcValue = Read_Adc(Channel,1,CURR_ADC_BITS);
	while((AdcValue<5) && (Flags.Apresent==0))
		AdcValue = Read_Adc(Channel,1,CURR_ADC_BITS);
	Flags.AcMeasure=0;
	if(Flags.Apresent==1)
	{
		ADC_Close();
		return 0;
	}
	TempValue=0;
	while(AdcValue>2)
	{
		AdcValue = Read_Adc(Channel,1,CURR_ADC_BITS);
		TempValue = TempValue + AdcValue;
		TempCount++;
	}
	
	ADC_Close();
	
	return (TempValue/(TempCount/2));
}

void DisplayVolt(void)
{
	if((Flags.VoltDisplay)==1)
	{ 
		  Dp = SEG_OFF;
		  CovertToDigi(456);
			LedStatus.Led3 = SEG_ON;
	}
}

void DisplayCurrent(void)
{
	if(((Flags.VoltDisplay)==0) && (Flags.Motor==1))
	{ 
		  CovertToDigi(123);
		  Dp = SEG_ON;
			LedStatus.Led3 = SEG_OFF;
	}
}


