#ifndef __FUNCTION_H__
#define __FUNCTION_H__
//-----------------menu's---------------------//
#define TOTAL_MENU  5
#define POD					0
#define OLP					1
#define ONT					2
#define OFT					3
#define SDT					4



#define POD_MAX			300     //----------IN SEC---------//
#define POD_MIN     10
#define POD_STEP    5


#define OLP_MAX			300      //-----IN CURRENT 300 = 30.0 AMP
#define OLP_MIN     10
#define OLP_STEP    5

#define ONT_MAX			300     //---------IN MIN
#define ONT_MIN     10
#define ONT_STEP    5


#define OFT_MAX			300      //----------IN MIN
#define OFT_MIN     10
#define OFT_STEP    5

#define SDT_MAX			300      //----------IN SEC---------//
#define SDT_MIN     10
#define SDT_STEP    5


#define MOTORON    RELAY1=RELAYON;RELAY2=RELAYON;Flags.Motor=1;
#define MOTOROFF   RELAY1=RELAYOFF;RELAY2=RELAYOFF;Flags.Motor=0;


#define AUTO 			1
#define MANUAL 		0

#define DRY_MIN_CURR     100
#define DRY_MAX_CURR     200

#define DRYRUNFAULT      1


#define ALL_LED_OFF LedStatus.Led1 = SEG_OFF;LedStatus.Led2 = SEG_OFF;LedStatus.Led3 = SEG_OFF;LedStatus.Led4 = SEG_OFF;LedStatus.Led5 = SEG_OFF;




struct Setting
	{
		UINT16 PodSetVal;
		UINT16 SdtSetVal;
		UINT16 SptSetVal;
		UINT16 DrSetVal;
		UINT16 OlSetVal;
		//UINT16 OlSetMinVal;
		//UINT16 OlSetMaxVal;
		//UINT16 OltSetVal;
		//UINT16 DrtSetVal;
		//UINT16 UvSetVal;
		//UINT16 OvSetVal;
		UINT16 OntSetVal;
		UINT16 OftSetVal;
    //UINT16 PasSetVal;		
		//UINT8 RevEnbDis:1;
		UINT8 MotorONOFF:1;
	};
	
struct Time
{
	UINT16 PodDelay;
	UINT16 SdtDelay;
	UINT16 SptDelay;
	UINT16 OnTime;
	UINT16 OffTime;
	UINT16 DrtTime;
};




void Menu(void);
void DisplayMenu(UINT8 MenuNo);
UINT16 Up_DownOnOff(UINT16 count,UINT8 Step,UINT16 Max_L,UINT16 Min_L);
UINT16 ReadDryPot(void);
void ReadDrtSetCurr(void);

#endif
