
#include "ML51.h"
#include "RAdc.h"
#include "RTimer0.h"
#include "IO_define.h"
#include <math.h>
extern BIT Apresent;
extern UINT16 Timer1Loadvalue;

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2019
//***********************************************************************************************************



  
  /**
  * @brief This API configures ADC module to be ready for convert the input from selected channel
  * @param[in] u8OpMode Decides the ADC operation mode. Valid values are:
  *                       - \ref ADC_SINGLE               :Single mode.
  *                       - \ref ADC_CONTINUOUS           :Continuous scan mode.
  * @param[in] u8ChMask Channel enable bit. Each bit corresponds to a input channel. 0 is channel 0, 1 is channel 1..., 7 is channel 7.
  *              VBG means band-gap voltage, VTEMP means temperature sensor, VLDO means LDO voltage.
  * @return  None
  * @note ML51 series MCU ADC can only convert 1 channel at a time. If more than 1 channels are enabled, only channel
  *       with smallest number will be convert.
  * @exmaple :  ADC_Open(ADC_SINGLE,0);
  */
void ADC_Open(unsigned char u8ADCOpMode, unsigned char u8ADCChMask)
{
  set_ADCCON1_ADCEN;                              /* enable ADC circuit*/
  switch (u8ADCOpMode)                            //ADC signle mode or continus mode select
  {
    case ADC_SINGLE: SFRS=0x00;ADCCON1&=0xEF; break;
    case ADC_CONTINUOUS: SFRS=0x00;ADCCON1|=0x10; break;
    default: break;
  }
  
  switch (u8ADCChMask)                            //ADC input channel digtial function disable
  {
    case 0: SFRS=0x00;ADCCON0&=0xF0;AINDIDS=0xFE;MFP_P25_ADC_CH0;P25_INPUT_MODE; break;
    case 1: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x01;SFRS=0x01;AINDIDS=0xFD;MFP_P24_ADC_CH1;P24_INPUT_MODE; break;
    case 2: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x02;SFRS=0x01;AINDIDS=0xFB;MFP_P23_ADC_CH2;P23_INPUT_MODE; break;
    case 3: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x03;SFRS=0x01;AINDIDS=0xF7;MFP_P22_ADC_CH3;P22_INPUT_MODE; break;
    case 4: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x04;SFRS=0x01;AINDIDS=0xEF;MFP_P21_ADC_CH4;P21_INPUT_MODE; break;
    case 5: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x05;SFRS=0x01;AINDIDS=0xDF;MFP_P20_ADC_CH5;P20_INPUT_MODE; break;
    case 6: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x06;SFRS=0x01;AINDIDS=0xBF;MFP_P31_ADC_CH6;P31_INPUT_MODE; break;
    case 7: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x07;SFRS=0x01;AINDIDS=0x7F;MFP_P32_ADC_CH7;P32_INPUT_MODE; break;
    case VBG: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x08; break;
    case VTEMP: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x09; break;
    case VLDO: SFRS=0x00;ADCCON0&=0xF0;ADCCON0|=0x0A; break;
  }
}

/**
  * @brief Disable ADC module
  * @param[in] adc Base address of ADC module
  * @return None
  */
void ADC_Close(void)
{
    clr_ADCCON1_ADCEN;
}


 /**
  * @brief This configures ADC module to be ready for convert the input from selected channel
  * @param[in] u16ADCRBase Decides the ADC RAM Base Address High byte + Low byte total
  * @param[in] u8ADCRLength Decides the ADC continui The total sampling numbers for ADC continue sampling select. 
  * @param[in] u8ADCSpeed is the ADCSR.7 Setting for select ADC low speed. 
  * @return  None
  * @note ML51 series MCU ADC can only convert 1 channel at a time. If more than 1 channels are enabled, only channel
  *       with smallest number will be convert.
  * @example ADC_InitialContinous(0x0300,128,ADCSlowSpeed);
  */
void ADC_InitialContinous(unsigned int u16ADCRBase, unsigned char u8ADCRLength, unsigned char u8ADCSpeed)
{
  SFRS=0x00;
  ADCBAL=u16ADCRBase;
  ADCBAH = u16ADCRBase>>8;
  ADCSN  = u8ADCRLength - 1;   //Offset value, since the actually sampling number= ADCSN[7:0] + 1
  switch (u8ADCSpeed)
  {
    case ADCSlowSpeed: set_ADCSR_SLOW; break;
    case ADCHighSpeed: clr_ADCSR_SLOW; break;
    default: break;
  }
}


 /**
  * @brief This configures ADC Sampling time 
  * @param[in] u8ADCDIV Decides the ADC clock devider value. Value from 0 ~ 7, devider is from 1 ~ 128, default value is 0 means Fadc = Fsys/1 
  * @param[in] u8ADCAQT Decides the ADC acquisition time base to add sampling time for ADC input, value is from 0 ~ 7, time = (4*u8ADCAQT+10)/Fadc, default value is 10/Fsys = 417ns.
  * @return  None
  * @note 
  * @example ADC_ConvertTime(2,7);
  */
