#ifndef __RADC_H__
#define __RADC_H__

#define ADC_SINGLE	1
#define ADC_CONTINUOUS	2
#define VBG				8
#define VTEMP			9
#define VLDO			10

#define ADC_HWT_PWM0CH0		0
#define ADC_HWT_PWM0CH2		1
#define ADC_HWT_PWM0CH4		2
#define ADC_HWT_STADC		3

#define ADC_HWT_FALLINGEDGE	0
#define ADC_HWT_RISINGEDGE 		1
#define ADC_HWT_CENTRAL		2
#define ADC_HWT_END    		3

#define ADC_INT_HALFDONE 0
#define ADC_INT_CONTDONE 1
#define ADC_INT_SINGLE 3

#define ADCSlowSpeed    0
#define ADCHighSpeed    1


#define CURR_ADC_BITS   12
#define ADC_BITS   		  10
#define MID_VAL		 		  2048//1493//1638 //1.8v
#define MUL_FACTOR      0.32970454//0.2682//0.324//0.2682//0.2692//0.310215685//0.253394//0.28//0.277924//0.310945//0.298507//0.283//0//61063//0.28774721
#define CURR_MUL_FACTOR 0.102361//0.191514948//0.08311//32


//-----------------------SWITCH------------------------------//

#define MIN_KEY_VAL      200
#define MAX_KEY_VAL      1023

#define MIN_MENU_KEY      300
#define MAX_MENU_KEY      380

#define MIN_UP_KEY       900
#define MAX_UP_KEY       1023

#define MIN_SET_KEY      630
#define MAX_SET_KEY      700

//-------------KEYS RETURN CODES------------------//
#define MENU	    1
#define SET    		2
#define UP   		3




         
void ADC_Open(unsigned char u8ADCOpMode, unsigned char u8ADCChMask);
void ADC_Close(void);
void ADC_InitialContinous(unsigned int u16ADCRBase, unsigned char u8ADCRLength, unsigned char u8ADCSpeed);
void ADC_ConvertTime(unsigned char u8ADCDIV, unsigned char u8ADCAQT);
void ADC_EnableHWTrigger(unsigned char u8ADCSHWTource, unsigned char u8ADCHWTParam, unsigned char u8ADCHWTDelay);
void ADC_DisableHWTrigger(void);
void ADC_Interrupt(unsigned char u8ADCINT, unsigned char u8ADCIntSource);
unsigned int READ_BANDGAP();
void ADC_Calibration(void);
void VREF_Open(unsigned char u8VREFValue);
void VREF_Reload(unsigned char u8VREFtrimValue);
void AdcInit(void);

UINT16 Read_Adc(UINT8 Channel, UINT8 Samples, UINT8 Bits);
UINT16 Read_Adc(UINT8 Channel, UINT8 Samples, UINT8 Bits);
UINT16 ReadAcWaveform(UINT8 Channel);
UINT8 Read_Key(void);


#endif//__RADC_H__