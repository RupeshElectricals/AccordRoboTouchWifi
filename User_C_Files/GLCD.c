#include "ML51.h"
#include <IO_Define.h>

struct Flag Flags;

UINT8 xdata QrArray[1024];
extern code char PhonePeLogo[];
extern code char  paytmlogo [];
extern code char loading_up [];
extern code char loading_side [];
extern code char chair_left[];
extern code char chair_Right[];
extern code char Zero [];
extern code char One [];
extern code char Two [];
extern code char Three [];
extern code char Four [];
extern code char Five [];
extern code char Six [];
extern code char Seven [];
extern code char Eight [];
extern code char Nine [];
extern code char Dot [];


extern unsigned int TimerForChair;


extern UINT8 LoadingTime;
extern bit FlagSystemConnected;
extern bit FlagChairWorking;
void Delay_1ms(UINT16 mSec)
{
    UINT16 j,t;
    for(t=0;t<mSec;t++)
		{
			for(j=0;j<2124;j++);
		}
}
void GLCD_Command(UINT8 Command)									/* GLCD command function */
{
	GLCD_LOW_PORT = (Command &0x0F);													/* Copy command on data pin */	
	GLCD_HIGH_PORT = ((Command>>4) &0x0F);
	RS = 0;																				/* Make RS LOW to select command register */
	//RW = 0;																				/* Make RW LOW to select write operation */
	EN = 1;																				/* Make HIGH to LOW transition on Enable pin */
	Delay_1ms(1);
	EN = 0;														
	Delay_1ms(1);
}

void GLCD_Data(UINT8 Data)												/* GLCD data function */
{
	GLCD_LOW_PORT = (Data &0x0F);														/* Copy data on data pin */
	GLCD_HIGH_PORT = ((Data>>4) &0x0F);
	RS = 1;																				/* Make RS HIGH to select data register */
	//RW = 0;																				/* Make RW LOW to select write operation */
	EN = 1;																				/* Make HIGH to LOW transition on Enable pin */
	//Delay_1ms(1);
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;

	EN = 0;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	nop;nop;nop;nop;nop;nop;nop;
	//Delay_1ms(1);
	
}

void GLCD_Init()																/* GLCD initialize function */
{
	CS1(1); CS2=1;															/* Select both left & right half of display */
	RST(1);																			/* Keep reset pin high */
	Delay_1ms(2000);
	GLCD_Command(0x3E);														/* Display OFF */
	GLCD_Command(0x40);														/* Set Y address (column=0) */
	GLCD_Command(0xB8);														/* Set x address (page=0) */
	GLCD_Command(0xC0);														/* Set z address (start line=0) */
	GLCD_Command(0x3F);														/* Display ON */
}

void GLCD_ClearAll()														/* GLCD all display clear function */
{
	int i,j;
	CS1(1); CS2=1;															/* Select both left & right half of display */
	for(i=0;i<8;i++)
	{
		GLCD_Command((0xB8)+i);											/* Increment page each time after 64 column */
		for(j=0;j<64;j++)
		{
			GLCD_Data(0x00);															/* Write zeros to all 64 column */
		}
	}
	GLCD_Command(0x40);														/* Set Y address (column=0) */
	GLCD_Command(0xB8);														/* Set x address (page=0) */
}

void GLCD_SetCursor(UINT8 x,UINT8 y,UINT8 Page)  //X(0-63),Y(0-7),Page(0/1)
{
	CS1(!Page); CS2=Page;
	GLCD_Command(0x40+x);
	GLCD_Command(0xB8+y);
}