void ADC_ConvertTime(unsigned char u8ADCDIV, unsigned char u8ADCAQT)
{
  SFRS=0x00;
  ADCSR &= 0x8F;
  ADCSR |= (u8ADCDIV&0x07)<<4;
  ADCCON2&=0xF1;
  ADCCON2|=(u8ADCAQT&0x07)<<1;
}

/**
  * @brief Configure the hardware trigger condition and enable hardware trigger
  * @param[in] adc Base address of ADC module
  * @param[in] u8Source Decides the hardware trigger source. Valid values are:
  *                       - \ref ADC_HWT_PWM0CH0            :A/D conversion is started by PWM0CH0 status.
  *                       - \ref ADC_HWT_PWM0CH2            :A/D conversion is started by PWM0CH2 status.
  *                       - \ref ADC_HWT_PWM0CH4            :A/D conversion is started by PWM0CH4 status.
  *                       - \ref ADC_HWT_STADC              :A/D conversion is started by STADC pin status.
  * @param[in] u8Param While ADC trigger by PWM, this parameter is used to set the delay between PWM
  *                     trigger and ADC conversion. Valid values are from 0 ~ 0xFF, and actual delay
  *                     time is (4 * u32Param * HCLK). While ADC trigger by external pin, this parameter
  *                     is used to set trigger condition. Valid values are:
  *                      - \ref ADC_HWT_FALLINGEDGE     :STADC pin or PWM channel falling edge active
  *                      - \ref ADC_HWT_RISINGEDGE      :STADC pin or PWM channel rising edge active
  *                      - \ref ADC_HWT_CENTRAL          :PWM channel period central active
  *                      - \ref ADC_HWT_END             :PWM channel period end active
  * @param[in] u8ADCHWTDelay Define External trigger delay time between PWM
  *                     External trigger delay time = FADC / ADCDLY
  * @return None
  * @note ADC hardware trigger source does not support PWM trigger (M05xxBN only).
  * @example ADC_EnableHWTrigger(ADC_HWT_STADC, ADC_HWT_FALLINGEDGE,0);
  */
void ADC_EnableHWTrigger(unsigned char u8ADCSHWTource, unsigned char u8ADCHWTParam, unsigned char u8ADCHWTDelay)
{
  SFRS=0x00;
  switch(u8ADCSHWTource)
  {
    case ADC_HWT_PWM0CH0: ADCCON0&=0xCF; break;
    case ADC_HWT_PWM0CH2: ADCCON0&=0xCF;ADCCON0|=0x10;break;
    case ADC_HWT_PWM0CH4: ADCCON0&=0xCF;ADCCON0|=0x20;break;
    case ADC_HWT_STADC:   ADCCON0&=0xCF;ADCCON0|=0x30;break;
    default: break;
  }
  switch(u8ADCHWTParam)
  {
    case ADC_HWT_FALLINGEDGE: ADCCON1&=0xF3;break;
    case ADC_HWT_RISINGEDGE:  ADCCON1&=0xF3;ADCCON1|=0x04;break;
    case ADC_HWT_CENTRAL:     ADCCON1&=0xF3;ADCCON1|=0x08;break;
    case ADC_HWT_END:         ADCCON1&=0xF3;ADCCON1|=0x0C;break;
    default: break;
  }
  ADCDLY = u8ADCHWTDelay;
  clr_ADCCON0_ADCS;
  ADCCON1|=0x02;              //ADC external conversion trigger enable bit
}

void ADC_DisableHWTrigger(void)
{
  clr_ADCCON1_ADCEX;
}

/**
  * @brief Enable the interrupt(s) selected by u8IntSource parameter.
  * @param[in] adc Base address of ADC module
  * @param[in] u8ADCINT Select Enable or disable 
  *                     - \ref Enable :then difine ADC interrupt source.
  *                     - \ref Disable
  * @param[in] u8IntSource The combination of interrupt status bits listed below. Each bit
  *                    corresponds to a interrupt status. This parameter decides which
  *                    interrupts will be enabled.
  *                     - \ref ADC_INT_HALFDONE :ADC convert continuse mode half done interrupt mode
  *                     - \ref ADC_INT_CONTDONE :ADC convert continuse mode full complete interrupt mode
  *                     - \ref ADC_INT_SINGLE   :ADC convert signle mode complete
  * @return None
  * @example ADC_EnableInt(ADC_INT_SINGLE);
  */
