#ifndef __VOLT_CURRENT_H__
#define __VOLT_CURRENT_H__

#define RVOLTMUL     0.2895 
#define YVOLTMUL     0.2814
#define BVOLTMUL     0.2599
#define CURMUL       1.932

#define UBVOLT				60
#define UBHYST				10


struct Flag
		{
			UINT8 AcMeasure:1;
			UINT8 Apresent:1;
			UINT8 VoltDisplay:1;
			//UINT8 LastVal:1;
			//UINT8 DrtFault:1;
			UINT8 Fault;
      UINT8 Motor:1;		
			//UINT8 KeyOut:1;
			//UINT8 PhaseReversal:1;
			//UINT8 PhaseRev:1;
		};

		
		
void Calibration(void);
void ReadVolts(void);
void ReadCurrent(void);
UINT16 ReadVolt(uint8_t Channel);
UINT16 ReadCurrentCount(uint8_t Channel);
void DisplayVolt(void);		

#endif //__VOLT_CURRENT_H__