#ifndef __GLCD_H__
#define __GLCD_H__

struct Flag
{
	UINT8 GlcdImgStart;
	UINT8 PidReceived;
	UINT16 UartNo;
	UINT8 GetPidCnt;
	UINT8 LoadScreen;
	UINT8 RunnigScreen;
};

enum ScreenType
{
	
	LoadingScr=1,
	QrScr,
	TimerScr,
	ChairScr,
	
};


void Delay_1ms(UINT16 mSec);
void GLCD_Command(UINT8 Command);
void GLCD_Data(UINT8 Data);
void GLCD_Init(void);
void GLCD_ClearAll(void);
void GLCD_SetCursor(UINT8 x,UINT8 y,UINT8 Page);
void GLCD_Image(UINT8 x,UINT8 y,UINT8 StartPage, UINT8 *Image,UINT8 ImageX,UINT8 ImageY);
void QrScreen(void);
void LoadingScreen(void);
void ChairAnimation(void);
void ShowRemainigTime(UINT16 Time);
void DisplayNum(UINT8 x,UINT8 y,UINT8 Page,UINT8 Num);
#endif //__GLCD_H__