void GLCD_Image(UINT8 x,UINT8 y,UINT8 StartPage, UINT8 *Image,UINT8 ImageX,UINT8 ImageY)
{
	UINT8 GlcdX=x,GlcdY=y;
	UINT8 ImageLenX=0,ImageLenY=0;
	UINT8 Lines=ImageY/8;
	UINT16 k=0;
	
	GLCD_SetCursor(x,y,StartPage);
	for(ImageLenY=y;ImageLenY<Lines+y;ImageLenY++)
	{
		GLCD_SetCursor(x,ImageLenY,StartPage);
		for(ImageLenX=x;ImageLenX<ImageX+x;ImageLenX++)
		{
			GLCD_Data(Image[k]);k++;
			if(ImageLenX==63)
			{GLCD_SetCursor(0,ImageLenY,StartPage+1);
				CS1(0); CS2=1;
			}
		}
  }
}

void QrScreen(void)
{
	if(FlagSystemConnected==1 && Flags.RunnigScreen!=QrScr)
	{
	GLCD_ClearAll();
	GLCD_Image(32,0,0, QrArray,64,64);}
	Flags.RunnigScreen=QrScr;
//	GLCD_Image(10,0,1, PhonePeLogo,32,32);
//	GLCD_Image(0,4,1, paytmlogo,64,32);
	
}
void LoadingScreen(void)
{
	if((FlagSystemConnected==0) && (FlagChairWorking==0) && (TimerForChair==0))
	{
		Flags.RunnigScreen=LoadingScr;
	if(LoadingTime%2==0)
			{
				GLCD_ClearAll();
				GLCD_Image(45,0,0, loading_up,35,64);
			}
			else
			{
			GLCD_ClearAll();
			GLCD_Image(32,2,0, loading_side,64,35);
			}
	}
}

void ChairAnimation(void)
{
	if(Flags.RunnigScreen!=ChairScr)
	{
	Flags.RunnigScreen=ChairScr;
//	if(TimerForChair>0)
//				{
//					if(TimerForChair%2==0)
//						GLCD_Image(24,0,0, chair_left,80,64);
//					else
						GLCD_ClearAll();
						GLCD_Image(24,0,0, chair_Right,80,64);
//				}
	}
}

void ShowRemainigTime(UINT16 Time)
{
	UINT8 x,y,page, tempcnt=0;
	UINT16 Min,Sec;
	UINT8 Min1,Min2,Sec1,Sec2;
	Min=Time/60;
	Min1=Min/10;
	Min2=Min%10;
	Sec=Time%60;
	Sec1=Sec/10;
	Sec2=Sec%10;
	Flags.RunnigScreen=TimerScr;
	Again:
	 x=32*tempcnt;
	 y=2;
	 page=tempcnt/2;
		
	  switch(tempcnt)
		{
			case 0:
				DisplayNum(0,y,page,Min1);
				break;
			case 1:
				DisplayNum(30,y,page,Min2);
			  GLCD_Image(60,y,page,Dot ,8,48);
				break;		
			case 2:
				DisplayNum(4,y,page,Sec1);
				break;
      case 3:
				DisplayNum(34,y,page,Sec2);
				break;							
		}
		tempcnt++;
		if(tempcnt<4)
			goto Again;
		
}

void DisplayNum(UINT8 x,UINT8 y,UINT8 Page,UINT8 Num)
{
	switch(Num)
	{
		case 0:
			GLCD_Image(x,y,Page,Zero ,30,48);
			break;
		case 1:
			GLCD_Image(x,y,Page,One ,30,48);
			break;
		case 2:
			GLCD_Image(x,y,Page,Two ,30,48);
			break;
		case 3:
			GLCD_Image(x,y,Page,Three ,30,48);
			break;
		case 4:
			GLCD_Image(x,y,Page,Four ,30,48);
			break;
		case 5:
			GLCD_Image(x,y,Page,Five ,30,48);
			break;
		case 6:
			GLCD_Image(x,y,Page,Six ,30,48);
			break;
		case 7:
			GLCD_Image(x,y,Page,Seven ,30,48);
			break;
		case 8:
			GLCD_Image(x,y,Page,Eight ,30,48);
			break;
		case 9:
			GLCD_Image(x,y,Page,Nine ,30,48);
			break;
		
	}
}