void ADC_Interrupt(unsigned char u8ADCINT, unsigned char u8ADCIntSource)
{
    SFRS=0x00;
    switch (u8ADCINT)
    {
      case Disable: 
        clr_IE_EADC; 
      break;
      
      case Enable: 
        switch (u8ADCIntSource)
          {
            case ADC_INT_HALFDONE:  ADCCON1|=0x30; break;
            case ADC_INT_CONTDONE:  ADCCON1|=0x01; ADCCON1&=0xDF; break;
            case ADC_INT_SINGLE:    ADCCON1&=0xCF; break;
            default: break;
          }
        set_IE_EADC;
      break;
      default: break;
    }
}

/**
  * @brief Read the bandgap value base on Vref = 3.072V storage address after UID area.
  * @param[in] none
  * @return 12bit bandgap value
  * @example temp = READ_BANDGAP();
  */
unsigned int READ_BANDGAP()
{
    unsigned char BandgapHigh,BandgapLow;
    unsigned int u16bgvalue;
    set_CHPCON_IAPEN;
    IAPCN = READ_UID;
    IAPAL = 0x0d;
    IAPAH = 0x00;
    set_IAPTRG_IAPGO;
    BandgapLow = IAPFD&0x0F;
    IAPAL = 0x0C;
    IAPAH = 0x00;
    set_IAPTRG_IAPGO;
    BandgapHigh = IAPFD;
    u16bgvalue = (BandgapHigh<<4)+BandgapLow;
    clr_CHPCON_IAPEN;
    return (u16bgvalue);
}

void ADC_Calibration(void)
{
  unsigned char calibration;
  SFRS=2;
  calibration =ADCCALI&0x8F;
  TA=0XAA;
  TA=0X55;
  ADCCALI=calibration;
  SFRS =0;
  ADCCON1|=SET_BIT7;
}

void AdcInit(void)
{
//	VREF_Reload(1);
//	VREF_Open(4);
	ADC_ConvertTime(0,0);
}

//**********************************************************************//
//---------------------ADC READ FUNCTIONS-------------------------------//
//@RETURN : ADC COUNT 
//@INPUTS : CHANNEL NO (0-7) , No Of Samples , No of Bits for output result (8-12)
//Note : Make sure that respective cahnnel pin Set as an Input mode like P04_Input_Mode;
//**********************************************************************//

UINT16 Read_Adc(UINT8 Channel, UINT8 Samples, UINT8 Bits)
{
	UINT32 xdata Adc_Val=0, Temp_Adc;
	UINT16 xdata Adc_Avg=0,Count=0,Temp=0;

	// ADC_Open(ADC_SINGLE,Channel); 
	
   for(Count=0;Count<Samples;Count++)
		{	
			clr_ADCCON0_ADCF;
			set_ADCCON0_ADCS;   
       while((ADCCON0|CLR_BIT7)==CLR_BIT7);         // wait ADCF = 1;
			clr_ADCCON0_ADCS;
			Temp_Adc = 0;
			Temp_Adc = ((ADCRH<<4) | ADCRL);			
			Adc_Val= ((Adc_Val+Temp_Adc));
		}	
		
	 	Adc_Avg = (Adc_Val/(Samples));
		Adc_Avg = (Adc_Avg >>(12-Bits));// & (0xFFF>>(12-Bits));
		
   // ADC_Close();
		return(Adc_Avg);		
}

/**
  * @brief  Configure the specified ACMP module
  * @param[in]  u8VREFValue from 0~5
  *                  - \ref LEVEL0 = 1.538V , when VDD > 1.8V
  *                  - \ref LEVEL1 = 2.048V , when VDD > 2.4V
  *                  - \ref LEVEL2 = 2.560V , when VDD > 2.9V
  *                  - \ref LEVEL3 = 3.072V , when VDD > 3.4V
  *                  - \ref LEVEL4 = 4.096V , when VDD > 4.5V
*/

void VREF_Open(unsigned char u8VREFValue)
{
    SFRS = 0x01;
    TA_REG_TMP|=(u8VREFValue<<4)|0x03;
    TA=0xAA;TA=0x55;
    VRFCON=TA_REG_TMP;
    _delay_();
    _delay_();
    TA=0xAA;TA=0x55;
    VRFCON&=0xFD;
}
void VREF_Reload(unsigned char u8VREFtrimValue)
{
  unsigned char u8Count;
 //( switch(u8VREFtrimValue)
  {
   // case LEVEL1:
        set_CHPCON_IAPEN;
        IAPAL = 0x43;
        IAPAH = 0x00;
        IAPCN = READ_UID;
        set_IAPTRG_IAPGO;
        u8Count = IAPFD;
        SFRS=1;
        TA=0XAA;TA=0X55;
        VRFTRIM = u8Count;
        clr_CHPCON_IAPEN;
    //break;
   /* case LEVEL3:
        set_CHPCON_IAPEN;
        IAPAL = 0x46;
        IAPAH = 0x00;
        IAPCN = READ_UID;
        set_IAPTRG_IAPGO;
        u8Count = IAPFD;
        SFRS=1;
        TA=0XAA;TA=0X55;
        VRFTRIM = u8Count;
        clr_CHPCON_IAPEN;
    break;*/
  }
  
}

UINT16 ReadAcWaveform(UINT8 Channel)
{
	UINT32 TempCount=0;
	UINT16 CheckAdc=0,TempAdc1=0;
	UINT8 Test=0;
	UINT32 TempAdc=0,AdcCount=0,AdcValue=0;
	next:
	TempCount=0;TempAdc1=0;TempAdc=0;AdcCount=0;
	
	ADC_Open(ADC_SINGLE,Channel); 
	TIMER1ON;
	//if(Read_Adc(Channel,25,ADC_BITS)>MID_VAL)
	while ((Read_Adc(Channel,25,ADC_BITS)>MID_VAL) && (Apresent==0));
	//else
	while ((Read_Adc(Channel,25,ADC_BITS)<=MID_VAL) && (Apresent==0));
	TIMER1OFF;
	if(Apresent==1)
	{
		ADC_Close();
    TIMER1OFF;
		CheckAdc++;
		if(CheckAdc>4)
    return 0;
		goto next;			
	}
	//---------------------------------------------------------------------//
//	AdcValue=(Read_Adc(Channel,5,ADC_BITS));
//	while (AdcValue>MID_VAL)
//	{
//		AdcValue=(Read_Adc(Channel,1,ADC_BITS));
//		TempAdc = AdcValue - MID_VAL;
//		TempAdc = TempAdc*TempAdc;
//		AdcCount = TempAdc + AdcCount;
//		TempCount++;
//	}
//	
//	AdcValue=(Read_Adc(Channel,1,ADC_BITS));
//	while (AdcValue<=MID_VAL)
//	{
//		AdcValue=(Read_Adc(Channel,1,ADC_BITS));
//		TempAdc = MID_VAL - AdcValue;
//		TempAdc = TempAdc*TempAdc;
//		AdcCount = TempAdc + AdcCount;
//		TempCount++;
//	}
//	if(TempCount>10000)
//			goto next;
////	
//-----------------------------------------------------------------------------------------//
  CheckAdc=0;
		while(CheckAdc<2)
	{
		TempAdc =0;
		TempAdc = Read_Adc(Channel,1,ADC_BITS);
		
	//	if((TempAdc==MID_VAL))// && (TempAdc1==0))
	//		CheckAdc++;
		
		if(TempAdc>MID_VAL)
		{
			TempAdc = TempAdc - MID_VAL;
			if(CheckAdc==1)
				CheckAdc=2;
		}
		else
		{
			TempAdc = MID_VAL - TempAdc;
			if(CheckAdc==0)
				CheckAdc=1;
		}
		TempAdc = TempAdc*TempAdc;
    
		
		AdcCount = TempAdc + AdcCount;
		
		TempCount ++;
		if(TempCount>10000)
			goto next;
	}




	AdcCount = AdcCount/TempCount;
	ADC_Close();
	TIMER1OFF;
	return (sqrt(AdcCount));
	//return (AdcCount);
}


//----------------Read key--------------------------------------------//
UINT8 Read_Key(void)
{
    UINT16 xdata Adc_Value=0x0000;
	  ADC_Open(ADC_SINGLE,SW_ADC);
	  Adc_Value=Read_Adc(SW_ADC,1,10);
	  ADC_Close();
	//  CovertToDigi(Adc_Value);
	  if((Adc_Value>=MIN_KEY_VAL) && (Adc_Value<=MAX_KEY_VAL))
    {
        if((Adc_Value>=MIN_MENU_KEY) && (Adc_Value<=MAX_MENU_KEY))
        {
            return (MENU);
        }

        else if((Adc_Value>=MIN_SET_KEY) && (Adc_Value<=MAX_SET_KEY))
        {
          return (SET);
        }
				
				else if((Adc_Value>=MIN_UP_KEY) && (Adc_Value<=MAX_UP_KEY))
        {
          return (UP);
        }
				else
					return (0);
		 }
		else
			return (0);

}